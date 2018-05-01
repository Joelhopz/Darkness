#include "gtest/gtest.h"
#include "GlobalTestFixture.h"

#ifdef _WIN32
#if 0
#include <Windows.h>
#include <stdio.h>
#include <shellapi.h>
int CALLBACK WinMain(
    _In_ HINSTANCE, // hInstance,
    _In_ HINSTANCE,  // hPrevInstance,
    _In_ LPSTR, //     lpCmdLine,
    _In_ int   //       nCmdShow
)
{
    ::testing::InitGoogleTest(&__argc, __argv);
    
    return RUN_ALL_TESTS();
}
#endif

int main(int argc, char* argv[])
{
    
    ::testing::InitGoogleTest(&argc, argv);
    envPtr = new GlobalEnvironment();
    ::testing::AddGlobalTestEnvironment(envPtr);

    return RUN_ALL_TESTS();
}


#endif
