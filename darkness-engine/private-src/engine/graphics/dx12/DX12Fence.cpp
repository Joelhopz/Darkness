#include "engine/graphics/dx12/DX12Fence.h"
#include "engine/graphics/dx12/DX12Headers.h"

#include "engine/graphics/Device.h"
#include "engine/graphics/dx12/DX12Device.h"

#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        FenceImpl::FenceImpl(const DeviceImpl& device)
            : m_fenceValue{ 0 }
        {
            auto res = device.device()->CreateFence(
                m_fenceValue,
                D3D12_FENCE_FLAG_NONE,
                __uuidof(ID3D12Fence),
                (void**)&m_fence);
            ASSERT(SUCCEEDED(res));

            m_fenceEvent = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);
            ASSERT(m_fenceEvent);
        }

        FenceImpl::~FenceImpl()
        {
            ASSERT(CloseHandle(m_fenceEvent) == TRUE);

            if (m_fence)
            {
                m_fence->Release();
                m_fence = NULL;
            }
        }

        ID3D12Fence* FenceImpl::native()
        {
            return m_fence;
        }

        ID3D12Fence* FenceImpl::native() const
        {
            return m_fence;
        }

        void FenceImpl::reset()
        {
            m_fenceValue = 0;
        }

        bool FenceImpl::signaled() const
        {
            return false;
        }

        unsigned long long FenceImpl::nextValue()
        {
            ++m_fenceValue;
            return m_fenceValue;
        }

        unsigned long long FenceImpl::value() const
        {
            return m_fenceValue;
        }

        void FenceImpl::blockUntilSignaled()
        {
            // advance one
            auto currentValue = m_fence->GetCompletedValue();
            while (currentValue < m_fenceValue)
            {
                ASSERT(SUCCEEDED(m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent)));
                WaitForSingleObject(m_fenceEvent, INFINITE);
                currentValue = m_fence->GetCompletedValue();
            }
        }

        void FenceImpl::blockUntilSignaled(unsigned long long value)
        {
            auto currentValue = m_fence->GetCompletedValue();
            while (currentValue < value)
            {
                ASSERT(SUCCEEDED(m_fence->SetEventOnCompletion(value, m_fenceEvent)));
                WaitForSingleObject(m_fenceEvent, INFINITE);
                currentValue = m_fence->GetCompletedValue();
            }
        }
    }
}
