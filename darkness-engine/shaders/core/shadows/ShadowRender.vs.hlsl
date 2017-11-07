
Buffer<float3> vertices;

cbuffer ConstData
{
    float4x4 modelViewProjectionMatrix;
};

float4 main(uint id : SV_VertexID) : SV_Position
{
    return mul(modelViewProjectionMatrix, float4(vertices[id], 1.0f));
}
