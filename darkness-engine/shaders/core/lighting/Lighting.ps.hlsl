
#include "../Common.hlsli"
#include "../shared_types/InstanceMaterial.hlsli"

struct PSInput
{
    float4 position         : SV_Position0;
    float2 uv               : TEXCOORD0;
};

TextureCube<float4> environmentIrradianceCubemap;
Texture2D<float4> environmentIrradiance;
TextureCube<float4> environmentSpecular;
Texture2D<float4> environmentBrdfLut;

Texture2D<float2> gbufferNormals;
Texture2D<uint2> gbufferUV;
Texture2D<uint> gbufferInstanceId;

StructuredBuffer<InstanceMaterial> instanceMaterials;

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

Texture2D<float> depth;

Texture2D<float4> materialTextures[];

cbuffer ConstData
{
    // 16 floats
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

    // 16 floats
    uint hasEnvironmentIrradianceEquirect;
    uint hasEnvironmentSpecular;
    float environmentStrength;
    float padding;

    float4 probePositionRange;

    float4 probeBBmin;

    float4 probeBBmax;

    // 16 floats
    float4x4 cameraInverseProjectionMatrix;

    // 16 floats
    float4x4 cameraInverseViewMatrix;

    // the rest
    float2 shadowSize;
    uint2 frameSize;
    uint usingProbe;
};

sampler tex_sampler;
sampler tri_sampler;
sampler depth_sampler;
sampler point_sampler;
SamplerComparisonState shadow_sampler;

