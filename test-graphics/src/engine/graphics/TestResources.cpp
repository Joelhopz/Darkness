#include "GlobalTestFixture.h"
#include "shaders/core/tests/RtvClearTest.h"
#include "shaders/core/tests/Texture2DArrayVisualizer.h"

TEST(TestResources, ClearRTVSlices)
{
    GlobalEnvironment& env = *envPtr;
    Device& dev = env.device();

    auto sliceCount = 4;
    auto mipCount = 4;
    Texture texture = dev.createTexture(TextureDescription()
        .width(512)
        .height(512)
        .arraySlices(sliceCount)
        .mipLevels(mipCount)
        .dimension(ResourceDimension::Texture2DArray)
        .format(engine::Format::R8G8B8A8_UNORM)
        .name("Resource test texture")
        .usage(ResourceUsage::GpuRenderTargetReadWrite)
    );

    {
        TextureRTV slice0 = dev.createTextureRTV(texture, SubResource{ 0, mipCount, 0, 1 });
        TextureRTV slice1 = dev.createTextureRTV(texture, SubResource{ 0, mipCount, 1, 1 });
        TextureRTV slice2 = dev.createTextureRTV(texture, SubResource{ 0, mipCount, 2, 1 });
        TextureRTV slice3 = dev.createTextureRTV(texture, SubResource{ 0, mipCount, 3, 1 });

        auto cmd = dev.createCommandList();
        cmd.clearRenderTargetView(slice0, { 1.0f, 0.0f, 0.0f, 1.0f });
        cmd.clearRenderTargetView(slice1, { 0.0f, 1.0f, 0.0f, 1.0f });
        cmd.clearRenderTargetView(slice2, { 0.0f, 0.0f, 1.0f, 1.0f });
        cmd.clearRenderTargetView(slice3, { 1.0f, 1.0f, 0.0f, 1.0f });

        auto pipe = dev.createPipeline<RtvClearTest>(env.shaderStorage());
        pipe.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
        pipe.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
        pipe.ps.samp = dev.createSampler(SamplerDescription().filter(engine::Filter::Bilinear));

        for (int slice = 0; slice < sliceCount; ++slice)
        {
            for (int mip = 0; mip < mipCount - 1; ++mip)
            {
                auto hiResLod = dev.createTextureSRV(texture, SubResource{ static_cast<uint32_t>(mip), 1, static_cast<uint32_t>(slice), 1 });
                auto loResLod = dev.createTextureRTV(texture, SubResource{ static_cast<uint32_t>(mip + 1), 1, static_cast<uint32_t>(slice), 1 });

                pipe.ps.sourceLod = hiResLod;
                cmd.setRenderTargets({ loResLod });
                cmd.bindPipe(pipe);
                cmd.draw(4);
            }
        }
        env.submit(std::move(cmd));
    }

    auto pip = dev.createPipeline<Texture2DArrayVisualizer>(env.shaderStorage());
    do
    {
        auto cmd = dev.createCommandList();
        env.submit(std::move(cmd));
        env.present();
        env.window().processMessages();
    } while (env.canContinue(false));

    env.device().queue().waitForIdle();

}

