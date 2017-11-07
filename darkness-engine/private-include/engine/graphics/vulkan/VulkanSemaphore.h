#pragma once

#include "engine/graphics/vulkan/VulkanHeaders.h"

namespace engine
{
    class Device;
    namespace implementation
    {
        class SemaphoreImpl
        {
        public:
            SemaphoreImpl(const Device& device);

            VkSemaphore& native();
            const VkSemaphore& native() const;
        private:
            std::shared_ptr<VkSemaphore> m_semaphore;
        };
    }
}

