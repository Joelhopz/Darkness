#pragma once

#include "engine/graphics/vulkan/VulkanHeaders.h"

namespace engine
{
    class CommandList;
    class Buffer;
    class TextureRTV;
    class Semaphore;
    enum class ResourceBarrierFlags;
    enum class ResourceState;

    namespace implementation
    {
        class BarrierImpl
        {
        public:
            BarrierImpl(
                const CommandList& commandList, 
                ResourceBarrierFlags flags,
                const TextureRTV& resource,
                ResourceState before,
                ResourceState after,
                unsigned int subResource,
                const Semaphore& waitSemaphore,
                const Semaphore& signalSemaphore);
            ~BarrierImpl();

            BarrierImpl(const BarrierImpl&) = delete;
            BarrierImpl(BarrierImpl&&) = delete;
            BarrierImpl& operator=(const BarrierImpl&) = delete;
            BarrierImpl& operator=(BarrierImpl&&) = delete;

            void update(
                ResourceState before,
                ResourceState after);

        private:
            VkSemaphore m_waitSem[1];
            VkSemaphore m_signalSemaphores[1];
            VkPipelineStageFlags m_waitStages[1];

            VkSubmitInfo m_submitInfo;
            const CommandList& m_commandList;
        };
    }
}

