#pragma once

#include <vector>
#include <memory>
#include "engine/graphics/vulkan/VulkanHeaders.h"

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
        struct QueueInfo;

        class QueueImpl
        {
        public:
            QueueImpl(const Device& device, CommandListType type);
            ~QueueImpl();

            QueueImpl(const QueueImpl&) = delete;
            QueueImpl(QueueImpl&&) = delete;
            QueueImpl& operator=(const QueueImpl&) = delete;
            QueueImpl& operator=(QueueImpl&&) = delete;

            void submit(std::vector<CommandList>& commandLists);
            void submit(const CommandList& commandList);

            void submit(std::vector<CommandList>& commandLists, Fence& fence);
            void submit(const CommandList& commandList, Fence& fence);

            void submit(std::vector<CommandList>& commandLists, Semaphore& semaphore);
            void submit(const CommandList& commandList, Semaphore& semaphore);

            void waitForIdle() const;

            void signal(const Semaphore& semaphore);

            void present(
                const Semaphore& signalSemaphore,
                const SwapChain& swapChain,
                unsigned int chainIndex);

            bool needRefresh() const;

            const VkQueue& native() const;
        private:
            VkQueue m_queue;
            const Device& m_device;
            std::unique_ptr<QueueInfo> m_queueInfo;
            bool m_needRefresh;
        };
    }
}

