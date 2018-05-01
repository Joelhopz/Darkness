#include "engine/graphics/vulkan/VulkanQueue.h"
#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/vulkan/VulkanCommandList.h"
#include "engine/graphics/vulkan/VulkanSemaphore.h"
#include "engine/graphics/vulkan/VulkanSwapChain.h"
#include "engine/graphics/vulkan/VulkanDevice.h"
#include "engine/graphics/vulkan/VulkanFence.h"

#include "engine/graphics/Device.h"
#include "engine/graphics/CommandList.h"
#include "engine/graphics/Semaphore.h"
#include "engine/graphics/SwapChain.h"
#include "engine/graphics/Fence.h"

#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        QueueImpl::QueueImpl(const Device& device, CommandListType type)
            : m_queue{ nullptr }
            , m_device{ device }
            , m_queueInfo{ std::make_unique<QueueInfo>(DeviceImplGet::impl(device).createQueue(type)) }
            , m_needRefresh{ false }
        {
            ASSERT(m_queueInfo->queueFamilyIndex != InvalidFamilyIndex);
            
            vkGetDeviceQueue(
                DeviceImplGet::impl(device).device(),
                static_cast<uint32_t>(m_queueInfo->queueFamilyIndex),
                static_cast<uint32_t>(m_queueInfo->queueIndex),
                &m_queue);
            ASSERT(m_queue != nullptr);
        }

        QueueImpl::~QueueImpl()
        {
            DeviceImplGet::impl(m_device).destroyQueue(*m_queueInfo.get());
        }

        const VkQueue& QueueImpl::native() const
        {
            return m_queue;
        }

        void QueueImpl::submit(std::vector<CommandList>& commandLists)
        {
            ASSERT(false);
        }

        void QueueImpl::submit(const CommandList& commandList)
        {
            VkPipelineStageFlags m_waitStages[1];
            m_waitStages[0] = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

            VkSubmitInfo m_submitInfo = {};
            m_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            m_submitInfo.waitSemaphoreCount = 1;

            ASSERT(false, "some semaphore shit");
            /*m_submitInfo.pWaitSemaphores = &SemaphoreImplGet::impl(
                CommandListImplGet::impl(commandList).finishedSemaphore()).native();*/
            m_submitInfo.pWaitDstStageMask = m_waitStages;
            m_submitInfo.commandBufferCount = 1;
            m_submitInfo.pCommandBuffers = &CommandListImplGet::impl(commandList).native();
            m_submitInfo.signalSemaphoreCount = 0;
            m_submitInfo.pSignalSemaphores = nullptr;
            //m_submitInfo.signalSemaphoreCount = 1;
            //m_submitInfo.pSignalSemaphores = &CommandListImplGet::impl(commandList).barrier();
                

            //auto result = vkQueueSubmit(m_queue, 1, &m_submitInfo, VK_NULL_HANDLE);
            auto result = vkQueueSubmit(m_queue, 1, &CommandListImplGet::impl(commandList).barrier(), VK_NULL_HANDLE);
            ASSERT(result == VK_SUCCESS);

            vkQueueWaitIdle(m_queue);
        }

        void QueueImpl::submit(std::vector<CommandList>& commandLists, Fence& fence)
        {
            ASSERT(false);
        }

        void QueueImpl::submit(const CommandList& commandList, Fence& fence)
        {
            vkCmdEndRenderPass(CommandListImplGet::impl(commandList).native());
            auto res = vkEndCommandBuffer(CommandListImplGet::impl(commandList).native());
            ASSERT(res == VK_SUCCESS);

            VkPipelineStageFlags m_waitStages[1];
            m_waitStages[0] = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

            VkSubmitInfo m_submitInfo = {};
            m_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            /*m_submitInfo.waitSemaphoreCount = 1;
            m_submitInfo.pWaitSemaphores = &SemaphoreImplGet::impl(
                CommandListImplGet::impl(commandList).finishedSemaphore()).native();*/
            m_submitInfo.waitSemaphoreCount = 0;
            m_submitInfo.pWaitSemaphores = nullptr;
            m_submitInfo.pWaitDstStageMask = m_waitStages;
            m_submitInfo.commandBufferCount = 1;
            m_submitInfo.pCommandBuffers = &CommandListImplGet::impl(commandList).native();
            m_submitInfo.signalSemaphoreCount = 0;
            m_submitInfo.pSignalSemaphores = nullptr;
            //m_submitInfo.signalSemaphoreCount = 1;
            //m_submitInfo.pSignalSemaphores = &CommandListImplGet::impl(commandList).barrier();
                
            auto result = vkQueueSubmit(m_queue, 1, &m_submitInfo, FenceImplGet::impl(fence).native());
            ASSERT(result == VK_SUCCESS);

            vkQueueWaitIdle(m_queue);
        }

        void QueueImpl::submit(std::vector<CommandList>& commandLists, Semaphore& semaphore)
        {
            ASSERT(false);
        }

        void QueueImpl::submit(const CommandList& commandList, Semaphore& semaphore)
        {
            vkCmdEndRenderPass(CommandListImplGet::impl(commandList).native());
            auto res = vkEndCommandBuffer(CommandListImplGet::impl(commandList).native());
            ASSERT(res == VK_SUCCESS);

            VkPipelineStageFlags m_waitStages[1];
            m_waitStages[0] = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

            VkSubmitInfo m_submitInfo = {};
            m_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            m_submitInfo.waitSemaphoreCount = 0;
            m_submitInfo.pWaitSemaphores = nullptr;
            m_submitInfo.pWaitDstStageMask = m_waitStages;
            m_submitInfo.commandBufferCount = 1;
            m_submitInfo.pCommandBuffers = &CommandListImplGet::impl(commandList).native();
            m_submitInfo.signalSemaphoreCount = 1;
            m_submitInfo.pSignalSemaphores = &SemaphoreImplGet::impl(semaphore).native();

            auto result = vkQueueSubmit(m_queue, 1, &m_submitInfo, VK_NULL_HANDLE);
            ASSERT(result == VK_SUCCESS);

            vkQueueWaitIdle(m_queue);
        }

        void QueueImpl::waitForIdle() const
        {
            vkQueueWaitIdle(m_queue);
        }

        void QueueImpl::signal(const Semaphore& /*semaphore*/)
        {
            // TODO
            ASSERT(false);
        }

        void QueueImpl::present(
            const Semaphore& signalSemaphore,
            const SwapChain& swapChain,
            unsigned int chainIndex)
        {
            VkSemaphore signalSemaphores[] = { SemaphoreImplGet::impl(signalSemaphore).native() };

            VkPresentInfoKHR presentInfo = {};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = signalSemaphores;

            VkSwapchainKHR swapChains[] = { SwapChainImplGet::impl(swapChain).native() };
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = swapChains;

            uint32_t imageIndex = static_cast<uint32_t>(chainIndex);
            presentInfo.pImageIndices = &imageIndex;

            VkResult res = vkQueuePresentKHR(m_queue, &presentInfo);
            if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR)
            {
                m_needRefresh = true;
                res = VK_SUCCESS;
            }
            ASSERT(res == VK_SUCCESS);
        }

        bool QueueImpl::needRefresh() const
        {
            return m_needRefresh;
        }
    }
}
