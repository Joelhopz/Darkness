
Buffer<float3> vertices;
Buffer<float2> uv;

cbuffer ConstData
{
    float4x4 modelViewProjectionMatrix;
};

struct VSOutput
{
    float4 position : SV_Position;
    float2 uv       : TEXCOORD0;
};

VSOutput main(uint id : SV_VertexID)
{
    VSOutput output;
    output.position = mul(modelViewProjectionMatrix, float4(vertices[id], 1.0f));
    output.uv = uv[id];
    return output;
}
