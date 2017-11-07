#include "engine/graphics/dx12/DX12CommandAllocator.h"
#include "engine/graphics/dx12/DX12Headers.h"

#include "engine/graphics/Device.h"
#include "engine/graphics/dx12/DX12Device.h"

#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        CommandAllocatorImpl::CommandAllocatorImpl(const DeviceImpl& device)
        {
            auto res = device.device()->CreateCommandAllocator(
                D3D12_COMMAND_LIST_TYPE_DIRECT,
                __uuidof(ID3D12CommandAllocator),
                (void**)&m_commandAllocator);
            ASSERT(SUCCEEDED(res));
        }

        ID3D12CommandAllocator* CommandAllocatorImpl::native() const
        {
            return m_commandAllocator.Get();
        }

        void CommandAllocatorImpl::reset()
        {
            auto res = m_commandAllocator->Reset();
            ASSERT(SUCCEEDED(res));
        }
    }
}
