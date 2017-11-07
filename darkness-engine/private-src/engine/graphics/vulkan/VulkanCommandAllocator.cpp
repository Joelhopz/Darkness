#include "engine/graphics/vulkan/VulkanCommandAllocator.h"
#include "engine/graphics/vulkan/VulkanHeaders.h"

#include "engine/graphics/Device.h"
#include "engine/graphics/vulkan/VulkanDevice.h"

#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        CommandAllocatorImpl::CommandAllocatorImpl(const DeviceImpl& device)
            : m_device{ device.device() }
            , m_commandAllocator{ vulkanPtr<VkCommandPool>(m_device, vkDestroyCommandPool) }
        {
            std::vector<VulkanQueue> deviceQueues = device.deviceQueues();
            int graphicsFamily = -1;
            int presentFamily = -1;
            int queueIndex = 0;
            for (const auto& queue : deviceQueues)
            {
                if (queue.taken.size() > 0 && queue.flags & VK_QUEUE_GRAPHICS_BIT)
                    graphicsFamily = queueIndex;

                if (queue.taken.size() > 0 && queue.presentSupport)
                    presentFamily = queueIndex;

                if (graphicsFamily >= 0 && presentFamily >= 0)
                    break;

                ++queueIndex;
            }


            VkCommandPoolCreateInfo poolInfo = {};
            poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            poolInfo.queueFamilyIndex = static_cast<uint32_t>(graphicsFamily);
            poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

            auto result = vkCreateCommandPool(
                device.device(),
                &poolInfo,
                nullptr,
                m_commandAllocator.get());
            ASSERT(result == VK_SUCCESS);
        }

        VkCommandPool& CommandAllocatorImpl::native()
        {
            return *m_commandAllocator;
        }

        const VkCommandPool& CommandAllocatorImpl::native() const
        {
            return *m_commandAllocator;
        }

        void CommandAllocatorImpl::reset()
        {
            auto result = vkResetCommandPool(m_device, *m_commandAllocator, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
            ASSERT(result == VK_SUCCESS);
        }
    }
}
