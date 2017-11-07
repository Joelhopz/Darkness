#include "engine/graphics/Device.h"
#include "engine/graphics/Queue.h"
#include "engine/graphics/SwapChain.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/CommandList.h"
#include "engine/graphics/Barrier.h"
#include "engine/graphics/Fence.h"
#include "engine/graphics/Pipeline.h"
#include "engine/graphics/Semaphore.h"
#include "engine/graphics/Sampler.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/RootSignature.h"
#include "engine/graphics/Format.h"

#if defined(_WIN32) && !defined(VULKAN)
#include "engine/graphics/dx12/DX12Device.h"
#include "engine/graphics/dx12/DX12Resources.h"
#include "engine/graphics/dx12/DX12Pipeline.h"
#endif

#if defined(VULKAN)
#include "engine/graphics/vulkan/VulkanDevice.h"
#include "engine/graphics/vulkan/VulkanResources.h"
#include "engine/graphics/vulkan/VulkanPipeline.h"
#endif

#ifdef __APPLE__
#include "engine/graphics/metal/MetalDevice.h"
#include "engine/graphics/metal/MetalBuffer.h"
#endif


using namespace tools;
using namespace platform;
using namespace engine::implementation;
using namespace std;

namespace engine
{
    PIMPL_ACCESS_IMPLEMENTATION(Device, DeviceImpl)

    Device::Device(shared_ptr<platform::Window> window)
        : m_impl{ make_unique_impl<DeviceImpl>(window) }
        , m_swapChain{}
        , m_queue{ nullptr }
        , m_resourceCache{ make_shared<ResourceCache>() }
        , m_nullResouces{ make_shared<NullResources>() }
    {
        m_queue = make_shared<Queue>(*this, CommandListType::Graphics);
        m_nullResouces->bufferSRV = createBufferSRV(BufferDescription().elements(1).elementSize(sizeof(Float4x4)).format(Format::Format_R8G8B8A8_UNORM).usage(ResourceUsage::GpuReadWrite).name("Null BufferSRV"));
        m_nullResouces->bufferUAV = createBufferUAV(m_nullResouces->bufferSRV.impl()->buffer());
        m_nullResouces->textureSRV = createTextureSRV(TextureDescription().width(1).height(1).format(Format::Format_R8G8B8A8_UNORM).usage(ResourceUsage::GpuReadWrite).name("Null TextureSRV"));
        m_nullResouces->textureUAV = createTextureUAV(m_nullResouces->textureSRV.impl()->texture());
        m_nullResouces->sampler = createSampler(SamplerDescription());
        m_impl->nullResources(m_nullResouces);
    }

    CommandList Device::createCommandList()
    {
        return CommandList{ *this };
    }

    shared_ptr<SwapChain> Device::createSwapChain(
        bool fullscreen, 
        bool vsync,
        SwapChain* oldSwapChain)
    {
        shared_ptr<SwapChain> result(new SwapChain(*this, *m_queue, fullscreen, vsync, oldSwapChain));
        m_swapChain = result;
        return result;
    }

    Queue& Device::queue()
    {
        return *m_queue;
    }

    weak_ptr<SwapChain> Device::currentSwapChain()
    {
        return m_swapChain;
    }

    const weak_ptr<SwapChain> Device::currentSwapChain() const
    {
        return m_swapChain;
    }

    Sampler Device::createSampler(const SamplerDescription& desc) const
    {
        return Sampler(*this, desc);
    }

    RootSignature Device::createRootSignature() const
    {
        return RootSignature(*this);
    }

    Buffer Device::createBuffer(const BufferDescription& desc)
    {
        if (desc.descriptor.usage == ResourceUsage::CpuToGpu)
        {
            auto gpuDesc = desc;
            auto cpuBuffer = Buffer{ *this, desc };
            auto gpuBuffer = createBuffer(gpuDesc
                .usage(ResourceUsage::GpuRead)
                .setInitialData(BufferDescription::InitialData())
                .elementSize(desc.descriptor.elementSize)
                .elements(desc.descriptor.elements)
                .firstElement(desc.descriptor.firstElement));

            auto cmdb = createCommandList();
            cmdb.copyBuffer(cpuBuffer, gpuBuffer, desc.descriptor.elements);
            Fence fence = createFence();
            m_queue->submit(cmdb, fence);
            fence.blockUntilSignaled();

            gpuBuffer.state(ResourceState::Common);

            return gpuBuffer;
        }
        else
            return Buffer{ *this, desc };
    }

