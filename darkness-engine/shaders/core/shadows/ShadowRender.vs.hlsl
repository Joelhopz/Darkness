#include "../VertexPacking.hlsli"

Buffer<uint2> vertices;

cbuffer ConstData
{
    float4x4 modelViewProjectionMatrix;
};

float4 main(uint id : SV_VertexID) : SV_Position
{
    return mul(modelViewProjectionMatrix, float4(unpackVertex(vertices[id]), 1.0f));
}
