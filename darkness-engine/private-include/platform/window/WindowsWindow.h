#pragma once

#include "platform/window/Window.h"
#include "platform/Platform.h"

namespace platform
{
    namespace implementation
    {
        class WindowImpl
        {
        public:
            WindowImpl(HWND handle, int width, int height);
            WindowImpl(const char* windowName, int width, int height);
            WindowImpl(const WindowImpl& impl);
            ~WindowImpl();

            void setResizeCallback(ResizeCallback onResize);

            bool processMessages() const;
            int width() const;
            int height() const;
            void resize(int width, int height);
            const HWND handle() const;

            void refreshSize();

        public:
            void setHandle(HWND handle);
            LRESULT handleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);

        private:
            bool m_needCleanup;
            int m_width, m_height;
            HWND m_windowHandle;
            HINSTANCE m_hInstance;
            ResizeCallback m_onResize;
            bool m_createdFromHandle;
            void createWindow(const char* windowName, int width, int height);
        };
    }
}
