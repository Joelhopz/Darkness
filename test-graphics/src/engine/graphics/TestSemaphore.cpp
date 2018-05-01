#include "GlobalTestFixture.h"

#include "platform/window/Window.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/Semaphore.h"
#include <memory>

using namespace std;
using namespace platform;
using namespace engine;

class TestSemaphore : public ::testing::Test
{
};

TEST(TestSemaphore, CreateSemaphore)
{
    GlobalEnvironment& env = *envPtr;
    Semaphore semaphore = env.device().createSemaphore();
    Semaphore copy = std::move(semaphore);
}
