#pragma once

#include "tools/ComPtr.h"
#include "tools/MemoryAllocator.h"
#include "engine/graphics/dx12/DX12Common.h"
#include <cstdint>
#include <memory>

struct ID3D12DescriptorHeap;
enum D3D12_DESCRIPTOR_HEAP_TYPE;
enum D3D12_DESCRIPTOR_HEAP_FLAGS;
struct D3D12_CPU_DESCRIPTOR_HANDLE;
struct D3D12_GPU_DESCRIPTOR_HANDLE;
struct D3D12_DESCRIPTOR_HEAP_DESC;

namespace engine
{
    class DescriptorHandle;
    class RootSignature;
    class Buffer;
    class Sampler;
    class BufferView;
    class TextureSRV;
    namespace shaders
    {
        class PipelineConfiguration;
    }

    namespace implementation
    {
        

        class DeviceImpl;
        class DescriptorHeapImpl
        {
        public:
            DescriptorHeapImpl(
                const DeviceImpl& device, 
                D3D12_DESCRIPTOR_HEAP_TYPE type,
                D3D12_DESCRIPTOR_HEAP_FLAGS flags,
                uint32_t numDescriptors);

            D3D12_DESCRIPTOR_HEAP_TYPE type() const;

            DescriptorHandle getDescriptor(size_t count = 1);

            void reset();
            ID3D12DescriptorHeap* native();
        private:
            std::shared_ptr<tools::MemoryAllocator> m_allocator;
            tools::ComPtr<ID3D12DescriptorHeap> m_heap;
            std::shared_ptr<D3D12_DESCRIPTOR_HEAP_DESC> m_desc;
            D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandle;
            D3D12_GPU_DESCRIPTOR_HANDLE m_gpuHandle;
            size_t m_descriptorSize;

            size_t m_numdescriptors;
            size_t availableCPUDescriptors;
            size_t availableGPUDescriptors;
        };
    }
}