    BufferSRV Device::createBufferSRV(const Buffer& buffer, const BufferDescription& desc) const
    {
        return BufferSRV{ *this, buffer, desc };
    }

    BufferUAV Device::createBufferUAV(const Buffer& buffer, const BufferDescription& desc) const
    {
        return BufferUAV{ *this, buffer, desc };
    }

    BufferIBV Device::createBufferIBV(const Buffer& buffer, const BufferDescription& desc) const
    {
        return BufferIBV{ *this, buffer, desc };
    }

    BufferCBV Device::createBufferCBV(const Buffer& buffer, const BufferDescription& desc) const
    {
        return BufferCBV{ *this, buffer, desc };
    }

    BufferVBV Device::createBufferVBV(const Buffer& buffer, const BufferDescription& desc) const
    {
        return BufferVBV{ *this, buffer, desc };
    }

    BufferSRV Device::createBufferSRV(const BufferDescription& desc)
    {
        return BufferSRV{ *this, createBuffer(desc), desc };
    }

    BufferUAV Device::createBufferUAV(const BufferDescription& desc)
    {
        return BufferUAV{ *this, createBuffer(desc), desc };
    }

    BufferIBV Device::createBufferIBV(const BufferDescription& desc)
    {
        return BufferIBV{ *this, createBuffer(desc), desc };
    }

    BufferCBV Device::createBufferCBV(const BufferDescription& desc)
    {
        return BufferCBV{ *this, createBuffer(desc), desc };
    }

    BufferVBV Device::createBufferVBV(const BufferDescription& desc)
    {
        return BufferVBV{ *this, createBuffer(desc), desc };
    }

    BufferSRV Device::createBufferSRV(const Buffer& buffer) const
    {
        return createBufferSRV(buffer, BufferDescription());
    }

    BufferUAV Device::createBufferUAV(const Buffer& buffer) const
    {
        return createBufferUAV(buffer, BufferDescription());
    }

    BufferIBV Device::createBufferIBV(const Buffer& buffer) const
    {
        return createBufferIBV(buffer, BufferDescription());
    }

    BufferCBV Device::createBufferCBV(const Buffer& buffer) const
    {
        return createBufferCBV(buffer, BufferDescription());
    }

    BufferVBV Device::createBufferVBV(const Buffer& buffer) const
    {
        return createBufferVBV(buffer, BufferDescription());
    }

    BufferSRV Device::createBufferSRV(ResourceKey key, const Buffer& buffer, const BufferDescription& desc) const
    {
        
        return m_resourceCache->createBufferSRV(key, [this, buffer, desc]()->BufferSRV
        {
            return BufferSRV{ *this, buffer, desc };
        });
    }

    BufferSRV Device::createBufferSRV(ResourceKey key, const BufferDescription& desc)
    {
        return m_resourceCache->createBufferSRV(key, [this, desc]()->BufferSRV
        {
            return BufferSRV{ *this, createBuffer(desc), desc };
        });
    }

    BufferSRV Device::createBufferSRV(ResourceKey key, const Buffer& buffer) const
    {
        return m_resourceCache->createBufferSRV(key, [this, buffer]()->BufferSRV
        {
            return createBufferSRV(buffer, BufferDescription());
        });
    }

    BufferIBV Device::createBufferIBV(ResourceKey key, const Buffer& buffer, const BufferDescription& desc) const
    {

        return m_resourceCache->createBufferIBV(key, [this, buffer, desc]()->BufferIBV
        {
            return BufferIBV{ *this, buffer, desc };
        });
    }

    BufferIBV Device::createBufferIBV(ResourceKey key, const BufferDescription& desc)
    {
        return m_resourceCache->createBufferIBV(key, [this, desc]()->BufferIBV
        {
            return BufferIBV{ *this, createBuffer(desc), desc };
        });
    }

