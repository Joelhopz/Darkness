
#include "../shared_types/ClusterInstanceData.hlsli"
#include "../shared_types/ClusterData.hlsli"
#include "../shared_types/TransformHistory.hlsli"
#include "../shared_types/InstanceMaterial.hlsli"
#include "../GBuffer.hlsli"
#include "../VertexPacking.hlsli"

Buffer<uint2> vertices;
Buffer<float2> normals;
Buffer<float2> tangents;
Buffer<float2> uv;
Buffer<uint> indexes;

StructuredBuffer<ClusterInstanceData> clusters;
StructuredBuffer<ClusterData> clusterData;
StructuredBuffer<TransformHistory> transformHistory;
Buffer<uint> instanceUV;

cbuffer ConstData
{
    float4x4 viewProjectionMatrix;
    float4x4 previousViewProjectionMatrix;
    float4x4 jitterViewProjectionMatrix;
};

struct VSOutput
{
    float4 position         : SV_Position0;
    float4 mvPosCurrent     : POSITION0;
    float4 mvPosPrevious    : POSITION1;
    float4 normal           : NORMAL0;
    float4 tangent          : TEXCOORD0;
    uint instancePointer    : BLENDINDICES0;
    uint clusterPointer     : BLENDINDICES1;

};

VSOutput main(uint id : SV_VertexID)
{
    uint clusterId = id / 192;
    uint indexId = id - (clusterId * 192);
    ClusterInstanceData cInstanceData = clusters[clusterId];
    ClusterData cdata = clusterData[cInstanceData.clusterPointer];
    uint instUV = instanceUV[cInstanceData.instancePointer];

    if (indexId < cdata.indexCount)
    {
        uint index = indexes[cdata.indexPointer + indexId];
        TransformHistory transforms = transformHistory[cInstanceData.instancePointer];

        float4x4 normalTransform = transpose(transforms.inverseTransform);
        uint vertexDataIndex = cdata.vertexPointer + index;

        float3 vertex = unpackVertex(vertices[vertexDataIndex]);

        VSOutput output;
        
		output.position = mul(mul(jitterViewProjectionMatrix, transforms.transform), float4(vertex, 1));
        output.mvPosCurrent = mul(mul(viewProjectionMatrix, transforms.transform), float4(vertex, 1));
        output.mvPosPrevious = mul(mul(previousViewProjectionMatrix, transforms.previousTransform), float4(vertex, 1));
        output.normal = normalize(mul(normalTransform, float4(unpackNormalOctahedron(normals[vertexDataIndex]), 0.0f)));
        output.tangent = normalize(mul(normalTransform, float4(unpackNormalOctahedron(tangents[vertexDataIndex]), 0.0f)));
        float2 uu = uv[instUV + index];
        output.normal.w = uu.x;
        output.tangent.w = uu.y;
        output.instancePointer = cInstanceData.instancePointer;
        output.clusterPointer = cInstanceData.clusterPointer;
        return output;
        
    }
    
    VSOutput output;
    output.position = float4(0.0f, 0.0f, 0.0f, 0.0f);
    output.mvPosCurrent = float4(0.0f, 0.0f, 0.0f, 0.0f);
    output.mvPosPrevious = float4(0.0f, 0.0f, 0.0f, 0.0f);
    output.normal = float4(0.0f, 0.0f, 0.0f, 0.0f);
    output.tangent = float4(0.0f, 0.0f, 0.0f, 0.0f);
    output.instancePointer = 0;
    output.clusterPointer = 0;
    return output;
}
