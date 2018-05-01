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
#include "engine/rendering/ModelResources.h"

#include "shaders/core/tools/cleartexture/ClearTexture1df.h"
#include "shaders/core/tools/cleartexture/ClearTexture2df.h"
#include "shaders/core/tools/cleartexture/ClearTexture3df.h"
#include "shaders/core/tools/cleartexture/ClearTexture1df2.h"
#include "shaders/core/tools/cleartexture/ClearTexture2df2.h"
#include "shaders/core/tools/cleartexture/ClearTexture3df2.h"
#include "shaders/core/tools/cleartexture/ClearTexture1df3.h"
#include "shaders/core/tools/cleartexture/ClearTexture2df3.h"
#include "shaders/core/tools/cleartexture/ClearTexture3df3.h"
#include "shaders/core/tools/cleartexture/ClearTexture1df4.h"
#include "shaders/core/tools/cleartexture/ClearTexture2df4.h"
#include "shaders/core/tools/cleartexture/ClearTexture3df4.h"
#include "shaders/core/tools/cleartexture/ClearTexture1du.h"
#include "shaders/core/tools/cleartexture/ClearTexture2du.h"
#include "shaders/core/tools/cleartexture/ClearTexture3du.h"
#include "shaders/core/tools/cleartexture/ClearTexture1du2.h"
#include "shaders/core/tools/cleartexture/ClearTexture2du2.h"
#include "shaders/core/tools/cleartexture/ClearTexture3du2.h"
#include "shaders/core/tools/cleartexture/ClearTexture1du3.h"
#include "shaders/core/tools/cleartexture/ClearTexture2du3.h"
#include "shaders/core/tools/cleartexture/ClearTexture3du3.h"
#include "shaders/core/tools/cleartexture/ClearTexture1du4.h"
#include "shaders/core/tools/cleartexture/ClearTexture2du4.h"
#include "shaders/core/tools/cleartexture/ClearTexture3du4.h"

