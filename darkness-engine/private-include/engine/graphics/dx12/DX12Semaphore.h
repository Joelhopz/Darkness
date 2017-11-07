#pragma once

#include "engine/graphics/dx12/DX12Headers.h"
#include "tools/ComPtr.h"

namespace engine
{
    class Device;
    namespace implementation
    {
        class SemaphoreImpl
        {
        public:
            SemaphoreImpl(const Device& device);

            ID3D12Fence* native();
            ID3D12Fence* native() const;

            void reset();
            bool signaled() const;
            void blockUntilSignaled() const;
        private:
            tools::ComPtr<ID3D12Fence> m_fence;
            void* m_fenceEvent;
            unsigned long long m_fenceValue;
        };
    }
}

