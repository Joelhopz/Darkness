#include "tools/Process.h"
#include "tools/Debug.h"
#include "tools/PathTools.h"
#include "platform/Platform.h"
#include <vector>

namespace engine
{
    Process::Process(
        const std::string& executable,
        const std::string& arguments,
        const std::string& workingDirectory,
        OnProcessMessage onMessage)
        : m_executable{ executable }
        , m_arguments{ arguments }
        , m_workingDirectory{ workingDirectory }
        , m_onMessage{ onMessage }
    {
        m_work = std::move(
            std::unique_ptr<std::thread, std::function<void(std::thread*)>>(
                new std::thread(
                // task
                [&]()
                {
                    run();
                }),

                // deleter
                [&](std::thread* ptr) {
                    ptr->join();
                    delete ptr;
                }));
    }

    int readFromProcessPipe(HANDLE pipe, std::vector<char>& output)
    {
        DWORD bytesRead = 0;
        DWORD availableBytes = 0;
        char tmp = 0;

        PeekNamedPipe(pipe, &tmp, 1, NULL, &availableBytes, NULL);
        if (availableBytes == 0)
            return bytesRead;

        output.resize(availableBytes);
        if (!ReadFile(pipe, output.data(), static_cast<DWORD>(output.size()), &bytesRead, NULL))
            ASSERT(false, "Could not read compilation output");
        return bytesRead;
    }

    void Process::run()
    {
        //auto widePath = toWideString(m_executable);
        //auto wideParameters = toWideString(m_arguments);
        auto wideWorkingDirectory = toWideString(m_workingDirectory);

        SECURITY_ATTRIBUTES securityAttributes = {};
        securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
        securityAttributes.bInheritHandle = TRUE;

        HANDLE readPipe, writePipe;
        CreatePipe(&readPipe, &writePipe, &securityAttributes, 0);

        PROCESS_INFORMATION processInfo = {};
        STARTUPINFO startupInfo = {};
        startupInfo.cb = sizeof(STARTUPINFO);
        startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
        startupInfo.wShowWindow = SW_HIDE;
        startupInfo.hStdOutput = writePipe;
        startupInfo.hStdError = writePipe;

        // for some reason having executable as part of arguments works best
        auto starting = toWideString(m_executable + " " + m_arguments);

        std::vector<wchar_t> params(starting.size() + 1, 0);
        memcpy(&params[0], starting.data(), starting.size() * sizeof(wchar_t));
        bool res = CreateProcess(
            NULL,//widePath.data(),
            params.data(),
            NULL, NULL,
            TRUE,
            0,
            NULL,
            wideWorkingDirectory.data(),
            &startupInfo,
            &processInfo
        );

        auto ut = toUtf8String(std::wstring(params.data()));
        ASSERT(res, "Failed to start process: %", ut.c_str());

        std::vector<char> buffer;
        DWORD waitResult = WAIT_TIMEOUT;
        while (waitResult != WAIT_OBJECT_0)
        {
            waitResult = WaitForSingleObject(processInfo.hProcess, 100);
            if (waitResult == WAIT_ABANDONED)
                break;

            int bytesRead = readFromProcessPipe(readPipe, buffer);
            while (bytesRead > 0)
            {
                std::string currentMessage = std::string(buffer.data(), bytesRead);
                //LOG("%s", soFar.c_str());
                if (m_onMessage)
                    m_onMessage(currentMessage);
                bytesRead = readFromProcessPipe(readPipe, buffer);
            }
        }

        //if (processInfo.hProcess)
        //    WaitForSingleObject(processInfo.hProcess, INFINITE);
        CloseHandle(readPipe);
        CloseHandle(writePipe);
        CloseHandle(processInfo.hThread);
        CloseHandle(processInfo.hProcess);
    }
}
