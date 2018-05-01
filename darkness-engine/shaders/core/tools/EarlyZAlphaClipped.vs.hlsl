#include "../VertexPacking.hlsli"

Buffer<uint2> vertices;
Buffer<float2> uv;

cbuffer ConstData
{
    float4x4 jitterModelViewProjectionMatrix;
};

struct VSOutput
{
    float4 position         : SV_Position0;
    float2 uv               : TEXCOORD0;
};

VSOutput main(uint id : SV_VertexID)
{
    VSOutput output;
    output.position = mul(jitterModelViewProjectionMatrix, float4(unpackVertex(vertices[id]), 1.0f));
    output.uv = uv[id];
    return output;
}
