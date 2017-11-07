
struct PSInput
{
    float4 position         : SV_Position0;
    float4 shadowPosition   : POSITION;
    float4 uv               : TEXCOORD0;
    float4 normal           : NORMAL0;
    float4 pos              : TEXCOORD1;
    float4 tangent          : TEXCOORD2;
};

Texture2D<float4> environmentIrradiance;
Texture2D<float4> environmentSpecular;

Texture2D<float4> albedo;
Texture2D<float4> normal;
Texture2D<float4> roughness;
Texture2D<float4> metalness;
Texture2D<float4> occlusion;

Texture2D<float4> shadowMap;

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

static const float PI =         3.14159265359f;
static const float TWO_PI =     6.28318530718f;
static const float ONE_DIV_PI = 0.3183098861837697f;

float2 envMapEquirect(float3 normal, float flipEnvMap)
{
    float phi = acos(normal.y);
    float theta = atan2(flipEnvMap * normal.x, normal.z) + PI;
    return float2(theta / TWO_PI, phi / PI);
}

float2 envMapEquirect(float3 normal)
{
    return envMapEquirect(normal, 1.0f);
}

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
    return float3x3(
        -T * invmax, 
        B * invmax, 
        normal);
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
    return float2(u * 1.0f / 1024, v * 1.0f / 1024);
}


// Lys constants
static const float k0 = 0.00098, k1 = 0.9921, fUserMaxSPow = 0.2425;
static const float g_fMaxT = (exp2(-10.0 / sqrt(fUserMaxSPow)) - k0) / k1;
static const int nMipOffset = 0;

float specPowerToMip(float specPower, int mips)
{
    float smulMax = (exp2(-10.0 / sqrt(specPower)) - k0) / k1;
    return (mips - 1 - nMipOffset) * (1.0f - clamp(smulMax / g_fMaxT, 0.0, 1.0));
}

float4 sRGBtoLinear(float4 sRGB)
{
    float3 sr = sRGB.xyz;
    //return float4(sr * (sr * (sr * 0.305306011 + 0.682171111) + 0.012522878), sRGB.w);

    // from nvidia HDR sdk
    return float4((sr <= 0.04045f) ? (sr / 12.92f) : (pow(sr + 0.055f, 2.4f) / 1.055f), sRGB.w);
}

float4 linearTosRGB(float4 RGB)
{
    /*float3 S1 = sqrt(RGB.xyz);
    float3 S2 = sqrt(S1);
    float3 S3 = sqrt(S2);
    return float4(0.662002687 * S1 + 0.684122060 * S2 - 0.323583601 * S3 - 0.0225411470 * RGB.xyz, RGB.w);*/

    float3 rg = RGB.xyz;
    return float4((rg <= 0.0031308f) ? (rg*12.92f) : (1.055f * pow(rg, 1.0f / 2.4f) - 0.055f), RGB.w);
}

float3 sRGBtoLinear(float3 sRGB)
{
    return (sRGB <= 0.04045f) ? (sRGB / 12.92f) : (pow(sRGB + 0.055f, 2.4f) / 1.055f);
}

float3 linearTosRGB(float3 RGB)
{
    return (RGB <= 0.0031308f) ? (RGB*12.92f) : (1.055f * pow(RGB, 1.0f / 2.4f) - 0.055f);
}

static const float A = 0.15;
static const float B = 0.50;
static const float C = 0.10;
static const float D = 0.20;
static const float E = 0.02;
static const float F = 0.30;
static const float W = 11.2;

float3 Uncharted2Tonemap(float3 x)
{
    return ((x*(A*x + C*B) + D*E) / (x*(A*x + B) + D*F)) - E / F;
}

float4 main(PSInput input) : SV_Target
{
    float2 uv = float2(input.uv.x * materialParameters.z, (input.uv.y * materialParameters.w));
    
    float3x3 TBN = float3x3(normalize(input.tangent.xyz), normalize(-cross(input.tangent.xyz, input.normal.xyz)), normalize(input.normal.xyz));
    //float3x3 TBN = tangentFrame(input.normal.xyz, input.pos.xyz, float2(uv.x, uv.y));
    TBN = transpose(TBN);

    float4 albedoColor = albedo.Sample(tex_sampler, uv);
    float4 roughnessColor = roughness.Sample(tex_sampler, uv);
    float metalnessColor = metalness.Sample(tex_sampler, uv).r;
    float occlusionColor = occlusion.Sample(tex_sampler, uv).r;
    float occlusionStrength = materialParameters2.x;
    
    float4 specularColor = float4(lerp(0.04f.rrr, albedoColor.rgb, metalnessColor), 1.0f);
    albedoColor.rgb = lerp(albedoColor.rgb, 0.0f.rrr, metalnessColor);

    // Gamma correct textures
    albedoColor = sRGBtoLinear(albedoColor);
    roughnessColor = sRGBtoLinear(roughnessColor);

    // load normal map value and rescale to proper range
    float4 normalTex = normal.Sample(tex_sampler, float2(uv.x, uv.y));
    float3 remapped = (normalTex.xyz * 2.0f) - 1.0f;
    float3 normalMap = normalize(float3(remapped.x, -remapped.y, remapped.z));
    
    float3 N = mul(TBN, normalMap);
    //float3 N = input.normal.xyz;

    float3 V = normalize(input.pos.xyz - cameraWorldSpacePosition.xyz);


    // environment
    float3 reflectionWorld = reflect(-V, normalize(N));
    float exposure = materialParameters2.y;
    float4 environmentIrr = environmentIrradiance.SampleLevel(tex_sampler, envMapEquirect(reflectionWorld), 0.0f);
    float4 environmentSpec = environmentSpecular.SampleLevel(tex_sampler, envMapEquirect(reflectionWorld), 0.0f);
    environmentIrr = sRGBtoLinear(environmentIrr);
    environmentSpec = sRGBtoLinear(environmentSpec);

    // occlusion
    float occlusion = 1 - (1 - occlusionColor) * 0.75f;
    environmentIrr *= occlusion;
    environmentSpec *= occlusion;

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


                    float specular = pow(max(dot(Rs, V), 0.0), shininess);
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
                    directLight += directContrib  * 0.5f;
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

    

    // diffuse
    float4 diffuse = float4(albedoColor.xyz * directLight.xyz, albedoColor.w);

    // master out
    float4 totalLight = (
        diffuse
        + (environmentIrr * (metalnessColor * materialParameters.y) * pow(roughnessColor.x, 1.5)) // environment
        + float4(specular.xyz * roughnessColor.x * materialParameters.x, 0.0f)  // specular
        ) * 0.0015f;
    

    // TONEMAPPING

    // uncharted 2
    /*totalLight *= exposure;
    
    float ExposureBias = 2.0f;
    float3 curr = Uncharted2Tonemap(ExposureBias*totalLight.xyz);

    float3 whiteScale = 1.0f / Uncharted2Tonemap(W);
    float3 color = curr*whiteScale;

    return float4(linearTosRGB(color), 1.0f);*/
    
    // Jim Hejl and Richard Burgess-Dawson
    totalLight *= exposure;
    float3 x = max(0, totalLight.xyz - 0.004);
    float3 retColor = (x*(6.2*x + .5)) / (x*(6.2*x + 1.7) + 0.06);
    return float4(retColor, 1);

    // simple Reinhard
    /*totalLight *= exposure;

    float3 texColor = totalLight.xyz;
    texColor = texColor / (1 + texColor);
    float3 retColor = pow(texColor, 1 / 2.2);

    return float4(retColor, 1.0f);*/
}
