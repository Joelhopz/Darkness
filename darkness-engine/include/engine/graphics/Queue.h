#pragma once

#include "tools/SmartPimpl.h"
#include <vector>

namespace engine
{
    PIMPL_FWD(Queue, QueueImpl)

    class Device;
    class CommandList;
    class Fence;
    class Semaphore;
    class SwapChain;
    class Buffer;
    enum class CommandListType;

    class Queue
    {
    public:
        Queue(Device& device, CommandListType type);

        void submit(std::vector<CommandList>& commandLists);
        void submit(CommandList& commandList);
        void submit(std::vector<CommandList>& commandLists, Fence& fence);
        void submit(CommandList& commandList, Fence& fence);

        void waitForIdle() const;

        void signal(const Semaphore& semaphore);

        void present(
            Semaphore& signalSemaphore,
            SwapChain& swapChain,
            unsigned int chainIndex);

        bool needRefresh() const;

    private:
        Device& m_device;
        PIMPL_FRIEND_ACCESS(QueueImpl)

    private:
        void handleShaderDebug(CommandList& commandList);
        std::shared_ptr<Buffer> m_debugOutputCpu;
        std::shared_ptr<Buffer> m_debugOutputCpuCount;

    };
}
