
Texture2D<float4> textures[];

struct BoundingBox
{
    float3 min;
    float3 max;
};

Buffer<uint> clusterId;
Buffer<uint> clusterIndexCount;
StructuredBuffer<BoundingBox> clusterBounds;
AppendStructuredBuffer<uint> clusterIndexCountDraw;

cbuffer CullingConstants
{
    uint clusterCount;
};

[numthreads(64, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    if (dispatchThreadID.x < clusterCount)
    {
        
    }
}
