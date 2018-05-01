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
    enum class CommandListType;

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
            void uploadBuffer(CommandList& commandList, BufferUAV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandListImpl& commandList, BufferUAV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandList& commandList, BufferCBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandListImpl& commandList, BufferCBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandList& commandList, BufferIBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandListImpl& commandList, BufferIBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandList& commandList, BufferVBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandListImpl& commandList, BufferVBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);

            void uploadRawBuffer(CommandListImpl& commandList, Buffer& buffer, const tools::ByteRange& data, uint32_t startBytes);

            const platform::Window& window() const;
            int width() const;
            int height() const;

            void waitForIdle();
            DescriptorHeaps& heaps();
            const DescriptorHeaps& heaps() const;

            void recycleUploadBuffers(uint32_t frameNumber);

            std::shared_ptr<CommandAllocatorImpl> createCommandAllocator(CommandListType type);
            void freeCommandAllocator(std::shared_ptr<CommandAllocatorImpl> allocator);

            ID3D12CommandSignature* drawIndirectSignature();
            ID3D12CommandSignature* drawIndexedIndirectSignature();
            ID3D12CommandSignature* dispatchIndirectSignature();

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

            std::vector<DescriptorHandle> m_frameDescriptors;
            std::queue<std::vector<DescriptorHandle>> m_frameDescriptorQueue;

            //do some kind of CommandAllocator list that DX12CommandList constructor can ask for
            std::queue<std::shared_ptr<CommandAllocatorImpl>> m_commandAllocators[4];
            std::queue<std::vector<std::shared_ptr<CommandAllocatorImpl>>> m_inFlightCommandAllocators[4]; // these are not referenced by command buffers but can still be in flight
            
            std::vector<std::shared_ptr<CommandAllocatorImpl>> m_returnedCommandAllocators[4];
            std::vector<std::shared_ptr<CommandAllocatorImpl>> m_inUseCommandAllocators[4]; // these are referenced by command buffers

            tools::ComPtr<ID3D12CommandSignature> m_drawSignature;
            tools::ComPtr<ID3D12CommandSignature> m_drawIndexedSignature;
            tools::ComPtr<ID3D12CommandSignature> m_dispatchSignature;

            /*void keepConstantUpdatesAlive(std::vector<ConstantBufferUpdates>&& constantBufferUpdates);
            std::vector<std::vector<ConstantBufferUpdates>> m_currentConstantBufferUpdates;
            std::vector<std::vector<std::vector<ConstantBufferUpdates>>> m_constantBufferUpdates;*/
        };
    }
}
