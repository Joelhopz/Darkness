#include "engine/rendering/tools/ExtractBrightness.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/CommandList.h"

namespace engine
{
    ExtractBrightness::ExtractBrightness(Device& device, ShaderStorage& shaderStorage)
        : m_device{ device }
        , m_bloomExtractBrightPipeline{ device.createPipeline<shaders::BloomExtractBright>(shaderStorage) }
    {
        m_bloomExtractBrightPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
        m_bloomExtractBrightPipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
        m_bloomExtractBrightPipeline.setDepthStencilState(DepthStencilDescription().depthEnable(false));
        m_bloomExtractBrightPipeline.ps.framebufferSampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear).textureAddressMode(TextureAddressMode::Clamp));
    }

    TextureSRV& ExtractBrightness::extract(CommandList& cmd, TextureSRV& src)
    {
        CPU_MARKER("Bloom extract bright areas");
        GPU_MARKER(cmd, "Bloom extract bright areas");

        refreshInternalTextures(src);

        cmd.clearRenderTargetView(m_brightTargetRTV);

        m_bloomExtractBrightPipeline.ps.framebuffer = src;
        m_bloomExtractBrightPipeline.ps.invSize = Vector2f{ 1.0f / (float)src.width(), 1.0f / (float)src.height() };
        m_bloomExtractBrightPipeline.ps.bloomThreshold = 1.0f;
        m_bloomExtractBrightPipeline.ps.exposure = 1.0f;
        cmd.setRenderTargets({ m_brightTargetRTV });
        cmd.bindPipe(m_bloomExtractBrightPipeline);
        cmd.draw(4u);

        return m_brightTargetSRV;
    }

    void ExtractBrightness::refreshInternalTextures(TextureSRV& src)
    {
        if (!m_brightTargetRTV.valid() ||
            m_brightTargetRTV.width() != src.width() ||
            m_brightTargetRTV.height() != src.height() ||
            m_brightTargetRTV.texture().format() != src.format())
        {
            m_brightTargetRTV = m_device.createTextureRTV(TextureDescription()
                .width(src.width())
                .height(src.height())
                .format(src.format())
                .usage(ResourceUsage::GpuRenderTargetReadWrite)
                .name("Bloom brightness extract RTV")
                .dimension(ResourceDimension::Texture2D));
            m_brightTargetSRV = m_device.createTextureSRV(m_brightTargetRTV.texture());
        }
    }
}
