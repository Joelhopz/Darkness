#include "platform/Environment.h"
#include <vector>

namespace engine
{

#ifdef _WIN32
#include <Windows.h>
std::string getExecutableDirectory()
{
#ifdef UNICODE
    std::vector<wchar_t> filename(2048, 0);
    int characters = GetModuleFileNameW(NULL, filename.data(), static_cast<DWORD>(filename.size()));
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, filename.data(), characters, NULL, 0, NULL, NULL);
    std::string result(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, filename.data(), characters, &result[0], sizeNeeded, NULL, NULL);
    return result;
#else
    std::vector<char> filename(2048, 0);
    int characters = GetModuleFileNameA(NULL, filename.data(), static_cast<DWORD>(filename.size()));
    std::string result(characters, 0);
    memcpy(&result[0], filename.data(), characters);
    return result;
#endif

}
#else
std::string getExecutableDirectory()
{
    std::vector<char> filename(2048, 0);
    char szTmp[32];
    sprintf(szTmp, "/proc/%d/exe", getpid());
    int bytes = MIN(readlink(szTmp, filename.data(), filename.size()), filename.size() - 1);
    if (bytes >= 0)
        filename[bytes] = '\0';
    std::string result(characters, 0);
    memcpy(&result[0], filename.data(), characters);
    return result;
}
#endif

}
