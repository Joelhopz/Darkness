
#include "../Common.hlsli"

struct PSInput
{
    float4 position         : SV_Position0;
    float4 shadowPosition   : POSITION;
    float4 uv               : TEXCOORD0;
    float4 normal           : NORMAL0;
    float4 pos              : TEXCOORD1;
    float4 tangent          : TEXCOORD2;
    float4 viewpos          : TEXCOORD3;
};

Texture2D<float4> environmentIrradiance;
Texture2D<float4> shadowMap;

Texture2D<float4> albedo;
Texture2D<float4> roughness;
Texture2D<float4> normal;
Texture2D<float4> metalness;

Buffer<float4> lightWorldPosition;
Buffer<float4> lightDirection;
Buffer<float4> lightColor;
Buffer<float4> lightParameters;
Buffer<uint> lightType;
Buffer<float> lightIntensity;
Buffer<float> lightRange;

cbuffer ConstData
{
    float4 cameraWorldSpacePosition;

    // x = specular
    // y = metalness
    // z = material scale X
    // w = material scale Y
    float4 materialParameters;
    float4 materialParameters2;
    uint lightCount;
};

sampler tex_sampler;
SamplerComparisonState shadow_sampler;

float3x3 tangentFrame(float3 normal, float3 pos, float2 uv)
{
    // get edge vectors of the pixel triangle
    float3 dp1 = ddx_fine(pos);
    float3 dp2 = ddy_fine(pos);
    float2 duv1 = ddx_fine(uv);
    float2 duv2 = ddy_fine(uv);

    // solve the linear system
    float3 dp2perp = cross(dp2, normal);
    float3 dp1perp = cross(normal, dp1);
    float3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    float3 B = dp2perp * duv1.y + dp1perp * duv2.y;

    // construct a scale-invariant frame 
    float invmax = rsqrt(max(dot(T, T), dot(B, B)));
    return float3x3(-T * invmax, B * invmax, normal);
}

float3 randomNormal(float2 tex)
{
    float noiseX = (frac(sin(dot(tex, float2(15.8989f, 76.132f)*1.0f))*46336.23745f));
    float noiseY = (frac(sin(dot(tex, float2(11.9899f, 62.223f)*2.0f))*34748.34744f));
    float noiseZ = (frac(sin(dot(tex, float2(13.3238f, 63.122f)*3.0f))*59998.47362f));
    return normalize(float3(noiseX, noiseY, noiseZ));
}

float2 texOffset(int u, int v)
{
    return float2(u * 1.0f / 4096, v * 1.0f / 4096);
}

struct PsOutput
{
    float4 color;
    float accum;
};

static const float colorResistance = 1.0f;
static const float rangeAdjustmentsClampBounds = 10.0f;
static const float orderingDiscrimination = 200.0f;
static const float orderingStrength = 5.0;
static const float minValue = 1e-2;
static const float maxValue = 3e3;
float weight(float a, float depth)
{
    float absDepth = abs(depth);
    return pow(a, colorResistance) * 
        clamp(
            rangeAdjustmentsClampBounds / 
                (1e-5 + (absDepth / orderingStrength) + 
                (absDepth / orderingDiscrimination)),
            minValue, maxValue);
}

