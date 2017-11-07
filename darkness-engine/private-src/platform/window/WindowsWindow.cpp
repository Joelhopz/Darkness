#include "platform/window/WindowsWindow.h"
#include "tools/Debug.h"
#ifdef _UNICODE
#include <cstdlib>
#endif

namespace platform
{
    namespace implementation
    {
        LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

        WindowImpl::WindowImpl(HWND handle, int width, int height)
            : m_windowHandle{ handle }
            , m_needCleanup{ false }
            , m_width(width)
            , m_height(height)
            , m_onResize{ nullptr }
            , m_createdFromHandle{ true }
        {
        }

        WindowImpl::WindowImpl(const WindowImpl& impl)
        {
            ASSERT(!impl.m_needCleanup);
            m_windowHandle = impl.m_windowHandle;
            m_width = impl.m_width;
            m_height = impl.m_height;
            m_needCleanup = impl.m_needCleanup;
            m_createdFromHandle = impl.m_createdFromHandle;
        }

        WindowImpl::WindowImpl(const char* windowName, int width, int height)
            : m_width(width)
            , m_height(height)
            , m_needCleanup{ true }
            , m_createdFromHandle{ false }
        {
            createWindow(windowName, width, height);
        }

        void WindowImpl::createWindow(const char* windowName, int width, int height)
        {
            #ifdef _UNICODE
            static TCHAR szAppName[1024] = {};
            size_t numCharacters;
            mbstowcs_s(&numCharacters, szAppName, windowName, 1024);
            #else
            TCHAR szAppName[1024] = {};
            strncpy_s(szAppName, windowName, strlen(windowName));
            #endif
            WNDCLASS     wndclass;
            m_hInstance = GetModuleHandle(NULL);

            wndclass.style = CS_HREDRAW | CS_VREDRAW;
            wndclass.lpfnWndProc = WndProc;
            wndclass.cbClsExtra = 0;
            wndclass.cbWndExtra = 0;
            wndclass.hInstance = m_hInstance;
            wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
            wndclass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
            wndclass.lpszMenuName = NULL;
            wndclass.lpszClassName = szAppName;

            if (!RegisterClass(&wndclass))
            {
                MessageBox(NULL, TEXT("This program requires Windows NT!"),
                    szAppName, MB_ICONERROR);
            }

            m_windowHandle = CreateWindowEx(
                0,
                szAppName,
                szAppName,
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT,
                CW_USEDEFAULT, CW_USEDEFAULT,
                NULL,
                NULL,
                m_hInstance,
                this);

            /*m_windowHandle = CreateWindow(szAppName, szAppName,
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT,
                CW_USEDEFAULT, CW_USEDEFAULT,
                NULL, NULL, m_hInstance, NULL);*/

            ShowWindow(m_windowHandle, SW_SHOW);
            SetWindowPos(m_windowHandle, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
            UpdateWindow(m_windowHandle);
        }

        WindowImpl::~WindowImpl()
        {
            if (m_needCleanup)
            {
                DestroyWindow(m_windowHandle);
                m_windowHandle = NULL;

                UnregisterClassA("Darkness", m_hInstance);
                m_hInstance = NULL;
            }
        }

        const HWND WindowImpl::handle() const
        {
            return m_windowHandle;
        }

        void WindowImpl::setHandle(HWND handle)
        {
            m_windowHandle = handle;
        }

        LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
        {
            WindowImpl* windowInstance = nullptr;
            if (message == WM_NCCREATE)
            {
                LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
                windowInstance = reinterpret_cast<WindowImpl*>(lpcs->lpCreateParams);
                windowInstance->setHandle(hwnd);
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(windowInstance));
            }
            else
            {
                windowInstance = reinterpret_cast<WindowImpl*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            }

            if (windowInstance)
            {
                return windowInstance->handleMsg(message, wParam, lParam);
            }
            else
            {
                return DefWindowProc(hwnd, message, wParam, lParam);
            }
        }

        void WindowImpl::refreshSize()
        {
            RECT clientRect = {};
            GetClientRect(m_windowHandle, &clientRect);
            m_width = clientRect.right - clientRect.left;
            m_height = clientRect.bottom - clientRect.top;
        }

        LRESULT WindowImpl::handleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            switch (uMsg)
            {
                case WM_DESTROY:
                {
                    PostQuitMessage(0);
                    return 0;
                }
                case WM_EXITSIZEMOVE:
                {
                    if (m_onResize)
                    {
                        refreshSize();
                        m_onResize(m_width, m_height);
                    }
                }
            }
            return DefWindowProc(m_windowHandle, uMsg, wParam, lParam);
        }

        bool WindowImpl::processMessages() const
        {
            MSG msg;
            if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    return false;
                }
                
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            return true;
        }

        void WindowImpl::setResizeCallback(ResizeCallback onResize)
        {
            m_onResize = onResize;
        }

        int WindowImpl::width() const
        {
            return m_width;
        }

        int WindowImpl::height() const
        {
            return m_height;
        }

        void WindowImpl::resize(int width, int height)
        {
            m_width = width;
            m_height = height;
            if (!m_createdFromHandle)
            {
                SetWindowPos(m_windowHandle, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
            }
        }
    }
}
