#pragma once

#include "tools/SmartPimpl.h"
#include "engine/graphics/CommonNoDep.h"
#include "engine/graphics/Format.h"
#include "engine/primitives/Color.h"
#include "engine/primitives/Vector4.h"
#include <vector>

namespace engine
{
    PIMPL_FWD(CommandList, CommandListImpl)

    namespace shaders
    {
        class PipelineConfiguration;
    }
    namespace implementation
    {
        class PipelineImpl;
        class DeviceImpl;
    }

    class Device;
    class Queue;
    class Barrier;
    class Buffer;
    class BufferSRV;
    class BufferUAV;
    class BufferIBV;
    class BufferCBV;
    class BufferVBV;
    class Texture;
    class TextureRTV;
    class TextureSRV;
    class TextureUAV;
    class TextureDSV;
    
    class ImageBuffer;
    class Semaphore;
    class DescriptorHandle;
    enum class ResourceBarrierFlags;
    enum class ResourceState;
    //enum class ImageLayout;
    struct SubResource;
    struct Viewport;
    struct Rectangle;
    struct CorePipelines;

    class CommandList
    {
    public:
        
        Barrier createBarrier(
            ResourceBarrierFlags flags,
            const TextureRTV& resource,
            ResourceState before,
            ResourceState after,
            unsigned int subResource,
            const Semaphore& waitSemaphore,
            const Semaphore& signalSemaphore
        );

        void transition(Texture& resource, ResourceState state);
        void transition(Texture& resource, ResourceState state, const SubResource& subResource);
        void transition(TextureRTV& resource, ResourceState state);
        void transition(TextureSRV& resource, ResourceState state);
        void transition(TextureDSV& resource, ResourceState state);

        void transition(Buffer& resource, ResourceState state);
        void transition(BufferSRV& resource, ResourceState state);
        void transition(BufferIBV& resource, ResourceState state);
        void transition(BufferVBV& resource, ResourceState state);
        void transition(BufferCBV& resource, ResourceState state);

        void setRenderTargets(
            std::vector<TextureRTV> targets);
        void setRenderTargets(
            std::vector<TextureRTV> targets,
            TextureDSV& dsv);
        void clearRenderTargetView(TextureRTV& target);
        void clearRenderTargetView(TextureRTV& target, const Color4f& color);
        void clearDepthStencilView(TextureDSV& target, float depth, uint8_t stencil = 0);
        void copyBuffer(
            Buffer& srcBuffer,
            Buffer& dstBuffer,
            uint64_t elements,
            uint32_t srcStartElement = 0,
            uint32_t dstStartElement = 0);

        template<typename T>
        void reset(T& pipeline)
        {
            reset(pipeline.m_impl);
        };

        template<typename T>
        void bindPipe(T& pipeline)
        {
            pipeline.setRenderTargetFormats(m_lastSetRTVFormats, m_lastSetDSVFormat);
            bindPipe(pipeline.m_impl.get(), &pipeline);
        };

        void setViewPorts(const std::vector<Viewport>& viewports);
        void setScissorRects(const std::vector<Rectangle>& rects);

    private:
        void reset(std::shared_ptr<implementation::PipelineImpl> pipelineState);
        void bindPipe(
            implementation::PipelineImpl* pipelineImpl,
            shaders::PipelineConfiguration* configuration);
        void beginRenderPass(std::shared_ptr<implementation::PipelineImpl> pipeline, int frameBufferIndex);
    public:

        void bindVertexBuffer(BufferVBV& buffer);
        //void bindIndexBuffer(BufferIBV& buffer);
        //void bindDescriptorSets(const Pipeline& pipeline, const DescriptorHandle& descriptor);

        void clearBuffer(Buffer& buffer, uint32_t value, uint32_t startElement, uint32_t numElements);
        void clearTexture(
            Texture& texture, 
            const Color4f& color = Color4f(0.0f, 0.0f, 0.0f, 0.0f),
            const SubResource& subResource = SubResource());

        void clearTexture(Texture& texture, float value,                const SubResource& subResource = SubResource());
        void clearTexture(Texture& texture, Vector4f value,             const SubResource& subResource = SubResource());
        void clearTexture(Texture& texture, uint32_t value,             const SubResource& subResource = SubResource());
        void clearTexture(Texture& texture, Vector4<uint32_t> value,    const SubResource& subResource = SubResource());

        void begin();
        void end();

        template<typename T>
        void beginRenderPass(T& pipeline, int frameBufferIndex)
        {
            beginRenderPass(pipeline.m_impl, frameBufferIndex);
        };

        void endRenderPass();

        void draw(uint32_t vertexCount);
        void drawIndexed(
            BufferIBV& buffer,
            uint32_t indexCount,
            uint32_t instanceCount,
            uint32_t firstIndex,
            int32_t vertexOffset,
            uint32_t firstInstance);

        void drawIndirect(Buffer& indirectArguments, uint64_t argumentBufferOffset);

        void drawIndexedIndirect(BufferIBV& buffer, Buffer& indirectArguments, uint64_t argumentBufferOffset);

        void dispatch(
            uint32_t threadGroupCountX,
            uint32_t threadGroupCountY,
            uint32_t threadGroupCountZ);

        void dispatchIndirect(
            Buffer& indirectArgs,
            uint64_t argumentBufferOffset);

        void executeBundle(CommandList& commandList);

        void copyTexture(TextureSRV& src, TextureUAV& dst);
        void copyTexture(TextureSRV& src, TextureSRV& dst);
        void copyTexture(TextureSRV& src, BufferUAV& dst);
        void copyTexture(TextureSRV& src, BufferSRV& dst);

        void setStructureCounter(BufferUAV& buffer, uint32_t value);
        void copyStructureCounter(BufferUAV& srcBuffer, Buffer& dst, uint32_t dstByteOffset);

    private:
        void copyTexture(
            TextureSRV& src, 
            TextureUAV& dst, 
            uint32_t srcLeft, 
            uint32_t srcTop, 
            uint32_t srcFront,
            uint32_t dstLeft, 
            uint32_t dstTop, 
            uint32_t dstFront,
            uint32_t width, 
            uint32_t height,
            uint32_t depth);

    private:
        friend class Device;
        friend class implementation::DeviceImpl;
        CommandList(const Device& device, CommandListType type = CommandListType::Direct);

        PIMPL_FRIEND_ACCESS(CommandListImpl)

    private:
        void savePipeline(shaders::PipelineConfiguration* configuration);
        std::vector<Texture> m_boundTextures;
        std::vector<Buffer> m_boundBuffers;

        std::vector<TextureSRV> m_boundTextureSRVs;
        std::vector<TextureUAV> m_boundTextureUAVs;

        std::vector<BufferSRV> m_boundBufferSRVs;
        std::vector<BufferUAV> m_boundBufferUAVs;

        std::shared_ptr<CorePipelines> m_corePipelines;

        std::vector<TextureUAV> m_clearUAVs;

        void transitionCommonSRV(TextureSRV& srv, ResourceState state);
        void transitionCommonUAV(TextureUAV& uav, ResourceState state);
    private:
        friend class Queue;

        void setDebugBuffers(shaders::PipelineConfiguration* configuration);
        struct ShaderDebugOutput
        {
            uint32_t itemType;
            uint32_t uvalue;
            float fvalue;
        };
        const Device* m_device;
        std::vector<BufferUAV> m_debugBuffers;

        std::vector<Format> m_lastSetRTVFormats;
        Format m_lastSetDSVFormat;
    };
}
