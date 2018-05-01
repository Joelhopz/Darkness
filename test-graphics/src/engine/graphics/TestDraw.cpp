#include "GlobalTestFixture.h"
#include "engine/graphics/Viewport.h"
#include "engine/graphics/Rect.h"

#include "shaders/core/tests/HLSLTest.h"
#include "shaders/core/tests/DrawWithoutBuffers.h"
#include "shaders/core/tests/DrawWithConstants.h"
#include "shaders/core/tests/DrawWithPixelShaderResources.h"
#include "shaders/core/tests/DrawWithTexture.h"

#include <iostream>
#include <fstream>

TEST(TestDraw, DrawWithoutBuffers)
{
    GlobalEnvironment& env = *envPtr;

    auto drawWithoutBuffers = env.device().createPipeline<DrawWithoutBuffers>(env.shaderStorage());
    drawWithoutBuffers.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
    drawWithoutBuffers.setRasterizerState(RasterizerDescription());

    do
    {
        auto cmdBuffer = env.device().createCommandList();

        cmdBuffer.clearRenderTargetView(env.currentRTV(), { 0.0f, 0.0f, 0.0f, 1.0f });
        cmdBuffer.setRenderTargets({ env.currentRTV() });
        cmdBuffer.bindPipe(drawWithoutBuffers);
        cmdBuffer.draw(3);

        env.submit(std::move(cmdBuffer));
        env.present();
    } while (env.canContinue(false));
    env.device().queue().waitForIdle();
}

TEST(TestDraw, DrawWithConstants)
{
    GlobalEnvironment& env = *envPtr;

    auto drawWithoutBuffers = env.device().createPipeline<DrawWithConstants>(env.shaderStorage());
    drawWithoutBuffers.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
    drawWithoutBuffers.setRasterizerState(RasterizerDescription());
    drawWithoutBuffers.vs.color1 = { 0.3f, 0.7f, 0.2f, 1.0f };
    drawWithoutBuffers.vs.color2 = { 0.4f, 0.5f, 1.0f, 1.0f };
    drawWithoutBuffers.vs.color3 = { 0.8f, 0.9f, 0.1f, 1.0f };

    do
    {
        auto cmdBuffer = env.device().createCommandList();
        cmdBuffer.clearRenderTargetView(env.currentRTV(), { 0.0f, 0.0f, 0.0f, 1.0f });
        cmdBuffer.setRenderTargets({ env.currentRTV() });
        cmdBuffer.bindPipe(drawWithoutBuffers);
        cmdBuffer.draw(3);

        env.submit(std::move(cmdBuffer));
        env.present();
    } while (env.canContinue(false));
    env.device().queue().waitForIdle();
}

TEST(TestDraw, DrawWithPixelShaderResources)
{
    GlobalEnvironment& env = *envPtr;

    auto colorBuffer = env.device().createBufferSRV(
        engine::BufferDescription()
        .name("color")
        .format(Format::R32G32B32A32_FLOAT)
        .setInitialData(BufferDescription::InitialData(std::vector<Float4>{
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f, 1.0f }
        })));


    auto drawWithoutBuffers = env.device().createPipeline<DrawWithPixelShaderResources>(env.shaderStorage());
    drawWithoutBuffers.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
    drawWithoutBuffers.setRasterizerState(RasterizerDescription());
    drawWithoutBuffers.ps.color = colorBuffer;

    do
    {
        auto cmdBuffer = env.device().createCommandList();
        cmdBuffer.clearRenderTargetView(env.currentRTV(), { 0.0f, 0.0f, 0.0f, 1.0f });
        cmdBuffer.setRenderTargets({ env.currentRTV() });
        cmdBuffer.bindPipe(drawWithoutBuffers);
        cmdBuffer.draw(3);
        
        env.submit(std::move(cmdBuffer));
        env.present();
    } while (env.canContinue(false));
    env.device().queue().waitForIdle();
}

TEST(TestDraw, Draw)
{
    GlobalEnvironment& env = *envPtr;

    auto positionSRV = env.device().createBufferSRV(BufferDescription()
        .name("position")
        .format(Format::R32G32B32A32_FLOAT)
        .setInitialData(BufferDescription::InitialData(std::vector<Float4>{
            { -0.5f, 0.5f, 0.0f, 1.0f }, { 0.0f, -0.5f, 0.0f, 1.0f }, { 0.5f, 0.5f, 0.0f, 1.0f }  })));
    
    auto colorSRV = env.device().createBufferSRV(BufferDescription()
        .name("colorStaging")
        .format(Format::R32G32B32A32_FLOAT)
        .setInitialData(BufferDescription::InitialData(std::vector<Float4>{
            { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } })));

    // create pipeline
    auto pipeline = env.device().createPipeline<shaders::HLSLTest>(env.shaderStorage());
    pipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
    pipeline.setRasterizerState(RasterizerDescription());
    pipeline.vs.position = positionSRV;
    pipeline.vs.color = colorSRV;

    // presentation
    do
    {
        auto cmdBuffer = env.device().createCommandList();
        cmdBuffer.clearRenderTargetView(env.currentRTV(), { 0.0f, 0.0f, 0.0f, 1.0f });
        cmdBuffer.setRenderTargets({ env.currentRTV() });
        cmdBuffer.bindPipe(pipeline);
        cmdBuffer.draw(3u);

        env.submit(std::move(cmdBuffer));
        env.present();
    } while (env.canContinue(false));

    // close
    env.device().queue().waitForIdle();
}

