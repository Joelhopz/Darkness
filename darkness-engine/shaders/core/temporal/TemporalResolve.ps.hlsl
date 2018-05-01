
#include "../Common.hlsli"

struct PSInput
{
    float4 position         : SV_Position0;
    float2 uv               : TEXCOORD0;
};

Texture2D<float4> currentFrame;
Texture2D<float4> history;
Texture2D<float> depth;
Texture2D<float2> gbufferMotion;
sampler pointSampler;
sampler bilinearSampler;

float luminance(float3 color)
{
    return dot(color, float3(0.299f, 0.587f, 0.114f));
};

float max3(float x, float y, float z)
{
    return max(x, max(y, z));
}

float3 tonemap(float3 color)
{
    return color / (1.0 + color);
    //return color * rcp(max3(color.r, color.g, color.b) + 1.0);
};

float3 inverseTonemap(float3 color)
{
    return color / (1.0 - min(color, 0.989999));
    //return color * rcp(1.0 - max3(color.r, color.g, color.b));
};

cbuffer Constants
{
    float4x4 inverseJitterMatrix;
    float2 textureSize;
    float2 texelSize;
    float2 nearFar;
    float2 jitter;
    float2 previousJitter;
};

#include "../GBuffer.hlsli"

float2 getClosestUV(float2 uv)
{
    float closest = 0.0f;
    float2 closestUV = float2(0.0f, 0.0f);
    
    float distance00 = depth.Sample(pointSampler, uv + (float2(-1, 1) * texelSize));
    float distance01 = depth.Sample(pointSampler, uv + (float2(0, 1) * texelSize));
    float distance02 = depth.Sample(pointSampler, uv + (float2(1, 1) * texelSize));
    
    float distance10 = depth.Sample(pointSampler, uv + (float2(-1, 0) * texelSize));
    float distance11 = depth.Sample(pointSampler, uv + (float2(0, 0) * texelSize));
    float distance12 = depth.Sample(pointSampler, uv + (float2(1, 0) * texelSize));
    
    float distance20 = depth.Sample(pointSampler, uv + (float2(-1, -1) * texelSize));
    float distance21 = depth.Sample(pointSampler, uv + (float2(0, -1) * texelSize));
    float distance22 = depth.Sample(pointSampler, uv + (float2(1, -1) * texelSize));
    
    if (distance00 > closest) closestUV = uv + (float2(-1, 1) * texelSize);
    if (distance01 > closest) closestUV = uv + (float2(0, 1) * texelSize);
    if (distance02 > closest) closestUV = uv + (float2(1, 1) * texelSize);
    
    if (distance10 > closest) closestUV = uv + (float2(-1, 0) * texelSize);
    if (distance11 > closest) closestUV = uv + (float2(0, 0) * texelSize);
    if (distance12 > closest) closestUV = uv + (float2(1, 0) * texelSize);
    
    if (distance20 > closest) closestUV = uv + (float2(-1, -1) * texelSize);
    if (distance21 > closest) closestUV = uv + (float2(0, -1) * texelSize);
    if (distance22 > closest) closestUV = uv + (float2(1, -1) * texelSize);
    return closestUV;
}

float3 clampToBox(float3 historySample, float2 uv, out float3 maxBox)
{
    // clamp to box
    float2 du = float2(texelSize.x, 0);
    float2 dv = float2(0, texelSize.y);
    float3 near0 = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv - du + dv).xyz));
    float3 near1 = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv + dv).xyz));
    float3 near2 = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv + du + dv).xyz));
    
    float3 near3 = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv - du).xyz));
    float3 near4 = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv).xyz));
    float3 near5 = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv + du).xyz));
    
    float3 near6 = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv - du - dv).xyz));
    float3 near7 = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv - dv).xyz));
    float3 near8 = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv + du - dv).xyz));
    
    float3 boxMin = min(near0, min(near1, min(near2, min(near3, min(near4, min(near5, min(near5, min(near7, near8))))))));
    float3 boxMax = max(near0, max(near1, max(near2, max(near3, max(near4, max(near5, max(near5, max(near7, near8))))))));
    
    maxBox = boxMax;
    
    return clamp(historySample, boxMin, boxMax);
}

float4 main(PSInput input) : SV_Target
{
    float2 uv = input.uv;
    float2 closestUV = getClosestUV(uv);
    float2 motion = gbufferMotion.Sample(pointSampler, closestUV);
    float2 previousUV = uv - motion;
    
    float3 currentSample = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv).xyz));
    float3 historySample = RGB_YCoCg(tonemap(SampleTextureCatmullRom(history, bilinearSampler, previousUV, textureSize).xyz));
    
    float3 maxBox;
    historySample = clampToBox(historySample, uv, maxBox);
    
    float blendFactor = 1.0 / 16.0;
    float currLum = luminance(currentSample);
    float historyLum = luminance(historySample);
    float diff = 1.0 - (abs(currLum - historyLum) / (FLT_EPS + max(currLum, max(historyLum, luminance(maxBox)))));
    diff *= diff;
    blendFactor *= diff;
    
    float3 result = lerp(historySample, currentSample, 0.05f);
    result = inverseTonemap(YCoCg_RGB(result));
#ifdef OPTION_VISUALIZE_MOTION
    return float4(abs(motion.xy) * 100.0f, 0.0f, 1.0);
#else
    return float4(result.xyz, 1.0);
#endif

}
