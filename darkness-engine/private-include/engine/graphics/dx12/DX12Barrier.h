#pragma once

#include <memory>

struct D3D12_RESOURCE_BARRIER;

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

            D3D12_RESOURCE_BARRIER& native();
        private:
            std::unique_ptr<D3D12_RESOURCE_BARRIER> m_barrier;
            const CommandList& m_commandList;
        };
    }
}

