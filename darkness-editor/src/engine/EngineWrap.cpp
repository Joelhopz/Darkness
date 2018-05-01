#include "EngineWrap.h"
#include "platform/window/Window.h"
#include <memory>

using namespace platform;
using namespace platform::implementation;
using namespace engine;

EngineWrap::EngineWrap(WId windowId, int width, int height, const QString& shaderRootPath)
    : m_shaderRootPath{ shaderRootPath.toStdString() }
    , m_engine{ std::make_shared<Window>((WindowHandle)windowId, width, height), m_shaderRootPath }
{
}

Engine& EngineWrap::engine()
{
    return m_engine;
}

EngineWrap::~EngineWrap()
{
    m_engine.shutdown();
}

void EngineWrap::refresh()
{
    m_engine.refresh();
}

void EngineWrap::update()
{
    m_engine.update();
}

std::shared_ptr<engine::SceneNode> EngineWrap::grabNode()
{
    return m_engine.grabSelected();
}

void EngineWrap::onMouseMove(int x, int y)
{
    m_engine.onMouseMove(x, y);
}

void EngineWrap::onMouseDown(MouseButton button, int x, int y)
{
    m_engine.onMouseDown(button, x, y);
}

void EngineWrap::onMouseUp(MouseButton button, int x, int y)
{
    m_engine.onMouseUp(button, x, y);
}

void EngineWrap::onMouseDoubleClick(MouseButton button, int x, int y)
{
    m_engine.onMouseDoubleClick(button, x, y);
}

void EngineWrap::onKeyDown(engine::Key key, const engine::ModifierState& modifierState)
{
    m_engine.onKeyDown(key, modifierState);
}

void EngineWrap::onKeyUp(engine::Key key, const engine::ModifierState& modifierState)
{
    m_engine.onKeyUp(key, modifierState);
}

void EngineWrap::resetCameraSize()
{
    m_engine.resetCameraSize();
}

void EngineWrap::playClicked(bool value)
{
    m_engine.playClicked(value);
}
