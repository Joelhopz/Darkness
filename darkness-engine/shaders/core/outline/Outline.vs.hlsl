#include "../GBuffer.hlsli"
#include "../VertexPacking.hlsli"

Buffer<uint2> vertices;
Buffer<float2> normals;

struct VSOutput
{
    float3 position         : POSITION0;
    float3 normal           : NORMAL0;
};

VSOutput main(uint id : SV_VertexID)
{
    VSOutput output;
    output.position = unpackVertex(vertices[id]);
    output.normal = unpackNormalOctahedron(normals[id]);
    return output;
}
