#pragma once

#include <memory>
#include "engine/graphics/dx12/DX12Headers.h"
#include "tools/MemoryAllocator.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include "tools/RefCounted.h"

namespace engine
{
    class BufferCBV;
    namespace implementation
    {
        struct UploadTask
        {
            tools::ByteRange data;
            std::shared_ptr<BufferCBV> target;
        };

        namespace hidden
        {
            class DescriptorHandleData
            {
            public:
                DescriptorHandleData()
                    : m_allocator{ nullptr }
                    , m_handleMemory{ nullptr }
                    , handle{}
                    , gpuHandle{}
                    , handleCount{ 0 }
                {}

                DescriptorHandleData(
                    std::shared_ptr<tools::MemoryAllocator> allocator,
                    void* handleMemory,
                    size_t numDescriptors,
                    const D3D12_CPU_DESCRIPTOR_HANDLE& cpuStartHandle,
                    const D3D12_GPU_DESCRIPTOR_HANDLE& gpuStartHandle)
                    : m_allocator{ allocator }
                    , m_handleMemory{ handleMemory }
                    , handle{ reinterpret_cast<size_t>(handleMemory) }
                    , gpuHandle{ gpuStartHandle.ptr + (handle.ptr - cpuStartHandle.ptr) }
                    , handleCount{ numDescriptors }
                {}

                // handle move
                DescriptorHandleData(DescriptorHandleData&& _handle)
                    : m_allocator{ nullptr }
                    , m_handleMemory{ nullptr }
                    , handle{}
                    , gpuHandle{}
                    , handleCount{ 0 }
                {
                    std::swap(m_allocator, _handle.m_allocator);
                    std::swap(m_handleMemory, _handle.m_handleMemory);
                    std::swap(handle, _handle.handle);
                    std::swap(gpuHandle, _handle.gpuHandle);
                    std::swap(handleCount, _handle.handleCount);
                }

                DescriptorHandleData& operator=(DescriptorHandleData&& _handle)
                {
                    std::swap(m_allocator, _handle.m_allocator);
                    std::swap(m_handleMemory, _handle.m_handleMemory);
                    std::swap(handle, _handle.handle);
                    std::swap(gpuHandle, _handle.gpuHandle);
                    std::swap(handleCount, _handle.handleCount);
                }

                // disable copy
                DescriptorHandleData(const DescriptorHandleData&) = delete;
                DescriptorHandleData& operator=(const DescriptorHandleData&) = delete;

                ~DescriptorHandleData()
                {
                    ASSERT((m_allocator && m_handleMemory) || (!m_allocator && !m_handleMemory), "Corrupted Descriptor handle");
                    if (m_allocator && m_handleMemory)
                    {
                        m_allocator->free(m_handleMemory);
                        m_allocator = nullptr;
                        m_handleMemory = nullptr;
                        handle = {};
                        gpuHandle = {};
                        handleCount = 0;
                    }
                }
            private:
                std::shared_ptr<tools::MemoryAllocator> m_allocator;
                void* m_handleMemory;
            public:
                D3D12_CPU_DESCRIPTOR_HANDLE handle;
                D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
                size_t handleCount;
            };
        }

        class DescriptorHeapImpl;
        class DescriptorHandle
        {
        public:
            DescriptorHandle()
                : m_data{ nullptr }
            {}

            operator bool() const
            {
                return (bool)m_data;
            }

            size_t count() const
            {
                if(m_data)
                    return m_data->handleCount;
                return 0;
            }
            size_t handleSize() const
            {
                return m_descriptorSize;
            }
            const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle() const
            {
                return m_data->handle;
            }
            const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle() const
            {
                return m_data->gpuHandle;
            }
            D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle()
            {
                return m_data->handle;
            }
            D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle()
            {
                return m_data->gpuHandle;
            }
            uint64_t uniqueId() const
            {
                return m_uniqueId;
            }

        protected:
            friend class DescriptorHeapImpl;
            DescriptorHandle(
                std::shared_ptr<tools::MemoryAllocator> allocator,
                size_t numDescriptors,
                size_t descriptorSize,
                const D3D12_CPU_DESCRIPTOR_HANDLE& cpuStartHandle,
                const D3D12_GPU_DESCRIPTOR_HANDLE& gpuStartHandle)
                : m_data{
                std::make_shared<hidden::DescriptorHandleData>(
                //new hidden::DescriptorHandleData(
                    allocator,
                    allocator->allocate(
                        numDescriptors * descriptorSize,
                        4),
                    numDescriptors,
                    cpuStartHandle,
                    gpuStartHandle
                    ) }
                , m_descriptorSize{ descriptorSize }
                , m_uniqueId{ m_globalUniqueId++ }
            {
            }
        private:
            std::shared_ptr<hidden::DescriptorHandleData> m_data;
            //tools::RefCounted<hidden::DescriptorHandleData> m_data;
            size_t m_descriptorSize;
            uint64_t m_uniqueId;
            static uint64_t m_globalUniqueId;
        };

        

        class DescriptorHeapImpl;
        struct DescriptorHeaps
        {
            std::shared_ptr<DescriptorHeapImpl> cbv_srv_uav;
            std::shared_ptr<DescriptorHeapImpl> sampler;
            std::shared_ptr<DescriptorHeapImpl> rtv;
            std::shared_ptr<DescriptorHeapImpl> dsv;

            std::shared_ptr<DescriptorHeapImpl> shaderVisible_cbv_srv_uav;
            std::shared_ptr<DescriptorHeapImpl> shaderVisible_sampler;
        };
    }
}
