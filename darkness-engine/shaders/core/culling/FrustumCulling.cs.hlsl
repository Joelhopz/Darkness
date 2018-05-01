
#include "../shared_types/BoundingBox.hlsli"
#include "../shared_types/SubMeshData.hlsli"
#include "../shared_types/TransformHistory.hlsli"
#include "../shared_types/ClusterInstanceData.hlsli"
#include "../shared_types/FrustumCullingOutput.hlsli"

StructuredBuffer<BoundingBox> subMeshBoundingBoxes;
Buffer<uint> instanceSubMeshBinding;
StructuredBuffer<SubMeshData> subMeshData;
StructuredBuffer<TransformHistory> transformHistory;
AppendStructuredBuffer<FrustumCullingOutput> cullingOutput;
RWBuffer<uint> clusterCountBuffer;
RWBuffer<uint> outputAllocationShared;

cbuffer CullingConstants
{
    // 16 floats
    float4 plane0;
    float4 plane1;
    float4 plane2;
    float4 plane3;

    // 12 floats
    float4 plane4;
    float4 plane5;
    float3 cameraPosition;
    uint instanceCount;
};

bool frustumCull(float3 bbmin, float3 bbmax, float4x4 transform, float3 cameraPos)
{
    float3 corner[8] =
    {
        mul(transform, float4(bbmin.xyz, 1.0f)).xyz,
        mul(transform, float4(bbmin.x, bbmax.y, bbmax.z, 1.0f)).xyz,
        mul(transform, float4(bbmin.x, bbmin.y, bbmax.z, 1.0f)).xyz,
        mul(transform, float4(bbmin.x, bbmax.y, bbmin.z, 1.0f)).xyz,
        mul(transform, float4(bbmax, 1.0f)).xyz,
        mul(transform, float4(bbmax.x, bbmin.y, bbmin.z, 1.0f)).xyz,
        mul(transform, float4(bbmax.x, bbmax.y, bbmin.z, 1.0f)).xyz,
        mul(transform, float4(bbmax.x, bbmin.y, bbmax.z, 1.0f)).xyz
    };

    float3 frustumPlanes[6] =
    {
        plane0.xyz,
        plane1.xyz,
        plane2.xyz,
        plane3.xyz,
        plane4.xyz,
        plane5.xyz
    };

    for (int i = 0; i < 6; ++i)
    {
        bool hit = false;
        for (int c = 0; c < 8; ++c)
        {
            if (dot(normalize(frustumPlanes[i]), (corner[c] - cameraPos)) >= 0)
            {
                hit = true;
                break;
            }
        }
        if (!hit)
            return false;
    }
    return true;
};

[numthreads(64, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint instanceId = dispatchThreadID.x;
    if (instanceId < instanceCount)
    {
        uint subMeshBind = instanceSubMeshBinding[instanceId];
        TransformHistory transforms = transformHistory[instanceId];
        BoundingBox bb = subMeshBoundingBoxes[subMeshBind];

        float3 bbmin = bb.min;
        float3 bbmax = bb.max;

        if (frustumCull(bbmin, bbmax, transforms.transform, cameraPosition))
        {
            SubMeshData sMeshData = subMeshData[subMeshBind];
            uint newLocation;
            InterlockedAdd(outputAllocationShared[0], sMeshData.clusterCount, newLocation);

            FrustumCullingOutput output;
            output.clusterPointer = sMeshData.clusterPointer;
            output.clusterCount = sMeshData.clusterCount;
            output.instancePointer = instanceId;
            output.outputPointer = newLocation;
            cullingOutput.Append(output);

            InterlockedAdd(clusterCountBuffer[0], sMeshData.clusterCount);
            InterlockedAdd(clusterCountBuffer[1], 1);
        }
    }
}