    BufferIBV Device::createBufferIBV(ResourceKey key, const Buffer& buffer) const
    {
        return m_resourceCache->createBufferIBV(key, [this, buffer]()->BufferIBV
        {
            return createBufferIBV(buffer, BufferDescription());
        });
    }

    template<>
    bool Device::cachedDataExists<TextureSRV>(ResourceKey key) const
    {
        return m_resourceCache->cachedDataExists<TextureSRV>(key);
    }

    template<>
    bool Device::cachedDataExists<BufferSRV>(ResourceKey key) const
    {
        return m_resourceCache->cachedDataExists<BufferSRV>(key);
    }

    template<>
    bool Device::cachedDataExists<BufferIBV>(ResourceKey key) const
    {
        return m_resourceCache->cachedDataExists<BufferIBV>(key);
    }

    template<>
    bool Device::cachedDataExists<image::ImageIf>(ResourceKey key) const
    {
        return m_resourceCache->cachedDataExists<image::ImageIf>(key);
    }

    template<>
    bool Device::cachedDataExists<Mesh>(ResourceKey key) const
    {
        return m_resourceCache->cachedDataExists<Mesh>(key);
    }

    Texture Device::createTexture(const TextureDescription& desc)
    {
        return m_impl->createTexture(*this, *m_queue, desc);
    }

    TextureSRV Device::createTextureSRV(const TextureDescription& desc)
    {
        return TextureSRV{ make_shared<TextureSRVImpl>(DeviceImplGet::impl(*this), createTexture(desc), desc) };
    }

    TextureUAV Device::createTextureUAV(const TextureDescription& desc)
    {
        return TextureUAV{ make_shared<TextureUAVImpl>(DeviceImplGet::impl(*this), createTexture(desc), desc) };
    }

    TextureDSV Device::createTextureDSV(const TextureDescription& desc, SubResource subResources)
    {
        return TextureDSV{ make_shared<TextureDSVImpl>(DeviceImplGet::impl(*this), createTexture(desc), desc, subResources) };
    }

    TextureRTV Device::createTextureRTV(const TextureDescription& desc, SubResource subResources)
    {
        return TextureRTV{ make_shared<TextureRTVImpl>(DeviceImplGet::impl(*this), createTexture(desc), desc, subResources) };
    }

    TextureSRV Device::createTextureSRV(const Texture& texture, const TextureDescription& desc) const
    {
        return TextureSRV{ make_shared<TextureSRVImpl>(DeviceImplGet::impl(*this), texture, desc) };
    }

    TextureUAV Device::createTextureUAV(const Texture& texture, const TextureDescription& desc) const
    {
        return TextureUAV{ make_shared<TextureUAVImpl>(DeviceImplGet::impl(*this), texture, desc) };
    }

    TextureDSV Device::createTextureDSV(const Texture& texture, const TextureDescription& desc, SubResource subResources) const
    {
        return TextureDSV{ make_shared<TextureDSVImpl>(DeviceImplGet::impl(*this), texture, desc, subResources) };
    }

    TextureRTV Device::createTextureRTV(const Texture& texture, const TextureDescription& desc, SubResource subResources) const
    {
        return TextureRTV{ make_shared<TextureRTVImpl>(DeviceImplGet::impl(*this), texture, desc, subResources) };
    }

    TextureSRV Device::createTextureSRV(const Texture& texture) const
    {
        return TextureSRV{ make_shared<TextureSRVImpl>(DeviceImplGet::impl(*this), texture, texture.description()) };
    }

    TextureUAV Device::createTextureUAV(const Texture& texture) const
    {
        return TextureUAV{ make_shared<TextureUAVImpl>(DeviceImplGet::impl(*this), texture, texture.description()) };
    }

    TextureDSV Device::createTextureDSV(const Texture& texture, SubResource subResources) const
    {
        return TextureDSV{ make_shared<TextureDSVImpl>(DeviceImplGet::impl(*this), texture, texture.description(), subResources) };
    }

