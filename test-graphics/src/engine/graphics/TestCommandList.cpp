#include "GlobalTestFixture.h"
#include "shaders/core/tests/Mrt.h"
#include "shaders/core/tests/MrtVisualize.h"

TEST(TestCommandList, SetRenderTargets_multiple)
{
    GlobalEnvironment& env = *envPtr;
    Device& dev = env.device();
    
    uint32_t rtvSize = 1024;
    uint32_t mrtCount = 4;
    std::vector<TextureRTV> targets;
    std::vector<TextureSRV> targetSRVs;
    std::vector<engine::Format> rtvFormats;
    for (uint32_t i = 0; i < mrtCount; ++i)
    {
        rtvFormats.emplace_back(Format::R8G8B8A8_UNORM);
        targets.emplace_back(dev.createTextureRTV(TextureDescription()
            .width(rtvSize)
            .height(rtvSize)
            .format(Format::R8G8B8A8_UNORM)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("RenderTargetTest RTV")));
    }

    for (uint32_t i = 0; i < mrtCount; ++i)
        targetSRVs.emplace_back(dev.createTextureSRV(targets[i].texture()));

    auto pipe = dev.createPipeline<Mrt>(env.shaderStorage());
    pipe.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
    pipe.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
    

    auto visualizePipe = dev.createPipeline<MrtVisualize>(env.shaderStorage());
    visualizePipe.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
    visualizePipe.vs.width = static_cast<float>(rtvSize);
    visualizePipe.vs.height = static_cast<float>(rtvSize);

    for (uint32_t i = 0; i < mrtCount; ++i)
        visualizePipe.ps.mrt.push(i, targetSRVs[i]);

    do
    {
        {
            auto cmd = dev.createCommandList();
            for (uint32_t i = 0; i < mrtCount; ++i)
                cmd.clearRenderTargetView(targets[i]);
            cmd.setRenderTargets(targets);
            cmd.bindPipe(pipe);

            cmd.draw(4);
            env.submit(std::move(cmd));
        }

        {
            auto cmd = dev.createCommandList();
            cmd.clearRenderTargetView(env.currentRTV(), {0.0f, 0.1f, 0.1f, 0.1f});

            cmd.setRenderTargets({ env.currentRTV() });
            cmd.bindPipe(visualizePipe);

            cmd.draw(24);
            env.submit(std::move(cmd));
        }

        env.present();
        env.window().processMessages();
    } while (env.canContinue(false));

    env.device().queue().waitForIdle();

}

