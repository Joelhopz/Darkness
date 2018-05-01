#include "engine/graphics/dx12/DX12CommandAllocator.h"
#include "engine/graphics/dx12/DX12Headers.h"
#include "engine/graphics/dx12/DX12Conversions.h"

#include "engine/graphics/Device.h"
#include "engine/graphics/dx12/DX12Device.h"

#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        CommandAllocatorImpl::CommandAllocatorImpl(const DeviceImpl& device, CommandListType type)
            : m_type{ type }
        {
            auto res = device.device()->CreateCommandAllocator(
                dxCommandListType(type),
                DARKNESS_IID_PPV_ARGS(m_commandAllocator.GetAddressOf()));
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

        CommandListType CommandAllocatorImpl::type() const
        {
            return m_type;
        }
    }
}
