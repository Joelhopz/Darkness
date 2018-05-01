#include "engine/rendering/DepthPyramid.h"
#include "engine/graphics/CommandList.h"
#include "engine/graphics/SamplerDescription.h"
#include <algorithm>

namespace engine
{
    DepthPyramid::DepthPyramid(
        Device& device,
        ShaderStorage& shaderStorage,
        int width,
        int height)
        : m_device{ device }
        , m_depthDownsample{ device.createPipeline<shaders::DepthDownsample>(shaderStorage) }
        , m_depthDownsampleSingle{ device.createPipeline<shaders::DepthDownsampleSingle>(shaderStorage) }

        , m_clearTexture{ device.createPipeline<shaders::ClearTexture2df>(shaderStorage) }
        , m_copyTexture{ device.createPipeline<shaders::CopyTexture2df>(shaderStorage) }

        , m_sampler{ device.createSampler(SamplerDescription().filter(Filter::Point)) }
        , m_width{ width }
        , m_height{ height }
        , m_widthPow2{ roundUpToPow2(width) }
        , m_heightPow2{ roundUpToPow2(height) }
        , m_needClean{ true }
        , m_dsv{ device.createTextureDSV(TextureDescription()
            .name("Depth Buffer")
            .format(Format::D32_FLOAT)
            .width(width)
            .height(height)
            .usage(ResourceUsage::DepthStencil)
            .optimizedDepthClearValue(0.0f)
            .dimension(ResourceDimension::Texture2D)
        ) }
        , m_dsvSRV{ device.createTextureSRV(m_dsv) }

        , m_depthPyramid{ device.createTexture(TextureDescription()
            .name("Depth Pyramid")
            .format(Format::R32_FLOAT)
            .width(m_widthPow2)
            .height(m_heightPow2)
            .mipLevels(mipCount(m_widthPow2, m_heightPow2))
            .usage(ResourceUsage::GpuReadWrite)
            .optimizedDepthClearValue(0.0f)
            .dimension(ResourceDimension::Texture2D)
        ) }
        , m_depthPyramidSRV{ device.createTextureSRV(m_depthPyramid) }
    {
        m_depthDownsample.cs.srcSampler = m_sampler;
        m_depthDownsampleSingle.cs.srcSampler = m_sampler;
        for (int i = 0; i < mipCount(m_widthPow2, m_heightPow2); ++i)
        {
            m_srvs.emplace_back(m_device.createTextureSRV(m_depthPyramid, m_depthPyramid.description().format(Format::R32_FLOAT), SubResource{ static_cast<uint32_t>(i), 1, 0u, 1 }));
            m_uavs.emplace_back(m_device.createTextureUAV(m_depthPyramid, m_depthPyramid.description().format(Format::R32_FLOAT), SubResource{ static_cast<uint32_t>(i), 1, 0u, 1 }));
        }
    }

    void DepthPyramid::refresh(engine::Vector2<int> virtualResolution)
    {
        m_width = virtualResolution.x;
        m_height = virtualResolution.y;
        m_widthPow2 = roundUpToPow2(m_width);
        m_heightPow2 = roundUpToPow2(m_height);

        m_srvs.clear();
        m_uavs.clear();

        m_dsv = m_device.createTextureDSV(TextureDescription()
            .name("Depth Buffer")
            .format(Format::D32_FLOAT)
            .width(m_width)
            .height(m_height)
            .usage(ResourceUsage::DepthStencil)
            .optimizedDepthClearValue(0.0f)
            .dimension(ResourceDimension::Texture2D));

        m_dsvSRV = m_device.createTextureSRV(m_dsv);

        m_depthPyramid = m_device.createTexture(TextureDescription()
            .name("Depth Pyramid")
            .format(Format::R32_FLOAT)
            .width(m_widthPow2)
            .height(m_heightPow2)
            .mipLevels(mipCount(m_widthPow2, m_heightPow2))
            .usage(ResourceUsage::GpuReadWrite)
            .optimizedDepthClearValue(0.0f)
            .dimension(ResourceDimension::Texture2D));
        m_depthPyramidSRV = m_device.createTextureSRV(m_depthPyramid);

        for (int i = 0; i < mipCount(m_widthPow2, m_heightPow2); ++i)
        {
            m_srvs.emplace_back(m_device.createTextureSRV(m_depthPyramid, m_depthPyramid.description().format(Format::R32_FLOAT), SubResource{ static_cast<uint32_t>(i), 1, 0u, 1 }));
            m_uavs.emplace_back(m_device.createTextureUAV(m_depthPyramid, m_depthPyramid.description().format(Format::R32_FLOAT), SubResource{ static_cast<uint32_t>(i), 1, 0u, 1 }));
        }
        m_needClean = true;
    }

    static const char* DownsampleMultiMessages[5] = {
        "Depth Downsample 0-3",  "Depth Downsample 4-7",  "Depth Downsample 8-11",  "Depth Downsample 11-14", "Depth Downsample 15-18" };

