#include "engine/graphics/Queue.h"
#include "engine/graphics/Common.h"
#include "engine/graphics/Barrier.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/SwapChain.h"
#include "engine/graphics/CommandList.h"

#if defined(_WIN32) && !defined(VULKAN)
#include "engine/graphics/Fence.h"
#include "engine/graphics/dx12/DX12Queue.h"
#endif

#if defined(VULKAN)
#include "engine/graphics/Semaphore.h"
#include "engine/graphics/vulkan/VulkanQueue.h"
#endif

#ifdef __APPLE__
#include "engine/graphics/metal/MetalQueue.h"
#endif


using namespace tools;
using namespace engine::implementation;

namespace engine
{
    PIMPL_ACCESS_IMPLEMENTATION(Queue, QueueImpl)

    Queue::Queue(Device& device, CommandListType type)
        : m_impl{ make_unique_impl<QueueImpl>(device, type) }
        , m_device{ device }
    {
    }

    void Queue::submit(std::vector<CommandList>& commandLists)
    {
        m_impl->submit(commandLists);
    }

    void Queue::submit(CommandList& commandList)
    {
        m_impl->submit(commandList);
    }

    void Queue::submit(std::vector<CommandList>& commandLists, Fence& fence)
    {
        m_impl->submit(commandLists, fence);
    }

    void Queue::submit(CommandList& commandList, Fence& fence)
    {
        m_impl->submit(commandList, fence);
    }

    void Queue::waitForIdle() const
    {
        m_impl->waitForIdle();
    }

    void Queue::signal(const Semaphore& semaphore)
    {
        m_impl->signal(semaphore);
    }

    void Queue::present(
        Semaphore& signalSemaphore,
        SwapChain& swapChain,
        unsigned int chainIndex)
    {
        auto transitionCmd = m_device.createCommandList();
        transitionCmd.transition(swapChain.renderTarget(chainIndex), ResourceState::Present);
        submit(transitionCmd);
        m_impl->present(signalSemaphore, swapChain, chainIndex);
    }

    bool Queue::needRefresh() const
    {
        return m_impl->needRefresh();
    }
}
