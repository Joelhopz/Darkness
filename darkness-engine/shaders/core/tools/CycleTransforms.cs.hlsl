#include "../shared_types/TransformHistory.hlsli"

RWStructuredBuffer<TransformHistory> historyBuffer;

cbuffer CycleTransformsConstants
{
    uint transformCount;
};

[numthreads(64, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    if (dispatchThreadID.x < transformCount)
    {
        historyBuffer[dispatchThreadID.x].previousTransform = historyBuffer[dispatchThreadID.x].transform;
    }
}
