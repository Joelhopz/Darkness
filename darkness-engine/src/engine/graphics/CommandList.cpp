#include "engine/graphics/CommandList.h"
#include "engine/graphics/Barrier.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/Queue.h"
#include "engine/graphics/Fence.h"
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

    CommandList::CommandList(const Device& device, CommandListType type)
        : m_impl{ make_unique_impl<CommandListImpl>(const_cast<DeviceImpl*>(&DeviceImplGet::impl(device)), type) }
        , m_device{ &device }
        , m_corePipelines{ const_cast<Device*>(&device)->corePipelines() }
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

    void CommandList::transition(Texture& resource, ResourceState state, const SubResource& subResource)
    {
        m_impl->transition(resource, state, subResource);
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
        m_lastSetRTVFormats.clear();
        for (auto&& target : targets)
        {
            m_lastSetRTVFormats.emplace_back(target.format());
            if(target.valid())
                transition(target, ResourceState::RenderTarget);
        }
        m_impl->setRenderTargets(targets);
    }

    void CommandList::setRenderTargets(
        std::vector<TextureRTV> targets,
        TextureDSV& dsv)
    {
        m_lastSetRTVFormats.clear();
        for (auto&& target : targets)
        {
            m_lastSetRTVFormats.emplace_back(target.format());
            if (target.valid())
                transition(target, ResourceState::RenderTarget);
        }
        transition(dsv, ResourceState::DepthWrite);
        m_lastSetDSVFormat = dsv.format();
        m_impl->setRenderTargets(targets, dsv);
    }

    void CommandList::clearRenderTargetView(TextureRTV& target)
    {
        transition(target, ResourceState::RenderTarget);
        auto clearValue = target.texture().description().descriptor.optimizedClearValue;
        m_impl->clearRenderTargetView(target, { clearValue.x, clearValue.y, clearValue.z, clearValue.w });
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

    void CommandList::clearTexture(Texture& texture, float value, const SubResource& subResource)
    {
        auto width = texture.width() >> subResource.firstMipLevel;
        auto height = texture.height() >> subResource.firstMipLevel;
        auto depth = texture.depth();

        switch (formatComponents(texture.format()))
        {
            case 1:
            {
                switch (texture.dimension())
                {
                    case ResourceDimension::Texture1D:
                    {
                        m_clearUAVs.emplace_back(m_device->createTextureUAV(texture, subResource));
                        m_corePipelines->clearTexture1df.cs.tex = m_clearUAVs.back();
                        m_corePipelines->clearTexture1df.cs.size = { width, 1u, 1u, 1u };
                        m_corePipelines->clearTexture1df.cs.value = value;
                        bindPipe(m_corePipelines->clearTexture1df);
                        dispatch(roundUpToMultiple(width, 64u) / 64u, 1, 1);
                        break;
                    }
                    case ResourceDimension::Texture2D:
                    {
                        m_clearUAVs.emplace_back(m_device->createTextureUAV(texture, subResource));
                        m_corePipelines->clearTexture2df.cs.tex = m_clearUAVs.back();
                        m_corePipelines->clearTexture2df.cs.size = { width, height, 1u, 1u };
                        m_corePipelines->clearTexture2df.cs.value = value;
                        bindPipe(m_corePipelines->clearTexture2df);
                        dispatch(
                            roundUpToMultiple(width, 9u) / 9u,
                            roundUpToMultiple(height, 9u) / 9u,
                            1);
                        break;
                    }
                    case ResourceDimension::Texture3D:
                    {
                        m_clearUAVs.emplace_back(m_device->createTextureUAV(texture, subResource));
                        m_corePipelines->clearTexture3df.cs.tex = m_clearUAVs.back();
                        m_corePipelines->clearTexture3df.cs.size = { width, height, depth, 1u };
                        m_corePipelines->clearTexture3df.cs.value = value;
                        bindPipe(m_corePipelines->clearTexture3df);
                        dispatch(
                            roundUpToMultiple(width, 4u) / 4u,
                            roundUpToMultiple(height, 4u) / 4u,
                            roundUpToMultiple(depth, 4u) / 4u);
                        break;
                    }
                }
                break;
            }
            case 2:
            {
                switch (texture.dimension())
                {
                    case ResourceDimension::Texture1D:
                    {
                        m_clearUAVs.emplace_back(m_device->createTextureUAV(texture, subResource));
                        m_corePipelines->clearTexture1df2.cs.tex = m_clearUAVs.back();
                        m_corePipelines->clearTexture1df2.cs.size = { width, 1u, 1u, 1u };
                        m_corePipelines->clearTexture1df2.cs.value = { value, value };
                        bindPipe(m_corePipelines->clearTexture1df2);
                        dispatch(roundUpToMultiple(width, 64u) / 64u, 1, 1);
                        break;
                    }
                    case ResourceDimension::Texture2D:
                    {
                        m_clearUAVs.emplace_back(m_device->createTextureUAV(texture, subResource));
                        m_corePipelines->clearTexture2df2.cs.tex = m_clearUAVs.back();
                        m_corePipelines->clearTexture2df2.cs.size = { width, height, 1u, 1u };
                        m_corePipelines->clearTexture2df2.cs.value = { value, value };
                        bindPipe(m_corePipelines->clearTexture2df2);
                        dispatch(
                            roundUpToMultiple(width, 9u) / 9u,
                            roundUpToMultiple(height, 9u) / 9u,
                            1);
                        break;
                    }
                    case ResourceDimension::Texture3D:
                    {
                        m_clearUAVs.emplace_back(m_device->createTextureUAV(texture, subResource));
                        m_corePipelines->clearTexture3df2.cs.tex = m_clearUAVs.back();
                        m_corePipelines->clearTexture3df2.cs.size = { width, height, depth, 1u };
                        m_corePipelines->clearTexture3df2.cs.value = { value, value };
                        bindPipe(m_corePipelines->clearTexture3df2);
                        dispatch(
                            roundUpToMultiple(width, 4u) / 4u,
                            roundUpToMultiple(height, 4u) / 4u,
                            roundUpToMultiple(depth, 4u) / 4u);
                        break;
                    }
                }
                break;
            }
            case 3:
            {
                switch (texture.dimension())
                {
                    case ResourceDimension::Texture1D:
                    {
                        m_clearUAVs.emplace_back(m_device->createTextureUAV(texture, subResource));
                        m_corePipelines->clearTexture1df3.cs.tex = m_clearUAVs.back();
                        m_corePipelines->clearTexture1df3.cs.size = { width, 1u, 1u, 1u };
                        m_corePipelines->clearTexture1df3.cs.value = { value, value, value };
                        bindPipe(m_corePipelines->clearTexture1df3);
                        dispatch(roundUpToMultiple(width, 64u) / 64u, 1, 1);
                        break;
                    }
                    case ResourceDimension::Texture2D:
                    {
                        m_clearUAVs.emplace_back(m_device->createTextureUAV(texture, subResource));
                        m_corePipelines->clearTexture2df3.cs.tex = m_clearUAVs.back();
                        m_corePipelines->clearTexture2df3.cs.size = { width, height, 1u, 1u };
                        m_corePipelines->clearTexture2df3.cs.value = { value, value, value };
                        bindPipe(m_corePipelines->clearTexture2df3);
                        dispatch(
                            roundUpToMultiple(width, 9u) / 9u,
                            roundUpToMultiple(height, 9u) / 9u,
                            1);
                        break;
                    }
                    case ResourceDimension::Texture3D:
                    {
                        m_clearUAVs.emplace_back(m_device->createTextureUAV(texture, subResource));
                        m_corePipelines->clearTexture3df3.cs.tex = m_clearUAVs.back();
                        m_corePipelines->clearTexture3df3.cs.size = { width, height, depth, 1u };
                        m_corePipelines->clearTexture3df3.cs.value = { value, value, value };
                        bindPipe(m_corePipelines->clearTexture3df3);
                        dispatch(
                            roundUpToMultiple(width, 4u) / 4u,
                            roundUpToMultiple(height, 4u) / 4u,
                            roundUpToMultiple(depth, 4u) / 4u);
                        break;
                    }
                }
                break;
            }
            case 4:
            {
                switch (texture.dimension())
                {
                    case ResourceDimension::Texture1D:
                    {
                        m_clearUAVs.emplace_back(m_device->createTextureUAV(texture, subResource));
                        m_corePipelines->clearTexture1df4.cs.tex = m_clearUAVs.back();
                        m_corePipelines->clearTexture1df4.cs.size = { width, 1u, 1u, 1u };
                        m_corePipelines->clearTexture1df4.cs.value = { value, value, value, value };
                        bindPipe(m_corePipelines->clearTexture1df4);
                        dispatch(roundUpToMultiple(width, 64u) / 64u, 1, 1);
                        break;
                    }
                    case ResourceDimension::Texture2D:
                    {
                        m_clearUAVs.emplace_back(m_device->createTextureUAV(texture, subResource));
                        m_corePipelines->clearTexture2df4.cs.tex = m_clearUAVs.back();
                        m_corePipelines->clearTexture2df4.cs.size = { width, height, 1u, 1u };
                        m_corePipelines->clearTexture2df4.cs.value = { value, value, value, value };
                        bindPipe(m_corePipelines->clearTexture2df4);
                        dispatch(
                            roundUpToMultiple(width, 9u) / 9u,
                            roundUpToMultiple(height, 9u) / 9u,
                            1);
                        break;
                    }
                    case ResourceDimension::Texture3D:
                    {
                        m_clearUAVs.emplace_back(m_device->createTextureUAV(texture, subResource));
                        m_corePipelines->clearTexture3df4.cs.tex = m_clearUAVs.back();
                        m_corePipelines->clearTexture3df4.cs.size = { width, height, depth, 1u };
                        m_corePipelines->clearTexture3df4.cs.value = { value, value, value, value };
                        bindPipe(m_corePipelines->clearTexture3df4);
                        dispatch(
                            roundUpToMultiple(width, 4u) / 4u,
                            roundUpToMultiple(height, 4u) / 4u,
                            roundUpToMultiple(depth, 4u) / 4u);
                        break;
                    }
                }
                break;
            }
        }
    }

    void CommandList::clearTexture(Texture& texture, Vector4f value, const SubResource& subResource)
    {
        switch (formatComponents(texture.format()))
        {
            case 1:
            {
                switch (texture.dimension())
                {
                    case ResourceDimension::Texture1D:
                    {
                        m_corePipelines->clearTexture1df.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture1df.cs.size = { texture.width(), 1u, 1u, 1u };
                        m_corePipelines->clearTexture1df.cs.value = value.x;
                        bindPipe(m_corePipelines->clearTexture1df);
                        dispatch(roundUpToMultiple(texture.width(), 64u) / 64u, 1, 1);
                        break;
                    }
                    case ResourceDimension::Texture2D:
                    {
                        m_corePipelines->clearTexture2df.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture2df.cs.size = { texture.width(), texture.height(), 1u, 1u };
                        m_corePipelines->clearTexture2df.cs.value = value.x;
                        bindPipe(m_corePipelines->clearTexture2df);
                        dispatch(
                            roundUpToMultiple(texture.width(), 9u) / 9u,
                            roundUpToMultiple(texture.height(), 9u) / 9u,
                            1);
                        break;
                    }
                    case ResourceDimension::Texture3D:
                    {
                        m_corePipelines->clearTexture3df.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture3df.cs.size = { texture.width(), texture.height(), texture.depth(), 1u };
                        m_corePipelines->clearTexture3df.cs.value = value.x;
                        bindPipe(m_corePipelines->clearTexture3df);
                        dispatch(
                            roundUpToMultiple(texture.width(), 4u) / 4u,
                            roundUpToMultiple(texture.height(), 4u) / 4u,
                            roundUpToMultiple(texture.depth(), 4u) / 4u);
                        break;
                    }
                }
                break;
            }
            case 2:
            {
                switch (texture.dimension())
                {
                    case ResourceDimension::Texture1D:
                    {
                        m_corePipelines->clearTexture1df2.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture1df2.cs.size = { texture.width(), 1u, 1u, 1u };
                        m_corePipelines->clearTexture1df2.cs.value = value.xy();
                        bindPipe(m_corePipelines->clearTexture1df2);
                        dispatch(roundUpToMultiple(texture.width(), 64u) / 64u, 1, 1);
                        break;
                    }
                    case ResourceDimension::Texture2D:
                    {
                        m_corePipelines->clearTexture2df2.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture2df2.cs.size = { texture.width(), texture.height(), 1u, 1u };
                        m_corePipelines->clearTexture2df2.cs.value = value.xy();
                        bindPipe(m_corePipelines->clearTexture2df2);
                        dispatch(
                            roundUpToMultiple(texture.width(), 9u) / 9u,
                            roundUpToMultiple(texture.height(), 9u) / 9u,
                            1);
                        break;
                    }
                    case ResourceDimension::Texture3D:
                    {
                        m_corePipelines->clearTexture3df2.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture3df2.cs.size = { texture.width(), texture.height(), texture.depth(), 1u };
                        m_corePipelines->clearTexture3df2.cs.value = value.xy();
                        bindPipe(m_corePipelines->clearTexture3df2);
                        dispatch(
                            roundUpToMultiple(texture.width(), 4u) / 4u,
                            roundUpToMultiple(texture.height(), 4u) / 4u,
                            roundUpToMultiple(texture.depth(), 4u) / 4u);
                        break;
                    }
                }
                break;
            }
            case 3:
            {
                switch (texture.dimension())
                {
                    case ResourceDimension::Texture1D:
                    {
                        m_corePipelines->clearTexture1df3.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture1df3.cs.size = { texture.width(), 1u, 1u, 1u };
                        m_corePipelines->clearTexture1df3.cs.value = value.xyz();
                        bindPipe(m_corePipelines->clearTexture1df3);
                        dispatch(roundUpToMultiple(texture.width(), 64u) / 64u, 1, 1);
                        break;
                    }
                    case ResourceDimension::Texture2D:
                    {
                        m_corePipelines->clearTexture2df3.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture2df3.cs.size = { texture.width(), texture.height(), 1u, 1u };
                        m_corePipelines->clearTexture2df3.cs.value = value.xyz();
                        bindPipe(m_corePipelines->clearTexture2df3);
                        dispatch(
                            roundUpToMultiple(texture.width(), 9u) / 9u,
                            roundUpToMultiple(texture.height(), 9u) / 9u,
                            1);
                        break;
                    }
                    case ResourceDimension::Texture3D:
                    {
                        m_corePipelines->clearTexture3df3.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture3df3.cs.size = { texture.width(), texture.height(), texture.depth(), 1u };
                        m_corePipelines->clearTexture3df3.cs.value = value.xyz();
                        bindPipe(m_corePipelines->clearTexture3df3);
                        dispatch(
                            roundUpToMultiple(texture.width(), 4u) / 4u,
                            roundUpToMultiple(texture.height(), 4u) / 4u,
                            roundUpToMultiple(texture.depth(), 4u) / 4u);
                        break;
                    }
                }
                break;
            }
            case 4:
            {
                switch (texture.dimension())
                {
                    case ResourceDimension::Texture1D:
                    {
                        m_corePipelines->clearTexture1df4.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture1df4.cs.size = { texture.width(), 1u, 1u, 1u };
                        m_corePipelines->clearTexture1df4.cs.value = value;
                        bindPipe(m_corePipelines->clearTexture1df4);
                        dispatch(roundUpToMultiple(texture.width(), 64u) / 64u, 1, 1);
                        break;
                    }
                    case ResourceDimension::Texture2D:
                    {
                        m_corePipelines->clearTexture2df4.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture2df4.cs.size = { texture.width(), texture.height(), 1u, 1u };
                        m_corePipelines->clearTexture2df4.cs.value = value;
                        bindPipe(m_corePipelines->clearTexture2df4);
                        dispatch(
                            roundUpToMultiple(texture.width(), 9u) / 9u,
                            roundUpToMultiple(texture.height(), 9u) / 9u,
                            1);
                        break;
                    }
                    case ResourceDimension::Texture3D:
                    {
                        m_corePipelines->clearTexture3df4.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture3df4.cs.size = { texture.width(), texture.height(), texture.depth(), 1u };
                        m_corePipelines->clearTexture3df4.cs.value = value;
                        bindPipe(m_corePipelines->clearTexture3df4);
                        dispatch(
                            roundUpToMultiple(texture.width(), 4u) / 4u,
                            roundUpToMultiple(texture.height(), 4u) / 4u,
                            roundUpToMultiple(texture.depth(), 4u) / 4u);
                        break;
                    }
                }
                break;
            }
        }
    }

    void CommandList::clearTexture(Texture& texture, uint32_t value, const SubResource& subResource)
    {
        switch (formatComponents(texture.format()))
        {
            case 1:
            {
                switch (texture.dimension())
                {
                    case ResourceDimension::Texture1D:
                    {
                        m_corePipelines->clearTexture1du.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture1du.cs.size = { texture.width(), 1u, 1u, 1u };
                        m_corePipelines->clearTexture1du.cs.value.x = value;
                        bindPipe(m_corePipelines->clearTexture1du);
                        dispatch(roundUpToMultiple(texture.width(), 64u) / 64u, 1, 1);
                        break;
                    }
                    case ResourceDimension::Texture2D:
                    {
                        m_corePipelines->clearTexture2du.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture2du.cs.size = { texture.width(), texture.height(), 1u, 1u };
                        m_corePipelines->clearTexture2du.cs.value.x = value;
                        bindPipe(m_corePipelines->clearTexture2du);
                        dispatch(
                            roundUpToMultiple(texture.width(), 8u) / 8u,
                            roundUpToMultiple(texture.height(), 8u) / 8u,
                            1);
                        break;
                    }
                    case ResourceDimension::Texture3D:
                    {
                        m_corePipelines->clearTexture3du.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture3du.cs.size = { texture.width(), texture.height(), texture.depth(), 1u };
                        m_corePipelines->clearTexture3du.cs.value.x = value;
                        bindPipe(m_corePipelines->clearTexture3du);
                        dispatch(
                            roundUpToMultiple(texture.width(), 4u) / 4u,
                            roundUpToMultiple(texture.height(), 4u) / 4u,
                            roundUpToMultiple(texture.depth(), 4u) / 4u);
                        break;
                    }
                }
                break;
            }
            case 2:
            {
                switch (texture.dimension())
                {
                    case ResourceDimension::Texture1D:
                    {
                        m_corePipelines->clearTexture1du2.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture1du2.cs.size = { texture.width(), 1u, 1u, 1u };
                        m_corePipelines->clearTexture1du2.cs.value.x = value;
                        m_corePipelines->clearTexture1du2.cs.value.y = value;
                        bindPipe(m_corePipelines->clearTexture1du2);
                        dispatch(roundUpToMultiple(texture.width(), 64u) / 64u, 1, 1);
                        break;
                    }
                    case ResourceDimension::Texture2D:
                    {
                        m_corePipelines->clearTexture2du2.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture2du2.cs.size = { texture.width(), texture.height(), 1u, 1u };
                        m_corePipelines->clearTexture2du2.cs.value.x = value;
                        m_corePipelines->clearTexture2du2.cs.value.y = value;
                        bindPipe(m_corePipelines->clearTexture2du2);
                        dispatch(
                            roundUpToMultiple(texture.width(), 8u) / 8u,
                            roundUpToMultiple(texture.height(), 8u) / 8u,
                            1);
                        break;
                    }
                    case ResourceDimension::Texture3D:
                    {
                        m_corePipelines->clearTexture3du2.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture3du2.cs.size = { texture.width(), texture.height(), texture.depth(), 1u };
                        m_corePipelines->clearTexture3du2.cs.value.x = value;
                        m_corePipelines->clearTexture3du2.cs.value.y = value;
                        bindPipe(m_corePipelines->clearTexture3du2);
                        dispatch(
                            roundUpToMultiple(texture.width(), 4u) / 4u,
                            roundUpToMultiple(texture.height(), 4u) / 4u,
                            roundUpToMultiple(texture.depth(), 4u) / 4u);
                        break;
                    }
                }
                break;
            }
            case 3:
            {
                switch (texture.dimension())
                {
                    case ResourceDimension::Texture1D:
                    {
                        m_corePipelines->clearTexture1du3.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture1du3.cs.size = { texture.width(), 1u, 1u, 1u };
                        m_corePipelines->clearTexture1du3.cs.value.x = value;
                        m_corePipelines->clearTexture1du3.cs.value.y = value;
                        m_corePipelines->clearTexture1du3.cs.value.z = value;
                        bindPipe(m_corePipelines->clearTexture1du3);
                        dispatch(roundUpToMultiple(texture.width(), 64u) / 64u, 1, 1);
                        break;
                    }
                    case ResourceDimension::Texture2D:
                    {
                        m_corePipelines->clearTexture2du3.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture2du3.cs.size = { texture.width(), texture.height(), 1u, 1u };
                        m_corePipelines->clearTexture2du3.cs.value.x = value;
                        m_corePipelines->clearTexture2du3.cs.value.y = value;
                        m_corePipelines->clearTexture2du3.cs.value.z = value;
                        bindPipe(m_corePipelines->clearTexture2du3);
                        dispatch(
                            roundUpToMultiple(texture.width(), 8u) / 8u,
                            roundUpToMultiple(texture.height(), 8u) / 8u,
                            1);
                        break;
                    }
                    case ResourceDimension::Texture3D:
                    {
                        m_corePipelines->clearTexture3du3.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture3du3.cs.size = { texture.width(), texture.height(), texture.depth(), 1u };
                        m_corePipelines->clearTexture3du3.cs.value.x = value;
                        m_corePipelines->clearTexture3du3.cs.value.y = value;
                        m_corePipelines->clearTexture3du3.cs.value.z = value;
                        bindPipe(m_corePipelines->clearTexture3du3);
                        dispatch(
                            roundUpToMultiple(texture.width(), 4u) / 4u,
                            roundUpToMultiple(texture.height(), 4u) / 4u,
                            roundUpToMultiple(texture.depth(), 4u) / 4u);
                        break;
                    }
                }
                break;
            }
            case 4:
            {
                switch (texture.dimension())
                {
                    case ResourceDimension::Texture1D:
                    {
                        m_corePipelines->clearTexture1du4.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture1du4.cs.size = { texture.width(), 1u, 1u, 1u };
                        m_corePipelines->clearTexture1du4.cs.value.x = value;
                        m_corePipelines->clearTexture1du4.cs.value.y = value;
                        m_corePipelines->clearTexture1du4.cs.value.z = value;
                        m_corePipelines->clearTexture1du4.cs.value.w = value;
                        bindPipe(m_corePipelines->clearTexture1du4);
                        dispatch(roundUpToMultiple(texture.width(), 64u) / 64u, 1, 1);
                        break;
                    }
                    case ResourceDimension::Texture2D:
                    {
                        m_corePipelines->clearTexture2du4.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture2du4.cs.size = { texture.width(), texture.height(), 1u, 1u };
                        m_corePipelines->clearTexture2du4.cs.value.x = value;
                        m_corePipelines->clearTexture2du4.cs.value.y = value;
                        m_corePipelines->clearTexture2du4.cs.value.z = value;
                        m_corePipelines->clearTexture2du4.cs.value.w = value;
                        bindPipe(m_corePipelines->clearTexture2du4);
                        dispatch(
                            roundUpToMultiple(texture.width(), 8u) / 8u,
                            roundUpToMultiple(texture.height(), 8u) / 8u,
                            1);
                        break;
                    }
                    case ResourceDimension::Texture3D:
                    {
                        m_corePipelines->clearTexture3du4.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture3du4.cs.size = { texture.width(), texture.height(), texture.depth(), 1u };
                        m_corePipelines->clearTexture3du4.cs.value.x = value;
                        m_corePipelines->clearTexture3du4.cs.value.y = value;
                        m_corePipelines->clearTexture3du4.cs.value.z = value;
                        m_corePipelines->clearTexture3du4.cs.value.w = value;
                        bindPipe(m_corePipelines->clearTexture3du4);
                        dispatch(
                            roundUpToMultiple(texture.width(), 4u) / 4u,
                            roundUpToMultiple(texture.height(), 4u) / 4u,
                            roundUpToMultiple(texture.depth(), 4u) / 4u);
                        break;
                    }
                }
                break;
            }
        }
    }

    void CommandList::clearTexture(Texture& texture, Vector4<uint32_t> value, const SubResource& subResource)
    {
        switch (formatComponents(texture.format()))
        {
            case 1:
            {
                switch (texture.dimension())
                {
                    case ResourceDimension::Texture1D:
                    {
                        m_corePipelines->clearTexture1du.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture1du.cs.size = { texture.width(), 1u, 1u, 1u };
                        m_corePipelines->clearTexture1du.cs.value.x = value.x;
                        bindPipe(m_corePipelines->clearTexture1du);
                        dispatch(roundUpToMultiple(texture.width(), 64u) / 64u, 1, 1);
                        break;
                    }
                    case ResourceDimension::Texture2D:
                    {
                        m_corePipelines->clearTexture2du.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture2du.cs.size = { texture.width(), texture.height(), 1u, 1u };
                        m_corePipelines->clearTexture2du.cs.value.x = value.x;
                        bindPipe(m_corePipelines->clearTexture2du);
                        dispatch(
                            roundUpToMultiple(texture.width(), 8u) / 8u,
                            roundUpToMultiple(texture.height(), 8u) / 8u,
                            1);
                        break;
                    }
                    case ResourceDimension::Texture3D:
                    {
                        m_corePipelines->clearTexture3du.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture3du.cs.size = { texture.width(), texture.height(), texture.depth(), 1u };
                        m_corePipelines->clearTexture3du.cs.value.x = value.x;
                        bindPipe(m_corePipelines->clearTexture3du);
                        dispatch(
                            roundUpToMultiple(texture.width(), 4u) / 4u,
                            roundUpToMultiple(texture.height(), 4u) / 4u,
                            roundUpToMultiple(texture.depth(), 4u) / 4u);
                        break;
                    }
                }
                break;
            }
            case 2:
            {
                switch (texture.dimension())
                {
                    case ResourceDimension::Texture1D:
                    {
                        m_corePipelines->clearTexture1du2.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture1du2.cs.size = { texture.width(), 1u, 1u, 1u };
                        m_corePipelines->clearTexture1du2.cs.value.x = value.x;
                        m_corePipelines->clearTexture1du2.cs.value.y = value.y;
                        bindPipe(m_corePipelines->clearTexture1du2);
                        dispatch(roundUpToMultiple(texture.width(), 64u) / 64u, 1, 1);
                        break;
                    }
                    case ResourceDimension::Texture2D:
                    {
                        m_corePipelines->clearTexture2du2.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture2du2.cs.size = { texture.width(), texture.height(), 1u, 1u };
                        m_corePipelines->clearTexture2du2.cs.value.x = value.x;
                        m_corePipelines->clearTexture2du2.cs.value.y = value.y;
                        bindPipe(m_corePipelines->clearTexture2du2);
                        dispatch(
                            roundUpToMultiple(texture.width(), 8u) / 8u,
                            roundUpToMultiple(texture.height(), 8u) / 8u,
                            1);
                        break;
                    }
                    case ResourceDimension::Texture3D:
                    {
                        m_corePipelines->clearTexture3du2.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture3du2.cs.size = { texture.width(), texture.height(), texture.depth(), 1u };
                        m_corePipelines->clearTexture3du2.cs.value.x = value.x;
                        m_corePipelines->clearTexture3du2.cs.value.y = value.y;
                        bindPipe(m_corePipelines->clearTexture3du2);
                        dispatch(
                            roundUpToMultiple(texture.width(), 4u) / 4u,
                            roundUpToMultiple(texture.height(), 4u) / 4u,
                            roundUpToMultiple(texture.depth(), 4u) / 4u);
                        break;
                    }
                }
                break;
            }
            case 3:
            {
                switch (texture.dimension())
                {
                    case ResourceDimension::Texture1D:
                    {
                        m_corePipelines->clearTexture1du3.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture1du3.cs.size = { texture.width(), 1u, 1u, 1u };
                        m_corePipelines->clearTexture1du3.cs.value.x = value.x;
                        m_corePipelines->clearTexture1du3.cs.value.y = value.y;
                        m_corePipelines->clearTexture1du3.cs.value.z = value.z;
                        bindPipe(m_corePipelines->clearTexture1du3);
                        dispatch(roundUpToMultiple(texture.width(), 64u) / 64u, 1, 1);
                        break;
                    }
                    case ResourceDimension::Texture2D:
                    {
                        m_corePipelines->clearTexture2du3.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture2du3.cs.size = { texture.width(), texture.height(), 1u, 1u };
                        m_corePipelines->clearTexture2du3.cs.value.x = value.x;
                        m_corePipelines->clearTexture2du3.cs.value.y = value.y;
                        m_corePipelines->clearTexture2du3.cs.value.z = value.z;
                        bindPipe(m_corePipelines->clearTexture2du3);
                        dispatch(
                            roundUpToMultiple(texture.width(), 8u) / 8u,
                            roundUpToMultiple(texture.height(), 8u) / 8u,
                            1);
                        break;
                    }
                    case ResourceDimension::Texture3D:
                    {
                        m_corePipelines->clearTexture3du3.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture3du3.cs.size = { texture.width(), texture.height(), texture.depth(), 1u };
                        m_corePipelines->clearTexture3du3.cs.value.x = value.x;
                        m_corePipelines->clearTexture3du3.cs.value.y = value.y;
                        m_corePipelines->clearTexture3du3.cs.value.z = value.z;
                        bindPipe(m_corePipelines->clearTexture3du3);
                        dispatch(
                            roundUpToMultiple(texture.width(), 4u) / 4u,
                            roundUpToMultiple(texture.height(), 4u) / 4u,
                            roundUpToMultiple(texture.depth(), 4u) / 4u);
                        break;
                    }
                }
                break;
            }
            case 4:
            {
                switch (texture.dimension())
                {
                    case ResourceDimension::Texture1D:
                    {
                        m_corePipelines->clearTexture1du4.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture1du4.cs.size = { texture.width(), 1u, 1u, 1u };
                        m_corePipelines->clearTexture1du4.cs.value.x = value.x;
                        m_corePipelines->clearTexture1du4.cs.value.y = value.y;
                        m_corePipelines->clearTexture1du4.cs.value.z = value.z;
                        m_corePipelines->clearTexture1du4.cs.value.w = value.w;
                        bindPipe(m_corePipelines->clearTexture1du4);
                        dispatch(roundUpToMultiple(texture.width(), 64u) / 64u, 1, 1);
                        break;
                    }
                    case ResourceDimension::Texture2D:
                    {
                        m_corePipelines->clearTexture2du4.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture2du4.cs.size = { texture.width(), texture.height(), 1u, 1u };
                        m_corePipelines->clearTexture2du4.cs.value.x = value.x;
                        m_corePipelines->clearTexture2du4.cs.value.y = value.y;
                        m_corePipelines->clearTexture2du4.cs.value.z = value.z;
                        m_corePipelines->clearTexture2du4.cs.value.w = value.w;
                        bindPipe(m_corePipelines->clearTexture2du4);
                        dispatch(
                            roundUpToMultiple(texture.width(), 8u) / 8u,
                            roundUpToMultiple(texture.height(), 8u) / 8u,
                            1);
                        break;
                    }
                    case ResourceDimension::Texture3D:
                    {
                        m_corePipelines->clearTexture3du4.cs.tex = m_device->createTextureUAV(texture, subResource);
                        m_corePipelines->clearTexture3du4.cs.size = { texture.width(), texture.height(), texture.depth(), 1u };
                        m_corePipelines->clearTexture3du4.cs.value.x = value.x;
                        m_corePipelines->clearTexture3du4.cs.value.y = value.y;
                        m_corePipelines->clearTexture3du4.cs.value.z = value.z;
                        m_corePipelines->clearTexture3du4.cs.value.w = value.w;
                        bindPipe(m_corePipelines->clearTexture3du4);
                        dispatch(
                            roundUpToMultiple(texture.width(), 4u) / 4u,
                            roundUpToMultiple(texture.height(), 4u) / 4u,
                            roundUpToMultiple(texture.depth(), 4u) / 4u);
                        break;
                    }
                }
                break;
            }
        }
    }

    void CommandList::copyBuffer(
        Buffer& srcBuffer,
        Buffer& dstBuffer,
        uint64_t elements,
        uint32_t srcStartElement,
        uint32_t dstStartElement)
    {
        m_boundBuffers.emplace_back(srcBuffer);
        m_boundBuffers.emplace_back(dstBuffer);

        if((srcBuffer.description().descriptor.usage != ResourceUsage::Upload))
            transition(srcBuffer, ResourceState::CopySource);
        if (srcBuffer.description().descriptor.usage != ResourceUsage::Upload)
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
        setDebugBuffers(configuration);
        m_impl->bindPipe(*pipelineImpl, configuration);
        savePipeline(configuration);
    }

    void CommandList::setDebugBuffers(shaders::PipelineConfiguration* configuration)
    {
        auto saveBinding = [this](shaders::Shader* shader)
        {
            if (shader->hasBufferUav("debugOutput"))
            {
                BufferUAV debugUav = m_device->createBufferUAV(BufferDescription()
                    .append(true)
                    .elementSize(sizeof(ShaderDebugOutput))
                    .elements(10000)
                    .name("shaderDebut output buffer")
                    .structured(true)
                    .usage(ResourceUsage::GpuReadWrite)
                );

                auto cmd = m_device->createCommandList();
                auto fence = m_device->createFence();
                //cmd.clearBuffer(debugUav.buffer(), 0, 0, 10000);
                cmd.setStructureCounter(debugUav, 0);
                const_cast<Device*>(m_device)->queue().submit(cmd, fence);
                fence.blockUntilSignaled();

                shader->bufferUav("debugOutput", debugUav);

                this->m_debugBuffers.emplace_back(debugUav);
            }
        };

        if (configuration->hasVertexShader())
        {
            saveBinding(const_cast<engine::shaders::Shader*>(configuration->vertexShader()));
        }
        if (configuration->hasPixelShader())
        {
            saveBinding(const_cast<engine::shaders::Shader*>(configuration->pixelShader()));
        }
        if (configuration->hasComputeShader())
        {
            saveBinding(const_cast<engine::shaders::Shader*>(configuration->computeShader()));
        }
        if (configuration->hasDomainShader())
        {
            saveBinding(const_cast<engine::shaders::Shader*>(configuration->domainShader()));
        }
        if (configuration->hasGeometryShader())
        {
            saveBinding(const_cast<engine::shaders::Shader*>(configuration->geometryShader()));
        }
        if (configuration->hasHullShader())
        {
            saveBinding(const_cast<engine::shaders::Shader*>(configuration->hullShader()));
        }
    }

    void CommandList::savePipeline(shaders::PipelineConfiguration* configuration)
    {
        auto saveBinding = [this](shaders::Shader* shader, bool pixelShader)
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
                        auto localSubRes = srv.subResource();

                        uint32_t sliceCount = localSubRes.arraySliceCount == AllArraySlices ?
                            srv.texture().arraySlices() :
                            min(static_cast<uint32_t>(localSubRes.arraySliceCount), srv.texture().arraySlices() - localSubRes.firstArraySlice);

                        uint32_t mipCount = localSubRes.mipCount == AllMipLevels ?
                            srv.texture().mipLevels() :
                            min(static_cast<uint32_t>(localSubRes.mipCount), srv.texture().mipLevels() - localSubRes.firstMipLevel);

                        for (uint32_t slice = localSubRes.firstArraySlice; slice < localSubRes.firstArraySlice + sliceCount; ++slice)
                        {
                            for (uint32_t mip = localSubRes.firstMipLevel; mip < localSubRes.firstMipLevel + mipCount; ++mip)
                            {
                                if (srv.texture().state(slice, mip) != ResourceState::Common)
                                    this->transition(srv.texture(), 
                                        pixelShader ? ResourceState::PixelShaderResource : ResourceState::NonPixelShaderResource,
                                        SubResource{ mip, 1, slice, 1 });
                                else
                                    srv.texture().state(slice, mip, pixelShader ? ResourceState::PixelShaderResource : ResourceState::NonPixelShaderResource);
                            }
                        }
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
                    {
                        auto localSubRes = uav.subResource();

                        uint32_t sliceCount = localSubRes.arraySliceCount == AllArraySlices ?
                            uav.texture().arraySlices() :
                            min(static_cast<uint32_t>(localSubRes.arraySliceCount), uav.texture().arraySlices() - localSubRes.firstArraySlice);

                        uint32_t mipCount = localSubRes.mipCount == AllMipLevels ?
                            uav.texture().mipLevels() :
                            min(static_cast<uint32_t>(localSubRes.mipCount), uav.texture().mipLevels() - localSubRes.firstMipLevel);

                        for (uint32_t slice = localSubRes.firstArraySlice; slice < localSubRes.firstArraySlice + sliceCount; ++slice)
                        {
                            for (uint32_t mip = localSubRes.firstMipLevel; mip < localSubRes.firstMipLevel + mipCount; ++mip)
                            {
                                this->transition(uav.texture(), ResourceState::UnorderedAccess, SubResource{ mip, 1, slice, 1 });
                            }
                        }
                    }
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
                        this->transition(bsrv.buffer(), ResourceState::GenericRead);
                        //this->transition(bsrv.buffer(), pixelShader ? ResourceState::PixelShaderResource : ResourceState::NonPixelShaderResource);
                    else
                        bsrv.buffer().state(ResourceState::GenericRead);
                        //bsrv.buffer().state(pixelShader ? ResourceState::PixelShaderResource : ResourceState::NonPixelShaderResource);

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
                        auto localSubRes = srv[i].subResource();

                        uint32_t sliceCount = localSubRes.arraySliceCount == AllArraySlices ?
                            srv[i].texture().arraySlices() :
                            min(static_cast<uint32_t>(localSubRes.arraySliceCount), srv[i].texture().arraySlices() - localSubRes.firstArraySlice);

                        uint32_t mipCount = localSubRes.mipCount == AllMipLevels ?
                            srv[i].texture().mipLevels() :
                            min(static_cast<uint32_t>(localSubRes.mipCount), srv[i].texture().mipLevels() - localSubRes.firstMipLevel);

                        for (uint32_t slice = localSubRes.firstArraySlice; slice < localSubRes.firstArraySlice + sliceCount; ++slice)
                        {
                            for (uint32_t mip = localSubRes.firstMipLevel; mip < localSubRes.firstMipLevel + mipCount; ++mip)
                            {
                                if (srv[i].texture().description().descriptor.usage != ResourceUsage::DepthStencil)
                                    this->transition(
                                        srv[i].texture(), 
                                        pixelShader ? ResourceState::PixelShaderResource : ResourceState::NonPixelShaderResource,
                                        SubResource{ mip, 1, slice, 1 });
                                else
                                    this->transition(srv[i].texture(), ResourceState::GenericRead);
                            }
                        }
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
                        auto localSubRes = uav[i].subResource();

                        uint32_t sliceCount = localSubRes.arraySliceCount == AllArraySlices ?
                            uav[i].texture().arraySlices() :
                            min(static_cast<uint32_t>(localSubRes.arraySliceCount), uav[i].texture().arraySlices() - localSubRes.firstArraySlice);

                        uint32_t mipCount = localSubRes.mipCount == AllMipLevels ?
                            uav[i].texture().mipLevels() :
                            min(static_cast<uint32_t>(localSubRes.mipCount), uav[i].texture().mipLevels() - localSubRes.firstMipLevel);

                        for (uint32_t slice = localSubRes.firstArraySlice; slice < localSubRes.firstArraySlice + sliceCount; ++slice)
                        {
                            for (uint32_t mip = localSubRes.firstMipLevel; mip < localSubRes.firstMipLevel + mipCount; ++mip)
                            {
                                if (uav[i].texture().description().descriptor.usage != ResourceUsage::DepthStencil)
                                    this->transition(
                                        uav[i].texture(), 
                                        ResourceState::UnorderedAccess,
                                        SubResource{ mip, 1, slice, 1 });
                                else
                                    this->transition(uav[i].texture(), ResourceState::UnorderedAccess);
                            }
                        }

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
            saveBinding(const_cast<engine::shaders::Shader*>(configuration->vertexShader()), false);
            /*for(auto&& range : const_cast<engine::shaders::Shader*>(configuration->vertexShader())->constants())
            {
                this->transition(range.buffer->buffer(), ResourceState::VertexAndConstantBuffer);
            }*/
        }
        if (configuration->hasPixelShader())
        {
            saveBinding(const_cast<engine::shaders::Shader*>(configuration->pixelShader()), true);
            /*for (auto&& range : const_cast<engine::shaders::Shader*>(configuration->pixelShader())->constants())
            {
                if(range.buffer)
                    this->transition(range.buffer->buffer(), ResourceState::VertexAndConstantBuffer);
            }*/
        }
        if (configuration->hasComputeShader())
        {
            saveBinding(const_cast<engine::shaders::Shader*>(configuration->computeShader()), false);
            /*for (auto&& range : const_cast<engine::shaders::Shader*>(configuration->computeShader())->constants())
            {
                if (range.buffer)
                    this->transition(range.buffer->buffer(), ResourceState::VertexAndConstantBuffer);
            }*/
        }
        if (configuration->hasDomainShader())
        {
            saveBinding(const_cast<engine::shaders::Shader*>(configuration->domainShader()), false);
            /*for (auto&& range : const_cast<engine::shaders::Shader*>(configuration->domainShader())->constants())
            {
                if (range.buffer)
                    this->transition(range.buffer->buffer(), ResourceState::VertexAndConstantBuffer);
            }*/
        }
        if (configuration->hasGeometryShader())
        {
            saveBinding(const_cast<engine::shaders::Shader*>(configuration->geometryShader()), false);
            /*for (auto&& range : const_cast<engine::shaders::Shader*>(configuration->geometryShader())->constants())
            {
                if (range.buffer)
                    this->transition(range.buffer->buffer(), ResourceState::VertexAndConstantBuffer);
            }*/
        }
        if (configuration->hasHullShader())
        {
            saveBinding(const_cast<engine::shaders::Shader*>(configuration->hullShader()), false);
            /*for (auto&& range : const_cast<engine::shaders::Shader*>(configuration->hullShader())->constants())
            {
                if (range.buffer)
                    this->transition(range.buffer->buffer(), ResourceState::VertexAndConstantBuffer);
            }*/
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

    /*void CommandList::bindIndexBuffer(BufferIBV& buffer)
    {
        transition(buffer, ResourceState::IndexBuffer);
        m_impl->bindIndexBuffer(buffer);
    }*/

    /*void CommandList::bindDescriptorSets(const Pipeline& pipeline, const DescriptorHandle& descriptor)
    {
        m_impl->bindDescriptorSets(pipeline, descriptor);
    }*/

    void CommandList::clearBuffer(Buffer& buffer, uint32_t value, uint32_t startElement, uint32_t numElements)
    {
        transition(buffer, ResourceState::UnorderedAccess);
        m_boundBuffers.emplace_back(buffer);
        m_impl->clearBuffer(buffer, value, startElement, numElements);
    }

    void CommandList::clearTexture(
        Texture& texture, 
        const Color4f& color,
        const SubResource& subResource)
    {
        m_boundTextures.emplace_back(texture);

        transition(texture, ResourceState::RenderTarget);
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
        BufferIBV& buffer,
        uint32_t indexCount,
        uint32_t instanceCount,
        uint32_t firstIndex,
        int32_t vertexOffset,
        uint32_t firstInstance)
    {
        transition(buffer, ResourceState::IndexBuffer);
        m_impl->bindIndexBuffer(buffer);
        m_impl->drawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    }

    void CommandList::drawIndexedIndirect(BufferIBV& buffer, Buffer& indirectArguments, uint64_t argumentBufferOffset)
    {
        transition(buffer, ResourceState::IndexBuffer);
        m_impl->bindIndexBuffer(buffer);

        transition(indirectArguments, ResourceState::IndirectArgument);
        m_impl->drawIndexedIndirect(indirectArguments, argumentBufferOffset);
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

    void CommandList::drawIndirect(Buffer& indirectArguments, uint64_t argumentBufferOffset)
    {
        transition(indirectArguments, ResourceState::IndirectArgument);
        m_impl->drawIndirect(indirectArguments, argumentBufferOffset);
    }

    void CommandList::dispatchIndirect(Buffer& indirectArgs, uint64_t argumentBufferOffset)
    {
        transition(indirectArgs, ResourceState::IndirectArgument);
        m_impl->dispatchIndirect(indirectArgs, argumentBufferOffset);
    }

    void CommandList::executeBundle(CommandList& commandList)
    {
        m_impl->executeBundle(CommandListImplGet::impl(commandList));
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

    void CommandList::copyTexture(
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
        uint32_t depth)
    {
        ASSERT(src.texture().description().descriptor.dimension == dst.texture().description().descriptor.dimension,
            "CopyTexture failed. Src and Dst need to have the same dimension (1D, 2D, 3D)");

        switch (src.texture().description().descriptor.dimension)
        {
        case ResourceDimension::Texture1D:
        {
            m_corePipelines->copyTexture1df.cs.src = src;
            m_corePipelines->copyTexture1df.cs.dst = dst;
            m_corePipelines->copyTexture1df.cs.srcLeft.x = srcLeft;
            m_corePipelines->copyTexture1df.cs.srcTextureWidth.x = src.width();
            m_corePipelines->copyTexture1df.cs.dstLeft.x = dstLeft;
            m_corePipelines->copyTexture1df.cs.dstTextureWidth.x = dst.width();
            m_corePipelines->copyTexture1df.cs.copyWidth.x = width;
            bindPipe(m_corePipelines->copyTexture1df);
            dispatch(roundUpToMultiple(width, 63u) / 63u, 1, 1);
            break;
        }
        case ResourceDimension::Texture2D:
        {
            m_corePipelines->copyTexture2df.cs.src = src;
            m_corePipelines->copyTexture2df.cs.dst = dst;
            m_corePipelines->copyTexture2df.cs.srcTop.x = srcTop;
            m_corePipelines->copyTexture2df.cs.srcLeft.x = srcLeft;
            m_corePipelines->copyTexture2df.cs.srcTextureWidth.x = src.width();
            m_corePipelines->copyTexture2df.cs.srcTextureHeight.x = src.height();
            m_corePipelines->copyTexture2df.cs.dstTop.x = dstTop;
            m_corePipelines->copyTexture2df.cs.dstLeft.x = dstLeft;
            m_corePipelines->copyTexture2df.cs.dstTextureWidth.x = dst.width();
            m_corePipelines->copyTexture2df.cs.dstTextureHeight.x = dst.height();
            m_corePipelines->copyTexture2df.cs.copyWidth.x = width;
            m_corePipelines->copyTexture2df.cs.copyHeight.x = height;
            bindPipe(m_corePipelines->copyTexture2df);
            dispatch(
                roundUpToMultiple(width, 9u) / 9u,
                roundUpToMultiple(height, 9u) / 9u,
                1);
            break;
        }
        case ResourceDimension::Texture3D:
        {
            m_corePipelines->copyTexture3df.cs.src = src;
            m_corePipelines->copyTexture3df.cs.dst = dst;
            m_corePipelines->copyTexture3df.cs.srcTop.x = srcTop;
            m_corePipelines->copyTexture3df.cs.srcLeft.x = srcLeft;
            m_corePipelines->copyTexture3df.cs.srcFront.x = srcFront;
            m_corePipelines->copyTexture3df.cs.srcTextureWidth.x = src.width();
            m_corePipelines->copyTexture3df.cs.srcTextureHeight.x = src.height();
            m_corePipelines->copyTexture3df.cs.srcTextureHeight.x = src.depth();
            m_corePipelines->copyTexture3df.cs.dstTop.x = dstTop;
            m_corePipelines->copyTexture3df.cs.dstLeft.x = dstLeft;
            m_corePipelines->copyTexture3df.cs.dstFront.x = dstFront;
            m_corePipelines->copyTexture3df.cs.dstTextureWidth.x = dst.width();
            m_corePipelines->copyTexture3df.cs.dstTextureHeight.x = dst.height();
            m_corePipelines->copyTexture3df.cs.dstTextureDepth.x = dst.depth();
            m_corePipelines->copyTexture3df.cs.copyWidth.x = width;
            m_corePipelines->copyTexture3df.cs.copyHeight.x = height;
            m_corePipelines->copyTexture3df.cs.copyDepth.x = depth;
            bindPipe(m_corePipelines->copyTexture3df);
            dispatch(
                roundUpToMultiple(width, 5u) / 5u,
                roundUpToMultiple(height, 5u) / 5u,
                roundUpToMultiple(depth, 5u) / 5u);
            break;
        }
        }
    }

    void CommandList::transitionCommonSRV(TextureSRV& srv, ResourceState state)
    {
        auto localSubRes = srv.subResource();
        
        uint32_t sliceCount = localSubRes.arraySliceCount == AllArraySlices ? 
            srv.texture().arraySlices() : 
            min(static_cast<uint32_t>(localSubRes.arraySliceCount), srv.texture().arraySlices() - localSubRes.firstArraySlice);

        uint32_t mipCount = localSubRes.mipCount == AllMipLevels ?
            srv.texture().mipLevels() :
            min(static_cast<uint32_t>(localSubRes.mipCount), srv.texture().mipLevels() - localSubRes.firstMipLevel);

        for (uint32_t slice = localSubRes.firstArraySlice; slice < localSubRes.firstArraySlice + sliceCount; ++slice)
        {
            for (uint32_t mip = localSubRes.firstMipLevel; mip < localSubRes.firstMipLevel + mipCount; ++mip)
            {
                if(srv.texture().state(slice, mip) != ResourceState::Common)
                    transition(srv.texture(), state, SubResource{ mip, 1, slice, 1 });
                srv.texture().state(slice, mip, state);
            }
        }
    }

    void CommandList::transitionCommonUAV(TextureUAV& uav, ResourceState state)
    {
        auto localSubRes = uav.subResource();

        uint32_t sliceCount = localSubRes.arraySliceCount == AllArraySlices ?
            uav.texture().arraySlices() :
            min(static_cast<uint32_t>(localSubRes.arraySliceCount), uav.texture().arraySlices() - localSubRes.firstArraySlice);

        uint32_t mipCount = localSubRes.mipCount == AllMipLevels ?
            uav.texture().mipLevels() :
            min(static_cast<uint32_t>(localSubRes.mipCount), uav.texture().mipLevels() - localSubRes.firstMipLevel);

        for (uint32_t slice = localSubRes.firstArraySlice; slice < localSubRes.firstArraySlice + sliceCount; ++slice)
        {
            for (uint32_t mip = localSubRes.firstMipLevel; mip < localSubRes.firstMipLevel + mipCount; ++mip)
            {
                if (uav.texture().state(slice, mip) != ResourceState::Common)
                    transition(uav.texture(), state, SubResource{ mip, 1, slice, 1 });
                uav.texture().state(slice, mip, state);
            }
        }
    }

    void CommandList::copyTexture(TextureSRV& src, TextureUAV& dst)
    {
        m_boundTextureSRVs.emplace_back(src);
        m_boundTextureUAVs.emplace_back(dst);

        transition(src, ResourceState::CopySource);
        
        /*if (dst.texture().state() != ResourceState::Common)
            transition(dst.texture(), ResourceState::CopyDest);
        dst.texture().state(ResourceState::CopyDest);*/
        transitionCommonUAV(dst, ResourceState::CopyDest);
        
        //m_impl->copyTexture(src, dst);
        copyTexture(src, dst, 0, 0, 0, 0, 0, 0, src.width(), src.height(), src.depth());
    }

    void CommandList::copyTexture(TextureSRV& src, TextureSRV& dst)
    {
        m_boundTextureSRVs.emplace_back(src);
        m_boundTextureSRVs.emplace_back(dst);

        transition(src, ResourceState::CopySource);

        /*if (dst.texture().state() != ResourceState::Common)
            transition(dst.texture(), ResourceState::CopyDest);
        dst.texture().state(ResourceState::CopyDest);*/
        transitionCommonSRV(dst, ResourceState::CopyDest);

        m_impl->copyTexture(src, dst);
    }

    void CommandList::copyTexture(TextureSRV& src, BufferUAV& dst)
    {
        m_boundTextureSRVs.emplace_back(src);
        m_boundBufferUAVs.emplace_back(dst);

        transition(src, ResourceState::CopySource);

        if (dst.buffer().state() != ResourceState::Common)
            transition(dst.buffer(), ResourceState::CopyDest);
        dst.buffer().state(ResourceState::CopyDest);
        m_impl->copyTexture(src, dst);
    }

    void CommandList::copyTexture(TextureSRV& src, BufferSRV& dst)
    {
        m_boundTextureSRVs.emplace_back(src);
        m_boundBufferSRVs.emplace_back(dst);

        transition(src, ResourceState::CopySource);

        if (dst.buffer().state() != ResourceState::Common)
            transition(dst.buffer(), ResourceState::CopyDest);
        dst.buffer().state(ResourceState::CopyDest);
        m_impl->copyTexture(src, dst);
    }

    void CommandList::setStructureCounter(BufferUAV& buffer, uint32_t value)
    {
        m_impl->setStructureCounter(buffer, value);
    }

    void CommandList::copyStructureCounter(BufferUAV& srcBuffer, Buffer& dst, uint32_t dstByteOffset)
    {
        transition(srcBuffer.buffer(), ResourceState::CopySource);
        transition(dst, ResourceState::CopyDest);
        m_impl->copyStructureCounter(srcBuffer, dst, dstByteOffset);
    }
}
