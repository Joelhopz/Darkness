
#include "../Common.hlsli"

struct PSInput
{
    float4 position         : SV_Position0;
    float2 uv               : TEXCOORD0;
};

Texture2D<float4> framebuffer;
sampler framebufferSampler;

float4 main(PSInput input) : SV_Target
{
    float4 color = framebuffer.SampleLevel(framebufferSampler, input.uv, 0);
    float brightness = dot(color.rgb, float3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        return float4(color.xyz, 1.0f);
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}
