#include "../GBuffer.hlsli"
#include "../VertexPacking.hlsli"

Buffer<uint2> vertices;
Buffer<float2> normals;
Buffer<float2> tangents;
Buffer<float2> uv;

cbuffer ConstData
{
    float4x4 modelMatrix;
    float4x4 modelViewProjectionMatrix;
    float4x4 shadowModelViewProjectionMatrix;
};

struct VSOutput
{
    float4 position         : SV_Position0;
    float4 shadowPosition   : POSITION;
    float4 uv               : TEXCOORD0;
    float4 normal           : NORMAL0;
    float4 pos              : TEXCOORD1;
    float4 tangent          : TEXCOORD2;
};

VSOutput main(uint id : SV_VertexID)
{
    VSOutput output;

    float3 vertex = unpackVertex(vertices[id]);

    output.position =       mul(modelViewProjectionMatrix, float4(vertex, 1.0f));
    output.shadowPosition = mul(shadowModelViewProjectionMatrix, float4(vertex, 1.0f));
    output.pos =            mul(modelMatrix, float4(vertex, 1.0f));
    output.normal =         mul(modelMatrix, float4(unpackNormalOctahedron(normals[id]), 0.0f));
    output.tangent =        mul(modelMatrix, float4(unpackNormalOctahedron(tangents[id]), 0.0f));

    output.uv = float4(uv[id], 0, 0);
    

    return output;
}
