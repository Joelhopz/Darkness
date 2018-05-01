#include "engine/graphics/dx12/DX12Headers.h"

#include "engine/graphics/dx12/DX12DescriptorHeap.h"
#include "engine/graphics/dx12/DX12Device.h"
#include "engine/graphics/dx12/DX12Conversions.h"

#include "engine/graphics/Device.h"
#include "tools/Debug.h"

using namespace tools;

namespace engine
{
    namespace implementation
    {
        DescriptorHeapImpl::DescriptorHeapImpl(
            const DeviceImpl& device,
            D3D12_DESCRIPTOR_HEAP_TYPE type,
            D3D12_DESCRIPTOR_HEAP_FLAGS flags,
            uint32_t numDescriptors)
            : m_allocator{ nullptr }
            , m_heap{}
            , m_desc{ std::make_shared<D3D12_DESCRIPTOR_HEAP_DESC>() }
            , m_cpuHandle{ }
            , m_gpuHandle{ }
            , m_descriptorSize{ 0 }
            , availableCPUDescriptors{ numDescriptors }
            , availableGPUDescriptors{ numDescriptors }
            , m_numdescriptors{ numDescriptors }
        {
            memset(m_desc.get(), 0, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
            m_desc->NumDescriptors = numDescriptors;
            m_desc->Type = type;
            m_desc->Flags = flags;
            m_desc->NodeMask = 0; // multi-adapter support

            auto res = device.device()->CreateDescriptorHeap(
                m_desc.get(),
                DARKNESS_IID_PPV_ARGS(m_heap.GetAddressOf()));
            ASSERT(SUCCEEDED(res));

            m_descriptorSize = device.device()->GetDescriptorHandleIncrementSize(type);
            m_cpuHandle = m_heap->GetCPUDescriptorHandleForHeapStart();
            m_gpuHandle = m_heap->GetGPUDescriptorHandleForHeapStart();

            m_allocator = std::make_shared<tools::MemoryAllocator>(
                ByteRange(m_cpuHandle.ptr, m_cpuHandle.ptr + (m_descriptorSize * m_numdescriptors)), 1);
        }

        D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapImpl::getCpuHeapStart()
        {
            return m_cpuHandle;
        }

        DescriptorHandle DescriptorHeapImpl::getDescriptor(size_t count)
        {
            return DescriptorHandle(m_allocator, count, m_descriptorSize, m_cpuHandle, m_gpuHandle);
        }

        D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapImpl::type() const
        {
            return m_desc->Type;
        }

        void DescriptorHeapImpl::reset()
        {
            m_allocator = std::make_shared<tools::MemoryAllocator>(
                ByteRange(m_cpuHandle.ptr, m_cpuHandle.ptr + (m_descriptorSize * m_numdescriptors)), 1);
        }

        ID3D12DescriptorHeap* DescriptorHeapImpl::native()
        {
            return m_heap.Get();
        }
    }
}
