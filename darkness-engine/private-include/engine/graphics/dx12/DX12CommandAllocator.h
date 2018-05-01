#pragma once

#include "engine/graphics/dx12/DX12Headers.h"
#include "engine/graphics/CommonNoDep.h"
#include "tools/ComPtr.h"

namespace engine
{
    namespace implementation
    {
        class DeviceImpl;
        class CommandAllocatorImpl
        {
        public:
            CommandAllocatorImpl(const DeviceImpl& device, CommandListType type);

            void reset();
            ID3D12CommandAllocator* native() const;
            CommandListType type() const;
        private:
            tools::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
            CommandListType m_type;
        };
    }
}

