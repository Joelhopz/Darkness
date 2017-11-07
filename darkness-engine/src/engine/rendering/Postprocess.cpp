#include "engine/rendering/Postprocess.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/Viewport.h"
#include "engine/graphics/Rect.h"
#include "engine/graphics/Pipeline.h"
#include "tools/Measure.h"

using namespace tools;

namespace engine
{
    Postprocess::Postprocess(Device& device, ShaderStorage& shaderStorage)
        : m_device{ device }
        , m_shaderStorage{ shaderStorage }
        , m_bloomExtractBrightPipeline{ device.createPipeline<shaders::BloomExtractBright>(shaderStorage) }
        , m_gaussianBlurPipeline{ device.createPipeline<shaders::GaussianBlur>(shaderStorage) }
        , m_bloomPipeline{ device.createPipeline<shaders::Bloom>(shaderStorage) }
        , m_tonemapPipeline{ device.createPipeline<shaders::Tonemap>(shaderStorage) }
        , m_lastFrameWidth{ 0u }
        , m_lastFrameHeight{ 0u }
    {
        m_bloomExtractBrightPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
        m_bloomExtractBrightPipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
        m_bloomExtractBrightPipeline.setRenderTargetFormat(Format::Format_R16G16B16A16_FLOAT);
        m_bloomExtractBrightPipeline.setDepthStencilState(DepthStencilDescription().depthEnable(false));
        m_bloomExtractBrightPipeline.ps.framebufferSampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear).textureAddressMode(TextureAddressMode::Clamp));

        m_gaussianBlurPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
        m_gaussianBlurPipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
        m_gaussianBlurPipeline.setRenderTargetFormat(Format::Format_R16G16B16A16_FLOAT);
        m_gaussianBlurPipeline.setDepthStencilState(DepthStencilDescription().depthEnable(false));
        m_gaussianBlurPipeline.ps.imageSampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear).textureAddressMode(TextureAddressMode::Clamp));

        m_bloomPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
        m_bloomPipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
        m_bloomPipeline.setRenderTargetFormat(Format::Format_R8G8B8A8_UNORM);
        m_bloomPipeline.setDepthStencilState(DepthStencilDescription().depthEnable(false));
        m_bloomPipeline.ps.framebufferSampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear).textureAddressMode(TextureAddressMode::Clamp));

        m_tonemapPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
        m_tonemapPipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
        m_tonemapPipeline.setRenderTargetFormat(Format::Format_R8G8B8A8_UNORM);
        m_tonemapPipeline.setDepthStencilState(DepthStencilDescription().depthEnable(false));
        m_tonemapPipeline.ps.imageSampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear));
    }

    void Postprocess::clearResources()
    {
    }

    void Postprocess::render(
        Device& device,
        TextureRTV& currentRenderTarget,
        TextureSRV& frame,
        CommandList& cmd,
        PostprocessComponent& postprocessComponent)
    {
        if (postprocessComponent.bloomEnabled())
        {
            if (m_lastFrameWidth != frame.texture().width() || m_lastFrameHeight != frame.texture().height())
            {
                m_lastFrameWidth = frame.texture().width();
                m_lastFrameHeight = frame.texture().height();

                m_brightTarget = device.createTextureRTV(TextureDescription()
                    .width(m_lastFrameWidth / 4)
                    .height(m_lastFrameHeight / 4)
                    .format(engine::Format::Format_R16G16B16A16_FLOAT)
                    .usage(ResourceUsage::GpuRenderTargetReadWrite)
                    .name("bool bright target")
                    .dimension(ResourceDimension::Texture2D)
                    .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f })
                );
                m_brightTargetSRV = device.createTextureSRV(m_brightTarget.texture());

                m_blurTarget.clear();
                m_blurTarget.emplace_back(device.createTextureRTV(TextureDescription()
                    .width(m_lastFrameWidth / 4)
                    .height(m_lastFrameHeight / 4)
                    .format(engine::Format::Format_R16G16B16A16_FLOAT)
                    .usage(ResourceUsage::GpuRenderTargetReadWrite)
                    .name("bool blur target")
                    .dimension(ResourceDimension::Texture2D)
                    .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f })));

                m_blurTarget.emplace_back(device.createTextureRTV(TextureDescription()
                    .width(m_lastFrameWidth / 4)
                    .height(m_lastFrameHeight / 4)
                    .format(engine::Format::Format_R16G16B16A16_FLOAT)
                    .usage(ResourceUsage::GpuRenderTargetReadWrite)
                    .name("bool blur target")
                    .dimension(ResourceDimension::Texture2D)
                    .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f })));

                m_blurTargetSRV.clear();
                m_blurTargetSRV.emplace_back(device.createTextureSRV(m_blurTarget[0].texture()));
                m_blurTargetSRV.emplace_back(device.createTextureSRV(m_blurTarget[1].texture()));
            }
            cmd.clearRenderTargetView(m_brightTarget, { 0.0f, 0.0f, 0.0f, 1.0f });

            {
                // extract bright areas
                m_bloomExtractBrightPipeline.ps.framebuffer = frame;
                cmd.setRenderTargets({ m_brightTarget });
                cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(m_brightTarget.width()), static_cast<float>(m_brightTarget.height()), 0.0f, 1.0f } });
                cmd.setScissorRects({ Rectangle{ 0, 0, m_brightTarget.width(), m_brightTarget.height() } });

                cmd.bindPipe(m_bloomExtractBrightPipeline);
                cmd.draw(4u);
            }

            int lastFrameBuffer = 0;
            {
                cmd.copyTexture(m_brightTargetSRV.texture(), m_blurTarget[0].texture());

                bool horizontal = true;
                bool firstIteration = true;
                int amount = 6;

                for (int i = 0; i < amount; ++i)
                {
                    // blur the bright target
                    m_gaussianBlurPipeline.ps.image = m_blurTargetSRV[!static_cast<bool>(horizontal)];
                    m_gaussianBlurPipeline.ps.width = static_cast<float>(m_lastFrameWidth / 4);
                    m_gaussianBlurPipeline.ps.height = static_cast<float>(m_lastFrameHeight / 4);
                    m_gaussianBlurPipeline.ps.horizontal.x = static_cast<unsigned int>(horizontal);

                    lastFrameBuffer = static_cast<int>(horizontal);
                    cmd.setRenderTargets({ m_blurTarget[lastFrameBuffer] });
                    cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(m_blurTarget[lastFrameBuffer].width()), static_cast<float>(m_blurTarget[lastFrameBuffer].height()), 0.0f, 1.0f } });
                    cmd.setScissorRects({ Rectangle{ 0, 0, m_blurTarget[lastFrameBuffer].width(), m_blurTarget[lastFrameBuffer].height() } });

                    cmd.bindPipe(m_gaussianBlurPipeline);
                    cmd.draw(4u);

                    horizontal = !horizontal;
                    firstIteration = false;
                }
            }

            {
                // compose final target
                m_bloomPipeline.ps.framebuffer = frame;
                m_bloomPipeline.ps.blur = m_blurTargetSRV[lastFrameBuffer];
                cmd.setRenderTargets({ currentRenderTarget });
                cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(currentRenderTarget.width()), static_cast<float>(currentRenderTarget.height()), 0.0f, 1.0f } });
                cmd.setScissorRects({ Rectangle{ 0, 0, currentRenderTarget.width(), currentRenderTarget.height() } });

                cmd.bindPipe(m_bloomPipeline);
                cmd.draw(4u);
            }
        }
        else
        {
            m_tonemapPipeline.ps.image = frame;
            cmd.setRenderTargets({ currentRenderTarget });
            cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(currentRenderTarget.width()), static_cast<float>(currentRenderTarget.height()), 0.0f, 1.0f } });
            cmd.setScissorRects({ Rectangle{ 0, 0, currentRenderTarget.width(), currentRenderTarget.height() } });

            cmd.bindPipe(m_tonemapPipeline);
            cmd.draw(4u);
        }
    }
}
