
#include "../Common.hlsli"

struct PSInput
{
    float4 position         : SV_Position0;
    float4 pos              : NORMAL;
};

TextureCube<float4> environmentMap;
sampler environmentSampler;

static const float2 invAtan = float2(0.1591, 0.3183);

float2 sampleSphericalMap(float3 v)
{
    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    return uv;
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

float4 main(PSInput input) : SV_Target
{
    float3 N = normalize(input.pos.xyz);
    N = float3(-N.x, N.y, N.z);

    float3 irradiance = float3(0.0f, 0.0f, 0.0f);
    
    float3 up = float3(0.0f, 1.0f, 0.0f);
    float3 right = cross(up, N);
    up = cross(N, right);

    float sampleDelta = 0.025;
    float nrSamples = 0.0;
    for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;
            sampleVec = float3(sampleVec.x, -sampleVec.y, sampleVec.z);
            irradiance += environmentMap.Sample(environmentSampler, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }

    irradiance = PI * irradiance * (1.0 / float(nrSamples));
    
    return float4(irradiance, 1.0f);
}
