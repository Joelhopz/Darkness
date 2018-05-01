
#include "../shared_types/ClusterInstanceData.hlsli"
#include "../shared_types/ClusterData.hlsli"
#include "../shared_types/TransformHistory.hlsli"
#include "../shared_types/InstanceMaterial.hlsli"
#include "../GBuffer.hlsli"
#include "../VertexPacking.hlsli"

Buffer<uint2> vertices;
Buffer<uint> indexes;

StructuredBuffer<ClusterInstanceData> clusters;
StructuredBuffer<ClusterData> clusterData;
StructuredBuffer<TransformHistory> transformHistory;

cbuffer ConstData
{
    float4x4 viewProjectionMatrix;
    float4x4 previousViewProjectionMatrix;
    float4x4 jitterViewProjectionMatrix;
};

struct VSOutput
{
    float4 position         : SV_Position0;
};

VSOutput main(uint id : SV_VertexID)
{
    uint clusterId = id / 192;
    uint indexId = id - (clusterId * 192);
    ClusterInstanceData cInstanceData = clusters[clusterId];
    ClusterData cdata = clusterData[cInstanceData.clusterPointer];
    
    if (indexId < cdata.indexCount)
    {
        uint index = indexes[cdata.indexPointer + indexId];
        TransformHistory transforms = transformHistory[cInstanceData.instancePointer];
    
        uint vertexDataIndex = cdata.vertexPointer + index;
        float3 vertex = unpackVertex(vertices[vertexDataIndex]);
    
        VSOutput output;
        output.position = mul(mul(viewProjectionMatrix, transforms.transform), float4(vertex, 1));
        return output;
    }
    
    VSOutput output;
    output.position = float4(0.0f, 0.0f, 0.0f, 0.0f);
    return output;
}
