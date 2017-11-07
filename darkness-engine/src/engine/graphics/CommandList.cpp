#include "engine/graphics/CommandList.h"
#include "engine/graphics/Barrier.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/Device.h"
#include "shaders/ShaderTypes.h"
#include "engine/primitives/Color.h"

#if defined(_WIN32) && !defined(VULKAN)
#include "engine/graphics/dx12/DX12CommandList.h"
#include "engine/graphics/dx12/DX12Device.h"
#endif

#if defined(VULKAN)
#include "engine/graphics/vulkan/VulkanCommandList.h"
#endif

#ifdef __APPLE__
#include "engine/graphics/metal/MetalCommandList.h"
#endif


using namespace tools;
using namespace engine::implementation;

namespace engine
{
    PIMPL_ACCESS_IMPLEMENTATION(CommandList, CommandListImpl)

    CommandList::CommandList(const Device& device)
        : m_impl{ make_unique_impl<CommandListImpl>(const_cast<DeviceImpl*>(&DeviceImplGet::impl(device))) }
    {
    }

    void CommandList::reset(std::shared_ptr<implementation::PipelineImpl> pipelineState)
    {
        m_impl->reset(*pipelineState);
    }

    void CommandList::transition(Texture& resource, ResourceState state)
    {
        m_impl->transition(resource, state);
    }

    void CommandList::transition(TextureRTV& resource, ResourceState state)
    {
        m_impl->transition(resource, state);
    }

    void CommandList::transition(TextureSRV& resource, ResourceState state)
    {
        m_impl->transition(resource, state);
    }

    void CommandList::transition(TextureDSV& resource, ResourceState state)
    {
        m_impl->transition(resource, state);
    }

    void CommandList::transition(Buffer& resource, ResourceState state)
    {
        m_impl->transition(resource, state);
    }

    void CommandList::transition(BufferSRV& resource, ResourceState state)
    {
        m_impl->transition(resource, state);
    }

    void CommandList::transition(BufferIBV& resource, ResourceState state)
    {
        m_impl->transition(resource, state);
    }

    void CommandList::transition(BufferVBV& resource, ResourceState state)
    {
        m_impl->transition(resource, state);
    }

    void CommandList::transition(BufferCBV& resource, ResourceState state)
    {
        m_impl->transition(resource, state);
    }

    Barrier CommandList::createBarrier(
        ResourceBarrierFlags flags,
        const TextureRTV& resource,
        ResourceState before,
        ResourceState after,
        unsigned int subResource,
        const Semaphore& waitSemaphore,
        const Semaphore& signalSemaphore
    )
    {
        return Barrier(
            *this, 
            flags, 
            resource, 
            before, 
            after, 
            subResource,
            waitSemaphore, 
            signalSemaphore
        );
    }

    void CommandList::setRenderTargets(std::vector<TextureRTV> targets)
    {
        for (auto&& target : targets)
        {
            if(target.valid())
                transition(target, ResourceState::RenderTarget);
        }
        m_impl->setRenderTargets(targets);
    }

    void CommandList::setRenderTargets(
        std::vector<TextureRTV> targets,
        TextureDSV& dsv)
    {
        for (auto&& target : targets)
        {
            if (target.valid())
                transition(target, ResourceState::RenderTarget);
        }
        transition(dsv, ResourceState::DepthWrite);
        m_impl->setRenderTargets(targets, dsv);
    }

    void CommandList::clearRenderTargetView(TextureRTV& target, const Color4f& color)
    {
        transition(target, ResourceState::RenderTarget);
        m_impl->clearRenderTargetView(target, color);
    }

    void CommandList::clearDepthStencilView(TextureDSV& target, float depth, uint8_t stencil)
    {
        transition(target, ResourceState::DepthWrite);
        m_impl->clearDepthStencilView(target, depth, stencil);
    }

    void CommandList::copyBuffer(
        Buffer& srcBuffer,
        Buffer& dstBuffer,
        uint64_t elements,
        uint32_t srcStartElement,
        uint32_t dstStartElement)
    {
        if(srcBuffer.description().descriptor.usage != ResourceUsage::CpuToGpu)
            transition(srcBuffer, ResourceState::CopySource);
        transition(dstBuffer, ResourceState::CopyDest);
        m_impl->copyBuffer(
            srcBuffer, 
            dstBuffer, 
            elements,
            srcStartElement,
            dstStartElement);
    }

    void CommandList::bindPipe(
        implementation::PipelineImpl* pipelineImpl,
        shaders::PipelineConfiguration* configuration)
    {
        m_impl->bindPipe(*pipelineImpl, configuration);
        savePipeline(configuration);
    }

