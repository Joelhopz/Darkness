
#include "../Common.hlsli"

struct PSInput
{
    float4 position         : SV_Position0;
    float4 mvPosCurrent     : POSITION0;
    float4 mvPosPrevious    : POSITION1;
    float4 normal           : NORMAL0;
    float4 tangent          : TEXCOORD0;
    float2 uv               : TEXCOORD1;
};

Texture2D<float4> materialTextures[];
/*Texture2D<float4> albedo;
Texture2D<float4> normal;
Texture2D<float4> roughness;
Texture2D<float4> metalness;
Texture2D<float4> occlusion;*/

RWBuffer<uint> pick;

cbuffer ConstData
{
    float roughnessStrength;
    float metalnessStrength;
    float occlusionStrength;
    uint hasAlbedo;

    uint hasNormal;
    uint hasRoughness;
    uint hasMetalness;
    uint hasOcclusion;

    float4 color;
    
    float2 materialScale;
    uint objectId;
    uint mouseX;

    uint mouseY;
    float2 jitter;
    uint albedoId;

    uint normalId;
    uint roughnessId;
    uint metalnessId;
    uint occlusionId;
};

sampler tex_sampler;

struct PsOutput
{
    float4 albedo;
    float4 normal;
    float2 motion;
    float roughness;
    float metalness;
    float occlusion;
};

[earlydepthstencil]
PsOutput main(PSInput input) : SV_Target
{
    uint2 screenPos = input.position.xy;
    if (screenPos.x == mouseX && screenPos.y == mouseY)
        pick[0] = objectId;

    // calculate UV
    float2 uv = float2(input.uv * materialScale);

    float4 albedoSample = color;
    if (hasAlbedo)
        albedoSample = materialTextures[albedoId].Sample(tex_sampler, uv);
        //albedoSample = albedo.Sample(tex_sampler, uv);

    if (albedoSample.w - 0.8 < 0.0)
        albedoSample.xyz = materialTextures[albedoId].SampleLevel(tex_sampler, float2(0.55, 0.55), 0).xyz;
        //albedoSample.xyz = albedo.SampleLevel(tex_sampler, float2(0.55, 0.55), 0).xyz;

    // create tangent frame
    float3x3 TBN = float3x3(normalize(input.tangent.xyz), normalize(-cross(input.tangent.xyz, input.normal.xyz)), normalize(input.normal.xyz));
    TBN = transpose(TBN);

    // sample texture resources
    
    float4 normalSample = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float  roughnessSample = 0;
    float  metalnessSample = 0;
    float  occlusionSample = 0;

    if(hasNormal)
        normalSample = materialTextures[normalId].Sample(tex_sampler, uv);
        //normalSample = normal.Sample(tex_sampler, uv);
    if(hasRoughness)
        roughnessSample = materialTextures[roughnessId].Sample(tex_sampler, uv).r;
        //roughnessSample = roughness.Sample(tex_sampler, uv).r;
    if(hasMetalness)
        metalnessSample = materialTextures[metalnessId].Sample(tex_sampler, uv).r;
        //metalnessSample = metalness.Sample(tex_sampler, uv).r;
    if(hasOcclusion)
        occlusionSample = materialTextures[occlusionId].Sample(tex_sampler, uv).r;
        //occlusionSample = occlusion.Sample(tex_sampler, uv).r;

    // create normal from normal map sample
    float3 normal = input.normal.xyz;
    if(hasNormal)
        normal = createNormal(TBN, normalSample);
    
    // occlusion
    float occlusion = 1.0f;
    if(hasOcclusion)
        occlusion = 1.0f - ((1.0 - occlusionSample) * occlusionStrength);

    float roughnessValue = roughnessStrength;
    float metalnessValue = metalnessStrength;

    if(hasRoughness)
        roughnessValue = roughnessSample * roughnessStrength;
    if(hasMetalness)
        metalnessValue = metalnessSample * metalnessStrength;

    roughnessValue = min(max(roughnessValue, 0.05f), 0.999);
    metalnessValue = min(max(metalnessValue, 0.0f), 1.0);

    PsOutput output;
    output.albedo = albedoSample;
    output.normal = float4(normalize(normal), 1.0f);

    float2 currentPos = ((input.mvPosCurrent.xy / input.mvPosCurrent.w) * 0.5 + 0.5);
    currentPos = float2(currentPos.x, 1.0f - currentPos.y);
    float2 previousPos = ((input.mvPosPrevious.xy / input.mvPosPrevious.w) * 0.5 + 0.5);
    previousPos = float2(previousPos.x, 1.0f - previousPos.y);

    output.motion = currentPos - previousPos;
    output.roughness = roughnessValue;
    output.metalness = metalnessValue;
    output.occlusion = occlusion;
    return output;
}
