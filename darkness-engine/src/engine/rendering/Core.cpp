#include "engine/rendering/Core.h"
#include "engine/graphics/Device.h"


namespace engine
{
    Core::Core(Device& device, ShaderStorage& shaderStorage, const std::string& shaderRootPath)
        : m_linearClamp{ SamplerDescription().filter(Filter::Bilinear)
            .textureAddressMode(TextureAddressMode::Clamp) }
        , m_pointClamp{ SamplerDescription().filter(Filter::Point)
            .textureAddressMode(TextureAddressMode::Clamp) }
        //, rootSignature{ device.createRootSignature() }
        /*, mainPipeline{ device.createPipeline(
            shaderStorage.loadShader(device, shaderRootPath + "vert.spv"),
            shaderStorage.loadShader(device, shaderRootPath + "frag.spv")
        ) }*/
        , mainPipeline{ device.createPipeline<shaders::HLSLTest>(shaderStorage) }

        , uniformBuffer{ device.createBuffer(BufferDescription()
            .structured(true)
            .elements(1000)
            .usage(ResourceUsage::CpuToGpu)
			.name("someBuffer")
            .elementSize(sizeof(UniformBufferObject))) }

        //, descriptorHeap{ device.createDescriptorHeap({ DescriptorType::UniformBuffer, DescriptorType::CombinedImageSampler }) }
    {
        /*InputElementDescription vertDesc;
        vertDesc.alignedByteOffset(sizeof(Vertex))
            .format(Format::Format_R32G32B32_FLOAT)
            .inputSlot(0)
            .inputSlotClass(InputClassification::PerVertexData)
            .offset(offsetof(Vertex, pos));

        InputElementDescription colorDesc;
        colorDesc.alignedByteOffset(sizeof(Vertex))
            .format(Format::Format_R32G32B32_FLOAT)
            .inputSlot(0)
            .inputSlotClass(InputClassification::PerVertexData)
            .offset(offsetof(Vertex, color));

        InputElementDescription uvDesc;
        uvDesc.alignedByteOffset(sizeof(Vertex))
            .format(Format::Format_R32G32_FLOAT)
            .inputSlot(0)
            .inputSlotClass(InputClassification::PerVertexData)
            .offset(offsetof(Vertex, texCoord));

        std::vector<InputElementDescription> layoutDescs;
        layoutDescs.emplace_back(vertDesc);
        layoutDescs.emplace_back(colorDesc);
        layoutDescs.emplace_back(uvDesc);

        mainPipeline->setInputLayout(static_cast<unsigned int>(layoutDescs.size()), layoutDescs.data());*/

        mainPipeline.setUniformBuffer(uniformBuffer);

        mainPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        /*mainPipeline->setVertexShader(shaderStorage.loadShader(device, shaderRootPath + "vert.spv"));
        mainPipeline->setPixelShader(shaderStorage.loadShader(device, shaderRootPath + "frag.spv"));*/
        

        /*m_rootSignature.reset(4, 2);
        m_rootSignature.initStaticSampler(0, m_linearClamp, ShaderVisibility::Pixel);
        m_rootSignature.initStaticSampler(1, m_pointClamp, ShaderVisibility::Pixel);
        m_rootSignature[0].initAsDescriptorRange(DescriptorRangeType::SRV, 0, 2, ShaderVisibility::Pixel);
        m_rootSignature[1].initAsConstants(0, 6, ShaderVisibility::Pixel);
        m_rootSignature[2].initAsSRV(2, ShaderVisibility::Vertex);
        m_rootSignature[3].initAsDescriptorRange(DescriptorRangeType::UAV, 0, 1, ShaderVisibility::Pixel);
        m_rootSignature.finalize();

        m_blendUIPipelineState.setRootSignature(m_rootSignature);
        m_blendUIPipelineState.setRasterizerState(RasterizerDescription().cullMode(CullMode::None));
        m_blendUIPipelineState.setBlendState(
            BlendDescription()
            .renderTarget(0, RenderTargetBlendDescription()
                .renderTargetWriteMask(static_cast<unsigned char>(ColorWriteEnable::All))
                .blendEnable(true)
                .srcBlend(Blend::One)));
        m_blendUIPipelineState.setDepthStencilState(DepthStencilDescription()
            .depthEnable(false)
            .depthWriteMask(DepthWriteMask::Zero)
            .depthFunc(ComparisonFunction::Always)
            .frontFace({ StencilOp::Keep, StencilOp::Keep, StencilOp::Keep, ComparisonFunction::Always })
            .backFace({ StencilOp::Keep, StencilOp::Keep, StencilOp::Keep, ComparisonFunction::Always })
            );
        m_blendUIPipelineState.setSampleMask(0xFFFFFFFF);

        InputElementDescription vertDesc;
        vertDesc.alignedByteOffset(sizeof(Vertex))
            .format(Format::Format_R32G32_FLOAT)
            .inputSlot(0)
            .inputSlotClass(InputClassification::PerVertexData)
            .offset(offsetof(Vertex, pos));

        InputElementDescription colorDesc;
        colorDesc.alignedByteOffset(sizeof(Vertex))
            .format(Format::Format_R32G32B32_FLOAT)
            .inputSlot(0)
            .inputSlotClass(InputClassification::PerVertexData)
            .offset(offsetof(Vertex, color));

        std::vector<InputElementDescription> layoutDescs;
        layoutDescs.emplace_back(vertDesc);
        layoutDescs.emplace_back(colorDesc);

        m_blendUIPipelineState.setInputLayout(static_cast<unsigned int>(layoutDescs.size()), layoutDescs.data());
        m_blendUIPipelineState.setPrimitiveTopologyType(PrimitiveTopologyType::Triangle);
#ifndef VULKAN
        m_blendUIPipelineState.setVertexShader(shaderStorage.loadShader(device, shaderRootPath + "ScreenQuadVS.cso"));
        m_blendUIPipelineState.setPixelShader(shaderStorage.loadShader(device, shaderRootPath + "BufferCopyPS.cso"));
#else
        m_blendUIPipelineState.setVertexShader(shaderStorage.loadShader(device, shaderRootPath + "vert.spv"));
        m_blendUIPipelineState.setPixelShader(shaderStorage.loadShader(device, shaderRootPath + "frag.spv"));
#endif
        m_blendUIPipelineState.setRenderTargetFormat(Format::Format_R10G10B10A2_UNORM, Format::Format_UNKNOWN);
        m_blendUIPipelineState.finalize();*/
    }
}
