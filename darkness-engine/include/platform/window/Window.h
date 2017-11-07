#pragma once

#include "tools/SmartPimpl.h"
#include "platform/Platform.h"
#include <functional>

namespace platform
{
    using ResizeCallback = std::function<void(int, int)>;

    PIMPL_FWD(Window, WindowImpl)
    class Window
    {
    public:
        Window(const implementation::WindowImpl& implementation);
        Window(WindowHandle handle, int width, int height);
        Window(const char* windowName, int width, int height);
        bool processMessages();
        void setResizeCallback(ResizeCallback onResize);

        int width() const;
        int height() const;
        void resize(int width, int height);
        void refreshSize();

        PIMPL_FRIEND_ACCESS(WindowImpl)
    };
}
