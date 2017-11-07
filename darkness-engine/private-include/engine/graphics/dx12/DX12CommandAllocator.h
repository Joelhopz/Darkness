#pragma once

#include "engine/graphics/dx12/DX12Headers.h"
#include "tools/ComPtr.h"

namespace engine
{
    namespace implementation
    {
        class DeviceImpl;

        class CommandAllocatorImpl
        {
        public:
            CommandAllocatorImpl(const DeviceImpl& device);

            void reset();
            ID3D12CommandAllocator* native() const;
        private:
            tools::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
        };
    }
}

