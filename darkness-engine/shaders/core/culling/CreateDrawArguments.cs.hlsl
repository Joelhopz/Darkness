
struct OcclusionDrawArgs
{
    uint IndexCountPerInstance;
    uint InstanceCount;
    uint StartIndexLocation;
    int BaseVertexLocation;
    uint StartInstanceLocation;
};

Buffer<uint> structureCounter;
RWStructuredBuffer<OcclusionDrawArgs> occlusionDrawArgs;

[numthreads(1, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint threadCount = structureCounter[0];
    uint threadGroupCount = threadCount / 64;
    if (threadGroupCount * 64 < threadCount)
        ++threadGroupCount;

    occlusionDrawArgs[0].IndexCountPerInstance = threadGroupCount;
    occlusionDrawArgs[0].InstanceCount = 1;
    occlusionDrawArgs[0].StartIndexLocation = 0;
    occlusionDrawArgs[0].BaseVertexLocation = 0;
    occlusionDrawArgs[0].StartInstanceLocation = 0;
}
