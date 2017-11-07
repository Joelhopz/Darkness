#pragma once

#include <QString>
#include <QWidget>
#include "engine/Engine.h"
#include "engine/InputEvents.h"
#include <memory>
#include <string>

class EngineWrap
{
public:
    EngineWrap(WId windowId, int width, int height, const QString& shaderRootPath);
    void update();
    void refresh();

	void onMouseMove(int x, int y);
	void onMouseDown(engine::MouseButton button, int x, int y);
	void onMouseUp(engine::MouseButton button, int x, int y);
	void onMouseDoubleClick(engine::MouseButton button, int x, int y);

	void onKeyDown(engine::Key key, const engine::ModifierState& modifierState);
	void onKeyUp(engine::Key key, const engine::ModifierState& modifierState);

    void resetCameraSize();
    void playClicked(bool value);

    Engine& engine();
    ~EngineWrap();
private:
    std::string m_shaderRootPath;
    Engine m_engine;
};
