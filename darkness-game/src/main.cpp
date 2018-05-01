#include "platform/Platform.h"
#include "application/Application.h"
#include "plugins/PluginManager.h"

using namespace application;
using namespace engine;

#ifdef _WIN32
int CALLBACK WinMain(
    _In_ HINSTANCE, // hInstance,
    _In_ HINSTANCE,  // hPrevInstance,
    _In_ LPSTR, //     lpCmdLine,
    _In_ int   //       nCmdShow
    )
{
    Application app;
    return 0;
}
#endif
