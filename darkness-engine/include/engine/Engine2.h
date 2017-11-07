#pragma once

#include "engine/rendering/Core.h"
#include "engine/rendering/MainRender.h"
#include "rendering/Rendering.h"
#include "platform/window/Window.h"
#include <memory>

class Engine2
{
public:
    Engine2(
        std::shared_ptr<platform::Window> window,
        const std::string& shaderRootPath);
    void update();

private:
    std::shared_ptr<platform::Window> m_window;
    engine::Device m_device;
    engine::CommandList m_commandList;
    engine::Fence m_fence;

    static engine::ShaderStorage m_shaderStorage;
};
