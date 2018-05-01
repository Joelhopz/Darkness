#pragma once

#include "gtest/gtest.h"

#include "tools/image/Image.h"
#include "platform/window/Window.h"
#include "engine/graphics/Graphics.h"
#include "platform/Environment.h"
#include "tools/StringTools.h"
#include "tools/PathTools.h"
#include "engine/Engine.h"
#include "engine/Drawer.h"
#include "engine/input/InputManager.h"
#include <chrono>
#include <memory>
#include <vector>
#include <algorithm>

using namespace std;
using namespace platform;
using namespace engine;
using namespace shaders;

constexpr const char* DataLocation = "..\\..\\data";
constexpr const char* ReferenceLocation = "..\\..\\data\\reference";
constexpr const char* FailedLocation = "..\\..\\data\\failed";
constexpr float MaximumAcceptableRms = 0.0001f;

namespace engine
{
    class DrawerAbs : public Drawer
    {
    public:
        virtual ~DrawerAbs() {};
        virtual void* native() { return nullptr; };
        virtual void setParent(void* /*parent*/) {};
    };
}

class GlobalEnvironment : public ::testing::Environment
{
public:
    void SetUp() override;
    void TearDown() override;

    Device& device();
    ShaderStorage& shaderStorage();
    SwapChain& swapChain();
    Window& window();
    unsigned int currentBackBufferIndex();
    TextureRTV& currentRTV();
    void submit(engine::CommandList&& commandList);
    void present();
    bool canContinue(bool defaultValue = false);
    engine::InputManager& inputManager();
private:
    std::shared_ptr<RenderSetup> m_env;
    std::shared_ptr<engine::InputManager> m_inputManager;
    std::vector<std::string> m_referencesChecked;

    std::string testName() const;
    void checkReference(const std::string& testName);
    bool hasBeenChecked(const std::string& testName) const;
    float computeRms(const uint8_t* reference, const uint8_t* current, uint32_t width, uint32_t height);

    void saveRTVToFile(const std::string& filePath, TextureRTV& rtv);
};

extern GlobalEnvironment* envPtr;
