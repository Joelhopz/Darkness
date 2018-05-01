#include "GlobalTestFixture.h"

using namespace std;
using namespace platform;
using namespace engine;

TEST(TestTexture, TestClearRenderTargetTexture)
{
    GlobalEnvironment& env = *envPtr;

    // start drawing
    auto cmdBuffer = env.device().createCommandList();
    cmdBuffer.begin();
    cmdBuffer.clearTexture(
        env.currentRTV().texture(),
        Color4f(0.0f, 1.0f, 0.0f, 1.0f));
    cmdBuffer.end();

    env.submit(std::move(cmdBuffer));
    env.present();
    env.device().queue().waitForIdle();
}
