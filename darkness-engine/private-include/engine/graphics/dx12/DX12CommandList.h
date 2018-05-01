#pragma once

#include "engine/graphics/dx12/DX12Headers.h"
#include "engine/graphics/dx12/DX12Common.h"
#include "engine/graphics/CommonNoDep.h"
#include "tools/ComPtr.h"
#include "tools/SmartPimpl.h"

#include <vector>
#include <memory>

namespace engine
{
    namespace shaders
    {
        class PipelineConfiguration;
    }

    class DescriptorHandle;
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
    class Semaphore;
    class Queue;
    //enum class ImageLayout;
    struct SubResource;
    struct Viewport;
    struct Rectangle;

    namespace implementation
    {
        class BufferUAVImpl;
        class DeviceImpl;
        class PipelineImpl;
        class CommandAllocatorImpl;
        class CommandListImpl
        {
        public:
            CommandListImpl(DeviceImpl* device, CommandListType type);
            ~CommandListImpl();

            CommandListImpl(const CommandListImpl&) = default;
            CommandListImpl(CommandListImpl&&) = default;
            CommandListImpl& operator=(const CommandListImpl&) = default;
            CommandListImpl& operator=(CommandListImpl&&) = default;

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
            //void bindDescriptorSets(const Pipeline& pipeline, const DescriptorHandle& descriptor);

            void clearBuffer(Buffer& buffer, uint32_t value, uint32_t startElement, uint32_t numElements);
            void clearTexture(
                Texture& texture,
                const Color4f& color,
                const SubResource& subResource);

            void clearBufferUAV(BufferUAV& buffer, uint32_t value, uint32_t startElement, uint32_t numElements);
            void clearTextureUAV(TextureUAV& texture, const Color4f& color, const SubResource& subResource);
            void clearTextureDSV(TextureDSV& texture, const Color4f& color, const SubResource& subResource);
            void clearTextureRTV(TextureRTV& texture, const Color4f& color, const SubResource& subResource);

            void setStructureCounter(BufferUAV& buffer, uint32_t value);
            void copyStructureCounter(BufferUAV& srcBuffer, Buffer& dst, uint32_t dstByteOffset);

            void draw(uint32_t vertexCount);
            void drawIndirect(Buffer& indirectArguments, uint64_t argumentBufferOffset);
            void drawIndexed(
                uint32_t indexCount,
                uint32_t instanceCount,
                uint32_t firstIndex,
                int32_t vertexOffset,
                uint32_t firstInstance);
            void drawIndexedIndirect(Buffer& indirectArguments, uint64_t argumentBufferOffset);

            void dispatch(
                uint32_t threadGroupCountX,
                uint32_t threadGroupCountY,
                uint32_t threadGroupCountZ);

            void dispatchIndirect(
                Buffer& indirectArguments,
                uint64_t argumentBufferOffset);

            void executeBundle(CommandListImpl& commandList);

            void begin();
            void end();

            void beginRenderPass(implementation::PipelineImpl& pipeline, int frameBufferIndex);
            void endRenderPass();

            void transition(Texture& resource, ResourceState state, const SubResource& subResource = SubResource());
            void transition(TextureRTV& resource, ResourceState state);
            void transition(TextureSRV& resource, ResourceState state);
            void transition(TextureDSV& resource, ResourceState state);

            void transition(Buffer& resource, ResourceState state);
            void transition(BufferSRV& resource, ResourceState state);
            void transition(BufferIBV& resource, ResourceState state);
            void transition(BufferCBV& resource, ResourceState state);
            void transition(BufferVBV& resource, ResourceState state);

            void copyTexture(const TextureSRV& src, TextureUAV& dst);
            void copyTexture(const TextureSRV& src, TextureSRV& dst);
            void copyTexture(const TextureSRV& src, BufferUAV& dst);
            void copyTexture(const TextureSRV& src, BufferSRV& dst);

            ID3D12GraphicsCommandList* native();
            ID3D12GraphicsCommandList* native() const;

            bool isOpen() const;
        private:
            DeviceImpl* m_device;
            std::shared_ptr<CommandAllocatorImpl> m_allocator;
            tools::ComPtr<ID3D12GraphicsCommandList> m_commandList;
            std::shared_ptr<Semaphore> m_temporary;
            std::vector<D3D12_RESOURCE_BARRIER> m_barriers;
            DescriptorHeaps m_descriptorHeaps;
            bool m_open;
            ID3D12PipelineState* m_lastPipelineState;
            ID3D12RootSignature* m_rootSignature;

            std::vector<D3D12_INDIRECT_ARGUMENT_DESC> m_argumentDescs;
            std::vector<ID3D12CommandSignature*> m_commandSignatures;

            std::vector<uint64_t> m_lastRootDescriptor;
            D3D_PRIMITIVE_TOPOLOGY m_lastTopology;
            std::vector<TextureRTV> m_clearTargetsAlive;
            std::vector<BufferUAVImpl> m_bufferUAVClearTargets;
        };
    }
}

