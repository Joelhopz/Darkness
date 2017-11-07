#pragma once

#include "engine/graphics/vulkan/VulkanHeaders.h"
#include <vector>

namespace engine
{
    namespace implementation
    {
        struct VulkanQueue
        {
            int familyIndex;
            int available;
            VkQueueFlags flags;
            bool presentSupport;
            std::vector<float> priority;
            std::vector<bool> taken;
        };
    }
}
