#include "engine/rendering/MainRender.h"
#include "engine/graphics/Device.h"

namespace engine
{
    MainRender::MainRender(
        Device& device, 
        ShaderStorage& shaderStorage,
        const std::string& shaderRootPath,
        int width, int height)
        : m_anisoWrap{ SamplerDescription().maxAnisotrophy(8) }
        , m_shadow{ SamplerDescription().filter(Filter::Point)
            .comparisonFunc(ComparisonFunction::GreaterEqual)
            .textureAddressMode(TextureAddressMode::Clamp) }
        /*, m_samplerAnisoWrap{ device.createSampler(m_anisoWrap) }
        , m_samplerShadow{ device.createSampler(m_shadow) }*/
        , m_rootSignature{ device.createRootSignature() }
        , m_depthPipelineState{ device.createPipeline<shaders::HLSLTest>(shaderStorage) }
        , m_shadowPipelineState{ device.createPipeline<shaders::HLSLTest>(shaderStorage) }
        , m_modelPipelineState{ device.createPipeline<shaders::HLSLTest>(shaderStorage) }
        /*, m_depthPipelineState{ device.createPipeline(
            shaderStorage.loadShader(device, shaderRootPath + "DepthViewerVS.cso"),
            shaderStorage.loadShader(device, shaderRootPath + "DepthViewerPS.cso")
        ) }
        , m_shadowPipelineState{ device.createPipeline(
            shaderStorage.loadShader(device, shaderRootPath + "ModelViewerVS.cso"),
            shaderStorage.loadShader(device, shaderRootPath + "ModelViewerPS.cso")
        ) }
        , m_modelPipelineState{ device.createPipeline(
            shaderStorage.loadShader(device, shaderRootPath + "ModelViewerVS.cso"),
            shaderStorage.loadShader(device, shaderRootPath + "ModelViewerPS.cso")
        ) }*/


        , m_sceneColorBuffer{ device.createTexture(TextureDescription()
            .width(width)
            .height(height)
            .format(Format::Format_R11G11B10_FLOAT)
            )}

        , m_sceneDepthBuffer{ device.createTexture(TextureDescription()
            .width(width)
            .height(height)
            .format(Format::Format_D32_FLOAT)
            )}

        , m_shadowBuffer{ device.createTexture(TextureDescription()
            .width(2048)
            .height(2048)
            .format(Format::Format_R16_FLOAT)
        ) }
        /*, m_sceneColorBuffer{ device.createColorBuffer(width, height, 1, Format::Format_R11G11B10_FLOAT, "Main Color buffer") }
        , m_sceneDepthBuffer{ device.createDepthBuffer(width, height, Format::Format_D32_FLOAT) }
        , m_shadowBuffer{ device.createShadowBuffer(2048, 2048, "Shadow Map") }*/
    {
        m_rootSignature.reset(6, 2);
        m_rootSignature.initStaticSampler(0, m_anisoWrap, static_cast<ShaderVisibility>(ShaderVisibilityBits::Pixel));
        m_rootSignature.initStaticSampler(1, m_shadow, static_cast<ShaderVisibility>(ShaderVisibilityBits::Pixel));
        m_rootSignature[0].initAsCBV(0, static_cast<ShaderVisibility>(ShaderVisibilityBits::Vertex));
        m_rootSignature[1].initAsCBV(0, static_cast<ShaderVisibility>(ShaderVisibilityBits::Pixel));
        m_rootSignature[2].initAsDescriptorRange(DescriptorRangeType::SRV, 0, 1, static_cast<ShaderVisibility>(ShaderVisibilityBits::Vertex));
        m_rootSignature[3].initAsDescriptorRange(DescriptorRangeType::SRV, 0, 6, static_cast<ShaderVisibility>(ShaderVisibilityBits::Pixel));
        m_rootSignature[4].initAsDescriptorRange(DescriptorRangeType::SRV, 64, 3, static_cast<ShaderVisibility>(ShaderVisibilityBits::Pixel));
        m_rootSignature[5].initAsConstants(1, 1, static_cast<ShaderVisibility>(ShaderVisibilityBits::Vertex));
        m_rootSignature.finalize();

        Format colorFormat = m_sceneColorBuffer.format();
        Format depthFormat = m_sceneDepthBuffer.format();
        Format shadowFormat = m_shadowBuffer.format();

        //m_depthPipelineState.setRootSignature(m_rootSignature);
        m_depthPipelineState.setRasterizerState(RasterizerDescription());
        m_depthPipelineState.setBlendState(BlendDescription());
        m_depthPipelineState.setDepthStencilState(DepthStencilDescription());
        m_depthPipelineState.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
		m_depthPipelineState.setRenderTargetFormats({}, depthFormat);
        //m_depthPipelineState.setVertexShader(shaderStorage.loadShader(device, shaderRootPath + "DepthViewerVS.cso"));
        //m_depthPipelineState.setPixelShader(shaderStorage.loadShader(device, shaderRootPath + "DepthViewerPS.cso"));
        m_depthPipelineState.finalize();

        //m_shadowPipelineState.setRootSignature(m_rootSignature);
        m_shadowPipelineState.setRasterizerState(RasterizerDescription().slopeScaledDepthBias(-1.5f).depthBias(-100));
        m_shadowPipelineState.setBlendState(BlendDescription());
        m_shadowPipelineState.setDepthStencilState(DepthStencilDescription());
        m_shadowPipelineState.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
		m_shadowPipelineState.setRenderTargetFormats({}, shadowFormat);
        //m_shadowPipelineState.setVertexShader(shaderStorage.loadShader(device, shaderRootPath + "ModelViewerVS.cso"));
        //m_shadowPipelineState.setPixelShader(shaderStorage.loadShader(device, shaderRootPath + "ModelViewerPS.cso"));
        m_shadowPipelineState.finalize();

        //m_modelPipelineState.setRootSignature(m_rootSignature);
        m_modelPipelineState.setRasterizerState(RasterizerDescription());
        m_modelPipelineState.setBlendState(BlendDescription());
        m_modelPipelineState.setDepthStencilState(
            DepthStencilDescription()
            .depthEnable(true)
            .depthWriteMask(DepthWriteMask::Zero)
            .depthFunc(ComparisonFunction::Equal));
        m_modelPipelineState.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
		m_modelPipelineState.setRenderTargetFormats({}, depthFormat);
        //m_modelPipelineState.setVertexShader(shaderStorage.loadShader(device, shaderRootPath + "ModelViewerVS.cso"));
        //m_modelPipelineState.setPixelShader(shaderStorage.loadShader(device, shaderRootPath + "ModelViewerPS.cso"));
        m_modelPipelineState.finalize();
    }
}
