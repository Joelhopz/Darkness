
#include "../Common.hlsli"

struct PSInput
{
    float4 position         : SV_Position0;
    float2 uv               : TEXCOORD0;
};

Texture2D<float4> framebuffer;
Texture2D<float4> blur;
sampler framebufferSampler;

float4 main(PSInput input) : SV_Target
{
    float3 hdrColor = framebuffer.SampleLevel(framebufferSampler, input.uv, 0).xyz;
    float3 blurColor = blur.SampleLevel(framebufferSampler, input.uv, 0).xyz;

    //return float4(JimHejlRichardBurgessDawsonTonemap(hdrColor + blurColor), 1.0f);
    return float4(ACESFitted(linearTosRGB(hdrColor + blurColor)), 1.0f);
}
