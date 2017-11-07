#pragma once

#include "engine/primitives/Vector3.h"
#include "engine/primitives/Vector4.h"
#include <vector>

namespace engine
{
    std::vector<uint32_t> meshGenerateAdjacency(const std::vector<uint32_t>& indices, const std::vector<Vector3f>& vertices);
}