#include "shaders/core/tools/copytexture/CopyTexture1df.h"
#include "shaders/core/tools/copytexture/CopyTexture2df.h"
#include "shaders/core/tools/copytexture/CopyTexture3df.h"

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
    class ShaderStorage;

    struct NullResources
    {
        BufferSRV bufferSRV;
        BufferUAV bufferUAV;
        TextureSRV textureSRV;
        TextureSRV textureCubeSRV;
        TextureUAV textureUAV;
        Sampler sampler;
    };

    struct CorePipelines
    {
        engine::Pipeline<shaders::ClearTexture1df> clearTexture1df;
        engine::Pipeline<shaders::ClearTexture2df> clearTexture2df;
        engine::Pipeline<shaders::ClearTexture3df> clearTexture3df;

        engine::Pipeline<shaders::ClearTexture1df2> clearTexture1df2;
        engine::Pipeline<shaders::ClearTexture2df2> clearTexture2df2;
        engine::Pipeline<shaders::ClearTexture3df2> clearTexture3df2;

        engine::Pipeline<shaders::ClearTexture1df3> clearTexture1df3;
        engine::Pipeline<shaders::ClearTexture2df3> clearTexture2df3;
        engine::Pipeline<shaders::ClearTexture3df3> clearTexture3df3;

        engine::Pipeline<shaders::ClearTexture1df4> clearTexture1df4;
        engine::Pipeline<shaders::ClearTexture2df4> clearTexture2df4;
        engine::Pipeline<shaders::ClearTexture3df4> clearTexture3df4;

        engine::Pipeline<shaders::ClearTexture1du> clearTexture1du;
        engine::Pipeline<shaders::ClearTexture2du> clearTexture2du;
        engine::Pipeline<shaders::ClearTexture3du> clearTexture3du;

        engine::Pipeline<shaders::ClearTexture1du2> clearTexture1du2;
        engine::Pipeline<shaders::ClearTexture2du2> clearTexture2du2;
        engine::Pipeline<shaders::ClearTexture3du2> clearTexture3du2;

        engine::Pipeline<shaders::ClearTexture1du3> clearTexture1du3;
        engine::Pipeline<shaders::ClearTexture2du3> clearTexture2du3;
        engine::Pipeline<shaders::ClearTexture3du3> clearTexture3du3;

        engine::Pipeline<shaders::ClearTexture1du4> clearTexture1du4;
        engine::Pipeline<shaders::ClearTexture2du4> clearTexture2du4;
        engine::Pipeline<shaders::ClearTexture3du4> clearTexture3du4;

        engine::Pipeline<shaders::CopyTexture1df> copyTexture1df;
        engine::Pipeline<shaders::CopyTexture2df> copyTexture2df;
        engine::Pipeline<shaders::CopyTexture3df> copyTexture3df;
    };

    class Device
    {
    public:
        Device(
            std::shared_ptr<platform::Window> window,
            ShaderStorage& shaderStorage);

        int width() const;
        int height() const;

        void recycleUploadBuffers(uint32_t frameNumber);

        CommandList createCommandList(CommandListType type = CommandListType::Direct) const;

        std::shared_ptr<SwapChain> createSwapChain(
            bool fullscreen = false, 
            bool vsync = true,
            SwapChain* oldSwapChain = nullptr);
        std::weak_ptr<SwapChain> currentSwapChain();
        const std::weak_ptr<SwapChain> currentSwapChain() const;

        Sampler createSampler(const SamplerDescription& desc) const;
        RootSignature createRootSignature() const;

        Buffer createBuffer(const BufferDescription& desc) const;
        BufferSRV createBufferSRV(const Buffer& buffer, const BufferDescription& desc) const;
        BufferUAV createBufferUAV(const Buffer& buffer, const BufferDescription& desc) const;
        BufferIBV createBufferIBV(const Buffer& buffer, const BufferDescription& desc) const;
        BufferCBV createBufferCBV(const Buffer& buffer, const BufferDescription& desc) const;
        BufferVBV createBufferVBV(const Buffer& buffer, const BufferDescription& desc) const;

        BufferSRV createBufferSRV(const BufferDescription& desc) const;
        BufferUAV createBufferUAV(const BufferDescription& desc) const;
        BufferIBV createBufferIBV(const BufferDescription& desc) const;
        BufferCBV createBufferCBV(const BufferDescription& desc) const;
        BufferVBV createBufferVBV(const BufferDescription& desc) const;

        BufferSRV createBufferSRV(const Buffer& buffer) const;
        BufferUAV createBufferUAV(const Buffer& buffer) const;
        BufferIBV createBufferIBV(const Buffer& buffer) const;
        BufferCBV createBufferCBV(const Buffer& buffer) const;
        BufferVBV createBufferVBV(const Buffer& buffer) const;

        BufferSRV createBufferSRV(ResourceKey key, const Buffer& buffer, const BufferDescription& desc) const;
        BufferSRV createBufferSRV(ResourceKey key, const BufferDescription& desc) const;
        BufferSRV createBufferSRV(ResourceKey key, const Buffer& buffer) const;

        BufferIBV createBufferIBV(ResourceKey key, const Buffer& buffer, const BufferDescription& desc) const;
        BufferIBV createBufferIBV(ResourceKey key, const BufferDescription& desc) const;
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
        TextureSRV createTextureSRV(const Texture& texture, const TextureDescription& desc) const;
        TextureSRV createTextureSRV(const Texture& texture) const;
        TextureSRV createTextureSRV(const TextureDSV& texture) const;
        TextureSRV createTextureSRV(const Texture& texture, SubResource subResources) const;
        TextureSRV createTextureSRV(const Texture& texture, const TextureDescription& desc, SubResource subResources) const;

        TextureUAV createTextureUAV(const TextureDescription& desc);
        TextureUAV createTextureUAV(const Texture& texture, const TextureDescription& desc) const;
        TextureUAV createTextureUAV(const Texture& texture) const;
        TextureUAV createTextureUAV(const Texture& texture, SubResource subResources) const;
        TextureUAV createTextureUAV(const Texture& texture, const TextureDescription& desc, SubResource subResources) const;

        TextureDSV createTextureDSV(const TextureDescription& desc, SubResource subResources = SubResource());
        TextureDSV createTextureDSV(const Texture& texture, const TextureDescription& desc, SubResource subResources = SubResource()) const;
        TextureDSV createTextureDSV(const Texture& texture, SubResource subResources = SubResource()) const;

        TextureRTV createTextureRTV(const TextureDescription& desc, SubResource subResources = SubResource());
        TextureRTV createTextureRTV(const Texture& texture, const TextureDescription& desc, SubResource subResources = SubResource()) const;
        TextureRTV createTextureRTV(const Texture& texture, SubResource subResources = SubResource()) const;

        // Resource cached textures
        TextureSRV createTextureSRV(ResourceKey key, const TextureDescription& desc);
        TextureSRV createTextureSRV(ResourceKey key, const Texture& texture, const TextureDescription& desc) const;
        TextureSRV createTextureSRV(ResourceKey key, const Texture& texture) const;

        std::shared_ptr<image::ImageIf> createImage(
            ResourceKey key,
            const std::string& filename,
            Format type = Format::BC7_UNORM,
            int width = -1,
            int height = -1,
            int slices = -1,
            int mips = -1);

        std::shared_ptr<SubMeshInstance> createMesh(
            ResourceKey key,
            const std::string& filename,
            uint32_t meshIndex);

        Fence createFence() const;
        Semaphore createSemaphore() const;

        void waitForIdle();

        Queue& queue();
        Queue& copyQueue();

        NullResources& nullResouces()
        {
            return *m_nullResouces;
        }

        ResourceCache& resourceCache()
        {
            return *m_resourceCache;
        }

        ModelResources& modelResources()
        {
            return *m_modelResources;
        }

        std::shared_ptr<CorePipelines>& corePipelines()
        {
            return m_corePipelines;
        }
    private:
        std::weak_ptr<SwapChain> m_swapChain;
        std::shared_ptr<Queue> m_queue;
        std::shared_ptr<Queue> m_copyQueue;
        std::shared_ptr<ResourceCache> m_resourceCache;
        std::shared_ptr<NullResources> m_nullResouces;
        std::shared_ptr<ModelResources> m_modelResources;
        std::shared_ptr<CorePipelines> m_corePipelines;
        uint64_t m_frameNumber;

        PIMPL_FRIEND_ACCESS(DeviceImpl)
    };
}
