
#include "../shared_types/SubMeshAdjacency.hlsli"

Buffer<uint> subMeshBinding;
StructuredBuffer<SubMeshAdjacency> subMeshAdjacency;
RWBuffer<uint> indexedDrawArguments;

cbuffer OutlineConstants
{
    uint instanceId;
};

[numthreads(1, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint subMeshId = subMeshBinding[instanceId];
    SubMeshAdjacency subMeshAdj = subMeshAdjacency[subMeshId];

    /*UINT IndexCountPerInstance;
    UINT InstanceCount;
    UINT StartIndexLocation;
    INT BaseVertexLocation;
    UINT StartInstanceLocation;*/

    indexedDrawArguments[0] = subMeshAdj.adjacencyCount;
    indexedDrawArguments[1] = 1;
    indexedDrawArguments[2] = subMeshAdj.adjacencyPointer;
    indexedDrawArguments[3] = subMeshAdj.baseVertexPointer;
    indexedDrawArguments[4] = 0;
}
