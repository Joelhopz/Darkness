
struct PSInput
{
    float4 position         : SV_Position0;
    float4 pos              : NORMAL;
};

Texture2D<float4> equirectangularMap;
sampler equirectangularSampler;

static const float2 invAtan = float2(0.1591, 0.3183);

float2 sampleSphericalMap(float3 v)
{
    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

float4 main(PSInput input) : SV_Target
{
    float2 uv = sampleSphericalMap(normalize(input.pos).xyz);
    float2 uvflip = float2(uv.x, -uv.y);
    float3 color = equirectangularMap.Sample(equirectangularSampler, uvflip).rgb;
    return float4(color, 1.0f);
}
