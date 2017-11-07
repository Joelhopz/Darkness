
struct PSInput
{
    float4 position     : SV_Position0;
    float4 colora       : COLOR0;
    float edgeFlag      : PSIZE;
};

cbuffer ConstData
{
    float4 color;
};

float4 main(PSInput input) : SV_Target
{
    return input.colora;
}
