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
            auto createRes = DeviceImplGet::impl(device).device()->CreateFence(
                0,
                D3D12_FENCE_FLAG_NONE,
                DARKNESS_IID_PPV_ARGS(m_fence.GetAddressOf()));
            ASSERT(SUCCEEDED(createRes));

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
                auto res = m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent);
                ASSERT(SUCCEEDED(res));
                WaitForSingleObject(m_fenceEvent, INFINITE);
            }
        }
    }
}
