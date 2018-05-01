#include "engine/graphics/Pipeline.h"
#include "engine/graphics/CommandList.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/Sampler.h"

#if defined(_WIN32) && !defined(VULKAN)
#include "engine/graphics/dx12/DX12Pipeline.h"
#endif

#if defined(VULKAN)
#include "engine/graphics/vulkan/VulkanPipeline.h"
#include "engine/graphics/vulkan/VulkanResources.h"
#include "engine/graphics/vulkan/VulkanSampler.h"
#endif

#ifdef __APPLE__
#include "engine/graphics/metal/MetalPipeline.h"
#endif


using namespace tools;
using namespace engine::implementation;

namespace engine
{
    PipelineAbs::PipelineAbs(
        Device& device,
        ShaderStorage& storage)
        : m_impl{ std::make_shared<implementation::PipelineImpl>(device, storage) }
    {}

    void PipelineAbs::setBlendState(const BlendDescription& desc) { m_impl->setBlendState(desc); };
    void PipelineAbs::setRasterizerState(const RasterizerDescription& desc) { m_impl->setRasterizerState(desc); };
    void PipelineAbs::setDepthStencilState(const DepthStencilDescription& desc) { m_impl->setDepthStencilState(desc); };
    void PipelineAbs::setSampleMask(unsigned int mask) { m_impl->setSampleMask(mask); };
    void PipelineAbs::setPrimitiveTopologyType(PrimitiveTopologyType type, bool adjacency) { m_impl->setPrimitiveTopologyType(type, adjacency); };
    void PipelineAbs::setRenderTargetFormat(
        Format RTVFormat, 
        Format DSVFormat, 
        unsigned int msaaCount, 
        unsigned int msaaQuality) 
    { m_impl->setRenderTargetFormat(RTVFormat, DSVFormat, msaaCount, msaaQuality); };

    void PipelineAbs::setRenderTargetFormats(
        std::vector<Format> RTVFormats, 
        Format DSVFormat, 
        unsigned int msaaCount, 
        unsigned int msaaQuality) 
    {
        m_impl->setRenderTargetFormats(RTVFormats, DSVFormat, msaaCount, msaaQuality);
    };

    void PipelineAbs::setInputLayout(
        unsigned int numElements,
        const InputElementDescription* inputElementDescs) { m_impl->setInputLayout(numElements, inputElementDescs); };
    void PipelineAbs::setPrimitiveRestart(IndexBufferStripCutValue value) { m_impl->setPrimitiveRestart(value); };
    void PipelineAbs::setUniformBuffer(const Buffer& buffer) { m_impl->setUniformBuffer(buffer); };
    void PipelineAbs::setTextureSRV(const TextureSRV& srv) { m_impl->setTextureSRV(srv);  };
    void PipelineAbs::setSampler(const Sampler& sampler) { m_impl->setSampler(sampler); };
    void PipelineAbs::setDepthBufferView(std::shared_ptr<TextureDSV> view) { m_impl->setDepthBufferView(view); };

    void PipelineAbs::configure(CommandListImpl& commandList, shaders::PipelineConfiguration* configuration) { m_impl->configure(commandList, configuration); }

}
