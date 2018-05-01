
#include "../Common.hlsli"
#include "../shared_types/ClusterInstanceData.hlsli"
#include "../shared_types/ClusterData.hlsli"
#include "../shared_types/InstanceMaterial.hlsli"

struct PSInput
{
    float4 position         : SV_Position0;
    float4 mvPosCurrent     : POSITION0;
    float4 mvPosPrevious    : POSITION1;
    float4 normal           : NORMAL0;
    float4 tangent          : TEXCOORD0;
    uint instancePointer    : BLENDINDICES0;
    uint clusterPointer     : BLENDINDICES1;
};

struct PSOutput
{
    float2 normal;
    uint2 uv;
    float2 motion;
    uint instanceId;
};

StructuredBuffer<InstanceMaterial> instanceMaterials;
Texture2D<float4> materialTextures[];

sampler tex_sampler;

float nrand(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453);
};

#include "../GBuffer.hlsli"

[earlydepthstencil]
PSOutput main(PSInput input) : SV_Target
{
    InstanceMaterial material = instanceMaterials[input.instancePointer];
    float2 materialScale = float2(material.scaleX, material.scaleY);

    PSOutput output;

    // create tangent frame
    float3x3 TBN = float3x3(
        normalize(input.tangent.xyz),
        normalize(-cross(input.tangent.xyz, input.normal.xyz)), 
        normalize(input.normal.xyz));
    TBN = transpose(TBN);

    float2 inuv = float2(input.normal.w, input.tangent.w);

    // sample texture resources
    bool hasNormal = (material.materialSet & 0x8) == 0x8;
    float4 normalSample = float4(0.0f, 0.0f, 0.0f, 0.0f);
    if (hasNormal)
        normalSample = materialTextures[material.normal].Sample(tex_sampler, inuv * materialScale);

    // create normal from normal map sample
    float3 normal = input.normal.xyz;
    if (hasNormal)
        normal = createNormal(TBN, normalSample.xyz);

    output.normal = packNormalOctahedron(normalize(normal));
    output.uv = packUV(inuv);
    //output.motion = input.mvPosCurrentPrevious.xy - input.mvPosCurrentPrevious.zw;

    float2 cur = (input.mvPosCurrent.xy / input.mvPosCurrent.w) * 0.5 + 0.5;
    float2 pre = (input.mvPosPrevious.xy / input.mvPosPrevious.w) * 0.5 + 0.5;
    cur.y = 1.0f - cur.y;
    pre.y = 1.0f - pre.y;
    output.motion = cur - pre;

#ifdef OPTION_DEBUG_CLUSTERS
    output.instanceId = input.clusterPointer;
#else
    output.instanceId = input.instancePointer;
#endif

    return output;
}
