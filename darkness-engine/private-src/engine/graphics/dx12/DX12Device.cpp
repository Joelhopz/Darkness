#include "engine/graphics/dx12/DX12Device.h"
#include "engine/graphics/dx12/DX12Headers.h"
#include "engine/graphics/dx12/DX12Resources.h"
#include "engine/graphics/dx12/DX12CommandList.h"
#include "engine/graphics/dx12/DX12DescriptorHandle.h"
#include "engine/graphics/dx12/DX12Debug.h"
#include "engine/graphics/dx12/DX12DescriptorHeap.h"
#include "engine/graphics/dx12/DX12CommandAllocator.h"

#include "engine/graphics/Resources.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/Fence.h"
#include "engine/graphics/Queue.h"
#include "engine/graphics/Barrier.h"
#include "engine/graphics/CommandList.h"
#include "platform/window/WindowsWindow.h"
#include "tools/Debug.h"

using namespace platform::implementation;
using namespace tools;

namespace engine
{
    namespace implementation
    {
        DeviceImpl::DeviceImpl(std::shared_ptr<platform::Window> window)
            : m_device{}
            , m_window{ window }
            , m_currentHeap{ 0 }
        {
            D3D_FEATURE_LEVEL featureLevel{ D3D_FEATURE_LEVEL_12_1 };

            IDXGIFactory * pFactory;
            HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));

