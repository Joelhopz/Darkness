
Buffer<float3> vertices;
Buffer<float3> normals;
Buffer<float3> tangents;
Buffer<float2> uv;

cbuffer ConstData
{
    float4x4 modelMatrix;
    float4x4 modelViewProjectionMatrix;
    float4x4 previousModelViewProjectionMatrix;
    float4x4 jitterModelViewProjectionMatrix;
};

struct VSOutput
{
    float4 position         : SV_Position0;
    float4 mvPosCurrent     : POSITION0;
    float4 mvPosPrevious    : POSITION1;
    float4 normal           : NORMAL0;
    float4 tangent          : TEXCOORD0;
    float2 uv               : TEXCOORD1;
};

VSOutput main(uint id : SV_VertexID)
{
    VSOutput output;
    output.position =       mul(jitterModelViewProjectionMatrix, float4(vertices[id], 1));
    output.mvPosCurrent =   mul(modelViewProjectionMatrix, float4(vertices[id], 1));
    output.mvPosPrevious =  mul(previousModelViewProjectionMatrix, float4(vertices[id], 1));
    output.normal =         mul(modelMatrix, float4(normals[id], 0));
    output.tangent =        mul(modelMatrix, float4(tangents[id], 0));
    output.uv =             uv[id];

    return output;
}
