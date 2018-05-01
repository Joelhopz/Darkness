#pragma once

#include "engine/graphics/ShaderSupport.h"
#include <string>
#include <vector>

namespace engine
{
    std::string permutationName(const std::string& filename, int permutationId);
    std::string permutationName(int permutationId);
    std::string recompile(const implementation::ShaderSupport& support, int permutationId, const std::vector<std::string>& defines);
}
