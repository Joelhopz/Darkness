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

            IDXGIFactory* pFactory;
            HRESULT hr = CreateDXGIFactory(DARKNESS_IID_PPV_ARGS(&pFactory));
            ASSERT(hr == S_OK, "Could not create DXGI Factory");

            UINT i = 0;
            IDXGIAdapter* pAdapter;
            std::vector <IDXGIAdapter*> vAdapters;
            while (pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
            {
                vAdapters.push_back(pAdapter);

                DXGI_ADAPTER_DESC desc;
                pAdapter->GetDesc(&desc);
                ++i;
            }

            auto createResult = D3D12CreateDevice(
                vAdapters[0],
                featureLevel,
                DARKNESS_IID_PPV_ARGS(m_device.GetAddressOf()));
            ASSERT(SUCCEEDED(createResult));

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
                10000);

            m_descriptorHeaps.shaderVisible_sampler = std::make_shared<DescriptorHeapImpl>(
                *this,
                D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
                D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
                2048);
            
            m_uploadBuffer = std::make_shared<Buffer>(std::make_shared<BufferImpl>(
                *this,
                BufferDescription()
                .usage(ResourceUsage::Upload)
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

            {
                // create drawIndirect signature
                D3D12_INDIRECT_ARGUMENT_DESC indirectArgumentDesc;
                indirectArgumentDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;
                D3D12_COMMAND_SIGNATURE_DESC signatureDesc;
                signatureDesc.ByteStride = sizeof(D3D12_DRAW_ARGUMENTS);
                signatureDesc.NodeMask = 0;
                signatureDesc.NumArgumentDescs = 1;
                signatureDesc.pArgumentDescs = &indirectArgumentDesc;
                m_device->CreateCommandSignature(&signatureDesc, nullptr, DARKNESS_IID_PPV_ARGS(m_drawSignature.GetAddressOf()));
            }
            {
                // create drawIndexedIndirect signature
                D3D12_INDIRECT_ARGUMENT_DESC indirectArgumentDesc;
                indirectArgumentDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;
                D3D12_COMMAND_SIGNATURE_DESC signatureDesc;
                signatureDesc.ByteStride = sizeof(D3D12_DRAW_INDEXED_ARGUMENTS);
                signatureDesc.NodeMask = 0;
                signatureDesc.NumArgumentDescs = 1;
                signatureDesc.pArgumentDescs = &indirectArgumentDesc;
                m_device->CreateCommandSignature(&signatureDesc, nullptr, DARKNESS_IID_PPV_ARGS(m_drawIndexedSignature.GetAddressOf()));
            }
            {
                // create dispatchIndirect signature
                D3D12_INDIRECT_ARGUMENT_DESC indirectArgumentDesc;
                indirectArgumentDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH;
                D3D12_COMMAND_SIGNATURE_DESC signatureDesc;
                signatureDesc.ByteStride = sizeof(D3D12_DISPATCH_ARGUMENTS);
                signatureDesc.NodeMask = 0;
                signatureDesc.NumArgumentDescs = 1;
                signatureDesc.pArgumentDescs = &indirectArgumentDesc;
                m_device->CreateCommandSignature(&signatureDesc, nullptr, DARKNESS_IID_PPV_ARGS(m_dispatchSignature.GetAddressOf()));
            }
        }

        ID3D12CommandSignature* DeviceImpl::drawIndirectSignature()
        {
            return m_drawSignature.Get();
        }

        ID3D12CommandSignature* DeviceImpl::drawIndexedIndirectSignature()
        {
            return m_drawIndexedSignature.Get();
        }

        ID3D12CommandSignature* DeviceImpl::dispatchIndirectSignature()
        {
            return m_dispatchSignature.Get();
        }

        ID3D12Device* DeviceImpl::device() const
        {
            return m_device.Get();
        }

        std::shared_ptr<CommandAllocatorImpl> DeviceImpl::createCommandAllocator(CommandListType type)
        {
            std::queue<std::shared_ptr<CommandAllocatorImpl>>& allocatorQueue = m_commandAllocators[static_cast<int>(type)];
            std::vector<std::shared_ptr<CommandAllocatorImpl>>& inUseAllocatorQueue = m_inUseCommandAllocators[static_cast<int>(type)];

            if (allocatorQueue.size() > 0)
            {
                auto res = allocatorQueue.front();
                allocatorQueue.pop();
                inUseAllocatorQueue.emplace_back(res);
                return res;
            }

            inUseAllocatorQueue.emplace_back(std::make_shared<CommandAllocatorImpl>(*this, type));
            return inUseAllocatorQueue.back();
        }

        void DeviceImpl::freeCommandAllocator(std::shared_ptr<CommandAllocatorImpl> allocator)
        {
            std::vector<std::shared_ptr<CommandAllocatorImpl>>& inUseAllocatorQueue = m_inUseCommandAllocators[static_cast<int>(allocator->type())];
            std::vector<std::shared_ptr<CommandAllocatorImpl>>& returnedAllocators = m_returnedCommandAllocators[static_cast<int>(allocator->type())];
            inUseAllocatorQueue.erase(std::find(inUseAllocatorQueue.begin(), inUseAllocatorQueue.end(), allocator));
            returnedAllocators.emplace_back(allocator);
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
            for (int i = 0; i < 4; ++i)
            {
                // handle command allocators
                if (m_inFlightCommandAllocators[i].size() > BackBufferCount)
                {
                    std::vector<std::shared_ptr<CommandAllocatorImpl>>& toBeFreed = m_inFlightCommandAllocators[i].front();
                    for (auto&& free : toBeFreed)
                    {
                        free->reset();
                        m_commandAllocators[i].push(free);
                    }
                    m_inFlightCommandAllocators[i].pop();
                }
                m_inFlightCommandAllocators[i].push(m_returnedCommandAllocators[i]);
                m_returnedCommandAllocators[i].clear();
            }

            m_frameDescriptorQueue.push(std::move(m_frameDescriptors));
            while (m_frameDescriptorQueue.size() > BackBufferCount)
            {
                m_frameDescriptorQueue.pop();
            }
        }

        /*void DeviceImpl::keepConstantUpdatesAlive(std::vector<ConstantBufferUpdates>&& constantBufferUpdates)
        {
            m_currentConstantBufferUpdates.emplace_back(std::move(constantBufferUpdates));
        }*/

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

        void DeviceImpl::uploadRawBuffer(CommandListImpl& commandList, Buffer& buffer, const ByteRange& data, uint32_t startBytes)
        {
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
            auto allocator = &m_allocator;
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
            auto uploadData = m_allocator.allocate(data.sizeBytes(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
            memcpy(uploadData, reinterpret_cast<uint8_t*>(data.start), data.sizeBytes());
            commandList.transition(buffer, ResourceState::CopyDest);
            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(buffer)->native(),
                startBytes,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData), data.sizeBytes());
#endif
        }

        void DeviceImpl::uploadBuffer(CommandListImpl& commandList, BufferSRV& buffer, const ByteRange& data, uint32_t startElement)
        {
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
            auto allocator = &m_allocator;
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
            auto uploadData = m_allocator.allocate(data.sizeBytes(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
            memcpy(uploadData, reinterpret_cast<uint8_t*>(data.start), data.sizeBytes());
            commandList.transition(buffer.buffer(), ResourceState::CopyDest);
            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferSRVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferSRVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData), data.sizeBytes());
#endif
        }

        void DeviceImpl::uploadBuffer(CommandList& commandList, BufferUAV& buffer, const ByteRange& data, uint32_t startElement)
        {
            uploadBuffer(CommandListImplGet::impl(commandList), buffer, data, startElement);
        }

        void DeviceImpl::uploadBuffer(CommandListImpl& commandList, BufferUAV& buffer, const ByteRange& data, uint32_t startElement)
        {
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
            auto allocator = &m_allocator;
            std::shared_ptr<void> uploadData = std::shared_ptr<void>(
                m_allocator.allocate(data.size(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT),
                [allocator](void* ptr) { allocator->free(ptr); });
            m_currentUploadBufferList.emplace_back(uploadData);
            memcpy(uploadData.get(), data.start, data.size());

            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferUAVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferUAVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData.get()), data.size());
#endif

#ifdef UPLOADBUFFER_MEMORYALLOCATOR_RINGBUFFER
            auto uploadData = m_allocator.allocate(data.sizeBytes(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
            memcpy(uploadData, reinterpret_cast<uint8_t*>(data.start), data.sizeBytes());
            commandList.transition(buffer.buffer(), ResourceState::CopyDest);
            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferUAVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferUAVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData), data.sizeBytes());
#endif
        }

        void DeviceImpl::uploadBuffer(CommandList& commandList, BufferCBV& buffer, const ByteRange& data, uint32_t startElement)
        {
            uploadBuffer(CommandListImplGet::impl(commandList), buffer, data, startElement);
        }

        void DeviceImpl::uploadBuffer(CommandListImpl& commandList, BufferCBV& buffer, const ByteRange& data, uint32_t startElement)
        {
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
            auto allocator = &m_allocator;
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
            auto uploadData = m_allocator.allocate(data.sizeBytes(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
            memcpy(uploadData, reinterpret_cast<uint8_t*>(data.start), data.sizeBytes());
            commandList.transition(buffer.buffer(), ResourceState::CopyDest);
            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferCBVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferCBVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData), data.sizeBytes());
#endif
            
        }

        void DeviceImpl::uploadBuffer(CommandList& commandList, BufferIBV& buffer, const ByteRange& data, uint32_t startElement)
        {
            uploadBuffer(CommandListImplGet::impl(commandList), buffer, data, startElement);
        }

        void DeviceImpl::uploadBuffer(CommandListImpl& commandList, BufferIBV& buffer, const ByteRange& data, uint32_t startElement)
        {
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
            auto allocator = &m_allocator;
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
            auto uploadData = m_allocator.allocate(data.sizeBytes(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
            memcpy(uploadData, reinterpret_cast<uint8_t*>(data.start), data.sizeBytes());
            commandList.transition(buffer.buffer(), ResourceState::CopyDest);
            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferIBVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferIBVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData), data.sizeBytes());
#endif
        }

        void DeviceImpl::uploadBuffer(CommandList& commandList, BufferVBV& buffer, const ByteRange& data, uint32_t startElement)
        {
            uploadBuffer(CommandListImplGet::impl(commandList), buffer, data, startElement);
        }

        void DeviceImpl::uploadBuffer(CommandListImpl& commandList, BufferVBV& buffer, const ByteRange& data, uint32_t startElement)
        {
#ifdef UPLOADBUFFER_MEMORYALLOCATOR_ALIGNEDCHUNKS
            auto allocator = &m_allocator;
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
            auto uploadData = m_allocator.allocate(data.sizeBytes(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
            memcpy(uploadData, reinterpret_cast<uint8_t*>(data.start), data.sizeBytes());
            commandList.transition(buffer.buffer(), ResourceState::CopyDest);
            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferVBVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferVBVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData), data.sizeBytes());
#endif
        }

        void transitionCommon(CommandList& cmd, Texture& tex, ResourceState state)
        {
            uint32_t sliceCount = tex.arraySlices();
            uint32_t mipCount = tex.mipLevels();

            for (uint32_t slice = 0; slice < sliceCount; ++slice)
            {
                for (uint32_t mip = 0; mip < mipCount; ++mip)
                {
                    if (tex.state(slice, mip) != ResourceState::Common)
                        cmd.transition(tex, state, SubResource{ mip, 1, slice, 1 });
                    tex.state(slice, mip, state);
                }
            }
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

            if (descmod.initialData.data.size() > 0)
            {
                auto gpuDesc = descmod;
                auto gpuTexture = createTexture(device, queue, gpuDesc
                    .usage(ResourceUsage::GpuRead)
                    .setInitialData(TextureDescription::InitialData()));

                if (descmod.initialData.data.size() > 0)
                {
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

                            unsigned int mipSlice = mip;
                            unsigned int arraySlice = slice;
                            unsigned int mipLevels = descmod.descriptor.mipLevels;
                            unsigned int subResourceIndex = mipSlice + (arraySlice * mipLevels);

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

                            /*if (gpuTexture.state() != ResourceState::Common)
                                texcmdb.transition(gpuTexture, ResourceState::CopyDest);
                            gpuTexture.state(ResourceState::CopyDest);*/
                            transitionCommon(texcmdb, gpuTexture, ResourceState::CopyDest);

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
