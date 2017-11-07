#include "engine/graphics/vulkan/VulkanFence.h"
#include "engine/graphics/vulkan/VulkanHeaders.h"

#include "engine/graphics/Device.h"
#include "engine/graphics/vulkan/VulkanDevice.h"

#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        FenceImpl::FenceImpl(const DeviceImpl& device)
            : m_device{ device }
            , m_fenceValue{ 0 }
        {
            VkFenceCreateInfo fenceInfo = {};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            auto result = vkCreateFence(
                device.device(),
                &fenceInfo,
                nullptr,
                &m_fence);
            ASSERT(result == VK_SUCCESS);

            reset();
        }

        FenceImpl::~FenceImpl()
        {
            vkDestroyFence(m_device.device(), m_fence, nullptr);
        }

        VkFence& FenceImpl::native()
        {
            return m_fence;
        }

        const VkFence& FenceImpl::native() const
        {
            return m_fence;
        }

        unsigned long long FenceImpl::nextValue()
        {
            ++m_fenceValue;
            return m_fenceValue;
        }

        unsigned long long FenceImpl::value() const
        {
            return m_fenceValue;
        }

        void FenceImpl::reset()
        {
            m_fenceValue = 0;
            vkResetFences(m_device.device(), m_fenceValue, &m_fence);
        }

        bool FenceImpl::signaled() const
        {
            return vkGetFenceStatus(m_device.device(), m_fence) == VK_SUCCESS;
        }

        void FenceImpl::blockUntilSignaled() const
        {
            auto result = vkWaitForFences(
                m_device.device(), 
                m_fenceValue + 1,
                &m_fence, 
                VK_TRUE, 
                VulkanInfinity);
            ASSERT(result == VK_SUCCESS);
        }

        void FenceImpl::blockUntilSignaled(unsigned long long value)
        {
            auto result = vkWaitForFences(
                m_device.device(),
                value,
                &m_fence,
                VK_TRUE,
                VulkanInfinity);
            ASSERT(result == VK_SUCCESS);
        }
    }
}
