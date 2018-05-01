
Texture1D<float4> src;
RWTexture1D<float4> dst;

cbuffer CopyConstants
{
    uint srcLeft;
    uint srcTextureWidth;
    uint dstLeft;
    uint dstTextureWidth;

    uint copyWidth;
};

[numthreads(63, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint srcLocation = srcLeft + dispatchThreadID.x;
    uint dstLocation = dstLeft + dispatchThreadID.x;

    if (
        (dispatchThreadID.x < copyWidth) &&
        (srcLocation.x < srcTextureWidth) &&
        (dstLocation.x < dstTextureWidth)
        )
    {
        dst[dstLocation] = src[srcLocation];
    }
}
