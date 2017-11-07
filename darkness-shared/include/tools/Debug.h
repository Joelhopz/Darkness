#pragma once

void Debug(const char* location, const char* msg, ...);
void DebugInfo(const char* location, const char* msg, ...);
void DebugWarning(const char* location, const char* msg, ...);
void DebugError(const char* location, const char* msg, ...);

void DebugAssert(const char* condition, const char* location);
void DebugAssert(const char* condition, const char* location, const char* msg, ...);

#define DBGSTRINGIFY(x) #x
#define DBGTOSTRING(x) DBGSTRINGIFY(x)
#define DBGLOC __FILE__ "(" DBGTOSTRING(__LINE__) "): "


#define ASSERT(condition, ...) \
    (void)((condition) || (DebugAssert(#condition, DBGLOC, __VA_ARGS__),0))


#define LOG(...) Debug(DBGLOC, __VA_ARGS__)
#define LOG_INFO(...) DebugInfo(DBGLOC, __VA_ARGS__)
#define LOG_WARNING(...) DebugWarning(DBGLOC, __VA_ARGS__)
#define LOG_ERROR(...) DebugError(DBGLOC, __VA_ARGS__)
