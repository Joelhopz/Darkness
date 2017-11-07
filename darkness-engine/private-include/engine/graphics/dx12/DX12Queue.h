#pragma once

#include <vector>
#include <memory>

struct ID3D12CommandQueue;

namespace engine
{
    class Device;
    class CommandList;
    class Semaphore; 
    class SwapChain;
    class Fence;
    enum class CommandListType;

    namespace implementation
    {
        class QueueImpl
        {
        public:
            QueueImpl(Device& device, CommandListType type);
            ~QueueImpl();

            QueueImpl(const QueueImpl&) = delete;
            QueueImpl(QueueImpl&&) = delete;
            QueueImpl& operator=(const QueueImpl&) = delete;
            QueueImpl& operator=(QueueImpl&&) = delete;

            void submit(std::vector<CommandList>& commandLists);
            void submit(CommandList& commandList);

            void submit(std::vector<CommandList>& commandLists, Fence& fence);
            void submit(CommandList& commandList, Fence& fence);

            void waitForIdle();

            void signal(const Semaphore& semaphore);

            void present(
                Semaphore& signalSemaphore,
                SwapChain& swapChain,
                unsigned int chainIndex);

            bool needRefresh() const;

            ID3D12CommandQueue* native();
        private:
            ID3D12CommandQueue* m_queue;
            std::shared_ptr<Fence> m_waitForClearFence;
        };
    }
}

