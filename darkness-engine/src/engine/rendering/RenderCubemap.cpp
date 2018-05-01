#include "engine/rendering/RenderCubemap.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/Viewport.h"
#include "engine/graphics/Rect.h"
#include "engine/graphics/Common.h"
#include "engine/graphics/Pipeline.h"
#include "engine/graphics/Fence.h"
#include "tools/Measure.h"

using namespace tools;

namespace engine
{
    RenderCubemap::RenderCubemap(Device& device, ShaderStorage& shaderStorage)
        : m_device{ device }
        , m_shaderStorage{ shaderStorage }
        , m_cubemapPipeline{ device.createPipeline<shaders::Cubemap>(shaderStorage) }
        , m_equirectToCubemapPipeline{ device.createPipeline<shaders::EquirectToCubemap>(shaderStorage) }
        , m_irradiancePipeline{ device.createPipeline<shaders::Irradiance>(shaderStorage) }
        , m_prefilterConvolutionPipeline{ device.createPipeline<shaders::PrefilterConvolution>(shaderStorage) }
        , m_brdfConvolutionPipeline{ device.createPipeline<shaders::BrdfConvolution>(shaderStorage) }
        , m_cubemapCamera{ std::make_shared<Transform>() }

        , m_lastCubemapSize{ 0u }
        , m_lastIrradianceSize{ 0u }
        , m_lastPrefilterConvolutionSize{ 0u }
    {
        m_cubemapPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_cubemapPipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false).cullMode(CullMode::Back));
        m_cubemapPipeline.setDepthStencilState(DepthStencilDescription().depthEnable(false));
        m_cubemapPipeline.ps.cubemapSampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear));

        DepthStencilOpDescription front;
        front.StencilFailOp = StencilOp::Keep;
        front.StencilDepthFailOp = StencilOp::Incr;
        front.StencilPassOp = StencilOp::Keep;
        front.StencilFunc = ComparisonFunction::Always;

        DepthStencilOpDescription back;
        back.StencilFailOp = StencilOp::Keep;
        back.StencilDepthFailOp = StencilOp::Decr;
        back.StencilPassOp = StencilOp::Keep;
        back.StencilFunc = ComparisonFunction::Always;

        m_cubemapPipeline.setDepthStencilState(DepthStencilDescription()
            .depthEnable(false)
            );

        m_brdfConvolution = device.createTextureSRV(TextureDescription()
            .format(Format::R16G16B16A16_FLOAT)
            .width(512)
            .height(512)
            .dimension(ResourceDimension::Texture2D)
            .name("Environment BRDF convolution")
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .optimizedClearValue(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        m_brdfConvolutionRTV = device.createTextureRTV(m_brdfConvolution.texture());

        m_equirectToCubemapPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_equirectToCubemapPipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(true).cullMode(CullMode::Back));
        m_equirectToCubemapPipeline.setDepthStencilState(DepthStencilDescription().depthEnable(false));
        m_equirectToCubemapPipeline.ps.equirectangularSampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear));

        m_irradiancePipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_irradiancePipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false).cullMode(CullMode::Back));
        m_irradiancePipeline.setDepthStencilState(DepthStencilDescription().depthEnable(false));
        m_irradiancePipeline.ps.environmentSampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear));

        m_prefilterConvolutionPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_prefilterConvolutionPipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false).cullMode(CullMode::Back));
        m_prefilterConvolutionPipeline.setDepthStencilState(DepthStencilDescription().depthEnable(false));
        m_prefilterConvolutionPipeline.ps.environmentSampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear));

        m_brdfConvolutionPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
        m_brdfConvolutionPipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false).cullMode(CullMode::Back));
        m_brdfConvolutionPipeline.setDepthStencilState(DepthStencilDescription().depthEnable(false));

        m_cubemapCamera.nearPlane(0.1f);
        m_cubemapCamera.farPlane(1000.0f);
        m_cubemapCamera.fieldOfView(90.0f);
        
        m_cubemapCamera.projection(Projection::Perspective);
    }

    void RenderCubemap::clearResources()
    {
        m_cubemap = TextureSRV();
        m_irradiance = TextureSRV();
        m_prefilteredEnvironmentMap = TextureSRV();

        m_brdfConvolution = TextureSRV();
        m_brdfConvolutionRTV = TextureRTV();

        m_cubemapRenderTargets.clear();
        m_lastCubemapSize = 0u;

        m_irradianceRenderTargets.clear();
        m_lastIrradianceSize = 0u;

        m_prefilterConvolutionTargets.clear();
        m_lastPrefilterConvolutionSize = 0u;
    }

    void RenderCubemap::createCubemapFromEquirect(Device& device, TextureSRV& equirectEnvironmentMap)
    {
        uint32_t width = 2048;// std::min(equirectEnvironmentMap.texture().width(), equirectEnvironmentMap.texture().height());
        uint32_t height = width;

        if (m_lastCubemapSize != width)
        {
            m_cubemap = device.createTextureSRV(TextureDescription()
                .format(Format::R16G16B16A16_FLOAT)
                .width(width)
                .height(height)
                .arraySlices(6)
                .dimension(ResourceDimension::TextureCubemap)
                .mipLevels(static_cast<uint32_t>(mipCount(static_cast<int>(width), static_cast<int>(height))))
                .name("Environment Cubemap equi")
                .usage(ResourceUsage::GpuRenderTargetReadWrite)
                .optimizedClearValue(Float4(0.0f, 0.0f, 0.0f, 0.0f)));

            m_cubemapRenderTargets.clear();
            for (uint32_t i = 0; i < m_cubemap.texture().mipLevels(); ++i)
            {
                std::vector<TextureRTV> mips;

                mips.emplace_back(std::move(device.createTextureRTV(m_cubemap.texture(), m_cubemap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ i, 1, 0, 1 })));
                mips.emplace_back(std::move(device.createTextureRTV(m_cubemap.texture(), m_cubemap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ i, 1, 1, 1 })));
                mips.emplace_back(std::move(device.createTextureRTV(m_cubemap.texture(), m_cubemap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ i, 1, 2, 1 })));
                mips.emplace_back(std::move(device.createTextureRTV(m_cubemap.texture(), m_cubemap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ i, 1, 3, 1 })));
                mips.emplace_back(std::move(device.createTextureRTV(m_cubemap.texture(), m_cubemap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ i, 1, 4, 1 })));
                mips.emplace_back(std::move(device.createTextureRTV(m_cubemap.texture(), m_cubemap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ i, 1, 5, 1 })));

                m_cubemapRenderTargets.emplace_back(std::move(mips));
            }
            m_lastCubemapSize = width;
        }
        
        m_cubemapCamera.width(static_cast<int>(width));
        m_cubemapCamera.height(static_cast<int>(height));

        auto cmd = device.createCommandList();
        for (int i = 0; i < static_cast<int>(m_cubemap.texture().mipLevels()); ++i)
        {
            std::vector<std::pair<TextureRTV&, Matrix4f>> sides;
            sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_cubemapRenderTargets[i][0], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f( 1.0f,  0.0f,  0.0f), Vector3f(0.0f, 1.0f,  0.0f)) });
            sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_cubemapRenderTargets[i][1], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(-1.0f,  0.0f,  0.0f), Vector3f(0.0f, 1.0f,  0.0f)) });
            sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_cubemapRenderTargets[i][2], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f( 0.0f,  1.0f,  0.0f), Vector3f(0.0f, 0.0f,  1.0f)) });
            sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_cubemapRenderTargets[i][3], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f( 0.0f, -1.0f,  0.0f), Vector3f(0.0f, 0.0f, -1.0f)) });
            sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_cubemapRenderTargets[i][4], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f( 0.0f,  0.0f, -1.0f), Vector3f(0.0f, 1.0f,  0.0f)) });
            sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_cubemapRenderTargets[i][5], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f( 0.0f,  0.0f,  1.0f), Vector3f(0.0f, 1.0f,  0.0f)) });

            for (auto&& side : sides)
            {
                cmd.setRenderTargets({ side.first });
                cmd.setViewPorts({ Viewport{ 0.0f, 0.0f,
                    static_cast<float>(std::max(static_cast<int>(width) >> i, 1)),
                    static_cast<float>(std::max(static_cast<int>(height) >> i, 1)),
                    0.0f, 1.0f } });
                cmd.setScissorRects({ Rectangle{ 0, 0,
                    static_cast<unsigned int>(std::max(static_cast<int>(width) >> i, 1)),
                    static_cast<unsigned int>(std::max(static_cast<int>(height) >> i, 1)) } });

                //m_cubemapCamera.rotation(side.second);
                m_cubemapCamera.rotation(Quaternionf::fromMatrix(side.second));

                m_equirectToCubemapPipeline.vs.viewProjectionMatrix = fromMatrix(m_cubemapCamera.projectionMatrix() * m_cubemapCamera.viewMatrix());
                m_equirectToCubemapPipeline.ps.equirectangularMap = equirectEnvironmentMap;

                cmd.bindPipe(m_equirectToCubemapPipeline);
                cmd.draw(36);
            }
        }
        auto fence = device.createFence();
        device.queue().submit(cmd, fence);
        fence.blockUntilSignaled();
    }

    void RenderCubemap::createIrradianceCubemap(Device& device, TextureSRV& diffuseEnvironmentMap,
        CommandList& cmd)
    {
        CPU_MARKER("Create irradiance cubemap");
        GPU_MARKER(cmd, "Create irradiance cubemap");

        uint32_t width = 256;// std::min(diffuseEnvironmentMap.texture().width() >> 4, diffuseEnvironmentMap.texture().height() >> 4);
        uint32_t height = width;

        if (m_lastIrradianceSize != width)
        {
            m_irradianceRenderTargets.clear();

            m_irradiance = device.createTextureSRV(TextureDescription()
                .format(Format::R16G16B16A16_FLOAT)
                .width(width)
                .height(height)
                .arraySlices(6)
                .dimension(ResourceDimension::TextureCubemap)
                .name("Environment Irradiance")
                .usage(ResourceUsage::GpuRenderTargetReadWrite)
                .optimizedClearValue(Float4(0.0f, 0.0f, 0.0f, 0.0f)));


            m_irradianceRenderTargets.emplace_back(std::move(device.createTextureRTV(m_irradiance.texture(), m_irradiance.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ 0, 1, 0, 1 })));
            m_irradianceRenderTargets.emplace_back(std::move(device.createTextureRTV(m_irradiance.texture(), m_irradiance.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ 0, 1, 1, 1 })));
            m_irradianceRenderTargets.emplace_back(std::move(device.createTextureRTV(m_irradiance.texture(), m_irradiance.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ 0, 1, 2, 1 })));
            m_irradianceRenderTargets.emplace_back(std::move(device.createTextureRTV(m_irradiance.texture(), m_irradiance.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ 0, 1, 3, 1 })));
            m_irradianceRenderTargets.emplace_back(std::move(device.createTextureRTV(m_irradiance.texture(), m_irradiance.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ 0, 1, 4, 1 })));
            m_irradianceRenderTargets.emplace_back(std::move(device.createTextureRTV(m_irradiance.texture(), m_irradiance.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ 0, 1, 5, 1 })));

            m_lastIrradianceSize = width;
        }

        std::vector<std::pair<TextureRTV&, Matrix4f>> sides;
        sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_irradianceRenderTargets[0], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f( 1.0f,  0.0f,  0.0f), Vector3f(0.0f, 1.0f,  0.0f)) });
        sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_irradianceRenderTargets[1], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(-1.0f,  0.0f,  0.0f), Vector3f(0.0f, 1.0f,  0.0f)) });
        sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_irradianceRenderTargets[2], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f( 0.0f,  1.0f,  0.0f), Vector3f(0.0f, 0.0f,  1.0f)) });
        sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_irradianceRenderTargets[3], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f( 0.0f, -1.0f,  0.0f), Vector3f(0.0f, 0.0f, -1.0f)) });
        sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_irradianceRenderTargets[4], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f( 0.0f,  0.0f, -1.0f), Vector3f(0.0f, 1.0f,  0.0f)) });
        sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_irradianceRenderTargets[5], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f( 0.0f,  0.0f,  1.0f), Vector3f(0.0f, 1.0f,  0.0f)) });

        m_cubemapCamera.width(static_cast<int>(width));
        m_cubemapCamera.height(static_cast<int>(height));

        //cmd.clearTexture(m_irradiance.texture());

        for (auto&& side : sides)
        {
            cmd.clearRenderTargetView(side.first, { 0.0f, 0.0f, 0.0f, 0.0f });
            cmd.setRenderTargets({ side.first });
            m_cubemapCamera.rotation(Quaternionf::fromMatrix(side.second));

            m_irradiancePipeline.vs.viewProjectionMatrix = fromMatrix(m_cubemapCamera.projectionMatrix() * m_cubemapCamera.viewMatrix());
            m_irradiancePipeline.ps.environmentMap = diffuseEnvironmentMap;

            cmd.bindPipe(m_irradiancePipeline);
            cmd.draw(36);
        }
        
    }

    void RenderCubemap::prefilterConvolution(Device& device, TextureSRV& diffuseEnvironmentMap,
        CommandList& cmd)
    {
        CPU_MARKER("Create convolution cubemap");
        GPU_MARKER(cmd, "Create convolution cubemap");

        uint32_t width = 256;// std::min(diffuseEnvironmentMap.texture().width(), diffuseEnvironmentMap.texture().height());
        uint32_t height = width;
        unsigned int maxMipLevels = std::min(static_cast<uint32_t>(mipCount(static_cast<int>(width), static_cast<int>(height))), 5u);

        if (m_lastPrefilterConvolutionSize != width)
        {
            m_prefilteredEnvironmentMap = device.createTextureSRV(TextureDescription()
                .format(Format::R16G16B16A16_FLOAT)
                .width(width)
                .height(height)
                .arraySlices(6)
                .mipLevels(maxMipLevels)
                .dimension(ResourceDimension::TextureCubemap)
                .name("Environment Prefiltered convolution")
                .usage(ResourceUsage::GpuRenderTargetReadWrite)
                .optimizedClearValue(Float4(0.0f, 0.0f, 0.0f, 0.0f)));

            m_prefilterConvolutionTargets.clear();
            for (uint32_t i = 0; i < maxMipLevels; ++i)
            {
                std::vector<TextureRTV> mips;

                mips.emplace_back(std::move(device.createTextureRTV(m_prefilteredEnvironmentMap.texture(), m_prefilteredEnvironmentMap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ i, 1, 0, 1 })));
                mips.emplace_back(std::move(device.createTextureRTV(m_prefilteredEnvironmentMap.texture(), m_prefilteredEnvironmentMap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ i, 1, 1, 1 })));
                mips.emplace_back(std::move(device.createTextureRTV(m_prefilteredEnvironmentMap.texture(), m_prefilteredEnvironmentMap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ i, 1, 2, 1 })));
                mips.emplace_back(std::move(device.createTextureRTV(m_prefilteredEnvironmentMap.texture(), m_prefilteredEnvironmentMap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ i, 1, 3, 1 })));
                mips.emplace_back(std::move(device.createTextureRTV(m_prefilteredEnvironmentMap.texture(), m_prefilteredEnvironmentMap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ i, 1, 4, 1 })));
                mips.emplace_back(std::move(device.createTextureRTV(m_prefilteredEnvironmentMap.texture(), m_prefilteredEnvironmentMap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ i, 1, 5, 1 })));

                m_prefilterConvolutionTargets.emplace_back(std::move(mips));
            }
            m_lastPrefilterConvolutionSize = width;
        }

        m_cubemapCamera.width(static_cast<int>(width));
        m_cubemapCamera.height(static_cast<int>(height));

        cmd.clearTexture(m_prefilteredEnvironmentMap.texture());

        for (int i = 0; i < static_cast<int>(maxMipLevels); ++i)
        {
            std::vector<std::pair<TextureRTV&, Matrix4f>> sides;
            sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_prefilterConvolutionTargets[i][0], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f( 1.0f,  0.0f,  0.0f), Vector3f(0.0f, 1.0f,  0.0f)) });
            sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_prefilterConvolutionTargets[i][1], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(-1.0f,  0.0f,  0.0f), Vector3f(0.0f, 1.0f,  0.0f)) });
            sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_prefilterConvolutionTargets[i][2], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f( 0.0f,  1.0f,  0.0f), Vector3f(0.0f, 0.0f,  1.0f)) });
            sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_prefilterConvolutionTargets[i][3], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f( 0.0f, -1.0f,  0.0f), Vector3f(0.0f, 0.0f, -1.0f)) });
            sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_prefilterConvolutionTargets[i][4], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f( 0.0f,  0.0f, -1.0f), Vector3f(0.0f, 1.0f,  0.0f)) });
            sides.emplace_back(std::pair<TextureRTV&, Matrix4f>{ m_prefilterConvolutionTargets[i][5], m_cubemapCamera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f( 0.0f,  0.0f,  1.0f), Vector3f(0.0f, 1.0f,  0.0f)) });

            for (auto&& side : sides)
            {
                cmd.clearRenderTargetView(side.first, { 0.0f, 0.0f, 0.0f, 0.0f });

                cmd.setRenderTargets({ side.first });
                cmd.setViewPorts({ Viewport{ 0.0f, 0.0f,
                    static_cast<float>(std::max(static_cast<int>(width) >> i, 1)),
                    static_cast<float>(std::max(static_cast<int>(height) >> i, 1)),
                    0.0f, 1.0f } });
                cmd.setScissorRects({ Rectangle{ 0, 0,
                    static_cast<unsigned int>(std::max(static_cast<int>(width) >> i, 1)),
                    static_cast<unsigned int>(std::max(static_cast<int>(height) >> i, 1)) } });

                m_cubemapCamera.rotation(Quaternionf::fromMatrix(side.second));

                m_prefilterConvolutionPipeline.vs.viewProjectionMatrix = fromMatrix(m_cubemapCamera.projectionMatrix() * m_cubemapCamera.viewMatrix());
                m_prefilterConvolutionPipeline.ps.environmentMap = diffuseEnvironmentMap;

                m_prefilterConvolutionPipeline.ps.roughness = static_cast<float>(i) / static_cast<float>(maxMipLevels - 1);

                cmd.bindPipe(m_prefilterConvolutionPipeline);
                cmd.draw(36);
            }
        }
    }

    void RenderCubemap::brdfConvolution(CommandList& cmd)
    {
        CPU_MARKER("Create brdf convolution cubemap");
        GPU_MARKER(cmd, "Create brdf convolution cubemap");

        cmd.clearRenderTargetView(m_brdfConvolutionRTV, { 0.0f, 0.0f, 0.0f, 0.0f });
        cmd.setRenderTargets({ m_brdfConvolutionRTV });
        cmd.bindPipe(m_brdfConvolutionPipeline);
        cmd.draw(4);
    }

    void RenderCubemap::render(
        TextureRTV& currentRenderTarget,
        Camera& camera,
        CommandList& cmd)
    {
        cmd.clearRenderTargetView(currentRenderTarget, { 0.0f, 0.0f, 0.0f, 1.0f });

        cmd.setRenderTargets({ currentRenderTarget });

        m_cubemapPipeline.vs.viewProjectionMatrix = fromMatrix(camera.projectionMatrix() * camera.viewMatrix(Vector3f(0.0f, 0.0f, 0.0f)));
        m_cubemapPipeline.ps.cubemap = m_cubemap;
        m_cubemapPipeline.ps.irradiance = m_irradiance;
        m_cubemapPipeline.ps.convolution = m_prefilteredEnvironmentMap;

        cmd.bindPipe(m_cubemapPipeline);
        cmd.draw(36);
    }

}