TEST(TestDraw, DrawIndexed)
{
    GlobalEnvironment& env = *envPtr;

    auto positionSRV = env.device().createBufferSRV(BufferDescription()
        .name("position")
        .format(Format::R32G32B32A32_FLOAT)
        .setInitialData(BufferDescription::InitialData(std::vector<Float4>{
            { -0.5f, 0.5f, 0.0f, 1.0f }, { 0.0f, -0.5f, 0.0f, 1.0f }, { 0.5f, 0.5f, 0.0f, 1.0f }  })));

    auto colorSRV = env.device().createBufferSRV(BufferDescription()
        .name("colorStaging")
        .format(Format::R32G32B32A32_FLOAT)
        .setInitialData(BufferDescription::InitialData(std::vector<Float4>{
            { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } })));

    auto indexBufferView = env.device().createBufferIBV(BufferDescription()
        .name("indexStaging")
        .setInitialData(BufferDescription::InitialData(std::vector<uint32_t>{0, 1, 2})));

    // create pipeline
    auto pipeline = env.device().createPipeline<shaders::HLSLTest>(env.shaderStorage());
    pipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
    pipeline.setRasterizerState(RasterizerDescription());
    pipeline.vs.position = positionSRV;
    pipeline.vs.color = colorSRV;

    // presentation
    do
    {
        engine::CommandList cmdBuffer = env.device().createCommandList();
        cmdBuffer.clearRenderTargetView(env.currentRTV(), { 0.0f, 0.0f, 0.0f, 1.0f });
        cmdBuffer.setRenderTargets({ env.currentRTV() });
        cmdBuffer.bindPipe(pipeline);
        cmdBuffer.drawIndexed(indexBufferView, static_cast<uint32_t>(3), 1, 0, 0, 0);

        env.submit(std::move(cmdBuffer));
        env.present();
    } while (env.canContinue(false));

    // close
    env.device().queue().waitForIdle();
}

TEST(TestDraw, DrawTextured)
{
    GlobalEnvironment& env = *envPtr;

    std::shared_ptr<engine::image::ImageIf> srcTexture = image::Image::createImage(
        //pathJoin(getWorkingDirectory(), pathJoin(DataLocation, "images\\test_bc7.DDS")));
        "..\\..\\..\\test-graphics\\data\\images\\test_bc7.DDS", image::ImageType::DDS);

    ASSERT(srcTexture, "Could not find test image");

    auto colorSRV = env.device().createTextureSRV(TextureDescription()
        .name("color")
        .width(static_cast<uint32_t>(srcTexture->width()))
        .height(static_cast<uint32_t>(srcTexture->height()))
        .format(srcTexture->format())
        .setInitialData(TextureDescription::InitialData(
            std::vector<uint8_t>(srcTexture->data(), srcTexture->data() + srcTexture->bytes()),
            static_cast<uint32_t>(srcTexture->width()), static_cast<uint32_t>(srcTexture->width() * srcTexture->height()))));

    // create pipeline
    auto pipeline = env.device().createPipeline<shaders::DrawWithTexture>(env.shaderStorage());
    pipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
    pipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
    pipeline.vs.width = static_cast<float>(env.window().width());
    pipeline.vs.height = static_cast<float>(env.window().height());
    pipeline.ps.tex_sampler = env.device().createSampler(SamplerDescription().filter(engine::Filter::Bilinear));
    pipeline.ps.tex = colorSRV;

    // presentation
    do
    {
        auto cmdBuffer = env.device().createCommandList();
        cmdBuffer.clearRenderTargetView(env.currentRTV(), { 0.0f, 0.0f, 0.0f, 1.0f });
        cmdBuffer.setRenderTargets({ env.currentRTV() });
        cmdBuffer.bindPipe(pipeline);
        cmdBuffer.draw(4u);

        env.submit(std::move(cmdBuffer));
        env.present();

        env.window().processMessages();
    } while (env.canContinue(false));

    // close
    env.device().queue().waitForIdle();
}
