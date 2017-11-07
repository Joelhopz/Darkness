#include "engine/graphics/dx12/DX12Barrier.h"
#include "engine/graphics/dx12/DX12Headers.h"
#include "engine/graphics/Barrier.h"
#include "engine/graphics/CommandList.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/dx12/DX12CommandList.h"
#include "engine/graphics/dx12/DX12Resources.h"
#include "engine/graphics/dx12/DX12Conversions.h"

#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        BarrierImpl::BarrierImpl(
            const CommandList& commandList,
            ResourceBarrierFlags flags,
            const TextureRTV& resource,
            ResourceState before,
            ResourceState after,
            unsigned int subResource,
            const Semaphore& /*waitSemaphore*/,
            const Semaphore& /*signalSemaphore*/)
            : m_barrier{ std::make_unique<D3D12_RESOURCE_BARRIER>() }
            , m_commandList{ commandList }
        {
            D3D12_RESOURCE_BARRIER& barrier = *m_barrier;
            barrier.Flags = dxFlags(flags);
            barrier.Transition.pResource = TextureImplGet::impl(TextureRTVImplGet::impl(resource)->texture())->native();
            barrier.Transition.StateBefore = dxResourceStates(before);
            barrier.Transition.StateAfter = dxResourceStates(after);
            barrier.Transition.Subresource = subResource;
            barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

            CommandListImplGet::impl(m_commandList).native()->ResourceBarrier(1, m_barrier.get());
        }

        void BarrierImpl::update(
            ResourceState before,
            ResourceState after)
        {
            D3D12_RESOURCE_BARRIER& barrier = *m_barrier;
            barrier.Transition.StateBefore = dxResourceStates(before);
            barrier.Transition.StateAfter = dxResourceStates(after);
            
            CommandListImplGet::impl(m_commandList).native()->ResourceBarrier(1, m_barrier.get());
        }

        BarrierImpl::~BarrierImpl()
        {
        }

        D3D12_RESOURCE_BARRIER& BarrierImpl::native()
        {
            return *m_barrier;
        }
    }
}
