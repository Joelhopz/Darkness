
struct OcclusionDispatchArgs
{
    uint threadGroupX;
    uint threadGroupY;
    uint threadGroupZ;
};

Buffer<uint> clusterCountBuffer;
RWStructuredBuffer<OcclusionDispatchArgs> gatherDispatchArgs;
RWStructuredBuffer<OcclusionDispatchArgs> occlusionDispatchArgs;

[numthreads(1, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint clusterCount = clusterCountBuffer[0];
    uint threadGroupCount = clusterCount / 64;
    if (threadGroupCount * 64 < clusterCount)
        ++threadGroupCount;

    occlusionDispatchArgs[0].threadGroupX = threadGroupCount;
    occlusionDispatchArgs[0].threadGroupY = 1;
    occlusionDispatchArgs[0].threadGroupZ = 1;

    uint instanceCount = clusterCountBuffer[1] * 64;
    uint iThreadGroupCount = instanceCount / 64;
    if (iThreadGroupCount * 64 < instanceCount)
        ++iThreadGroupCount;

    gatherDispatchArgs[0].threadGroupX = iThreadGroupCount;
    gatherDispatchArgs[0].threadGroupY = 1;
    gatherDispatchArgs[0].threadGroupZ = 1;

}
