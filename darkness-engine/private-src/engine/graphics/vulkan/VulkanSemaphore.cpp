#include "engine/graphics/vulkan/VulkanSemaphore.h"
#include "engine/graphics/vulkan/VulkanHeaders.h"

#include "engine/graphics/Device.h"
#include "engine/graphics/vulkan/VulkanDevice.h"

#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        SemaphoreImpl::SemaphoreImpl(const Device& device)
            : m_semaphore{ vulkanPtr<VkSemaphore>(DeviceImplGet::impl(device).device(), vkDestroySemaphore) }
        {
            VkSemaphoreCreateInfo semaphoreInfo = {};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            auto result = vkCreateSemaphore(
                DeviceImplGet::impl(device).device(),
                &semaphoreInfo,
                nullptr,
                m_semaphore.get());
            ASSERT(result == VK_SUCCESS);
        }

        VkSemaphore& SemaphoreImpl::native()
        {
            return *m_semaphore;
        }

        const VkSemaphore& SemaphoreImpl::native() const
        {
            return *m_semaphore;
        }
    }
}