    void CommandList::savePipeline(shaders::PipelineConfiguration* configuration)
    {
        auto saveBinding = [this](const shaders::Shader* shader, bool pixelShader)
        {
            auto srvs = shader->texture_srvs();
            auto uavs = shader->texture_uavs();
            auto bsrvs = shader->buffer_srvs();
            auto buavs = shader->buffer_uavs();

            auto bindless_srvs = shader->bindless_texture_srvs();
            auto bindless_uavs = shader->bindless_texture_uavs();
            auto bindless_bsrvs = shader->bindless_buffer_srvs();
            auto bindless_buavs = shader->bindless_buffer_uavs();

            /*for (auto&& srv : srvs) { if (srv.valid()) this->m_boundTextureSRVs.emplace_back(std::move(srv)); }
            for (auto&& uav : uavs) { if (uav.valid()) this->m_boundTextureUAVs.emplace_back(std::move(uav)); }
            for (auto&& bsrv : bsrvs) { if (bsrv.valid()) this->m_boundBufferSRVs.emplace_back(std::move(bsrv)); }
            for (auto&& buav : buavs) { if (buav.valid()) this->m_boundBufferUAVs.emplace_back(std::move(buav)); }*/

            for (auto&& srv : srvs)
            {
                if (srv.valid())
                {
                    if (srv.texture().description().descriptor.usage != ResourceUsage::DepthStencil)
                    {
                        if (srv.texture().state() != ResourceState::Common)
                            this->transition(srv.texture(), pixelShader ? ResourceState::PixelShaderResource : ResourceState::NonPixelShaderResource);
                        else
                            srv.texture().state(pixelShader ? ResourceState::PixelShaderResource : ResourceState::NonPixelShaderResource);
                    }
                    else
                        this->transition(srv.texture(), ResourceState::GenericRead);
                    this->m_boundTextureSRVs.emplace_back(std::move(srv));
                }
            }

            for (auto&& uav : uavs)
            {
                if (uav.valid())
                {
                    if (uav.texture().description().descriptor.usage != ResourceUsage::DepthStencil)
                        this->transition(uav.texture(), ResourceState::UnorderedAccess); 
                    else
                        this->transition(uav.texture(), ResourceState::UnorderedAccess);

                    this->m_boundTextureUAVs.emplace_back(std::move(uav));
                }
            }
            
            for (auto&& bsrv : bsrvs)
            {
                if (bsrv.valid())
                {
                    if (bsrv.buffer().state() != ResourceState::Common)
                        this->transition(bsrv.buffer(), pixelShader ? ResourceState::PixelShaderResource : ResourceState::NonPixelShaderResource);
                    else
                        bsrv.buffer().state(pixelShader ? ResourceState::PixelShaderResource : ResourceState::NonPixelShaderResource);

                    this->m_boundBufferSRVs.emplace_back(std::move(bsrv));
                }
            }
            
            for (auto&& buav : buavs)
            { 
                if (buav.valid())
                {
                    this->transition(buav.buffer(), ResourceState::UnorderedAccess);
                    this->m_boundBufferUAVs.emplace_back(std::move(buav));
                }
            }

            for (auto&& srv : bindless_srvs)
            {
                for (int i = 0; i < srv.size(); ++i)
                {
                    if (srv[i].valid())
                    {
                        if (srv[i].texture().description().descriptor.usage != ResourceUsage::DepthStencil)
                            this->transition(srv[i].texture(), pixelShader ? ResourceState::PixelShaderResource : ResourceState::NonPixelShaderResource);
                        else
                            this->transition(srv[i].texture(), ResourceState::GenericRead);
                        this->m_boundTextureSRVs.emplace_back(std::move(srv[i]));
                    }
                }
            }

            for (auto&& uav : bindless_uavs)
            {
                for (int i = 0; i < uav.size(); ++i)
                {
                    if (uav[i].valid())
                    {
                        if (uav[i].texture().description().descriptor.usage != ResourceUsage::DepthStencil)
                            this->transition(uav[i].texture(), ResourceState::UnorderedAccess);
                        else
                            this->transition(uav[i].texture(), ResourceState::UnorderedAccess);

                        this->m_boundTextureUAVs.emplace_back(std::move(uav[i]));
                    }
                }
            }

            for (auto&& bsrv : bindless_bsrvs)
            {
                for (int i = 0; i < bsrv.size(); ++i)
                {
                    if (bsrv[i].valid())
                    {
                        this->transition(bsrv[i].buffer(), pixelShader ? ResourceState::PixelShaderResource : ResourceState::NonPixelShaderResource);
                        this->m_boundBufferSRVs.emplace_back(std::move(bsrv[i]));
                    }
                }
            }

            for (auto&& buav : bindless_buavs)
            {
                for (int i = 0; i < buav.size(); ++i)
                {
                    if (buav[i].valid())
                    {
                        this->transition(buav[i].buffer(), ResourceState::UnorderedAccess);
                        this->m_boundBufferUAVs.emplace_back(std::move(buav[i]));
                    }
                }
            }
        };

        if (configuration->hasVertexShader())
        {
            saveBinding(configuration->vertexShader(), false);
            for(auto&& range : const_cast<engine::shaders::Shader*>(configuration->vertexShader())->constants())
            {
                this->transition(range.buffer->buffer(), ResourceState::GenericRead);
            }
        }
        if (configuration->hasPixelShader())
        {
            saveBinding(configuration->pixelShader(), true);
            for (auto&& range : const_cast<engine::shaders::Shader*>(configuration->pixelShader())->constants())
            {
                if(range.buffer)
                    this->transition(range.buffer->buffer(), ResourceState::GenericRead);
            }
        }
        if (configuration->hasComputeShader())
        {
            saveBinding(configuration->computeShader(), false);
            for (auto&& range : const_cast<engine::shaders::Shader*>(configuration->computeShader())->constants())
            {
                if (range.buffer)
                    this->transition(range.buffer->buffer(), ResourceState::GenericRead);
            }
        }
        if (configuration->hasDomainShader())
        {
            saveBinding(configuration->domainShader(), false);
            for (auto&& range : const_cast<engine::shaders::Shader*>(configuration->domainShader())->constants())
            {
                if (range.buffer)
                    this->transition(range.buffer->buffer(), ResourceState::GenericRead);
            }
        }
        if (configuration->hasGeometryShader())
        {
            saveBinding(configuration->geometryShader(), false);
            for (auto&& range : const_cast<engine::shaders::Shader*>(configuration->geometryShader())->constants())
            {
                if (range.buffer)
                    this->transition(range.buffer->buffer(), ResourceState::GenericRead);
            }
        }
        if (configuration->hasHullShader())
        {
            saveBinding(configuration->hullShader(), false);
            for (auto&& range : const_cast<engine::shaders::Shader*>(configuration->hullShader())->constants())
            {
                if (range.buffer)
                    this->transition(range.buffer->buffer(), ResourceState::GenericRead);
            }
        }
    }

