#include "engine/graphics/dx12/DX12Semaphore.h"
#include "engine/graphics/dx12/DX12Device.h"

#include "engine/graphics/Device.h"
#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        SemaphoreImpl::SemaphoreImpl(const Device& device)
            : m_fenceValue{ 0 }
        {
            ASSERT(SUCCEEDED(DeviceImplGet::impl(device).device()->CreateFence(
                0,
                D3D12_FENCE_FLAG_NONE,
                __uuidof(ID3D12Fence),
                (void**)m_fence.GetAddressOf())));

            m_fenceEvent = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);
            ASSERT(m_fenceEvent);
        }

        ID3D12Fence* SemaphoreImpl::native()
        {
            return m_fence.Get();
        }

        ID3D12Fence* SemaphoreImpl::native() const
        {
            return m_fence.Get();
        }

        void SemaphoreImpl::reset()
        {
        }

        bool SemaphoreImpl::signaled() const
        {
            return m_fence->GetCompletedValue() < m_fenceValue;
        }

        void SemaphoreImpl::blockUntilSignaled() const
        {
            if (m_fence->GetCompletedValue() < m_fenceValue)
            {
                ASSERT(SUCCEEDED(m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent)));
                WaitForSingleObject(m_fenceEvent, INFINITE);
            }
        }
    }
}
