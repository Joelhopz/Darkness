
struct PSInput
{
    float4 position : SV_Position;
    float4 uv : TEXCOORD0;
    uint texid : BLENDINDICES0;
};

Texture2D<float4> mrt[];

sampler tex_sampler;

float4 main(PSInput input) : SV_Target
{
    return mrt[input.texid].SampleLevel(tex_sampler, input.uv.xy, 0.0f);
}
