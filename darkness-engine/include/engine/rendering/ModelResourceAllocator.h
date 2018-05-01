#pragma once

#include "tools/ByteRange.h"
#include "tools/MemoryAllocator.h"
#include "engine/graphics/Resources.h"
#include <vector>

namespace engine
{
    class Device;

    struct ModelResourceAllocation
    {
        void* ptr;
        uint32_t gpuIndex;
        uint32_t elements;
        tools::ByteRange range;
    };

    class ModelResourceAllocator
    {
    public:
        ModelResourceAllocator(Device& device, uint32_t maxElements, std::vector<Format> formats, const char* resourceName, bool indexBuffer = false);
        ModelResourceAllocator(Device& device, uint32_t maxElements, std::vector<size_t> elementSize, const char* resourceName);
        ModelResourceAllocator(Device& device, uint32_t maxElements, std::vector<Format> formats, std::vector<size_t> elementSize, const char* resourceName);
        std::vector<BufferSRV>& gpuBuffers();
        std::vector<BufferIBV>& gpuIndexBuffers();

        ModelResourceAllocation allocate(uint32_t elements);
        void free(ModelResourceAllocation allocation);
        void free(void* allocation);
        void free(uint32_t allocation);

    private:
        std::vector<BufferSRV> m_buffers;
        std::vector<BufferIBV> m_indexBuffers;
        tools::MemoryAllocator m_allocator;
    };

    class ModelResourceLinearAllocator
    {
    public:
        struct ResourceBufferSettingsFormat
        {
            Format format = Format::UNKNOWN;
            ResourceUsage usage = ResourceUsage::GpuRead;
        };

        struct ResourceBufferSettingsStructured
        {
            size_t elementSize = 0;
            ResourceUsage usage = ResourceUsage::GpuRead;
        };

        ModelResourceLinearAllocator(
            Device& device, 
            uint32_t maxElements, 
            std::vector<ResourceBufferSettingsFormat> bufferSettings, 
            std::vector<ResourceBufferSettingsStructured> structuredSettings,
            const char* resourceName);

        std::vector<BufferSRV>& gpuBuffers();

        ModelResourceAllocation allocate(uint32_t elements);
        void free(ModelResourceAllocation allocation);
        void free(void* allocation);
        void free(uint32_t allocation);

    private:
        std::vector<BufferSRV> m_buffers;
        uint32_t m_currentOffset;
    };
}
