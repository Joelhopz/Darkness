#include "tools/Recompile.h"
#include "tools/StringTools.h"
#include "tools/PathTools.h"
#include "tools/Debug.h"
#include "platform/Platform.h"
#include "engine/graphics/ShaderSupport.h"

namespace engine
{
    int readFromPipe(HANDLE pipe, std::vector<char>& output)
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

    std::string fixedLength(int value, int digits = 3) {
        unsigned int uvalue;
        if (value < 0) {
            uvalue = -value;
        }
        else
        {
            uvalue = value;
        }
        std::string result;
        while (digits-- > 0) {
            result += ('0' + uvalue % 10);
            uvalue /= 10;
        }
        if (value < 0) {
            result += '-';
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

    std::string permutationName(const std::string& filename, int permutationId)
    {
        if (permutationId != -1)
        {
            auto ext = pathExtractExtension(filename);
            auto file = pathExtractFilenameWithoutExtension(filename);
            auto path = pathExtractFolder(filename);

            return pathJoin(path, file + "_" + fixedLength(permutationId) + "." + ext);
        }
        return filename;
    }

    std::string permutationName(int permutationId)
    {
        if (permutationId != -1)
        {
            return fixedLength(permutationId);
        }
        return "";
    }

    std::string recompile(const implementation::ShaderSupport& support, int permutationId, const std::vector<std::string>& defines)
    {
#ifdef UNICODE
        auto widePath = toWideString(support.executable);

        std::string defineStr = "";
        for (auto&& define : defines)
        {
            defineStr += " -D" + define;
        }

        auto binaryFileName = permutationName(support.binaryFile, permutationId);

        std::wstring wideParams;
        if (defines.size() > 0)
        {
            wideParams = toWideString(
                support.executable + " \"" +
                support.shaderCompilerPath + "\"" +
                " -g " + support.graphicsApi +
                " -i " + support.file +
                " -o " + binaryFileName +
                defineStr +
                "\"");
        }
        else
        {
            wideParams = toWideString(
                support.executable + " \"" +
                support.shaderCompilerPath + "\"" +
                " -g " + support.graphicsApi +
                " -i " + support.file +
                " -o " + binaryFileName +
                "\"");
        }
        auto wideRoot = toWideString(engine::pathExtractFolder(support.rootPath));

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

        std::vector<wchar_t> params(wideParams.size() + 1, 0);
        memcpy(&params[0], wideParams.data(), wideParams.size() * sizeof(wchar_t));
        bool res = CreateProcess(
            NULL,//widePath.data(),
            params.data(),
            NULL, NULL,
            TRUE,
            0,
            NULL,
            wideRoot.data(),
            &startupInfo,
            &processInfo
        );
        ASSERT(res, "Could not execute shader compilation process");

        std::string result = "";
        std::vector<char> buffer;
        DWORD waitResult = WAIT_TIMEOUT;
        while (waitResult != WAIT_OBJECT_0)
        {
            waitResult = WaitForSingleObject(processInfo.hProcess, 100);
            if (waitResult == WAIT_ABANDONED)
                break;

            int bytesRead = readFromPipe(readPipe, buffer);
            while (bytesRead > 0)
            {
                std::string soFar = std::string(buffer.data(), bytesRead);
                //LOG("%s", soFar.c_str());
                result += soFar;
                bytesRead = readFromPipe(readPipe, buffer);
            }
        }


        //if (processInfo.hProcess)
        //    WaitForSingleObject(processInfo.hProcess, INFINITE);
        CloseHandle(readPipe);
        CloseHandle(writePipe);
        CloseHandle(processInfo.hThread);
        CloseHandle(processInfo.hProcess);

        return result;
#else
#endif
    }
}
