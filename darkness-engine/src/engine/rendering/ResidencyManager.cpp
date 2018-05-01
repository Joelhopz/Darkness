#include "engine/rendering/ResidencyManager.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/Queue.h"
#include "engine/graphics/Fence.h"
#include "engine/graphics/CommandList.h"
#include "tools/ByteRange.h"

#include <thread>
#include <chrono>
#include <future>

namespace engine
{
    MakeResidentTask::MakeResidentTask(Device& device, Buffer& uploadBuffer, UploadAllocation resource)
    {
        // eventually this will be threaded
        auto cmd = device.createCommandList(CommandListType::Copy);

        cmd.copyBuffer(
            uploadBuffer, 
            resource.buffer.valid() ? resource.buffer.buffer() : resource.indexBuffer.buffer(),
            resource.bytes,
            resource.srcElement,
            resource.gpuIndex);

        auto fence = device.createFence();
        device.copyQueue().submit(cmd, fence);
        fence.blockUntilSignaled();
    }

    ResidencyManager::ResidencyManager(Device& device)
        : m_device{ &device }
        , m_uploadBuffer{ device.createBuffer(BufferDescription()
            .elementSize(1)
            .elements(ResidencyUploadBufferSizeBytes)
            .usage(ResourceUsage::Upload)
            .name("ResidencyManager UploadBuffer")) }
        , m_relocationBuffer{ device.createBuffer(BufferDescription()
            .elementSize(1)
            .elements(ResidencyUploadBufferSizeBytes)
            .name("ResidencyManager UploadBuffer")) }

        , m_uploadMemory{ static_cast<uint8_t*>(m_uploadBuffer.map(device)) }
        , m_memoryInUse{ 0u }
    {}

    // TODO: ALL OF THIS MEMORY MANAGEMENT IS DODGY AT BEST
    //       IT MAKES A LOT OF ASSUMPTIONS ON THE WAY IT'S CALLED
    //       AND FREED. SHOULD REPLACE WITH REAL MEMORY MANAGEMENT!!
    UploadAllocation ResidencyManager::createUpdateAllocation(uint32_t bytes)
    {
        ASSERT(bytes < ResidencyUploadBufferSizeBytes, "Residency manager can't handle this big allocation: %u", bytes);

        while (bytes > ResidencyUploadBufferSizeBytes - m_memoryInUse)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        UploadAllocation res;
        res.bytes = bytes;
        res.ptr = m_uploadMemory + m_memoryInUse;
        res.srcElement = m_memoryInUse;
        m_memoryInUse += bytes;
        return res;
    }

    void ResidencyManager::makeResident(const UploadAllocation& resource)
    {
        m_tasks.emplace_back(MakeResidentTask{ *m_device, m_uploadBuffer, resource });
    }

    void ResidencyManager::freeUpdateAllocation(const UploadAllocation& resource)
    {
        m_memoryInUse -= resource.bytes;
    }

    Buffer& ResidencyManager::uploadBuffer()
    {
        return m_relocationBuffer;
    }

    Device& ResidencyManager::device()
    {
        return *m_device;
    }
}
