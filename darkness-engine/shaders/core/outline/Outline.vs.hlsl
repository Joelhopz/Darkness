
Buffer<float3> vertices;
Buffer<float3> normals;

struct VSOutput
{
    float3 position         : POSITION0;
    float3 normal           : NORMAL0;
};

VSOutput main(uint id : SV_VertexID)
{
    VSOutput output;
    output.position = vertices[id];
    output.normal = normals[id];
    return output;
}
