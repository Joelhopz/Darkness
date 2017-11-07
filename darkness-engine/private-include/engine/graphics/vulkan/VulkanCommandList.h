#pragma once

#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/Semaphore.h"
#include <vector>
#include <memory>

namespace engine
{
    namespace shaders
    {
        class PipelineConfiguration;
    }

    enum class ResourceState;
    class Barrier;
    class Color4f;
    class Buffer;
    class BufferUAV;
    class BufferSRV;
    class BufferVBV;
    class BufferIBV;
    class BufferCBV;

    class Texture;
    class TextureRTV;
    class TextureSRV;
    class TextureUAV;
    class TextureDSV;
    class Queue;
    //enum class ImageLayout;
    struct SubResource;
    struct Viewport;
    struct Rectangle;

    namespace implementation
    {
        class DeviceImpl;
        class PipelineImpl;

        class CommandAllocatorImpl;
        class DescriptorHandleImpl;
        class CommandListImpl
        {
        public:
            CommandListImpl(DeviceImpl* device);

            CommandListImpl(const CommandListImpl&) = delete;
            CommandListImpl(CommandListImpl&&) = delete;
            CommandListImpl& operator=(const CommandListImpl&) = delete;
            CommandListImpl& operator=(CommandListImpl&&) = delete;

            void reset(implementation::PipelineImpl& pipelineState);

            void setRenderTargets(std::vector<TextureRTV> targets);
            void setRenderTargets(std::vector<TextureRTV> targets, TextureDSV& dsv);
            void clearRenderTargetView(const TextureRTV& target, const Color4f& color);
            void clearDepthStencilView(const TextureDSV& target, float depth, uint8_t stencil);
            void copyBuffer(
                const Buffer& srcBuffer,
                Buffer& dstBuffer,
                uint64_t elements,
                uint32_t srcStartElement = 0,
                uint32_t dstStartElement = 0);
            
            void bindPipe(
                implementation::PipelineImpl& pipelineImpl,
                shaders::PipelineConfiguration* configuration);

            void setViewPorts(const std::vector<Viewport>& viewports);
            void setScissorRects(const std::vector<Rectangle>& rects);

            void bindVertexBuffer(const BufferVBV& buffer);
            void bindIndexBuffer(const BufferIBV& buffer);

            void clearBuffer(Buffer& buffer, uint32_t value, uint32_t startElement, uint32_t numElements);
            void clearTexture(
                Texture& texture, 
                const Color4f& color,
                const SubResource& subResource);

            void clearBufferUAV(BufferUAV& buffer, uint32_t value, uint32_t startElement, uint32_t numElements);
            void clearTextureUAV(TextureUAV& texture, const Color4f& color, const SubResource& subResource);
            void clearTextureDSV(TextureDSV& texture, const Color4f& color, const SubResource& subResource);
            void clearTextureRTV(TextureRTV& texture, const Color4f& color, const SubResource& subResource);

            void draw(uint32_t vertexCount);
            void drawIndexed(
                uint32_t indexCount,
                uint32_t instanceCount,
                uint32_t firstIndex,
                int32_t vertexOffset,
                uint32_t firstInstance);

            void dispatch(
                uint32_t threadGroupCountX,
                uint32_t threadGroupCountY,
                uint32_t threadGroupCountZ);

            void executeIndirect(
                Buffer& argumentBuffer,
                uint64_t argumentBufferOffset
            );

            void begin();
            void end();

            void beginRenderPass(implementation::PipelineImpl& pipeline, int frameBufferIndex);
            void endRenderPass();


            // dunno about these
            // -------------------------------
            void setResourceBarrier(VkSubmitInfo barrier) const;
            VkSubmitInfo& barrier();
            const VkSubmitInfo& barrier() const;
            // -------------------------------
            
            void transition(Texture& resource, ResourceState state);
            void transition(TextureRTV& resource, ResourceState state);
            void transition(TextureSRV& resource, ResourceState state);
            void transition(TextureDSV& resource, ResourceState state);

            void transition(Buffer& resource, ResourceState state);
            void transition(BufferSRV& resource, ResourceState state);
            void transition(BufferIBV& resource, ResourceState state);
            void transition(BufferCBV& resource, ResourceState state);
            void transition(BufferVBV& resource, ResourceState state);

            //void transitionTexture(const Texture& image, ImageLayout from, ImageLayout to);
            void copyTexture(const Texture& src, Texture& dst);

            VkCommandBuffer& native();
            const VkCommandBuffer& native() const;

            bool isOpen() const;
        private:
            DeviceImpl* m_device;
            VkCommandBuffer m_commandList;
            mutable VkSubmitInfo m_barrier;

            VkRenderPassBeginInfo m_renderPassBeginInfo;
            VkClearValue m_clearColor;

            std::shared_ptr<CommandAllocatorImpl> m_allocator;
            bool m_open;

            void bindDescriptorSets(implementation::PipelineImpl& pipeline, const tools::impl_ptr<DescriptorHandleImpl> descriptor);
        };
    }
}

