#include "engine/Engine2.h"

using namespace engine;

ShaderStorage Engine2::m_shaderStorage;

Engine2::Engine2(
    std::shared_ptr<platform::Window> window,
    const std::string& /* shaderRootPath */)
    : m_window{ window }
    , m_device{ m_window }
    , m_commandList{ m_device.createCommandList() }
    , m_fence{ m_device.createFence() }
{

}

void Engine2::update()
{
}
