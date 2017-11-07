
Texture2D<float4> transparencyMap;
sampler transparencySampler;

struct PSInput
{
    float4 position : SV_Position;
    float2 uv       : TEXCOORD0;
};

void main(PSInput input)
{
    if (transparencyMap.Sample(transparencySampler, input.uv).a < 0.95f)
        discard;
}
