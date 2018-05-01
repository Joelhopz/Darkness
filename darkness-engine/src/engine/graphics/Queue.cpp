#include "engine/graphics/Queue.h"
#include "engine/graphics/Common.h"
#include "engine/graphics/Barrier.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/SwapChain.h"
#include "engine/graphics/CommandList.h"
#include "engine/graphics/Resources.h"

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

        for (auto&& commandList : commandLists)
            handleShaderDebug(commandList);
    }

    void Queue::submit(CommandList& commandList)
    {
        m_impl->submit(commandList);
        handleShaderDebug(commandList);
    }

    void Queue::submit(std::vector<CommandList>& commandLists, Fence& fence)
    {
        m_impl->submit(commandLists, fence);

        for (auto&& commandList : commandLists)
            handleShaderDebug(commandList);
    }

    void Queue::submit(CommandList& commandList, Fence& fence)
    {
        m_impl->submit(commandList, fence);
        handleShaderDebug(commandList);
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

    void Queue::handleShaderDebug(CommandList& commandList)
    {
        CPU_MARKER("Handle shader debug");
        
        bool thereWasShaderDebug = false;
        for (auto&& debugBuffer : commandList.m_debugBuffers)
        {
            if (!m_debugOutputCpu)
            {
                m_debugOutputCpu = std::make_shared<Buffer>(m_device.createBuffer(BufferDescription()
                    .elementSize(sizeof(CommandList::ShaderDebugOutput))
                    .elements(10000)
                    .name("shaderDebut cpu buffer")
                    .structured(true)
                    .usage(ResourceUsage::GpuToCpu)
                ));
                m_debugOutputCpuCount = std::make_shared<Buffer>(m_device.createBuffer(BufferDescription()
                    .elementSize(sizeof(uint32_t))
                    .elements(1)
                    .name("shaderDebut buffer counter")
                    .usage(ResourceUsage::GpuToCpu)
                ));
            }

            // copy shader debug structure count
            {
                auto cmd = m_device.createCommandList();
                
                {
                    CPU_MARKER("Copy shader debug structure count");
                    GPU_MARKER(cmd, "Copy shader debug structure count");

                    cmd.copyStructureCounter(debugBuffer, *m_debugOutputCpuCount, 0);
                }
                auto fence = m_device.createFence();
                submit(cmd, fence);
                fence.blockUntilSignaled();
            }

            // copy shader debug output to cpu buffer
            uint32_t counter = 0;
            {
                {
                    auto contents = static_cast<uint32_t*>(m_debugOutputCpuCount->map(m_device));
                    memcpy(&counter, contents, sizeof(uint32_t));
                    m_debugOutputCpuCount->unmap(m_device);

                    if(counter > 0u && counter < static_cast<uint32_t>(m_debugOutputCpu->description().descriptor.elements))
                    {
                        auto cmd = m_device.createCommandList();
                        {
                            CPU_MARKER("Copy shader debug buffer");
                            GPU_MARKER(cmd, "Copy shader debug buffer");
                            cmd.copyBuffer(debugBuffer.buffer(), *m_debugOutputCpu, counter, 0, 0);
                        }
                        auto fence = m_device.createFence();
                        submit(cmd, fence);
                        fence.blockUntilSignaled();
                    }
                    else if (counter > 0)
                    {
                        LOG_ERROR("Debug output counter was bigger than debug buffer: %u", counter);
                    }
                }
                
            }

            // output shader debug contents
            if(counter > 0u && counter < static_cast<uint32_t>(m_debugOutputCpu->description().descriptor.elements))
            {
                auto debugContents = static_cast<CommandList::ShaderDebugOutput*>(m_debugOutputCpu->map(m_device));
                for (uint32_t i = 0; i < counter; ++i)
                {
                    switch (debugContents[i].itemType)
                    {
                    case 1: LOG("ShaderDebug: %s", debugContents[i].uvalue > 0 ? "true" : "false"); break;
                    case 2: LOG("ShaderDebug: %i", static_cast<int>(debugContents[i].uvalue)); break;
                    case 3: LOG("ShaderDebug: %u", static_cast<uint32_t>(debugContents[i].uvalue)); break;
                    case 4: LOG("ShaderDebug: %f", debugContents[i].fvalue); break;
                    }
                }
                m_debugOutputCpu->unmap(m_device);
            }

            if (counter > 0u && counter < static_cast<uint32_t>(m_debugOutputCpu->description().descriptor.elements))
                thereWasShaderDebug = true;
        }
        commandList.m_debugBuffers.clear();

        if (thereWasShaderDebug)
            LOG("SHADER DEBUG NEXT");
    }
}
