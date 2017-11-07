#pragma once

#include "engine/graphics/vulkan/VulkanHeaders.h"

namespace engine
{
    namespace implementation
    {
        class DeviceImpl;

        class CommandAllocatorImpl
        {
        public:
            CommandAllocatorImpl(const DeviceImpl& device);

            void reset();

            VkCommandPool& native();
            const VkCommandPool& native() const;
            
        private:
            VkDevice m_device;
            std::shared_ptr<VkCommandPool> m_commandAllocator;
        };
    }
}

