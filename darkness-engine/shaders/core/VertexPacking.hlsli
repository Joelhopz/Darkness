
float3 unpackVertex(uint2 data)
{
    uint xpart = (data.x & 0xffff0000) >> 16;
    uint ypart = data.x & 0xffff;
    uint zpart = (data.y & 0xffff0000) >> 16;
    uint wpart = data.y & 0xffff;
    float mult = (float)wpart * 0.15259021896696;// 0.0305180437933928;// 0.015259021896696;
    return float3(
        ((((float)xpart / 65535.0) * 2.0) - 1.0) * mult,
        ((((float)ypart / 65535.0) * 2.0) - 1.0) * mult,
        ((((float)zpart / 65535.0) * 2.0) - 1.0) * mult
        );
};
