#include "ShaderTypes.h"

namespace engine
{
    Float4x4 fromMatrix(Matrix4f mat)
    {
        return {
            mat.m00, mat.m01, mat.m02, mat.m03,
            mat.m10, mat.m11, mat.m12, mat.m13,
            mat.m20, mat.m21, mat.m22, mat.m23,
            mat.m30, mat.m31, mat.m32, mat.m33
        };
    };
}
