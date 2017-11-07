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
	drawWithoutBuffers.setRenderTargetFormat(engine::Format::Format_R8G8B8A8_UNORM, engine::Format::Format_UNKNOWN);

	do
	{
		auto cmdBuffer = env.device().createCommandList();

        cmdBuffer.setViewPorts({
            engine::Viewport{
            0, 0,
            static_cast<float>(env.window().width()),
            static_cast<float>(env.window().height()),
            1.0f, 0.0f } });
        cmdBuffer.setScissorRects({ engine::Rectangle{ 0, 0,
            static_cast<unsigned int>(env.window().width()),
            static_cast<unsigned int>(env.window().height()) } });

		cmdBuffer.bindPipe(drawWithoutBuffers);
		cmdBuffer.setRenderTargets({ env.currentRTV() });
		cmdBuffer.clearRenderTargetView(env.currentRTV(), { 0.0f, 0.0f, 0.0f, 1.0f });

		cmdBuffer.draw(3);

		env.submit(std::move(cmdBuffer));
		env.present();
	} while (false);
	env.device().queue().waitForIdle();
}

TEST(TestDraw, DrawWithConstants)
{
	GlobalEnvironment& env = *envPtr;

	auto drawWithoutBuffers = env.device().createPipeline<DrawWithConstants>(env.shaderStorage());
	drawWithoutBuffers.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
	drawWithoutBuffers.setRasterizerState(RasterizerDescription());
	drawWithoutBuffers.setRenderTargetFormat(engine::Format::Format_R8G8B8A8_UNORM, engine::Format::Format_UNKNOWN);
	drawWithoutBuffers.vs.color1 = { 0.3f, 0.7f, 0.2f, 1.0f };
	drawWithoutBuffers.vs.color2 = { 0.4f, 0.5f, 1.0f, 1.0f };
	drawWithoutBuffers.vs.color3 = { 0.8f, 0.9f, 0.1f, 1.0f };

	do
	{
		auto cmdBuffer = env.device().createCommandList();

        cmdBuffer.setViewPorts({
            engine::Viewport{
            0, 0,
            static_cast<float>(env.window().width()),
            static_cast<float>(env.window().height()),
            1.0f, 0.0f } });
        cmdBuffer.setScissorRects({ engine::Rectangle{ 0, 0,
            static_cast<unsigned int>(env.window().width()),
            static_cast<unsigned int>(env.window().height()) } });

		cmdBuffer.bindPipe(drawWithoutBuffers);
		cmdBuffer.setRenderTargets({ env.currentRTV() });
		cmdBuffer.clearRenderTargetView(env.currentRTV(), { 0.0f, 0.0f, 0.0f, 1.0f });


		cmdBuffer.draw(3);

		env.submit(std::move(cmdBuffer));
		env.present();
	} while (false);
	env.device().queue().waitForIdle();
}

TEST(TestDraw, DrawWithPixelShaderResources)
{
	GlobalEnvironment& env = *envPtr;

	auto colorBuffer = env.device().createBufferSRV(
		engine::BufferDescription()
		.usage(ResourceUsage::CpuToGpu)
		.name("color")
		.format(Format::Format_R32G32B32A32_FLOAT)
		.setInitialData(BufferDescription::InitialData(std::vector<Float4>{
			{ 1.0f, 0.0f, 0.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f }
		})));


    auto drawWithoutBuffers = env.device().createPipeline<DrawWithPixelShaderResources>(env.shaderStorage());
    drawWithoutBuffers.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
	drawWithoutBuffers.setRasterizerState(RasterizerDescription());
	drawWithoutBuffers.ps.color = colorBuffer;
	drawWithoutBuffers.setRenderTargetFormat(engine::Format::Format_R8G8B8A8_UNORM, engine::Format::Format_UNKNOWN);

	do
	{
		auto cmdBuffer = env.device().createCommandList();

        cmdBuffer.setViewPorts({
            engine::Viewport{
            0, 0,
            static_cast<float>(env.window().width()),
            static_cast<float>(env.window().height()),
            1.0f, 0.0f } });
        cmdBuffer.setScissorRects({ engine::Rectangle{ 0, 0,
            static_cast<unsigned int>(env.window().width()),
            static_cast<unsigned int>(env.window().height()) } });

		cmdBuffer.bindPipe(drawWithoutBuffers);
		cmdBuffer.setRenderTargets({ env.currentRTV() });
		cmdBuffer.clearRenderTargetView(env.currentRTV(), { 0.0f, 0.0f, 0.0f, 1.0f });

		cmdBuffer.draw(3);
		
		env.submit(std::move(cmdBuffer));
		env.present();
	} while (false);
	env.device().queue().waitForIdle();
}

