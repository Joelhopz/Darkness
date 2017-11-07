
#include "../Common.hlsli"

struct PSInput
{
    float4 position         : SV_Position0;
    float2 uv               : TEXCOORD0;
};

Texture2D<float4> currentFrame;
Texture2D<float4> history;
Texture2D<float4> depth;
Texture2D<float2> motion;
sampler pointSampler;
sampler bilinearSampler;
sampler trilinearSampler;
sampler anisotropicSampler;

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

float4 main(PSInput input) : SV_Target
{
    float2 jitteruv = (texelSize * jitter);
    float2 uv = input.uv;

    // motion vector dilation
    float closest = 0.0f;
    float2 closestUV = float2(0.0f, 0.0f);

    float distance00 = depth.Sample(pointSampler, uv + (float2(-1, 1) * texelSize)).x;
    float distance01 = depth.Sample(pointSampler, uv + (float2(0, 1) * texelSize)).x;
    float distance02 = depth.Sample(pointSampler, uv + (float2(1, 1) * texelSize)).x;

    float distance10 = depth.Sample(pointSampler, uv + (float2(-1, 0) * texelSize)).x;
    float distance11 = depth.Sample(pointSampler, uv + (float2(0, 0) * texelSize)).x;
    float distance12 = depth.Sample(pointSampler, uv + (float2(1, 0) * texelSize)).x;

    float distance20 = depth.Sample(pointSampler, uv + (float2(-1, -1) * texelSize)).x;
    float distance21 = depth.Sample(pointSampler, uv + (float2(0, -1) * texelSize)).x;
    float distance22 = depth.Sample(pointSampler, uv + (float2(1, -1) * texelSize)).x;

    if (distance00 > closest) closestUV = uv + (float2(-1, 1) * texelSize);
    if (distance01 > closest) closestUV = uv + (float2(0, 1) * texelSize);
    if (distance02 > closest) closestUV = uv + (float2(1, 1) * texelSize);

    if (distance10 > closest) closestUV = uv + (float2(-1, 0) * texelSize);
    if (distance11 > closest) closestUV = uv + (float2(0, 0) * texelSize);
    if (distance12 > closest) closestUV = uv + (float2(1, 0) * texelSize);

    if (distance20 > closest) closestUV = uv + (float2(-1, -1) * texelSize);
    if (distance21 > closest) closestUV = uv + (float2(0, -1) * texelSize);
    if (distance22 > closest) closestUV = uv + (float2(1, -1) * texelSize);


    float2 motionValue = motion.Sample(pointSampler, closestUV);
    float2 previousUV = uv - motionValue;

    float3 currentSample = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv).xyz));
    float3 historySample = RGB_YCoCg(tonemap(SampleTextureCatmullRom(history, bilinearSampler, previousUV, textureSize).xyz));

    float blendFactor = 1.0 / 8.0;

    // clamp to box
    /*float2 du = float2(texelSize.x, 0);
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
    float3 boxMax = max(near0, max(near1, max(near2, max(near3, max(near4, max(near5, max(near5, max(near7, near8))))))));*/

    /*const float varianceClippingGamma = 1.0;
    float3 m1 = near0 + near1 + near2 + near3 + near4 + near5 + near6 + near7 + near8;
    float3 m2 = near0 * near0 + near1 * near1 + near2 * near2 + near3 * near3 + near4 * near4 + near5 * near5 + near6 * near6 + near7 * near7 + near8 * near8;
    float3 mu = m1 / 9.0;
    float3 sigma = sqrt(m2 / 9.0 - mu * mu);
    float3 boxMin = mu - varianceClippingGamma * sigma;
    float3 boxMax = mu + varianceClippingGamma * sigma;*/
    
    float2 du = float2(texelSize.x, 0);
    float2 dv = float2(0, texelSize.y);
    float3 near0 = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv + du).xyz));
    float3 near1 = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv + dv).xyz));
    float3 near2 = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv - du).xyz));
    float3 near3 = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv - dv).xyz));

    float3 boxMin = min(currentSample, min(near0, min(near1, min(near2, near3))));
    float3 boxMax = max(currentSample, max(near0, max(near1, max(near2, near3))));

    // variance clipping
    /*const float varianceClippingGamma = 1.0;
    float3 m1 = currentSample + near0 + near1 + near2 + near3;
    float3 m2 = currentSample * currentSample + near0 * near0 + near1 * near1 + near2 * near2 + near3 * near3;
    float3 mu = m1 / 5.0;
    float3 sigma = sqrt(m2 / 5.0 - mu * mu);
    float3 boxMin = mu - varianceClippingGamma * sigma;
    float3 boxMax = mu + varianceClippingGamma * sigma;*/

    historySample = clamp(historySample, boxMin, boxMax);

    // create modulation factor based on luminescense
    float currLum = luminance(currentSample);
    float historyLum = luminance(historySample);
    /*float currLum = max(currentSample.x, max(currentSample.y, currentSample.z));
    float historyLum = max(historySample.x, max(historySample.y, historySample.z));*/

    float diff = 1.0 - (abs(currLum - historyLum) / (FLT_EPS + max(currLum, max(historyLum, luminance(boxMax)))));
    diff *= diff;
    blendFactor *= diff;

    // unjitter
    float3 unJitteredCurrentSample = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv - (texelSize * previousJitter) - (texelSize * jitter)).xyz));
    //float3 unJitteredCurrentSample = RGB_YCoCg(tonemap(currentFrame.Sample(pointSampler, uv).xyz));

    float3 result = lerp(historySample, unJitteredCurrentSample, max(min(blendFactor, 1.0), 0.0));
    
    result = inverseTonemap(YCoCg_RGB(result));
    
    //return float4(min(result, float3(100.0, 100.0, 100.0)), 1.0);
    //return float4(inverseTonemap(YCoCg_RGB(currentSample)), 1.0);
    return float4(result, 1.0);

    /*float2 motionValue = motion.Sample(pointSampler, uv);
    float2 previousUV = uv - motionValue;

    float blendFactor = 1.0 / 16.0;
    float3 currentSample = currentFrame.Sample(pointSampler, uv).xyz;
    //float3 currentSample = SampleTextureCatmullRom(currentFrame, pointSampler, uv, textureSize).xyz;
    float3 historySample = SampleTextureCatmullRom(history, bilinearSampler, previousUV, textureSize).xyz;
    float3 result = lerp(historySample, currentSample, blendFactor);
    //return float4(0.0f, 0.0f, 0.0f, 1.0f);
    //return float4(currentSample, 1.0);
    return float4(result, 1.0);*/
}
