
Buffer<float3> vertices;
Buffer<float3> normals;
Buffer<float3> tangents;
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

    output.position =       mul(modelViewProjectionMatrix, float4(vertices[id], 1.0f));
    output.shadowPosition = mul(shadowModelViewProjectionMatrix, float4(vertices[id], 1.0f));
    output.pos =            mul(modelMatrix, float4(vertices[id], 1.0f));
    output.normal =         mul(modelMatrix, float4(normals[id], 0.0f));
    output.tangent =        mul(modelMatrix, float4(tangents[id], 0.0f));

    output.uv = float4(uv[id], 0, 0);
    

    return output;
}
