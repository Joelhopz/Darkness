#include "platform/Uuid.h"
#include "tools/Debug.h"
#include "tools/StringTools.h"

#ifdef _WIN32
#include "Rpc.h"
#pragma comment(lib, "Rpcrt4.lib")
#endif

using namespace std;

namespace platform
{
    string uuid()
    {
        string result;
#ifdef _WIN32
#ifndef _UNICODE
        UUID uid;
        auto status = UuidCreate(&uid);
        ASSERT(status == RPC_S_OK, "Failed to create UUID");

        char* str;
        auto res = UuidToStringA(&uid, reinterpret_cast<RPC_CSTR*>(&str));
        ASSERT(res == RPC_S_OK, "Failed to create string from UUID");
        result = reinterpret_cast<const char*>(str);

        RpcStringFreeA(reinterpret_cast<RPC_CSTR*>(&str));
#else
        UUID uid;
        auto status = UuidCreate(&uid);
        ASSERT(status == RPC_S_OK, "Failed to create UUID");

        wchar_t* str;
        auto res = UuidToStringW(&uid, reinterpret_cast<RPC_WSTR*>(&str));
        ASSERT(res == RPC_S_OK, "Failed to create string from UUID");
        wstring wideRes = reinterpret_cast<const wchar_t*>(str);
        result = engine::toUtf8String(wideRes);

        RpcStringFreeW(reinterpret_cast<RPC_WSTR*>(&str));
#endif
#endif
        return result;
    }
}
