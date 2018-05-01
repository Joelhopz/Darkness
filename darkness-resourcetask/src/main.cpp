#include "platform/Platform.h"
#include "tools/ArgParser.h"
#include "ResourceTask.h"
#include "tools/Debug.h"

using namespace resource_task;

#ifdef _WIN32
int CALLBACK WinMain(
    _In_ HINSTANCE, // hInstance,
    _In_ HINSTANCE,  // hPrevInstance,
    _In_ LPSTR, //     lpCmdLine,
    _In_ int   //       nCmdShow
)
{
    LOG_INFO("Resource task started");
    engine::ArgParser argParser(__argc, __argv);
    ResourceTask task(
        argParser.value("ip"), 
        std::stoi(argParser.value("port")),
        argParser.value("id"),
        argParser.value("hostid"));
    return task.join();
}
#endif