PsOutput main(PSInput input) : SV_Target
{
    float2 uv = float2(input.uv.x * materialParameters.z, (input.uv.y * materialParameters.w));

    float3x3 TBN = float3x3(normalize(input.tangent.xyz), normalize(-cross(input.tangent.xyz, input.normal.xyz)), normalize(input.normal.xyz));
    //float3x3 TBN = tangentFrame(input.normal.xyz, input.pos.xyz, uv);
    TBN = transpose(TBN);

    float4 albedoColor = albedo.Sample(tex_sampler, uv);
    float4 roughnessColor = roughness.Sample(tex_sampler, uv);
    float4 metalnessColor = metalness.Sample(tex_sampler, uv);

    // load normal map value and rescale to proper range
    float4 normalTex = normal.Sample(tex_sampler, float2(uv.x, uv.y));
    float3 remapped = (normalTex.xyz * 2.0f) - 1.0f;
    float3 normalMap = normalize(float3(remapped.x, -remapped.y, remapped.z));

    float3 N = mul(TBN, normalMap);
    //float3 N = input.normal.xyz;

    float3 camVector = input.pos.xyz - cameraWorldSpacePosition.xyz;
    float3 V = normalize(camVector);

    float3 directLight = float3(0.0f, 0.0f, 0.0f);
    float3 specular = float3(0.0f, 0.0f, 0.0f);

    for (uint i = 0; i < lightCount; ++i)
    {
        float3 light = lightColor[i].xyz * lightIntensity[i];
        float shininess = 5.0f;
        float3 spec = float3(0.0f, 0.0f, 0.0f);

        float3 L = lightWorldPosition[i].xyz - input.pos.xyz;
        float R = lightRange[i];
        float dist = length(L);
        L /= dist;

        // fresnel
        float fZero = 0.03f;
        float base = 1 - dot(L, N);
        float exp = pow(base, 5);
        float fresnel = exp + fZero*(1.0 - exp);
        spec += spec * fresnel;

        if (lightType[i] == 0)
        {
            // directional
            float3 Ld = normalize(lightDirection[i].xyz);
            float NLd = max(dot(N, Ld), 0);
            float3 Rsd = reflect(Ld, normalize(N));
            directLight += light * NLd;

            // specular
            spec += (light * pow(saturate(dot(Rsd, V)), shininess));
        }
        else
        {
            float D = max(dist - R, 0);

            // attenuation
            float denom = D / R + 1;
            float attenuation = 1 / (denom * denom);

            /*float cutOff = 0.5f;
            attenuation = (attenuation - cutOff) / (1 - cutOff);
            attenuation = max(attenuation, 0);*/
            float NL = dot(N, L);
            float intensity = saturate(NL);
            float3 Rs = reflect(normalize(L), normalize(N));

            if (lightType[i] == 1)
            {
                // spot
                float3 spotDirection = normalize(lightDirection[i].xyz);
                float cos_cur_angle = dot(L, spotDirection);

                float outer = lightParameters[i].y;
                float inner = lightParameters[i].x;
                bool shadowCasting = lightParameters[i].z > 0.0f;

                float angleDegrees = degrees(acos(cos_cur_angle));
                float angleDistance = outer - inner;

                if (angleDegrees < lightParameters[i].y)
                {
                    float spot = clamp((outer - angleDegrees) / angleDistance, 0.0f, 1.0f);

                    float3 directContrib = intensity * spot * light * attenuation;


                    float specular = pow(max(dot(Rs, V), 0.0),
                        shininess);
                    float specContrib = specular * spot * attenuation;


                    // spot shadow
                    if (shadowCasting)
                    {
                        float2 shadowUV = (0.5 * (input.shadowPosition.xy / input.shadowPosition.w)) + float2(0.5, 0.5);
                        shadowUV.y = 1.0f - shadowUV.y;
                        float2 satShadowUV = saturate(shadowUV);
                        if (satShadowUV.x == shadowUV.x && satShadowUV.y == shadowUV.y && (input.shadowPosition.z / input.shadowPosition.w > 0))
                        {
                            //PCF sampling for shadow map
                            float sum = 0;
                            float z = (input.shadowPosition.z / input.shadowPosition.w) + 0.00003f;
                            float x, y;

                            for (y = -1.5f; y <= 1.5f; y += 1.0f)
                            {
                                for (x = -1.5f; x <= 1.5f; x += 1.0f)
                                {
                                    sum += shadowMap.SampleCmpLevelZero(
                                        shadow_sampler,
                                        shadowUV + texOffset(x, y),
                                        z
                                    );
                                }
                            }
                            float shadowFactor = sum / 16.0f;

                            directContrib *= 1.0f - shadowFactor;
                            specContrib *= 1.0f - shadowFactor;
                        }
                    }
                    directLight += directContrib;
                    spec += specContrib;

                }
            }
            else if (lightType[i] == 2)
            {
                // point

                // diffuse
                directLight += intensity * light * NL * attenuation;

                // specular
                spec += (light * pow(saturate(dot(Rs, V)), shininess)) * attenuation;
            }
        }

        specular += spec;
    }

    // environment
    float3 reflectionWorld = reflect(-V, normalize(N));
    float exposure = materialParameters2.x;
    float4 environmentColor = environmentIrradiance.SampleLevel(tex_sampler, envMapEquirect(reflectionWorld), 0.0f);

    // diffuse
    float4 diffuse = float4(albedoColor.xyz * directLight.xyz, albedoColor.w);

    // master out
    float4 totalLight = (
        diffuse
        + (environmentColor * (metalnessColor.x * materialParameters.y) * pow(roughnessColor.x, 1.5)) // environment
        + float4(specular.xyz * roughnessColor.x * materialParameters.x, 0.0f)  // specular
        ) * exposure;

    //float zDistance = length(cameraWorldSpacePosition.xyz - input.pos.xyz) / 200.0f;
    /*float weight =
        max(
            min(
                1.0, 
                max(max(totalLight.r, totalLight.g), totalLight.b) * totalLight.a
            )
        , totalLight.a)
        * clamp(0.03 / (1e-5 + pow(zDistance / 200, 4.0)), 1e-2, 3e3)
        ;*/

    //float weight = totalLight.a * clamp(0.03 / (1e-5 + pow(zDistance / 200, 4.0)), 1e-2, 3e3);
    //float we = weight(totalLight.a, input.viewpos.z);
    //float we = weight(totalLight.a, input.viewpos.z);
    //float we = weight(totalLight.a, input.viewpos.z);
    float we = weight(totalLight.a, pow(min(length(input.viewpos), 20.0f), 5));
    //input.viewpos.z - 0.0656565577    float    v7.z

    PsOutput output;
    output.color = linearTosRGB(float4(totalLight.rgb * totalLight.a, totalLight.a) * we);
    output.accum = we;
    return output;
}