    TextureRTV Device::createTextureRTV(const Texture& texture, SubResource subResources) const
    {
        return TextureRTV{ make_shared<TextureRTVImpl>(DeviceImplGet::impl(*this), texture, texture.description(), subResources) };
    }

    TextureSRV Device::createTextureSRV(ResourceKey key, const TextureDescription& desc)
    {
        return m_resourceCache->createTextureSRV(key, [this, desc]()->TextureSRV
        {
            return TextureSRV{ make_shared<TextureSRVImpl>(DeviceImplGet::impl(*this), createTexture(desc), desc) };
        });
    }

    TextureSRV Device::createTextureSRV(ResourceKey key, const Texture& texture, const TextureDescription& desc) const
    {
        return m_resourceCache->createTextureSRV(key, [this, texture, desc]()->TextureSRV
        {
            return TextureSRV{ make_shared<TextureSRVImpl>(DeviceImplGet::impl(*this), texture, desc) };
        });
    }

    TextureSRV Device::createTextureSRV(ResourceKey key, const Texture& texture) const
    {
        return m_resourceCache->createTextureSRV(key, [this, texture]()->TextureSRV
        {
            return TextureSRV{ make_shared<TextureSRVImpl>(DeviceImplGet::impl(*this), texture, texture.description()) };
        });
    }

    shared_ptr<image::ImageIf> Device::createImage(
        ResourceKey key,
        const string& filename,
        Format type,
        int width,
        int height,
        int slices,
        int mips)
    {
        return m_resourceCache->createImage(key, filename, type, width, height, slices, mips);
    }

    shared_ptr<Mesh> Device::createMesh(
        ResourceKey key,
        const string& filename)
    {
        return m_resourceCache->createMesh(key, filename);
    }

    TextureSRV Device::createTextureSRV(const TextureDSV& texture) const
    {
        auto desc = TextureDSVImplGet::impl(texture)->texture().description();
        desc.format(Format::Format_R32_FLOAT);
        return TextureSRV{ make_shared<TextureSRVImpl>(
            DeviceImplGet::impl(*this), 
            TextureDSVImplGet::impl(texture)->texture(),
            desc) };
    }

    uint64_t Device::frameNumber() const
    {
        return m_frameNumber;
    }

    void Device::frameNumber(uint64_t frame)
    {
        m_frameNumber = frame;
    }

    void Device::uploadBuffer(CommandList& commandList, BufferSRV& buffer, const ByteRange& data, uint32_t startElement)
    {
        commandList.transition(buffer, ResourceState::CopyDest);
        m_impl->uploadBuffer(commandList, buffer, data, startElement);
    }

    void Device::uploadBuffer(CommandList& commandList, BufferCBV& buffer, const tools::ByteRange& data, uint32_t startElement)
    {
        commandList.transition(buffer, ResourceState::CopyDest);
        m_impl->uploadBuffer(commandList, buffer, data, startElement);
    }

    void Device::uploadBuffer(CommandList& commandList, BufferIBV& buffer, const tools::ByteRange& data, uint32_t startElement)
    {
        commandList.transition(buffer, ResourceState::CopyDest);
        m_impl->uploadBuffer(commandList, buffer, data, startElement);
    }

    void Device::uploadBuffer(CommandList& commandList, BufferVBV& buffer, const tools::ByteRange& data, uint32_t startElement)
    {
        commandList.transition(buffer, ResourceState::CopyDest);
        m_impl->uploadBuffer(commandList, buffer, data, startElement);
    }

    int Device::width() const
    {
        return m_impl->width();
    }

    int Device::height() const
    {
        return m_impl->height();
    }

    void Device::recycleUploadBuffers(uint32_t frameNumber)
    {
        m_impl->recycleUploadBuffers(frameNumber);
    }

    Fence Device::createFence()
    {
        return Fence(*this);
    }

    Semaphore Device::createSemaphore()
    {
        return Semaphore(*this);
    }

    void Device::waitForIdle()
    {
        auto cmd = createCommandList();
        auto fence = createFence();
        m_queue->submit(cmd, fence);
        fence.blockUntilSignaled();
        m_impl->waitForIdle();
    }

}
