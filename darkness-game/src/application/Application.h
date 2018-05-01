#pragma once

#include "platform/window/Window.h"
#include "engine/Engine.h"
#include "platform/network/SocketServer.h"
#include <memory>

namespace application
{
    static int ScreenWidth = 1024;
    static int ScreenHeight = 768;
    class Application
    {
    public:
        Application();

    private:
        std::shared_ptr<platform::Window> m_window;
        Engine m_engine;
        //platform::SocketServer m_socketServer;
    };
}
