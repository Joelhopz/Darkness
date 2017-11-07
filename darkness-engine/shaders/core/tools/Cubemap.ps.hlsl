
struct PSInput
{
    float4 position         : SV_Position0;
    float4 pos              : NORMAL;
};

TextureCube<float4> cubemap;
TextureCube<float4> irradiance;
TextureCube<float4> convolution;
sampler cubemapSampler;

float4 main(PSInput input) : SV_Target
{
    float3 N = normalize(input.pos.xyz);
    N = float3(N.x, -N.y, N.z);
    float4 cubeSample = cubemap.SampleLevel(cubemapSampler, N, 0);
    float4 irradianceSample = irradiance.SampleLevel(cubemapSampler, N, 0);
    float4 convolutionSample = convolution.SampleLevel(cubemapSampler, N, 0);
    return float4(cubeSample.xyz, 1.0f);
}