            UINT i = 0;
            IDXGIAdapter * pAdapter;
            std::vector <IDXGIAdapter*> vAdapters;
            while (pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
            {
                vAdapters.push_back(pAdapter);

                DXGI_ADAPTER_DESC desc;
                pAdapter->GetDesc(&desc);
                ++i;
            }

            ASSERT(SUCCEEDED(D3D12CreateDevice(
                vAdapters[0],
                featureLevel,
                __uuidof(ID3D12Device),
                (void**)&m_device)));

            GraphicsDebug::addDevice(m_device.Get());

            m_descriptorHeaps.cbv_srv_uav = std::make_shared<DescriptorHeapImpl>(
                *this,
                D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
                D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
                50000);

            m_descriptorHeaps.sampler = std::make_shared<DescriptorHeapImpl>(
                *this,
                D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
                D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
                10000);

            m_descriptorHeaps.rtv = std::make_shared<DescriptorHeapImpl>(
                *this,
                D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
                D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
                2000);

            m_descriptorHeaps.dsv = std::make_shared<DescriptorHeapImpl>(
                *this,
                D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
                D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
                100);

            m_descriptorHeaps.shaderVisible_cbv_srv_uav = std::make_shared<DescriptorHeapImpl>(
                *this,
                D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
                D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
                100000);

            m_descriptorHeaps.shaderVisible_sampler = std::make_shared<DescriptorHeapImpl>(
                *this,
                D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
                D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
                2048);
            
            m_uploadBuffer = std::make_shared<Buffer>(std::make_shared<BufferImpl>(
                *this,
                BufferDescription()
                .elementSize(1)
                .elements(UploadBufferSizeBytes)
                .name("UploadBuffer")));

            auto ptr = BufferImplGet::impl(*m_uploadBuffer)->map(*this);

#ifdef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
            m_allocator = MemoryAllocator(ByteRange{ static_cast<uint8_t*>(ptr), static_cast<uint8_t*>(ptr) + UploadBufferSizeBytes }, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
#endif

#ifdef UPLOADBUFFER_MEMORYALLOCATOR_RINGBUFFER
            m_allocator = RingBuffer(
                ByteRange{ 
                    static_cast<uint8_t*>(ptr), 
                    static_cast<uint8_t*>(ptr) + UploadBufferSizeBytes });
#endif

        }

        ID3D12Device* DeviceImpl::device() const
        {
            return m_device.Get();
        }

        std::shared_ptr<CommandAllocatorImpl> DeviceImpl::createCommandAllocator()
        {
            if (m_commandAllocators.size() > 0)
            {
                auto res = m_commandAllocators.front();
                m_commandAllocators.pop();
                m_inUseCommandAllocators.emplace_back(res);
                return res;
            }

            m_inUseCommandAllocators.emplace_back(std::make_shared<CommandAllocatorImpl>(*this));
            return m_inUseCommandAllocators.back();
        }

        void DeviceImpl::freeCommandAllocator(std::shared_ptr<CommandAllocatorImpl> allocator)
        {
            m_inUseCommandAllocators.erase(std::find(m_inUseCommandAllocators.begin(), m_inUseCommandAllocators.end(), allocator));
            m_returnedCommandAllocators.emplace_back(allocator);
        }

        const platform::Window& DeviceImpl::window() const
        {
            return *m_window;
        }

        int DeviceImpl::width() const
        {
            return WindowImplGet::impl(*m_window).width();
        }

        int DeviceImpl::height() const
        {
            return WindowImplGet::impl(*m_window).height();
        }

        SIZE_T Align(SIZE_T uLocation, SIZE_T uAlign)
        {
            if ((0 == uAlign) || (uAlign & (uAlign - 1)))
            {
                ASSERT(false, "Could not allocate upload buffer");
            }

            return ((uLocation + (uAlign - 1)) & ~(uAlign - 1));
        }

        void DeviceImpl::recycleUploadBuffers(uint32_t /*frameNumber*/)
        {
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
            // handle upload buffer releases
            if (m_uploadBuffers.size() > BackBufferCount)
                m_uploadBuffers.pop();

            m_uploadBuffers.push(m_currentUploadBufferList);
            m_currentUploadBufferList.clear();
#endif

            // handle command allocators
            if (m_inFlightCommandAllocators.size() > BackBufferCount)
            {
                std::vector<std::shared_ptr<CommandAllocatorImpl>>& toBeFreed = m_inFlightCommandAllocators.front();
                for (auto&& free : toBeFreed)
                {
                    free->reset();
                    m_commandAllocators.push(free);
                }
                m_inFlightCommandAllocators.pop();
            }
            m_inFlightCommandAllocators.push(m_returnedCommandAllocators);
            m_returnedCommandAllocators.clear();
        }

        void DeviceImpl::nullResources(std::shared_ptr<NullResources> nullResources)
        {
            m_nullResources = nullResources;
        }

        NullResources& DeviceImpl::nullResources()
        {
            return *m_nullResources;
        }

        void DeviceImpl::uploadBuffer(CommandList& commandList, BufferSRV& buffer, const ByteRange& data, uint32_t startElement)
        {
            uploadBuffer(CommandListImplGet::impl(commandList), buffer, data, startElement);
        }

        void DeviceImpl::uploadBuffer(CommandListImpl& commandList, BufferSRV& buffer, const ByteRange& data, uint32_t startElement)
        {
            auto allocator = &m_allocator;
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
            std::shared_ptr<void> uploadData = std::shared_ptr<void>(
                m_allocator.allocate(data.size(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT),
                [allocator](void* ptr) { allocator->free(ptr); });
            m_currentUploadBufferList.emplace_back(uploadData);
            memcpy(uploadData.get(), data.start, data.size());

            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferSRVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferSRVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData.get()), data.size());
#endif

#ifdef UPLOADBUFFER_MEMORYALLOCATOR_RINGBUFFER
            auto uploadData = m_allocator.allocate(data.size(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
            memcpy(uploadData, data.start, data.size());
            commandList.transition(buffer.buffer(), ResourceState::CopyDest);
            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferSRVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferSRVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData), data.size());
#endif
        }

        void DeviceImpl::uploadBuffer(CommandList& commandList, BufferCBV& buffer, const ByteRange& data, uint32_t startElement)
        {
            uploadBuffer(CommandListImplGet::impl(commandList), buffer, data, startElement);
        }

