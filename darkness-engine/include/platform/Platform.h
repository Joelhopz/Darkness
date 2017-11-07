#pragma once

#ifdef _WIN32
#pragma warning(push, 0) 
#include <Windows.h>
#pragma warning(pop)
namespace platform
{
    typedef HWND WindowHandle;
}
#endif

#ifdef __APPLE__
namespace platform
{
    typedef void* WindowHandle;
}
#endif
