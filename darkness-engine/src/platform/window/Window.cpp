#include "platform/window/Window.h"
#ifdef _WIN32
#include "platform/window/WindowsWindow.h"
#endif
#ifdef __APPLE__
#include "OsxWindow.h"
#endif

using namespace platform::implementation;

namespace platform
{
    PIMPL_ACCESS_IMPLEMENTATION(Window, WindowImpl)
    
    Window::Window(const implementation::WindowImpl& implementation)
        : m_impl{ tools::make_unique_impl<WindowImpl>(implementation) }
    {
    }

    Window::Window(WindowHandle handle, int width, int height)
        : m_impl{ tools::make_unique_impl<WindowImpl>(handle, width, height) }
    {}

    Window::Window(const char* windowName, int width, int height)
        : m_impl{ tools::make_unique_impl<WindowImpl>(windowName, width, height) }
    {}

    bool Window::processMessages()
    {
        return m_impl->processMessages();
    }

    void Window::setResizeCallback(ResizeCallback onResize)
    {
        m_impl->setResizeCallback(onResize);
    }

    int Window::width() const
    {
        return m_impl->width();
    }

    int Window::height() const
    {
        return m_impl->height();
    }

    void Window::resize(int width, int height)
    {
        m_impl->resize(width, height);
    }

    void Window::refreshSize()
    {
        m_impl->refreshSize();
    }
}
