#pragma once

#include "tools/SmartPimpl.h"
#include "tools/ByteRange.h"
#include <memory>
#include <vector>
#include <string>
#include "engine/graphics/Common.h"
#include "engine/graphics/CommonNoDep.h"
#include "engine/graphics/Sampler.h"
#include "engine/graphics/ResourceCache.h"
#include "shaders/ShaderTypes.h"
#include "engine/graphics/Pipeline.h"

namespace platform
{
    class Window;
}

namespace engine
{
    PIMPL_FWD(Device, DeviceImpl)

    namespace image
    {
        class Image;
    }
    class Mesh;

    class Queue;
    class SwapChain;
    class DescriptorHeap;
    class DescriptorHandle;
    struct BufferDescription;
    struct TextureDescription;

    class Buffer;
    class BufferView;
    class BufferSRV;
    class BufferUAV;
    class BufferIBV;
    class BufferCBV;
    class BufferVBV;

    
    class Texture;
    class TextureSRV;
    class TextureUAV;
    class TextureDSV;
    class TextureRTV;
    class TextureView;

    class CommandList;
    class ShaderBinary;
    class Fence;
    class Semaphore;
    class RootSignature;
    enum class DescriptorHeapFlags;
    class Sampler;
    struct SamplerDescription;
    enum class Format;

    struct NullResources
    {
        BufferSRV bufferSRV;
        BufferUAV bufferUAV;
        TextureSRV textureSRV;
        TextureUAV textureUAV;
        Sampler sampler;
    };

    class Device
    {
    public:
        Device(std::shared_ptr<platform::Window> window);

        int width() const;
        int height() const;

        void recycleUploadBuffers(uint32_t frameNumber);

        CommandList createCommandList();

        std::shared_ptr<SwapChain> createSwapChain(
            bool fullscreen = false, 
            bool vsync = true,
            SwapChain* oldSwapChain = nullptr);
        std::weak_ptr<SwapChain> currentSwapChain();
        const std::weak_ptr<SwapChain> currentSwapChain() const;

        Sampler createSampler(const SamplerDescription& desc) const;
        RootSignature createRootSignature() const;

        Buffer createBuffer(const BufferDescription& desc);
        BufferSRV createBufferSRV(const Buffer& buffer, const BufferDescription& desc) const;
        BufferUAV createBufferUAV(const Buffer& buffer, const BufferDescription& desc) const;
        BufferIBV createBufferIBV(const Buffer& buffer, const BufferDescription& desc) const;
        BufferCBV createBufferCBV(const Buffer& buffer, const BufferDescription& desc) const;
        BufferVBV createBufferVBV(const Buffer& buffer, const BufferDescription& desc) const;

        BufferSRV createBufferSRV(const BufferDescription& desc);
        BufferUAV createBufferUAV(const BufferDescription& desc);
        BufferIBV createBufferIBV(const BufferDescription& desc);
        BufferCBV createBufferCBV(const BufferDescription& desc);
        BufferVBV createBufferVBV(const BufferDescription& desc);

        BufferSRV createBufferSRV(const Buffer& buffer) const;
        BufferUAV createBufferUAV(const Buffer& buffer) const;
        BufferIBV createBufferIBV(const Buffer& buffer) const;
        BufferCBV createBufferCBV(const Buffer& buffer) const;
        BufferVBV createBufferVBV(const Buffer& buffer) const;

        BufferSRV createBufferSRV(ResourceKey key, const Buffer& buffer, const BufferDescription& desc) const;
        BufferSRV createBufferSRV(ResourceKey key, const BufferDescription& desc);
        BufferSRV createBufferSRV(ResourceKey key, const Buffer& buffer) const;

        BufferIBV createBufferIBV(ResourceKey key, const Buffer& buffer, const BufferDescription& desc) const;
        BufferIBV createBufferIBV(ResourceKey key, const BufferDescription& desc);
        BufferIBV createBufferIBV(ResourceKey key, const Buffer& buffer) const;

        template<typename T>
        bool cachedDataExists(ResourceKey key) const;

        void uploadBuffer(CommandList& commandList, BufferSRV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
        void uploadBuffer(CommandList& commandList, BufferCBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
        void uploadBuffer(CommandList& commandList, BufferIBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
        void uploadBuffer(CommandList& commandList, BufferVBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);

        template<typename T>
        Pipeline<T> createPipeline(ShaderStorage& storage)
        {
            return Pipeline<T>(*this, storage);
        }

        uint64_t frameNumber() const;
        void frameNumber(uint64_t frame);
    public:

        Texture createTexture(const TextureDescription& desc);
        TextureSRV createTextureSRV(const TextureDescription& desc);
        TextureUAV createTextureUAV(const TextureDescription& desc);
        TextureDSV createTextureDSV(const TextureDescription& desc, SubResource subResources = SubResource());
        TextureRTV createTextureRTV(const TextureDescription& desc, SubResource subResources = SubResource());
        TextureSRV createTextureSRV(const Texture& texture, const TextureDescription& desc) const;
        TextureUAV createTextureUAV(const Texture& texture, const TextureDescription& desc) const;
        TextureDSV createTextureDSV(const Texture& texture, const TextureDescription& desc, SubResource subResources = SubResource()) const;
        TextureRTV createTextureRTV(const Texture& texture, const TextureDescription& desc, SubResource subResources = SubResource()) const;
        TextureSRV createTextureSRV(const Texture& texture) const;
        TextureUAV createTextureUAV(const Texture& texture) const;
        TextureDSV createTextureDSV(const Texture& texture, SubResource subResources = SubResource()) const;
        TextureRTV createTextureRTV(const Texture& texture, SubResource subResources = SubResource()) const;

        TextureSRV createTextureSRV(const TextureDSV& texture) const;

        TextureSRV createTextureSRV(ResourceKey key, const TextureDescription& desc);
        TextureSRV createTextureSRV(ResourceKey key, const Texture& texture, const TextureDescription& desc) const;
        TextureSRV createTextureSRV(ResourceKey key, const Texture& texture) const;

        std::shared_ptr<image::ImageIf> createImage(
            ResourceKey key,
            const std::string& filename,
            Format type = Format::Format_BC7_UNORM,
            int width = -1,
            int height = -1,
            int slices = -1,
            int mips = -1);

        std::shared_ptr<Mesh> createMesh(
            ResourceKey key,
            const std::string& filename);

        Fence createFence();
        Semaphore createSemaphore();

        void waitForIdle();

        Queue& queue();

        NullResources& nullResouces()
        {
            return *m_nullResouces;
        }

        ResourceCache& resourceCache()
        {
            return *m_resourceCache;
        }
    private:
        std::weak_ptr<SwapChain> m_swapChain;
        std::shared_ptr<Queue> m_queue;
        std::shared_ptr<ResourceCache> m_resourceCache;
        std::shared_ptr<NullResources> m_nullResouces;
        uint64_t m_frameNumber;

        PIMPL_FRIEND_ACCESS(DeviceImpl)
    };
}
