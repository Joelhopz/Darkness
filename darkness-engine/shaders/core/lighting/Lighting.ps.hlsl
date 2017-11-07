
#include "../Common.hlsli"

struct PSInput
{
    float4 position         : SV_Position0;
    float2 uv               : TEXCOORD0;
};

TextureCube<float4> environmentIrradianceCubemap;
Texture2D<float4> environmentIrradiance;
TextureCube<float4> environmentSpecular;
Texture2D<float4> environmentBrdfLut;

Texture2D<float4> albedo;
Texture2D<float4> normal;
Texture2D<float4> roughness;
Texture2D<float4> metalness;
Texture2D<float4> occlusion;

Texture2DArray<float4> shadowMap;
StructuredBuffer<float4x4> shadowVP;

Texture2D<float4> ssao;

Buffer<float4> lightWorldPosition;
Buffer<float4> lightDirection;
Buffer<float4> lightColor;
Buffer<float4> lightParameters;
Buffer<uint> lightType;
Buffer<float> lightIntensity;
Buffer<float> lightRange;

Texture2D<float4> depth;

cbuffer ConstData
{
    float3 cameraWorldSpacePosition;
    float roughnessStrength;

    float metalnessStrength;
    float occlusionStrength;
    float2 materialScale;

    float exposure;
    uint lightCount;
    uint hasAlbedo;
    uint hasNormal;

    uint hasRoughness;
    uint hasMetalness;
    uint hasOcclusion;
    uint hasEnvironmentIrradianceCubemap;

    uint hasEnvironmentIrradianceEquirect;
    uint hasEnvironmentSpecular;
    float environmentStrength;
    float padding;

    float4x4 cameraInverseProjectionMatrix;
    float4x4 cameraInverseViewMatrix;

    float2 shadowSize;
};

sampler tex_sampler;
sampler depth_sampler;
sampler point_sampler;
SamplerComparisonState shadow_sampler;

float3 sampleEnvironment(float3 direction, float lod)
{
    if (hasEnvironmentIrradianceCubemap)
        return environmentIrradianceCubemap.SampleLevel(tex_sampler, direction, lod).xyz;
    else if(hasEnvironmentIrradianceEquirect)
        return environmentIrradiance.SampleLevel(tex_sampler, envMapEquirect(direction), lod).xyz;
    else 
        return float3(0.0f, 0.0f, 0.0f);
}

// The normal distribution function DD or the specular distribution statistically 
// approximates the ratio of microfacets aligned to some (halfway) vector hh.
float distributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;

    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nominator = a2;
    float denominator = NdotH2 * (a2 - 1.0) + 1.0;
    denominator = PI * denominator * denominator;

    return nominator / denominator;
}

// The Fresnel equation (pronounced as Freh-nel) describes the ratio of light that 
// gets reflected over the light that gets refracted, which varies over the angle we're looking at a surface.
// this function "really works" only for dielectric mateirals. (ie. non-metal)
float3 fresnelSchlick(float cosTheta, float3 Fo_baseReflectivity)
{
    float3 Fo = Fo_baseReflectivity;
    return Fo + (1.0 - Fo) * pow(1 - cosTheta, 5.0);
}

// The geometry function statistically approximates the ratio of microfacets 
// that overshadow each other causing light rays to lose their energy in the process.
float geometryFunctionSchlicGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;

    float nominator = NdotV;
    float denominator = NdotV * (1.0 - k) + k;
    return nominator / denominator;
}

float geometryFunctionSmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = geometryFunctionSchlicGGX(NdotV, roughness);
    float ggx2 = geometryFunctionSchlicGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + (max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float3 worldPositionFromUV(float2 uv)
{
    float depthSample = depth.Sample(depth_sampler, uv).x;
    float4 screen = float4(float2(uv.x, 1.0f - uv.y) * 2.0 - 1.0, depthSample, 1.0);
    screen = mul(cameraInverseProjectionMatrix, screen);
    float3 cameraSpace = screen.xyz / screen.w;
    return mul(cameraInverseViewMatrix, float4(cameraSpace, 0.0f)).xyz + cameraWorldSpacePosition.xyz;
}

float4 main(PSInput input) : SV_Target
{
    // calculate UV
    float2 uv = input.uv;

    // sample texture resources
    float4 albedoSample = albedo.Sample(tex_sampler, uv);
    float4 normalSample = normal.Sample(tex_sampler, uv);
    float  roughnessSample = roughness.Sample(tex_sampler, uv).x;
    float  metalnessSample = metalness.Sample(tex_sampler, uv).x;
    float  occlusionSample = occlusion.Sample(tex_sampler, uv).x;
    float  ssaoSample = ssao.Sample(tex_sampler, uv).x;
    //ssaoSample = 1.0f;

    // create normal from normal map sample
    float3 normal = normalSample.xyz;
    
    // Gamma correct textures
    float4 albedoColor = albedoSample;
    albedoColor.xyz = albedoColor.xyz * ssaoSample;

    // occlusion
    float occlusion = occlusionSample;

    float roughnessValue = roughnessSample;
    float metalnessValue = metalnessSample;

    float3 camPos = cameraWorldSpacePosition.xyz;

    // calculate world position
    float3 worldPosition = worldPositionFromUV(uv);

    float3 N = normalize(normal);
    float3 V = normalize(camPos - worldPosition); // maybe switch?

    float3 F0 = float3(0.04, 0.04, 0.04);
    F0 = lerp(F0, albedoColor.xyz, metalnessValue);

    float3 Lo = float3(0.0f, 0.0f, 0.0f);
    uint shadowCasterIndex = 0;
    for (uint i = 0; i < lightCount; ++i)
    {
        

        float distance = length(lightWorldPosition[i].xyz - worldPosition);
        float distanceSqrd = distance * distance;
        float attenuation = 1.0 / distanceSqrd;

        float rangeSqrd = lightRange[i] * lightRange[i];
        float overDistance = distanceSqrd - rangeSqrd;
        const float dimArea = lightRange[i] * 0.1;
        float inDim = 1.0f - saturate((dimArea - overDistance) / dimArea);
        attenuation *= 1.0f - inDim;

        if (lightType[i] == 0)
        {
            // directional light
            float3 L = normalize(lightDirection[i].xyz);
            float3 H = normalize(V + L);

            float3 radiance = (lightColor[i].xyz * lightIntensity[i]);

            float NDF = distributionGGX(N, H, roughnessValue);
            float G = geometryFunctionSmith(N, V, L, roughnessValue);
            float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

            float3 kS = F;
            float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
            kD *= 1.0f - metalnessValue;

            float3 nominator = NDF * G * F;
            float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
            float3 specular = nominator / denominator;

            float NdotL = max(dot(N, L), 0.0);
            float3 directContrib = (kD * albedoColor.xyz / PI + specular) * radiance * NdotL;
            Lo += directContrib;
        }
        else if (lightType[i] == 1)
        {
            // spot light
            float3 L = normalize(lightWorldPosition[i].xyz - worldPosition);
            float3 H = normalize(V + L);

            float3 radiance = (lightColor[i].xyz * lightIntensity[i]) * attenuation;

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

                float NDF = distributionGGX(N, H, roughnessValue);
                float G = geometryFunctionSmith(N, V, L, roughnessValue);
                float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

                float3 kS = F;
                float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
                kD *= 1.0f - metalnessValue;

                float3 nominator = NDF * G * F;
                float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
                float3 specular = nominator / denominator;

                float NdotL = max(dot(N, L), 0.0);
                float3 directContrib = (kD * albedoColor.xyz / PI + specular) * radiance * NdotL * spot;

                // shadow bias
                float hi = 0.00015;
                float lo = 0.00005;
                float bias = lerp(hi, lo, pow(NdotL, 0.2));

                // spot shadow
                if (shadowCasting)
                {
                    //directContrib *= 1.0 - ((shadowSample | (1 << i)) >> i);
                    float4 shadowPosition = mul(float4(worldPosition, 1.0), shadowVP[shadowCasterIndex]);
                    float2 shadowUV = (0.5 * (shadowPosition.xy / shadowPosition.w)) + float2(0.5, 0.5);
                    
                    shadowUV.y = 1.0f - shadowUV.y;
                    float2 satShadowUV = saturate(shadowUV);

                    if (satShadowUV.x == shadowUV.x && satShadowUV.y == shadowUV.y && (shadowPosition.z / shadowPosition.w > 0))
                    {
                        //PCF sampling for shadow map
                        float sum = 0;
                        float z = (shadowPosition.z / shadowPosition.w) + bias;
                        float x, y;

                        for (y = -1.5f; y <= 1.5f; y += 1.0f)
                        {
                            for (x = -1.5f; x <= 1.5f; x += 1.0f)
                            {
                                sum += shadowMap.SampleCmpLevelZero(
                                    shadow_sampler,
                                    float3(shadowUV + (float2(x, y)*shadowSize), shadowCasterIndex),
                                    z);
                            }
                        }
                        float shadowFactor = sum / 16.0f;
                        
                        directContrib *= 1.0f - shadowFactor;
                    }
                }

                Lo += directContrib;
            }
            if(shadowCasting)
                shadowCasterIndex = shadowCasterIndex + 1;
        }
        else if (lightType[i] == 2)
        {
            // point light
            float3 L = normalize(lightWorldPosition[i].xyz - worldPosition);
            float3 H = normalize(V + L);

            float3 radiance = (lightColor[i].xyz * lightIntensity[i]) * attenuation;

            float NDF = distributionGGX(N, H, roughnessValue);
            float G = geometryFunctionSmith(N, V, L, roughnessValue);
            float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

            float3 kS = F;
            float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
            kD *= 1.0f - metalnessValue;

            float3 nominator = NDF * G * F;
            float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
            float3 specular = nominator / denominator;

            float NdotL = max(dot(N, L), 0.0);
            float3 directContrib = (kD * albedoColor.xyz / PI + specular) * radiance * NdotL;

            // point shadows
            bool shadowCasting = lightParameters[i].z > 0.0f;
            if (shadowCasting)
            {
                // shadow bias
                float hi = 0.00015;
                float lo = 0.00005;
                float bias = lerp(hi, lo, pow(NdotL, 0.2));

                for (int caster = 0; caster < 6; ++caster)
                {
                    float4 shadowPosition = mul(float4(worldPosition, 1.0), shadowVP[shadowCasterIndex]);
                    float2 shadowUV = (0.5 * (shadowPosition.xy / shadowPosition.w)) + float2(0.5, 0.5);

                    shadowUV.y = 1.0f - shadowUV.y;
                    float2 satShadowUV = saturate(shadowUV);

                    if (satShadowUV.x == shadowUV.x && satShadowUV.y == shadowUV.y && (shadowPosition.z / shadowPosition.w > 0))
                    {
                        //return float4(1.0f, 0.0f, 0.0f, 1.0f);

                        //PCF sampling for shadow map
                        float sum = 0;
                        float z = (shadowPosition.z / shadowPosition.w) + bias;
                        float x, y;

                        for (y = -1.5f; y <= 1.5f; y += 1.0f)
                        {
                            for (x = -1.5f; x <= 1.5f; x += 1.0f)
                            {
                                sum += shadowMap.SampleCmpLevelZero(
                                    shadow_sampler,
                                    float3(shadowUV + (float2(x, y)*shadowSize), shadowCasterIndex),
                                    z);
                            }
                        }
                        float shadowFactor = sum / 16.0f;

                        directContrib *= 1.0f - shadowFactor;
                    }
                    shadowCasterIndex++;
                }
            }

            Lo += directContrib;
        }
    }

    // ambient specular
    float3 R = reflect(-V, N);
    const float MaxReflectionLod = 4.0;
    float3 prefilteredColor = environmentSpecular.SampleLevel(tex_sampler, R, roughnessValue * MaxReflectionLod).xyz;

    float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughnessValue);
    float2 envBRDF = environmentBrdfLut.Sample(tex_sampler, float2(min(max(dot(N, V), 0.0), 0.999), roughnessValue)).rg;
    float3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    // ambient diffuse
    float3 environIrradianceSample = sampleEnvironment(N, 0.0f);
    float3 kS = F;
    float3 kD = 1.0 - kS;
    kD *= 1.0f - metalnessValue;
    float3 irradiance = environIrradianceSample;
    float3 diffuse = irradiance * albedoColor.xyz;

    float3 ambient = max((kD * diffuse + specular) * occlusion * environmentStrength * ssaoSample, float3(0.0, 0.0, 0.0));

    float3 color = (ambient + Lo);

    color *= exposure;

    // TONEMAPPING
    //return float4(Uncharted2Tonemap(color), 1.0f);
    //return float4(JimHejlRichardBurgessDawsonTonemap(color), 1.0f);
    //return float4(ReinhardTonemapping(color), 1.0f);

    //return float4(normalSample.xyz, 1.0);
    //return float4(ssaoSample, ssaoSample, ssaoSample, 1.0);
    return float4(color, 1.0f);
    /*float2  motionSample = motion.Sample(point_sampler, uv);
    float4  historySample = history.Sample(point_sampler, uv + motionSample);
    

    return lerp(historySample, float4(color, 1.0f), 1.0 / 16.0);*/
}
