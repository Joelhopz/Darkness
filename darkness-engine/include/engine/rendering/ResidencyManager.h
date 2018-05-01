#pragma once

#include "engine/rendering/ModelResourceAllocator.h"
#include <vector>

namespace engine
{
    class Device;

    struct UploadAllocation
    {
        uint8_t* ptr;
        uint32_t bytes;
        uint32_t srcElement;
        uint32_t gpuIndex;
        BufferSRV buffer;
        BufferIBV indexBuffer;
    };

    struct RelocateAllocation
    {
        Buffer srcBuffer;
        Buffer dstBuffer;
        uint32_t fromIndex;
        uint32_t toIndex;
    };

    struct ModelResource
    {
        bool allocated = false;
        ModelResourceAllocation modelResource;
        std::vector<UploadAllocation> uploads;
    };

    class MakeResidentTask
    {
    public:
        MakeResidentTask(Device& device, Buffer& uploadBuffer, UploadAllocation resource);
    };

    constexpr uint32_t ResidencyUploadBufferSizeBytes = 1024u * 1024u * 100u;

    class ResidencyManager
    {
    public:
        ResidencyManager(Device& device);
        UploadAllocation createUpdateAllocation(uint32_t bytes);
        void makeResident(const UploadAllocation& resource);
        void freeUpdateAllocation(const UploadAllocation& resource);

        Buffer& uploadBuffer();
        Device& device();
    private:
        Device* m_device;
        Buffer m_uploadBuffer;
        Buffer m_relocationBuffer;
        uint8_t* m_uploadMemory;
        uint32_t m_memoryInUse;
        std::vector<MakeResidentTask> m_tasks;
    };
}