TEST(TestDraw, Draw)
{
	GlobalEnvironment& env = *envPtr;

    auto positionSRV = env.device().createBufferSRV(BufferDescription()
        .usage(ResourceUsage::CpuToGpu)
        .name("position")
		.format(Format::Format_R32G32B32A32_FLOAT)
        .setInitialData(BufferDescription::InitialData(std::vector<Float4>{
			{ -0.5f, 0.5f, 0.0f, 1.0f }, { 0.0f, -0.5f, 0.0f, 1.0f }, { 0.5f, 0.5f, 0.0f, 1.0f }  })));
    
    auto colorSRV = env.device().createBufferSRV(BufferDescription()
        .usage(ResourceUsage::CpuToGpu)
        .name("colorStaging")
		.format(Format::Format_R32G32B32A32_FLOAT)
        .setInitialData(BufferDescription::InitialData(std::vector<Float4>{
            { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } })));

	// create pipeline
	auto pipeline = env.device().createPipeline<shaders::HLSLTest>(env.shaderStorage());
	pipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
	pipeline.setRasterizerState(RasterizerDescription());
	pipeline.vs.position = positionSRV;
	pipeline.vs.color = colorSRV;
	pipeline.setRenderTargetFormat(Format::Format_R8G8B8A8_UNORM, Format::Format_UNKNOWN);

    // presentation
    do
    {
		auto cmdBuffer = env.device().createCommandList();

        cmdBuffer.setViewPorts({
            engine::Viewport{
            0, 0,
            static_cast<float>(env.window().width()),
            static_cast<float>(env.window().height()),
            1.0f, 0.0f } });
        cmdBuffer.setScissorRects({ engine::Rectangle{ 0, 0,
            static_cast<unsigned int>(env.window().width()),
            static_cast<unsigned int>(env.window().height()) } });

		cmdBuffer.bindPipe(pipeline);
		cmdBuffer.setRenderTargets({ env.currentRTV() });
		cmdBuffer.clearRenderTargetView(env.currentRTV(), { 0.0f, 0.0f, 0.0f, 1.0f });

		cmdBuffer.draw(3u);

		env.submit(std::move(cmdBuffer));
		env.present();
    } while (false);

    // close
	env.device().queue().waitForIdle();
}

TEST(TestDraw, DrawIndexed)
{
	GlobalEnvironment& env = *envPtr;

	auto positionSRV = env.device().createBufferSRV(BufferDescription()
		.usage(ResourceUsage::CpuToGpu)
		.name("position")
		.format(Format::Format_R32G32B32A32_FLOAT)
		.setInitialData(BufferDescription::InitialData(std::vector<Float4>{
			{ -0.5f, 0.5f, 0.0f, 1.0f }, { 0.0f, -0.5f, 0.0f, 1.0f }, { 0.5f, 0.5f, 0.0f, 1.0f }  })));

	auto colorSRV = env.device().createBufferSRV(BufferDescription()
		.usage(ResourceUsage::CpuToGpu)
		.name("colorStaging")
		.format(Format::Format_R32G32B32A32_FLOAT)
		.setInitialData(BufferDescription::InitialData(std::vector<Float4>{
			{ 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } })));

	auto indexBufferView = env.device().createBufferIBV(BufferDescription()
		.usage(ResourceUsage::CpuToGpu)
		.name("indexStaging")
		.setInitialData(BufferDescription::InitialData(std::vector<uint32_t>{0, 1, 2})));

	// create pipeline
	auto pipeline = env.device().createPipeline<shaders::HLSLTest>(env.shaderStorage());
	pipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
	pipeline.setRasterizerState(RasterizerDescription());
	pipeline.vs.position = positionSRV;
	pipeline.vs.color = colorSRV;
	pipeline.setRenderTargetFormat(Format::Format_R8G8B8A8_UNORM, Format::Format_UNKNOWN);

	// presentation
	do
	{
		engine::CommandList cmdBuffer = env.device().createCommandList();

        cmdBuffer.setViewPorts({
            engine::Viewport{
            0, 0,
            static_cast<float>(env.window().width()),
            static_cast<float>(env.window().height()),
            1.0f, 0.0f } });
        cmdBuffer.setScissorRects({ engine::Rectangle{ 0, 0,
            static_cast<unsigned int>(env.window().width()),
            static_cast<unsigned int>(env.window().height()) } });

		cmdBuffer.bindPipe(pipeline);
		cmdBuffer.setRenderTargets({ env.currentRTV() });
		cmdBuffer.clearRenderTargetView(env.currentRTV(), { 0.0f, 0.0f, 0.0f, 1.0f });

		cmdBuffer.bindIndexBuffer(indexBufferView);
		cmdBuffer.drawIndexed(static_cast<uint32_t>(3), 1, 0, 0, 0);

		env.submit(std::move(cmdBuffer));
		env.present();
	} while (false);

	// close
	env.device().queue().waitForIdle();
}

TEST(TestDraw, DrawTextured)
{
	GlobalEnvironment& env = *envPtr;

	std::shared_ptr<engine::image::ImageIf> srcTexture = image::Image::createImage(
		"C:\\work\    estdata\    est3.dds");

	auto colorSRV = env.device().createTextureSRV(TextureDescription()
		.usage(ResourceUsage::CpuToGpu)
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
	pipeline.setRenderTargetFormat(Format::Format_R8G8B8A8_UNORM, Format::Format_UNKNOWN);

	// presentation
	do
	{
		auto cmdBuffer = env.device().createCommandList();

		cmdBuffer.bindPipe(pipeline);
		cmdBuffer.setRenderTargets({ env.currentRTV() });
		cmdBuffer.clearRenderTargetView(env.currentRTV(), { 0.0f, 0.0f, 0.0f, 1.0f });
        cmdBuffer.setViewPorts({ 
            engine::Viewport{ 
            0, 0, 
            static_cast<float>(env.window().width()), 
            static_cast<float>(env.window().height()),
            1.0f, 0.0f } });
        cmdBuffer.setScissorRects({ engine::Rectangle{0, 0,
            static_cast<unsigned int>(env.window().width()),
            static_cast<unsigned int>(env.window().height()) } });

		cmdBuffer.draw(4u);

		env.submit(std::move(cmdBuffer));
		env.present();

        env.window().processMessages();
	} while (false);

	// close
	env.device().queue().waitForIdle();
}