    void CommandList::setViewPorts(const std::vector<Viewport>& viewports)
    {
        m_impl->setViewPorts(viewports);
    }

    void CommandList::setScissorRects(const std::vector<Rectangle>& rects)
    {
        m_impl->setScissorRects(rects);
    }

    void CommandList::bindVertexBuffer(BufferVBV& buffer)
    {
        transition(buffer, ResourceState::VertexAndConstantBuffer);
        m_impl->bindVertexBuffer(buffer);
    }

    void CommandList::bindIndexBuffer(BufferIBV& buffer)
    {
        transition(buffer, ResourceState::IndexBuffer);
        m_impl->bindIndexBuffer(buffer);
    }

    /*void CommandList::bindDescriptorSets(const Pipeline& pipeline, const DescriptorHandle& descriptor)
    {
        m_impl->bindDescriptorSets(pipeline, descriptor);
    }*/

    void CommandList::clearBuffer(Buffer& buffer, uint32_t value, uint32_t startElement, uint32_t numElements)
    {
        m_impl->clearBuffer(buffer, value, startElement, numElements);
    }

    void CommandList::clearTexture(
        Texture& texture, 
        const Color4f& color,
        const SubResource& subResource)
    {
        m_impl->clearTexture(texture, color, subResource);
    }

    void CommandList::beginRenderPass(std::shared_ptr<implementation::PipelineImpl> pipeline, int frameBufferIndex)
    {
        m_impl->beginRenderPass(*pipeline, frameBufferIndex);
    }

    void CommandList::begin()
    {
        m_impl->begin();
    }

    void CommandList::draw(uint32_t vertexCount)
    {
        m_impl->draw(vertexCount);
    }

    void CommandList::drawIndexed(
        uint32_t indexCount,
        uint32_t instanceCount,
        uint32_t firstIndex,
        int32_t vertexOffset,
        uint32_t firstInstance)
    {
        m_impl->drawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    }

    void CommandList::dispatch(
        uint32_t threadGroupCountX,
        uint32_t threadGroupCountY,
        uint32_t threadGroupCountZ)
    {
        m_impl->dispatch(
            threadGroupCountX,
            threadGroupCountY,
            threadGroupCountZ);
    }

    void CommandList::end()
    {
        m_impl->end();
    }

    void CommandList::endRenderPass()
    {
        m_impl->endRenderPass();
    }

    /*void CommandList::transitionTexture(const Texture& image, ImageLayout from, ImageLayout to)
    {
        m_impl->transitionTexture(image, from, to);
    }*/

    void CommandList::copyTexture(Texture& src, Texture& dst)
    {
        transition(src, ResourceState::CopySource);
        
        if (dst.state() != ResourceState::Common)
            transition(dst, ResourceState::CopyDest);
        dst.state(ResourceState::CopyDest);
        m_impl->copyTexture(src, dst);
    }
}