        void DeviceImpl::uploadBuffer(CommandListImpl& commandList, BufferCBV& buffer, const ByteRange& data, uint32_t startElement)
        {
            auto allocator = &m_allocator;
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
            std::shared_ptr<void> uploadData = std::shared_ptr<void>(
                m_allocator.allocate(data.size(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT),
                [allocator](void* ptr) { allocator->free(ptr); });
            m_currentUploadBufferList.emplace_back(uploadData);
            memcpy(uploadData.get(), data.start, data.size());

            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferCBVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferCBVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData.get()), data.size());
#endif

#ifdef UPLOADBUFFER_MEMORYALLOCATOR_RINGBUFFER
            auto uploadData = m_allocator.allocate(data.size(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
            memcpy(uploadData, data.start, data.size());
            commandList.transition(buffer.buffer(), ResourceState::CopyDest);
            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferCBVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferCBVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData), data.size());
#endif
            
        }

        void DeviceImpl::uploadBuffer(CommandList& commandList, BufferIBV& buffer, const ByteRange& data, uint32_t startElement)
        {
            uploadBuffer(CommandListImplGet::impl(commandList), buffer, data, startElement);
        }

        void DeviceImpl::uploadBuffer(CommandListImpl& commandList, BufferIBV& buffer, const ByteRange& data, uint32_t startElement)
        {
            auto allocator = &m_allocator;
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
            std::shared_ptr<void> uploadData = std::shared_ptr<void>(
                m_allocator.allocate(data.size(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT),
                [allocator](void* ptr) { allocator->free(ptr); });
            m_currentUploadBufferList.emplace_back(uploadData);
            memcpy(uploadData.get(), data.start, data.size());

            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferIBVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferIBVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData.get()), data.size());
#endif

#ifdef UPLOADBUFFER_MEMORYALLOCATOR_RINGBUFFER
            auto uploadData = m_allocator.allocate(data.size(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
            memcpy(uploadData, data.start, data.size());
            commandList.transition(buffer.buffer(), ResourceState::CopyDest);
            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferIBVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferIBVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData), data.size());
#endif
        }

        void DeviceImpl::uploadBuffer(CommandList& commandList, BufferVBV& buffer, const ByteRange& data, uint32_t startElement)
        {
            uploadBuffer(CommandListImplGet::impl(commandList), buffer, data, startElement);
        }

        void DeviceImpl::uploadBuffer(CommandListImpl& commandList, BufferVBV& buffer, const ByteRange& data, uint32_t startElement)
        {
            auto allocator = &m_allocator;
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
            std::shared_ptr<void> uploadData = std::shared_ptr<void>(
                m_allocator.allocate(data.size(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT),
                [allocator](void* ptr) { allocator->free(ptr); });
            m_currentUploadBufferList.emplace_back(uploadData);
            memcpy(uploadData.get(), data.start, data.size());

            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferVBVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferVBVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData.get()), data.size());
#endif

#ifdef UPLOADBUFFER_MEMORYALLOCATOR_RINGBUFFER
            auto uploadData = m_allocator.allocate(data.size(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
            memcpy(uploadData, data.start, data.size());
            commandList.transition(buffer.buffer(), ResourceState::CopyDest);
            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferVBVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferVBVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData), data.size());