float3 sampleEnvironment(float3 direction, float lod)
{
    float3 inverseDirection = float3(direction.x, direction.y, -direction.z);
    if (hasEnvironmentIrradianceCubemap)
        return environmentIrradianceCubemap.SampleLevel(tri_sampler, inverseDirection, lod).xyz;
    else if(hasEnvironmentIrradianceEquirect)
        return environmentIrradiance.SampleLevel(tri_sampler, envMapEquirect(direction), lod).xyz;
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

float GGX_D(float dotNH, float roughness)
{
    float alpha = roughness*roughness;
    float alphaSqr = alpha*alpha;
    float pi = 3.14159f;
    float denom = dotNH * dotNH *(alphaSqr - 1.0) + 1.0f;

    float D = alphaSqr / (pi * denom * denom);
    return D;
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

float2 GGX_FV(float dotLH, float roughness)
{
    float alpha = roughness*roughness;

    // F
    float F_a, F_b;
    float dotLH5 = pow(1.0f - dotLH, 5);
    F_a = 1.0f;
    F_b = dotLH5;

    // V
    float vis;
    float k = alpha / 2.0f;
    float k2 = k*k;
    float invK2 = 1.0f - k2;
    vis = rcp(dotLH*dotLH*invK2 + k2);

    return float2(F_a*vis, F_b*vis);
}

float GGX_OPT3(float3 N, float3 V, float3 L, float roughness, float F0)
{
    float3 H = normalize(V + L);

    float dotNL = saturate(dot(N, L));
    float dotLH = saturate(dot(L, H));
    float dotNH = saturate(dot(N, H));

    float D = GGX_D(dotNH, roughness);
    float2 FV_helper = GGX_FV(dotLH, roughness);
    float FV = F0*FV_helper.x + (1.0f - F0)*FV_helper.y;
    float specular = dotNL * D * FV;

    return specular;
}

float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + (max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float3 worldPositionFromUV(float2 uv, float depthSample)
{
    
    float4 screen = float4(float2(uv.x, 1.0f - uv.y) * 2.0 - 1.0, depthSample, 1.0);
    screen = mul(cameraInverseProjectionMatrix, screen);
    float3 cameraSpace = screen.xyz / screen.w;
    return mul(cameraInverseViewMatrix, float4(cameraSpace, 0.0f)).xyz + cameraWorldSpacePosition.xyz;
}


#ifdef ENUM_DRAWMODE_FULL
#endif
#ifdef ENUM_DRAWMODE_DEBUG_CLUSTERS
float rand_1_05(in float2 uv)
{
    float2 noise = (frac(sin(dot(uv, float2(12.9898, 78.233)*2.0)) * 43758.5453));
    return abs(noise.x + noise.y) * 0.5;
}
#endif

#include "../GBuffer.hlsli"

float4 calculateDdxDdyFromUV(float2 gbufferUVSample, uint2 uintUV, float2 materialScale, uint instanceId)
{
    uint idUp = gbufferInstanceId[uintUV + uint2(0, 1)];
    uint idDown = gbufferInstanceId[uintUV + uint2(0, -1)];
    uint idLeft = gbufferInstanceId[uintUV + uint2(-1, 0)];
    uint idRight = gbufferInstanceId[uintUV + uint2(1, 0)];

    float2 dxUp = unpackUV(gbufferUV[uintUV + uint2(0, 1)]);
    float2 dxDown = unpackUV(gbufferUV[uintUV + uint2(0, -1)]);
    float2 dxLeft = unpackUV(gbufferUV[uintUV + uint2(-1, 0)]);
    float2 dxRight = unpackUV(gbufferUV[uintUV + uint2(1, 0)]);

    const float2 gradientThreshold = 0.03f;
    bool up = any(abs(gbufferUVSample - dxUp) <= gradientThreshold) && (idUp == instanceId);
    bool down = any(abs(gbufferUVSample - dxDown) <= gradientThreshold) && (idDown == instanceId);
    bool left = any(abs(gbufferUVSample - dxLeft) <= gradientThreshold) && (idLeft == instanceId);
    bool right = any(abs(gbufferUVSample - dxRight) <= gradientThreshold) && (idRight == instanceId);

    float2 uvDX = 0.0f;
    float2 uvDY = 0.0f;

    if (up)
        uvDY = gbufferUVSample - dxUp * materialScale;
    else if (down)
        uvDY = dxDown * materialScale - gbufferUVSample;

    if (left)
        uvDX = gbufferUVSample - dxLeft * materialScale;
    else if (right)
        uvDX = dxRight * materialScale - gbufferUVSample;

    if (uvDX.x > 1.0f)
        uvDX.x -= 2.0f;
    else if (uvDX.x < -1.0f)
        uvDX.x += 2.0f;
    if (uvDX.y > 1.0f)
        uvDX.y -= 2.0f;
    else if (uvDX.y < -1.0f)
        uvDX.y += 2.0f;

    if (uvDY.x > 1.0f)
        uvDY.x -= 2.0f;
    else if (uvDY.x < -1.0f)
        uvDY.x += 2.0f;
    if (uvDY.y > 1.0f)
        uvDY.y -= 2.0f;
    else if (uvDY.y < -1.0f)
        uvDY.y += 2.0f;

    return float4(uvDX, uvDY);
}

float mipFromDdxDdy(float4 ddxDdy)
{
    float deltaMaxSqr = max(dot(ddxDdy.xy, ddxDdy.xy), dot(ddxDdy.zw, ddxDdy.zw));
    float mml = 0.5 * log2(deltaMaxSqr);
    return max(0, mml);
}

float4 main(PSInput input) : SV_Target
{
    // calculate UV
    float2 uv = input.uv;

    float depthSample = depth.Sample(depth_sampler, uv);
    clip(depthSample - 0.0001);

    float2 floatUV = input.position.xy;
    uint2 uintUV = uint2((uint)floatUV.x, (uint)floatUV.y);

    uint instanceId = gbufferInstanceId[uintUV];

#ifdef ENUM_DRAWMODE_DEBUG_CLUSTERS
    bool hasAlbedo = false;
    bool hasMetalness = false;
    bool hasRoughness = false;
    bool hasNormal = false;
    bool hasOcclusion = false;
    float2 materialScale = float2(1.0f, 1.0f);
#else
    InstanceMaterial material = instanceMaterials[instanceId];
    float2 materialScale = float2(material.scaleX, material.scaleY);
    bool hasAlbedo = (material.materialSet & 0x1) == 0x1;
    bool hasMetalness = (material.materialSet & 0x2) == 0x2;
    bool hasRoughness = (material.materialSet & 0x4) == 0x4;
    bool hasNormal = (material.materialSet & 0x8) == 0x8;
    bool hasOcclusion = (material.materialSet & 0x10) == 0x10;
    float4 albedoSample = float4(material.color, 1.0f);
#endif

    float2 gbufferUVSample = unpackUV(gbufferUV[uintUV]) * materialScale;

    float  roughnessSample = 0;
    float  metalnessSample = 0;
    float  occlusionSample = 0;

#ifdef ENUM_DRAWMODE_DEBUG_CLUSTERS
    float4 albedoSample = float4(
        rand_1_05(float2(instanceId, instanceId)),
        rand_1_05(float2(instanceId + 1, instanceId)),
        rand_1_05(float2(instanceId + 2, instanceId)),
        1.0f);
#endif

    float2 normalSample = gbufferNormals.Sample(point_sampler, uv);

#ifndef ENUM_DRAWMODE_DEBUG_CLUSTERS

    float4 ddxDdy = calculateDdxDdyFromUV(gbufferUVSample, uintUV, materialScale, instanceId);
    // log2(0.7) comes from ubi presentation about mip bias for temporal antialiasing
    // log2(0.7) == -0.51457317283

    uint width;
    uint height;
#ifdef ENUM_DRAWMODE_MIP_ALBEDO
    float albedoMip = 0.0f;
    float albedoMaxMip = 0.0f;
#endif
#ifdef ENUM_DRAWMODE_MIP_ROUGHNESS
    float roughnessMip = 0.0f;
    float roughnessMaxMip = 0.0f;
#endif
#ifdef ENUM_DRAWMODE_MIP_METALNESS
    float metalnessMip = 0.0f;
    float metalnessMaxMip = 0.0f;
#endif
#ifdef ENUM_DRAWMODE_MIP_AO
    float aoMip = 0.0f;
    float aoMaxMip = 0.0f;
#endif
    if (hasAlbedo)
    {
        materialTextures[material.albedo].GetDimensions(width, height);
        float texSize = (float)max(width, height);
        float mip = max(mipFromDdxDdy(ddxDdy * texSize) + log2(0.70), 0);
#ifdef ENUM_DRAWMODE_MIP_ALBEDO
        albedoMip = mip;
        albedoMaxMip = 1.0f + floor(log2(max(width, height)));
#endif
        albedoSample *= materialTextures[material.albedo].SampleLevel(tex_sampler, gbufferUVSample, mip);
    }
    if (hasRoughness)
    {
        materialTextures[material.roughness].GetDimensions(width, height);
        float texSize = (float)max(width, height);
        float mip = max(mipFromDdxDdy(ddxDdy * texSize) + log2(0.70), 0);
#ifdef ENUM_DRAWMODE_MIP_ROUGHNESS
        roughnessMip = mip;
        roughnessMaxMip = 1.0f + floor(log2(max(width, height)));
#endif
        roughnessSample = materialTextures[material.roughness].SampleLevel(tex_sampler, gbufferUVSample, mip).r;
    }
    if (hasMetalness)
    {
        materialTextures[material.metalness].GetDimensions(width, height);
        float texSize = (float)max(width, height);
        float mip = max(mipFromDdxDdy(ddxDdy * texSize) + log2(0.70), 0);
#ifdef ENUM_DRAWMODE_MIP_METALNESS
        metalnessMip = mip;
        metalnessMaxMip = 1.0f + floor(log2(max(width, height)));
#endif
        metalnessSample = materialTextures[material.metalness].SampleLevel(tex_sampler, gbufferUVSample, mip).r;
    }
    if (hasOcclusion)
    {
        materialTextures[material.ao].GetDimensions(width, height);
        float texSize = (float)max(width, height);
        float mip = max(mipFromDdxDdy(ddxDdy * texSize) + log2(0.70), 0);
#ifdef ENUM_DRAWMODE_MIP_AO
        aoMip = mip;
        aoMaxMip = 1.0f + floor(log2(max(width, height)));
#endif
        occlusionSample = materialTextures[material.ao].SampleLevel(tex_sampler, gbufferUVSample, mip).r;
    }
#endif

    float  ssaoSample = ssao.Sample(tex_sampler, uv).x;
    //ssaoSample = 0.0f;

    // create normal from normal map sample
    float3 normal = unpackNormalOctahedron(normalSample);
    
    // Gamma correct textures
    float4 albedoColor = albedoSample;

    // occlusion
    float occlusion = 1.0f - ssaoSample;

#ifdef ENUM_DRAWMODE_DEBUG_CLUSTERS
    float roughnessValue = 1.0f;
    float metalnessValue = 0.0f;
#else
    if (hasOcclusion)
        occlusion = 1.0f - ((1.0 - (occlusionSample - ssaoSample)) * material.occlusionStrength);

    float roughnessValue = material.roughnessStrength;
    float metalnessValue = material.metalnessStrength;

    if (hasRoughness)
        roughnessValue = roughnessSample * material.roughnessStrength;
    if (hasMetalness)
        metalnessValue = metalnessSample * material.metalnessStrength;
#endif

    roughnessValue = min(max(roughnessValue, 0.0f), 1.0);
    metalnessValue = min(max(metalnessValue, 0.0f), 1.0);

    float3 camPos = cameraWorldSpacePosition.xyz;

    // calculate world position
    float2 inverseSize = float2(1.0f / (float)frameSize.x, 1.0f / (float)frameSize.y);
    float3 inputpos = float3(
        (input.position.x * inverseSize.x) * 2.0f - 1.0f,
        ((input.position.y * inverseSize.y) * 2.0f - 1.0f) * -1.0f,
        depthSample);

    float4 ci = mul(cameraInverseProjectionMatrix, float4(inputpos.xyz, 1.0f));
    ci.xyz /= ci.w;
    float3 worldPosition = mul(cameraInverseViewMatrix, float4(ci.xyz, 0.0f)).xyz + camPos.xyz;


    float3 N = normal;
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
        float distanceSqrdS = distanceSqrd + 10.0;

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
                float hi = 0.00005;
                float lo = 0.000005;
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
            //float NDF = GGX_D(dot(N, H), roughnessValue);
            float G = geometryFunctionSmith(N, V, L, roughnessValue);
            //float G = GGX_OPT3(N, V, L, roughnessValue, F0);
            float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

            float3 kS = F;
            float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
            kD *= 1.0f - metalnessValue;

            float3 nominator = NDF * G * F;
            float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
            float3 specular = nominator / denominator;

            float NdotL = max(dot(N, L), 0.0);
            float3 directContrib = (kD * albedoColor.xyz / PI + specular) * radiance * NdotL;
            //float3 directContrib = albedoColor.xyz* NdotL;

            // point shadows
            bool shadowCasting = lightParameters[i].z > 0.0f;
            if (shadowCasting)
            {
                // shadow bias
                float hi = 0.00030;// * distanceSqrdS;
                float lo = 0.00025;// * distanceSqrdS;
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
    const float MaxReflectionLod = 4;
    float3 from = worldPosition;
    float3 probePosition = probePositionRange.xyz;
    float probeRange = probePositionRange.w;

    // cubemap parallax correction
    float3 Rp = reflect(-V, N);
    float3 newRp;
    float3 probeValue;
    if (usingProbe)
    {
        newRp = boxLineIntersect(probeBBmin.xyz, probeBBmax.xyz, worldPosition, -Rp, probePosition);
        probeValue = float3(-newRp.x, -newRp.y, -newRp.z);
    }
    else
    {
        newRp = boxLineIntersect(probeBBmin.xyz, probeBBmax.xyz, worldPosition, Rp, probePosition);
        probeValue = float3(newRp.x, newRp.y, -newRp.z);
    }
    

    float3 prefilteredColor = environmentSpecular.SampleLevel(tri_sampler, probeValue, roughnessValue * MaxReflectionLod).xyz;

    float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughnessValue) * 0.7;
    float2 envBRDF = environmentBrdfLut.Sample(point_sampler, float2(min(max(dot(N, V), 0.0), 0.99999), roughnessValue)).rg;
    float3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);
    //specular = float3(0.0f, 0.0f, 0.0f);

    // ambient diffuse
    //float3 NN = boxLineIntersect(probeBBmin.xyz, probeBBmax.xyz, worldPosition, N, probePosition);
    //NN = float3(NN.x, NN.y, NN.z);

    float3 environIrradianceSample = sampleEnvironment(N, 0.0f);
    float3 kS = F;
    float3 kD = 1.0 - kS;
    kD *= 1.0f - metalnessValue;
    float3 irradiance = environIrradianceSample;
    float3 diffuse = irradiance * albedoColor.xyz;

    float3 ambient = max((kD * diffuse + specular) * occlusion * environmentStrength, float3(0.0, 0.0, 0.0));
    //float3 ambient = diffuse;

    float3 color = (ambient + Lo);

    color *= exposure;

#ifndef ENUM_DRAWMODE_DEBUG_CLUSTERS
#ifdef ENUM_DRAWMODE_MIP_ALBEDO
    return float4(albedoMip / albedoMaxMip, 0.0f, 0.0f, 1.0f);
#endif
#ifdef ENUM_DRAWMODE_MIP_ROUGHNESS
    return float4(roughnessMip / roughnessMaxMip, 0.0f, 0.0f, 1.0f);
#endif
#ifdef ENUM_DRAWMODE_MIP_METALNESS
    return float4(metalnessMip / metalnessMaxMip, 0.0f, 0.0f, 1.0f);
#endif
#ifdef ENUM_DRAWMODE_MIP_AO
    return float4(aoMip / aoMaxMip, 0.0f, 0.0f, 1.0f);
#endif
#endif

    /*color *= 0.00001f;
    color += float3(albedoMip / 5, 0, 0.0f);*/
    // TONEMAPPING
    //return float4(Uncharted2Tonemap(color), 1.0f);
    //return float4(JimHejlRichardBurgessDawsonTonemap(color), 1.0f);
    //return float4(ReinhardTonemapping(color), 1.0f);

    //return float4(normalSample.xyz, 1.0);
    //return float4(ssaoSample, ssaoSample, ssaoSample, 1.0);
    //return float4(abs(N.xyz), 1.0f);
#ifdef ENUM_DRAWMODE_DEBUG_CLUSTERS
    return float4(color.xyz, 1.0f);
#endif

#ifdef ENUM_DRAWMODE_FULL
    return float4(color.xyz, 1.0f);
#endif

#ifdef ENUM_DRAWMODE_ALBEDO
    return float4(albedoSample.xyz, 1.0f);
#endif

#ifdef ENUM_DRAWMODE_ROUGHNESS
    return float4(roughnessSample, roughnessSample, roughnessSample, 1.0f);
#endif

#ifdef ENUM_DRAWMODE_METALNESS
    return float4(metalnessSample, metalnessSample, metalnessSample, 1.0f);
#endif

#ifdef ENUM_DRAWMODE_OCCLUSION
    return float4(occlusion, occlusion, occlusion, 1.0f);
#endif

#ifdef ENUM_DRAWMODE_UV
    return float4(gbufferUVSample.xy, 0.0f, 1.0f);
#endif

#ifdef ENUM_DRAWMODE_DEBUG_NORMAL
    return float4(normal.xyz, 1.0f);
#endif

    return float4(0.0f, 0.0f, 0.0f, 0.0f);

    /*float2  motionSample = motion.Sample(point_sampler, uv);
    float4  historySample = history.Sample(point_sampler, uv + motionSample);
    

    return lerp(historySample, float4(color, 1.0f), 1.0 / 16.0);*/
}
