#pragma once

#include <memory>
#include "tools/ComPtr.h"
#include "engine/graphics/Common.h"
#include "engine/graphics/dx12/DX12Common.h"
#include "tools/MemoryAllocator.h"
#include "tools/RingBuffer.h"
#include "engine/graphics/Device.h"
#include <queue>

struct ID3D12DebugDevice;
extern ID3D12DebugDevice* debugInterface;

struct ID3D12Device;

#define DARKNESS_IID_PPV_ARGS IID_PPV_ARGS
#define D3D12_GPU_VIRTUAL_ADDRESS_NULL      ((D3D12_GPU_VIRTUAL_ADDRESS)0)
#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN   ((D3D12_GPU_VIRTUAL_ADDRESS)-1)

#undef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
#define UPLOADBUFFER_MEMORYALLOCATOR_RINGBUFFER

namespace platform
{
    class Window;
}

namespace engine
{
    namespace implementation
    {
        class CommandListImpl;
        class DescriptorHeapImpl;
        class CommandAllocatorImpl;
    }

    class Buffer;
    class BufferSRV;
    class BufferIBV;
    class BufferVBV;
    class BufferCBV;
    class Texture;
    class Queue;
    class Device;
    class CommandList;
    struct TextureDescription;

    enum class Format;

    namespace implementation
    {
        class CommandAllocatorImpl;
        class PipelineImpl;

        constexpr uint32_t UploadBufferSizeBytes = 1024u * 1024u * 100u;

        class DeviceImpl
        {
        public:
            DeviceImpl(std::shared_ptr<platform::Window> window);

            void nullResources(std::shared_ptr<NullResources> nullResources);
            NullResources& nullResources();

            ID3D12Device* device() const;
            std::shared_ptr<CommandAllocatorImpl> allocator() const;
            
            Texture createTexture(const Device& device, Queue& queue, const TextureDescription& desc);
            void uploadBuffer(CommandList& commandList, BufferSRV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandListImpl& commandList, BufferSRV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandList& commandList, BufferCBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandListImpl& commandList, BufferCBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandList& commandList, BufferIBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandListImpl& commandList, BufferIBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandList& commandList, BufferVBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandListImpl& commandList, BufferVBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);

            const platform::Window& window() const;
            int width() const;
            int height() const;

            void waitForIdle();
            DescriptorHeaps& heaps();
            const DescriptorHeaps& heaps() const;

            void recycleUploadBuffers(uint32_t frameNumber);

            std::shared_ptr<CommandAllocatorImpl> createCommandAllocator();
            void freeCommandAllocator(std::shared_ptr<CommandAllocatorImpl> allocator);

        private:
            tools::ComPtr<ID3D12Device> m_device;
            std::shared_ptr<platform::Window> m_window;
            DescriptorHeaps m_descriptorHeaps;
            std::shared_ptr<Buffer> m_uploadBuffer;
            std::shared_ptr<NullResources> m_nullResources;
            int m_currentHeap;
            
            friend class implementation::CommandListImpl;
            friend class implementation::PipelineImpl;

#ifdef UPLOADBUFFER_MEMORYALLOCATOR_RINGBUFFER
            tools::RingBuffer m_allocator;
#endif

#ifdef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
            tools::MemoryAllocator m_allocator;
            std::vector<std::shared_ptr<void>> m_currentUploadBufferList;
            std::queue<std::vector<std::shared_ptr<void>>> m_uploadBuffers;
#endif

            //do some kind of CommandAllocator list that DX12CommandList constructor can ask for
            std::queue<std::shared_ptr<CommandAllocatorImpl>> m_commandAllocators;
            std::queue<std::vector<std::shared_ptr<CommandAllocatorImpl>>> m_inFlightCommandAllocators; // these are not referenced by command buffers but can still be in flight
            
            std::vector<std::shared_ptr<CommandAllocatorImpl>> m_returnedCommandAllocators;
            std::vector<std::shared_ptr<CommandAllocatorImpl>> m_inUseCommandAllocators; // these are referenced by command buffers
        };
    }
}
