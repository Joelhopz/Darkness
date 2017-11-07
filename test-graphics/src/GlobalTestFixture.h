#pragma once

#include "gtest/gtest.h"

#include "tools/image/Image.h"
#include "platform/window/Window.h"
#include "engine/graphics/Graphics.h"
#include "platform/Environment.h"
#include "tools/StringTools.h"
#include "engine/Engine.h"
#include <chrono>
#include <memory>
#include <vector>

using namespace std;
using namespace platform;
using namespace engine;
using namespace shaders;

constexpr const char* shaderRelativeLocation = "\\..\\..\\..\\..\\..\\data\\engine\\graphics\\shaders\\";

class GlobalEnvironment : public ::testing::Environment
{
public:
	void SetUp() override
	{
		m_env = std::make_shared<RenderSetup>();
	}
	
	void TearDown() override
	{
		m_env = nullptr;
	}

	Device& device() { return m_env->device(); }
	ShaderStorage& shaderStorage() { return m_env->shaderStorage(); }
	SwapChain& swapChain() { return m_env->swapChain(); }
	Window& window() { return m_env->window(); }
	unsigned int currentBackBufferIndex() { return m_env->currentBackBufferIndex(); }

	TextureRTV currentRTV()
	{
		return m_env->currentRTV();
	}

	void submit(engine::CommandList&& commandList)
	{
		m_env->submit(std::move(commandList));
	}

	void present()
	{
		m_env->present();
	}

private:
	std::shared_ptr<RenderSetup> m_env;
	
};

extern GlobalEnvironment* envPtr;