#endif
        }

        Texture DeviceImpl::createTexture(const Device& device, Queue& queue, const TextureDescription& desc)
        {
            auto descmod = desc;
            if (isBlockCompressedFormat(descmod.descriptor.format))
            {
                if (descmod.descriptor.width < 4)
                    descmod.descriptor.width = 4;
                if (descmod.descriptor.height < 4)
                    descmod.descriptor.height = 4;
            }

            if (descmod.descriptor.usage == ResourceUsage::CpuToGpu)
            {
                auto gpuDesc = descmod;
                auto gpuTexture = createTexture(device, queue, gpuDesc
                    .usage(ResourceUsage::GpuRead)
                    .setInitialData(TextureDescription::InitialData()));

                if (descmod.initialData.data.size() > 0)
                {
                    unsigned int subResourceIndex = 0;
                    uint32_t dataIndex = 0;
                    for (int slice = 0; slice < static_cast<int>(descmod.descriptor.arraySlices); ++slice)
                    {
                        int width = descmod.descriptor.width;
                        int height = descmod.descriptor.height;

                        for (int mip = 0; mip < static_cast<int>(descmod.descriptor.mipLevels); ++mip)
                        {
                            auto info = surfaceInformation(descmod.descriptor.format, width, height);

                            // copy to CPU memory
                            D3D12_SUBRESOURCE_FOOTPRINT  srcFootprint;
                            srcFootprint.Depth = 1;
                            srcFootprint.Format = dxFormat(descmod.descriptor.format);
                            srcFootprint.Width = width;
                            srcFootprint.Height = height;
                            srcFootprint.RowPitch = static_cast<UINT>(Align(
                                info.rowBytes,
                                static_cast<size_t>(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT)));

                            auto allocator = &m_allocator;
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
                            std::shared_ptr<void> uploadData = std::shared_ptr<void>(
                                m_allocator.allocate(info.numBytes, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT),
                                [allocator](void* ptr) { allocator->free(ptr); });

                            m_currentUploadBufferList.emplace_back(uploadData);
#endif
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_RINGBUFFER
                            auto uploadData = m_allocator.allocate(info.numBytes, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
#endif

                            D3D12_PLACED_SUBRESOURCE_FOOTPRINT placedTexture2D = { 0 };
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
                            placedTexture2D.Offset = m_allocator.offset(uploadData.get());
#endif
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_RINGBUFFER
                            placedTexture2D.Offset = m_allocator.offset(uploadData);
#endif
                            placedTexture2D.Footprint = srcFootprint;

                            const uint8_t* srcData = &descmod.initialData.data[dataIndex];
                            for (int i = 0; i < static_cast<int>(info.numRows); ++i)
                            {
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
                                UINT8 *pScan = static_cast<uint8_t*>(uploadData.get()) + (i * srcFootprint.RowPitch);
#endif
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_RINGBUFFER
                                UINT8 *pScan = static_cast<uint8_t*>(uploadData) + (i * srcFootprint.RowPitch);
#endif
                                memcpy(pScan, srcData, info.rowBytes);
                                srcData += info.rowBytes;
                            }
                            dataIndex += info.numBytes;

                            // copy to GPU memory
                            D3D12_TEXTURE_COPY_LOCATION dst;
                            dst.pResource = TextureImplGet::impl(gpuTexture)->native();
                            dst.SubresourceIndex = subResourceIndex;
                            dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;

                            D3D12_TEXTURE_COPY_LOCATION srcLocation;
                            srcLocation.pResource = BufferImplGet::impl(*m_uploadBuffer)->native();
                            srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
                            srcLocation.PlacedFootprint = placedTexture2D;

                            D3D12_BOX srcBox;
                            srcBox.left = 0;
                            srcBox.top = 0;
                            srcBox.right = width;
                            srcBox.bottom = height;
                            srcBox.front = 0;
                            srcBox.back = 1;

                            CommandList texcmdb = CommandList(device);

                            if (gpuTexture.state() != ResourceState::Common)
                                texcmdb.transition(gpuTexture, ResourceState::CopyDest);
                            gpuTexture.state(ResourceState::CopyDest);

                            CommandListImplGet::impl(texcmdb).native()->CopyTextureRegion(
                                &dst,
                                0, 0, 0,
                                &srcLocation,
                                &srcBox);
                            Fence texfence = Fence(device);
                            queue.submit(texcmdb, texfence);
                            texfence.blockUntilSignaled();

                            width /= 2;
                            height /= 2;
                            if (isBlockCompressedFormat(descmod.descriptor.format))
                            {
                                if (width < 4)
                                    width = 4;
                                if (height < 4)
                                    height = 4;
                            }
                            else
                            {
                                if (width < 1)
                                    width = 1;
                                if (height < 1)
                                    height = 1;
                            }
                            ++subResourceIndex;
                        }
                    }
                }
                return gpuTexture;
            }
            else
                return Texture{ std::make_shared<TextureImpl>(*this, descmod) };
        }

        void DeviceImpl::waitForIdle()
        {
            // TODO
        }

        DescriptorHeaps& DeviceImpl::heaps()
        {
            return m_descriptorHeaps;
        }

        const DescriptorHeaps& DeviceImpl::heaps() const
        {
            return m_descriptorHeaps;
        }
    }
}
