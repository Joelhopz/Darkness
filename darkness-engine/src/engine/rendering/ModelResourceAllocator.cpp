#include "engine/rendering/ModelResourceAllocator.h"
#include "engine/graphics/Device.h"
#include <algorithm>

namespace engine
{
    ModelResourceAllocator::ModelResourceAllocator(Device& device, uint32_t maxElements, std::vector<Format> formats, const char* resourceName, bool indexBuffer)
        : m_allocator{ tools::MemoryAllocator{ tools::ByteRange{
        static_cast<unsigned long long>(0u),
        static_cast<unsigned long long>(maxElements) }, 1u } }
    {
        if (!indexBuffer)
        {
            for (auto&& format : formats)
            {
                m_buffers.emplace_back(device.createBufferSRV(BufferDescription()
                    .usage(ResourceUsage::GpuRead)
                    .format(format)
                    .name(resourceName)
                    .elements(maxElements)
                    .elementSize(formatBytes(format))));
            }
        }
        else
        {
            for (auto&& format : formats)
            {
                m_indexBuffers.emplace_back(device.createBufferIBV(BufferDescription()
                    .usage(ResourceUsage::GpuRead)
                    .format(format)
                    .name(resourceName)
                    .elements(maxElements)
                    .elementSize(formatBytes(format))));
            }
        }
    }

    ModelResourceAllocator::ModelResourceAllocator(Device& device, uint32_t maxElements, std::vector<size_t> elementSize, const char* resourceName)
        : m_allocator{ tools::MemoryAllocator{ tools::ByteRange{
        static_cast<unsigned long long>(0u),
        static_cast<unsigned long long>(maxElements) }, 1u } }
    {
        for (auto&& size : elementSize)
        {
            m_buffers.emplace_back(device.createBufferSRV(BufferDescription()
                .usage(ResourceUsage::GpuRead)
                .name(resourceName)
                .structured(true)
                .elements(maxElements)
                .elementSize(size)));
        }
    }

    ModelResourceAllocator::ModelResourceAllocator(Device& device, uint32_t maxElements, std::vector<Format> formats, std::vector<size_t> elementSize, const char* resourceName)
        : m_allocator{ tools::MemoryAllocator{ tools::ByteRange{
        static_cast<unsigned long long>(0u),
        static_cast<unsigned long long>(maxElements) }, 1u } }
    {
        for (auto&& format : formats)
        {
            m_buffers.emplace_back(device.createBufferSRV(BufferDescription()
                .usage(ResourceUsage::GpuRead)
                .format(format)
                .name(resourceName)
                .elements(maxElements)
                .elementSize(formatBytes(format))));
        }
        for (auto&& size : elementSize)
        {
            m_buffers.emplace_back(device.createBufferSRV(BufferDescription()
                .usage(ResourceUsage::GpuRead)
                .name(resourceName)
                .structured(true)
                .elements(maxElements)
                .elementSize(size)));
        }
    }

    std::vector<BufferSRV>& ModelResourceAllocator::gpuBuffers()
    {
        return m_buffers;
    }

    std::vector<BufferIBV>& ModelResourceAllocator::gpuIndexBuffers()
    {
        return m_indexBuffers;
    }

    ModelResourceAllocation ModelResourceAllocator::allocate(uint32_t elements)
    {
        ModelResourceAllocation res;
        res.ptr = m_allocator.allocate(elements);
        auto offset = m_allocator.offset(res.ptr);
        res.gpuIndex = static_cast<uint32_t>(offset);
        res.elements = elements;
        res.range = tools::ByteRange(offset, offset + elements);
        return res;
    }

    void ModelResourceAllocator::free(ModelResourceAllocation allocation)
    {
        m_allocator.free(allocation.ptr);
    }

    void ModelResourceAllocator::free(void* allocation)
    {
        m_allocator.free(allocation);
    }

    void ModelResourceAllocator::free(uint32_t allocation)
    {
        m_allocator.free(m_allocator.ptrFromOffset(allocation));
    }

    // ------------------------------------------------------------------

    ModelResourceLinearAllocator::ModelResourceLinearAllocator(
        Device& device, 
        uint32_t maxElements, 
        std::vector<ResourceBufferSettingsFormat> bufferSettings,
        std::vector<ResourceBufferSettingsStructured> structuredSettings,
        const char* resourceName)
        : m_currentOffset{ 0u }
    {
        for (auto&& settings : bufferSettings)
        {
            m_buffers.emplace_back(device.createBufferSRV(BufferDescription()
                .usage(settings.usage)
                .format(settings.format)
                .name(resourceName)
                .elements(maxElements)
                .elementSize(formatBytes(settings.format))));
        }
        for (auto&& structured : structuredSettings)
        {
            m_buffers.emplace_back(device.createBufferSRV(BufferDescription()
                .usage(structured.usage)
                .name(resourceName)
                .structured(true)
                .elements(maxElements)
                .elementSize(structured.elementSize)));
        }
    }

    std::vector<BufferSRV>& ModelResourceLinearAllocator::gpuBuffers()
    {
        return m_buffers;
    }

    ModelResourceAllocation ModelResourceLinearAllocator::allocate(uint32_t elements)
    {
        ASSERT(elements == 1, "This allocator supports only element count 1");
        ModelResourceAllocation res;
        res.ptr = reinterpret_cast<void*>(m_currentOffset);
        res.gpuIndex = m_currentOffset;
        res.elements = elements;
        res.range = tools::ByteRange(m_currentOffset, m_currentOffset + elements);
        m_currentOffset += elements;
        return res;
    }

    void ModelResourceLinearAllocator::free(ModelResourceAllocation allocation)
    {
        ASSERT(reinterpret_cast<uint32_t>(allocation.ptr) == m_currentOffset-1, "This allocator supports only freeing from end");
        m_currentOffset -= allocation.elements;
    }

    void ModelResourceLinearAllocator::free(void* allocation)
    {
        ASSERT(reinterpret_cast<uint32_t>(allocation) == m_currentOffset-1, "This allocator supports only freeing from end");
        --m_currentOffset;
    }

    void ModelResourceLinearAllocator::free(uint32_t allocation)
    {
        ASSERT(allocation == m_currentOffset-1, "This allocator supports only freeing from end");
        --m_currentOffset;
    }
}
