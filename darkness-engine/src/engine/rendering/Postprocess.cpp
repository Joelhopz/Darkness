#include "engine/rendering/Postprocess.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/Viewport.h"
#include "engine/graphics/Rect.h"
#include "engine/graphics/Pipeline.h"
#include "engine/graphics/Fence.h"
#include "tools/Measure.h"

using namespace tools;

namespace engine
{
    Postprocess::Postprocess(Device& device, ShaderStorage& shaderStorage)
        : m_device{ device }
        , m_shaderStorage{ shaderStorage }
        , m_blur{ device, shaderStorage }
        , m_downsample{ device, shaderStorage }
        , m_extractBrightness{ device, shaderStorage }
        , m_combineTextures{ device, shaderStorage }
        , m_bloomPipeline{ device.createPipeline<shaders::Bloom>(shaderStorage) }
        , m_tonemapPipeline{ device.createPipeline<shaders::Tonemap>(shaderStorage) }
        , m_bloomPipelineHDR{ device.createPipeline<shaders::Bloom>(shaderStorage) }
        , m_tonemapPipelineHDR{ device.createPipeline<shaders::Tonemap>(shaderStorage) }
        , m_copyTexture{ device.createPipeline<shaders::CopyTexture>(shaderStorage) }
        , m_lastFrameWidth{ 0u }
        , m_lastFrameHeight{ 0u }
    {
        m_copyTexture.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
        m_copyTexture.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
        m_copyTexture.setDepthStencilState(DepthStencilDescription().depthEnable(false));
        m_copyTexture.ps.samp = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear).textureAddressMode(TextureAddressMode::Clamp));

        // normal pipe
        {
            m_bloomPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
            m_bloomPipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
            m_bloomPipeline.setDepthStencilState(DepthStencilDescription().depthEnable(false));
            m_bloomPipeline.ps.framebufferSampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear).textureAddressMode(TextureAddressMode::Clamp));

            m_tonemapPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
            m_tonemapPipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
            m_tonemapPipeline.setDepthStencilState(DepthStencilDescription().depthEnable(false));
            m_tonemapPipeline.ps.imageSampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear));
        }

        // hdr pipe
        {
            m_bloomPipelineHDR.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
            m_bloomPipelineHDR.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
            m_bloomPipelineHDR.setDepthStencilState(DepthStencilDescription().depthEnable(false));
            m_bloomPipelineHDR.ps.framebufferSampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear).textureAddressMode(TextureAddressMode::Clamp));

            m_tonemapPipelineHDR.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
            m_tonemapPipelineHDR.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
            m_tonemapPipelineHDR.setDepthStencilState(DepthStencilDescription().depthEnable(false));
            m_tonemapPipelineHDR.ps.imageSampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear));
        }
    }

    void Postprocess::clearResources()
    {
    }

    void Postprocess::createResources(
        uint32_t width, uint32_t height, 
        Format brightFormat,
        Format frameFormat)
    {
        if (width != m_lastFrameWidth || height != m_lastFrameHeight)
        {
            m_lastFrameWidth = width;
            m_lastFrameHeight = height;
            
            uint32_t blurWidth = m_lastFrameWidth >> 1;
            uint32_t blurHeight = m_lastFrameHeight >> 1;
            
            for (uint32_t i = 0; i < BlurCount; ++i)
            {
                m_mipsRTV.emplace_back(m_device.createTextureRTV(TextureDescription()
                    .width(blurWidth >> i)
                    .height(blurHeight >> i)
                    .format(brightFormat)
                    .usage(ResourceUsage::GpuRenderTargetReadWrite)
                    .name("Bloom brightness mips")
                    .dimension(ResourceDimension::Texture2D)));
                m_mipsSRV.emplace_back(m_device.createTextureSRV(m_mipsRTV.back().texture()));
                m_mipsUAV.emplace_back(m_device.createTextureUAV(m_mipsRTV.back().texture()));
            }
            
            m_blurTargets.resize(BlurCount);
            m_blurTargetsSRV.resize(BlurCount);
            m_combineTargets.resize(BlurCount);
            m_combineTargetsSRV.resize(BlurCount);
            for (int i = BlurCount - 1; i >= 0; --i)
            {
                m_blurTargets[i] = m_device.createTextureRTV(TextureDescription()
                    .width(m_mipsSRV[i].width())
                    .height(m_mipsSRV[i].height())
                    .format(m_mipsSRV[i].format())
                    .usage(ResourceUsage::GpuRenderTargetReadWrite)
                    .name("Bloom blur target")
                    .dimension(ResourceDimension::Texture2D));
                m_blurTargetsSRV[i] = m_device.createTextureSRV(m_blurTargets[i].texture());
            
                m_combineTargets[i] = m_device.createTextureRTV(TextureDescription()
                    .width(m_mipsSRV[i].width())
                    .height(m_mipsSRV[i].height())
                    .format(m_mipsSRV[i].format())
                    .usage(ResourceUsage::GpuRenderTargetReadWrite)
                    .name("Bloom combine target")
                    .dimension(ResourceDimension::Texture2D));
                m_combineTargetsSRV[i] = m_device.createTextureSRV(m_combineTargets[i].texture());
            }
            
            m_blurTarget = m_device.createTextureRTV(TextureDescription()
                .width(m_mipsSRV[0].width())
                .height(m_mipsSRV[0].height())
                .format(m_mipsSRV[0].format())
                .usage(ResourceUsage::GpuRenderTargetReadWrite)
                .name("Bloom blur target")
                .dimension(ResourceDimension::Texture2D));
            m_blurTargetSRV = m_device.createTextureSRV(m_blurTarget.texture());
            
            m_frameAndBloom = m_device.createTextureRTV(TextureDescription()
                .width(width)
                .height(height)
                .format(frameFormat)
                .usage(ResourceUsage::GpuRenderTargetReadWrite)
                .name("Frame and bloom combinate RTV")
                .dimension(ResourceDimension::Texture2D));
            m_frameAndBloomSRV = m_device.createTextureSRV(m_frameAndBloom.texture());
        }
    }

    void Postprocess::render(
        TextureRTV& currentRenderTarget,
        TextureSRV& frame,
        CommandList& cmd,
        PostprocessComponent& postprocessComponent)
    {
        CPU_MARKER("Postprocess");
        GPU_MARKER(cmd, "Postprocess");
        if (postprocessComponent.bloomEnabled())
        {
            auto& brightParts = m_extractBrightness.extract(cmd, frame);

            createResources(
                frame.width(), frame.height(),
                brightParts.format(),
                frame.format());

            for (uint32_t i = 0; i < BlurCount; ++i)
            {
                m_downsample.downsample(cmd, brightParts, m_mipsRTV[i]);
            }

            for (int i = BlurCount -1; i >= 0; --i)
            {
                m_blur.blur(cmd, m_mipsSRV[i], m_blurTargetsSRV[i]);
                if (i > 0)
                {
                    m_combineTextures.combine(cmd, m_blurTargetsSRV[i], m_mipsSRV[i - 1], m_combineTargets[i]);

                    //cmd.copyTexture(combineTargetSRV, mipsUAV[i - 1]);
                    m_copyTexture.ps.src = m_combineTargetsSRV[i];
                    cmd.setRenderTargets({ m_mipsRTV[i - 1] });
                    cmd.bindPipe(m_copyTexture);
                    cmd.draw(4u);
                }
            }

            m_blur.blur(cmd, m_mipsSRV[0], m_blurTargetSRV);

            m_combineTextures.combine(cmd, m_blurTargetSRV, frame, m_frameAndBloom);

            CPU_MARKER("Tonemap pass");
            GPU_MARKER(cmd, "Tonemap pass");
            if (currentRenderTarget.texture().format() == Format::R8G8B8A8_UNORM)
            {
                m_tonemapPipeline.ps.image = m_frameAndBloomSRV;
                cmd.setRenderTargets({ currentRenderTarget });
                cmd.bindPipe(m_tonemapPipeline);
                cmd.draw(4u);
            }
            else
            {
                m_tonemapPipelineHDR.ps.image = m_frameAndBloomSRV;
                cmd.setRenderTargets({ currentRenderTarget });
                cmd.bindPipe(m_tonemapPipelineHDR);
                cmd.draw(4u);
            }
            
#if 0
                {
                    CPU_MARKER("Bloom blur passes");
                    GPU_MARKER(cmd, "Bloom blur passes");

                    cmd.copyTexture(m_brightTargetSRV.texture(), m_blurTarget[0].texture());

                    bool horizontal = true;
                    bool firstIteration = true;
                    int amount = 6;

                    // blur passes
                    for (int i = 0; i < amount; ++i)
                    {
                        // blur the bright target
                        m_gaussianBlurPipeline.ps.image = m_blurTargetSRV[!static_cast<bool>(horizontal)];
                        m_gaussianBlurPipeline.ps.width = static_cast<float>(m_lastFrameWidth / 4);
                        m_gaussianBlurPipeline.ps.height = static_cast<float>(m_lastFrameHeight / 4);
                        m_gaussianBlurPipeline.ps.horizontal.x = static_cast<unsigned int>(horizontal);

                        lastFrameBuffer = static_cast<int>(horizontal);
                        cmd.setRenderTargets({ m_blurTarget[lastFrameBuffer] });
                        cmd.bindPipe(m_gaussianBlurPipeline);
                        cmd.draw(4u);

                        horizontal = !horizontal;
                        firstIteration = false;
                    }
                }

                // downsample passes
                {
                    CPU_MARKER("Bloom downsample");
                    GPU_MARKER(cmd, "Bloom downsample");
                    if (currentRenderTarget.texture().format() == Format::R8G8B8A8_UNORM)
                    {
                        cmd.clearTexture(m_downSampleTargetRTV.texture(), { 0.0f, 0.0f, 0.0f, 1.0f });
                        m_downsamplePipeline.ps.image = m_blurTargetSRV[lastFrameBuffer];
                        for (uint32_t i = 0; i < m_downSampleTargetRTV.texture().description().descriptor.mipLevels; ++i)
                        {
                            auto mipTarget = m_device.createTextureRTV(m_downSampleTargetRTV.texture(), SubResource{ i, 1, 0, 1 });
                            cmd.setRenderTargets({ mipTarget });
                            cmd.bindPipe(m_downsamplePipeline);
                            cmd.draw(4u);
                        }
                    }
                    else
                    {
                        cmd.clearTexture(m_downSampleTargetRTVHDR.texture(), { 0.0f, 0.0f, 0.0f, 1.0f });
                        m_downsamplePipelineHDR.ps.image = m_blurTargetSRV[lastFrameBuffer];
                        for (uint32_t i = 0; i < m_downSampleTargetRTVHDR.texture().description().descriptor.mipLevels; ++i)
                        {
                            auto mipTarget = m_device.createTextureRTV(m_downSampleTargetRTVHDR.texture(), SubResource{ i, 1, 0, 1 });
                            cmd.setRenderTargets({ mipTarget });
                            cmd.bindPipe(m_downsamplePipelineHDR);
                            cmd.draw(4u);
                        }
                    }
                }
#endif
#if 0
            {
                CPU_MARKER("Bloom composite");
                GPU_MARKER(cmd, "Bloom composite");
                // compose final target
                if (currentRenderTarget.texture().format() == Format::R8G8B8A8_UNORM)
                {
                    m_bloomPipeline.ps.framebuffer = frame;
                    //m_bloomPipeline.ps.blur = m_blurTargetSRV[0];// m_downSampleTargetSRV;// m_blurTargetSRV[lastFrameBuffer];
                    cmd.setRenderTargets({ currentRenderTarget });
                    cmd.bindPipe(m_bloomPipeline);
                    cmd.draw(4u);
                }
                else
                {
                    m_bloomPipelineHDR.ps.framebuffer = frame;
                    m_bloomPipelineHDR.ps.blur = m_downSampleTargetSRVHDR;// m_blurTargetSRV[lastFrameBuffer];
                    cmd.setRenderTargets({ currentRenderTarget });
                    cmd.bindPipe(m_bloomPipelineHDR);
                    cmd.draw(4u);
                }
            }
#endif
        }
        else
        {
            CPU_MARKER("Tonemap pass");
            GPU_MARKER(cmd, "Tonemap pass");
            if (currentRenderTarget.texture().format() == Format::R8G8B8A8_UNORM)
            {
                m_tonemapPipeline.ps.image = frame;
                cmd.setRenderTargets({ currentRenderTarget });
                cmd.bindPipe(m_tonemapPipeline);
                cmd.draw(4u);
            }
            else
            {
                m_tonemapPipelineHDR.ps.image = frame;
                cmd.setRenderTargets({ currentRenderTarget });
                cmd.bindPipe(m_tonemapPipelineHDR);
                cmd.draw(4u);
            }
        }
    }
}