    static const char* DownsampleMessages[16] = {
        "Depth Downsample 0",  "Depth Downsample 1",  "Depth Downsample 2",  "Depth Downsample 3",  "Depth Downsample 4",
        "Depth Downsample 5",  "Depth Downsample 6",  "Depth Downsample 7",  "Depth Downsample 8",  "Depth Downsample 9",
        "Depth Downsample 10", "Depth Downsample 11", "Depth Downsample 12", "Depth Downsample 13", "Depth Downsample 14"
    };

    void DepthPyramid::performDownsample(CommandList& cmd)
    {
        CPU_MARKER("Depth downsample");
        GPU_MARKER(cmd, "Depth downsample");

        if(m_needClean)
        {
            m_needClean = false;
            GPU_MARKER(cmd, "Clear depth slice 0");
            m_clearTexture.cs.size = { m_depthPyramid.width(), m_depthPyramid.height(), 1u, 1u };
            m_clearTexture.cs.tex = m_uavs[0];
            m_clearTexture.cs.value = 1.0f;
            cmd.bindPipe(m_clearTexture);
            cmd.dispatch(
                std::max(roundUpToMultiple(m_depthPyramid.width(), 9u) / 9u, 1u), 
                std::max(roundUpToMultiple(m_depthPyramid.height(), 9u) / 9u, 1u), 1u);
        }

        {
            GPU_MARKER(cmd, "Copy initial depth data");
            m_copyTexture.cs.src = m_dsvSRV;
            m_copyTexture.cs.dst = m_uavs[0];
            m_copyTexture.cs.srcTop.x = 0;
            m_copyTexture.cs.srcLeft.x = 0;
            m_copyTexture.cs.srcTextureWidth.x = m_dsvSRV.width();
            m_copyTexture.cs.srcTextureHeight.x = m_dsvSRV.height();
            m_copyTexture.cs.dstTop.x = 0;
            m_copyTexture.cs.dstLeft.x = 0;
            m_copyTexture.cs.dstTextureWidth.x = m_uavs[0].width();
            m_copyTexture.cs.dstTextureHeight.x = m_uavs[0].height();
            m_copyTexture.cs.copyWidth.x = m_dsvSRV.width();
            m_copyTexture.cs.copyHeight.x = m_dsvSRV.height();
            cmd.bindPipe(m_copyTexture);
            cmd.dispatch(
                roundUpToMultiple(m_dsvSRV.width(), 9u) / 9u,
                roundUpToMultiple(m_dsvSRV.height(), 9u) / 9u,
                1u);
        }

        //cmd.clearTexture(m_depthPyramid, 1.0f, SubResource{ 0, 1, 0, 1 });
        //cmd.copyTexture(m_dsvSRV, m_uavs[0]);

        auto fourPerCycle = (m_srvs.size() - 1) / 4;

        int currentIndex = 0;
        for (int i = 0; i < fourPerCycle; ++i)
        {
            GPU_MARKER(cmd, DownsampleMultiMessages[i]);

            auto currentWidth = std::max(m_widthPow2 >> (currentIndex + 1), 1);
            auto currentHeight = std::max(m_heightPow2 >> (currentIndex + 1), 1);

            m_depthDownsample.cs.size = Vector2f(static_cast<float>(currentWidth), static_cast<float>(currentHeight));
            m_depthDownsample.cs.invSize = Vector2f(1.0f / static_cast<float>(currentWidth), 1.0f / static_cast<float>(currentHeight));
            m_depthDownsample.cs.src = m_srvs[currentIndex];
            m_depthDownsample.cs.dst0 = m_uavs[currentIndex + 1];
            m_depthDownsample.cs.dst1 = m_uavs[currentIndex + 2];
            m_depthDownsample.cs.dst2 = m_uavs[currentIndex + 3];
            m_depthDownsample.cs.dst3 = m_uavs[currentIndex + 4];
            cmd.bindPipe(m_depthDownsample);
            cmd.dispatch(
                std::max(roundUpToMultiple(currentWidth, 8) / 8, 1), 
                std::max(roundUpToMultiple(currentHeight, 8) / 8, 1), 1);

            currentIndex += 4;
        }

        for (int i = currentIndex; i < m_srvs.size()-1; ++i)
        {
            GPU_MARKER(cmd, DownsampleMessages[i]);

            auto currentWidth = std::max(m_widthPow2 >> (i + 1), 1);
            auto currentHeight = std::max(m_heightPow2 >> (i + 1), 1);

            m_depthDownsampleSingle.cs.size = Vector2f(static_cast<float>(currentWidth), static_cast<float>(currentHeight));
            m_depthDownsampleSingle.cs.invSize = Vector2f(1.0f / static_cast<float>(currentWidth), 1.0f / static_cast<float>(currentHeight));
            m_depthDownsampleSingle.cs.src = m_srvs[i];
            m_depthDownsampleSingle.cs.dst = m_uavs[i + 1];
            cmd.bindPipe(m_depthDownsampleSingle);
            cmd.dispatch(
                std::max(roundUpToMultiple(currentWidth, 8) / 8, 1), 
                std::max(roundUpToMultiple(currentHeight, 8) / 8, 1),
                1);

            currentIndex += 4;
        }
    }
}
