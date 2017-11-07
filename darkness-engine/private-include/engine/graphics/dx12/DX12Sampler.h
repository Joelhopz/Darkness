#pragma once

#include "engine/graphics/dx12/DX12Headers.h"
#include "engine/graphics/dx12/DX12Common.h"
#include <memory>

namespace engine
{
    struct SamplerDescription;
    class Device;

    namespace implementation
    {
        class SamplerImpl
        {
        public:
            SamplerImpl(
                const Device& device,
                const SamplerDescription& desc);
            
            D3D12_CPU_DESCRIPTOR_HANDLE& native();
            const D3D12_CPU_DESCRIPTOR_HANDLE& native() const;

            uint64_t uniqueId() const
            {
                return m_descriptorHandle.uniqueId();
            }
        private:
            DescriptorHandle m_descriptorHandle;
        };
    }
}

