#include "tools/Debug.h"

#include <string>
#include <vector>
#include <iostream>
#include <intrin.h>
#include "platform/Platform.h"

CustomDebugMessageHandler customDebugMessageHandler;

void DebugAssert(const char* condition, const char* location)
{
#if 1
    std::string smsg{ condition };
    std::string slocation{ location };

    std::string message = "####################### ASSERT!! ########################\n";
    message += "#\n";
    message += "# condition: ";
    message += smsg;
    message += "\n";
    message += slocation;
    message += "Error: ASSERT\n";
    message += "#\n";
    message += "####################### ASSERT!! ########################\n";
    message += "\n";
    message += "\n";

#ifdef _WIN32
    OutputDebugStringA(message.data());
#endif
    std::cout << message.data() << std::flush;
#endif

    if (customDebugMessageHandler)
        customDebugMessageHandler(message);

    __debugbreak();
    std::terminate();
}

void DebugAssert(const char* condition, const char* location, const char* msg, ...)
{
#if 1
    std::string smsg{ condition };
    std::string slocation{ location };

    std::vector<char> message(32768);
    va_list arguments;
    va_start(arguments, msg);
    vsprintf_s(message.data(), 32768, msg, arguments);
    va_end(arguments);

    std::vector<char> buffer(32768);
    sprintf_s(buffer.data(), 32768, "%s", message.data());

    std::string assertMessage = buffer.data();

    std::string assert_message = "####################### ASSERT!! ########################\n";
    assert_message += "#\n";
    assert_message += "# condition: ";
    assert_message += smsg;
    assert_message += "\n";
    assert_message += slocation;
    assert_message += "Error: ";
    assert_message += assertMessage;
    assert_message += "\n";
    assert_message += "#\n";
    assert_message += "####################### ASSERT!! ########################\n";
    assert_message += "\n";
    assert_message += "\n";

#ifdef _WIN32
    OutputDebugStringA(assert_message.data());
#endif
    std::cout << assert_message.data() << std::flush;
#endif

    if (customDebugMessageHandler)
        customDebugMessageHandler(assert_message);

    __debugbreak();
    std::terminate();
}

void Debug(const char* location, const char* msg, ...)
{
    std::vector<char> message(32768);
    va_list arguments;
    va_start(arguments, msg);
    vsprintf_s(message.data(), 32768, msg, arguments);
    va_end(arguments);

    std::vector<char> buffer(32768);
    sprintf_s(buffer.data(), 32768, "%s%s%s\n", location, "Log: ", message.data());

#ifdef _WIN32
    OutputDebugStringA(buffer.data());
#endif

    if (customDebugMessageHandler)
        customDebugMessageHandler(message.data());

    std::cout << buffer.data() << std::flush;
}

void DebugInfo(const char* location, const char* msg, ...)
{
    std::vector<char> message(32768);
    va_list arguments;
    va_start(arguments, msg);
    vsprintf_s(message.data(), 32768, msg, arguments);
    va_end(arguments);

    std::vector<char> buffer(32768);
    sprintf_s(buffer.data(), 32768, "%s%s%s\n", location, "Info: ", message.data());

#ifdef _WIN32
    OutputDebugStringA(buffer.data());
#endif

    if (customDebugMessageHandler)
        customDebugMessageHandler(buffer.data());

    std::cout << buffer.data() << std::flush;
}

void DebugWarning(const char* location, const char* msg, ...)
{
    std::vector<char> message(32768);
    va_list arguments;
    va_start(arguments, msg);
    vsprintf_s(message.data(), 32768, msg, arguments);
    va_end(arguments);

    std::vector<char> buffer(32768);
    sprintf_s(buffer.data(), 32768, "%s%s%s\n", location, "Warning: ", message.data());

#ifdef _WIN32
    OutputDebugStringA(buffer.data());
#endif

    if (customDebugMessageHandler)
        customDebugMessageHandler(buffer.data());

    std::cout << buffer.data() << std::flush;
}

void DebugError(const char* location, const char* msg, ...)
{
    std::vector<char> message(32768);
    va_list arguments;
    va_start(arguments, msg);
    vsprintf_s(message.data(), 32768, msg, arguments);
    va_end(arguments);

    std::vector<char> buffer(32768);
    sprintf_s(buffer.data(), 32768, "%s%s%s\n", location, "Error: ", message.data());

#ifdef _WIN32
    OutputDebugStringA(buffer.data());
#endif

    if (customDebugMessageHandler)
        customDebugMessageHandler(buffer.data());

    std::cout << buffer.data() << std::flush;
}

void DebugPure(const char* /*location*/, const char* msg, ...)
{
    std::vector<char> message(32768);
    va_list arguments;
    va_start(arguments, msg);
    vsprintf_s(message.data(), 32768, msg, arguments);
    va_end(arguments);

    std::vector<char> buffer(32768);
    sprintf_s(buffer.data(), 32768, "%s\n", message.data());

#ifdef _WIN32
    OutputDebugStringA(buffer.data());
#endif

    if (customDebugMessageHandler)
        customDebugMessageHandler(buffer.data());

    std::cout << buffer.data() << std::flush;
}

void DebugPureInfo(const char* /*location*/, const char* msg, ...)
{
    std::vector<char> message(32768);
    va_list arguments;
    va_start(arguments, msg);
    vsprintf_s(message.data(), 32768, msg, arguments);
    va_end(arguments);

    std::vector<char> buffer(32768);
    sprintf_s(buffer.data(), 32768, "%s\n", message.data());

#ifdef _WIN32
    OutputDebugStringA(buffer.data());
#endif

    if (customDebugMessageHandler)
        customDebugMessageHandler(buffer.data());

    std::cout << buffer.data() << std::flush;
}

void DebugPureWarning(const char* /*location*/, const char* msg, ...)
{
    std::vector<char> message(32768);
    va_list arguments;
    va_start(arguments, msg);
    vsprintf_s(message.data(), 32768, msg, arguments);
    va_end(arguments);

    std::vector<char> buffer(32768);
    sprintf_s(buffer.data(), 32768, "%s\n", message.data());

#ifdef _WIN32
    OutputDebugStringA(buffer.data());
#endif

    if (customDebugMessageHandler)
        customDebugMessageHandler(buffer.data());

    std::cout << buffer.data() << std::flush;
}

void DebugPureError(const char* /*location*/, const char* msg, ...)
{
    std::vector<char> message(32768);
    va_list arguments;
    va_start(arguments, msg);
    vsprintf_s(message.data(), 32768, msg, arguments);
    va_end(arguments);

    std::vector<char> buffer(32768);
    sprintf_s(buffer.data(), 32768, "%s\n", message.data());

#ifdef _WIN32
    OutputDebugStringA(buffer.data());
#endif

    if (customDebugMessageHandler)
        customDebugMessageHandler(buffer.data());

    std::cout << buffer.data() << std::flush;
}
