#include "GlobalTestFixture.h"
#include "tools/image/Image.h"
#include "tools/image/Color.h"
#include "tools/PathTools.h"

GlobalEnvironment* envPtr;

namespace engine
{
    template <>
    std::shared_ptr<Drawer> createDrawer<int>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<float>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<double>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<Vector2f>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<Vector3f>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<Vector4f>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<Matrix3f>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<Matrix4f>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<Projection>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<CollisionShape>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<std::string>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<engine::Quaternionf>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<LightType>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<engine::ButtonToggle>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<engine::ButtonPush>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<bool>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<char>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<short>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<unsigned char>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<unsigned short>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<unsigned int>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }
}

engine::InputManager& GlobalEnvironment::inputManager()
{
    return *m_inputManager;
}

void GlobalEnvironment::SetUp()
{
    m_env = std::make_shared<RenderSetup>();
    m_inputManager = std::make_shared<engine::InputManager>(m_env->device().width(), m_env->device().height());
}

void GlobalEnvironment::TearDown()
{
    m_inputManager = nullptr;
    m_env = nullptr;
}

Device& GlobalEnvironment::device()
{
    return m_env->device();
}

ShaderStorage& GlobalEnvironment::shaderStorage()
{
    return m_env->shaderStorage();
}

SwapChain& GlobalEnvironment::swapChain()
{
    return m_env->swapChain();
}

Window& GlobalEnvironment::window()
{
    return m_env->window();
}

unsigned int GlobalEnvironment::currentBackBufferIndex()
{
    return m_env->currentBackBufferIndex();
}

TextureRTV& GlobalEnvironment::currentRTV()
{
    return m_env->currentRTV();
}

void GlobalEnvironment::submit(engine::CommandList&& commandList)
{
    m_env->submit(std::move(commandList));
}

void GlobalEnvironment::present()
{
    m_env->present();
    m_env->window().processMessages();
    m_inputManager->update();
}

bool GlobalEnvironment::canContinue(bool defaultValue)
{
    if (!hasBeenChecked(testName()))
    {
        checkReference(testName());
    }
    return defaultValue;
}

std::string GlobalEnvironment::testName() const
{
    return ::testing::UnitTest::GetInstance()->current_test_info()->name();
}

void GlobalEnvironment::checkReference(const std::string& testName)
{
    m_referencesChecked.emplace_back(testName);

    // do we have the reference ?
    auto reference = image::Image::createImage(pathJoin(getWorkingDirectory(), pathJoin(std::string(ReferenceLocation), testName + ".bmp")), image::ImageType::BMP);
    if (!reference)
    {
        LOG_ERROR("TEST MISSING REFERENCE: %s", testName.c_str());
    }

    // capture current frame
    TextureRTV& rtv = m_env->previousRTV();
    TextureSRV srv = device().createTextureSRV(rtv.texture());
    BufferSRV frame = device().createBufferSRV(engine::BufferDescription()
        .format(engine::Format::R8G8B8A8_UINT)
        .elementSize(engine::formatBytes(engine::Format::R8G8B8A8_UINT))
        .elements(rtv.width() * rtv.height())
        .name("Frame capture")
        .usage(engine::ResourceUsage::GpuToCpu)
    );

    CommandList cmd = device().createCommandList();
    cmd.copyTexture(srv, frame);
    submit(std::move(cmd));

    auto ptr = frame.buffer().map(device());

    bool writeFailed = false;
    if (reference)
    {
        reference->flipVertical();
        reference->convert();
        auto ref = reference->map(0, 0);
        
        auto rms = computeRms(static_cast<uint8_t*>(ptr), ref.data, rtv.width(), rtv.height());
        if (rms > MaximumAcceptableRms)
        {
            writeFailed = true;
        }
    }
    else
    {
        writeFailed = true;
    }

    if(writeFailed)
    {
        auto failed = image::Image::createImage(pathJoin(std::string(FailedLocation), testName + ".bmp"), image::ImageType::BMP, engine::Format::R8G8B8A8_UINT, rtv.width(), rtv.height());
        failed->reserve();
        auto fail = failed->map(0, 0);
        memcpy(const_cast<uint8_t*>(fail.data), ptr, formatBytes(engine::Format::R8G8B8A8_UINT, rtv.width(), rtv.height()));
        failed->flipVertical();
        failed->convert();
        failed->save();
    }
}

bool GlobalEnvironment::hasBeenChecked(const std::string& testName) const
{
    return std::find(m_referencesChecked.begin(), m_referencesChecked.end(), testName) != m_referencesChecked.end();
}

float GlobalEnvironment::computeRms(const uint8_t* reference, const uint8_t* current, uint32_t width, uint32_t height)
{
    float rms = 0.0f;
    auto ref = reinterpret_cast<const unsigned int*>(reference);
    auto cur = reinterpret_cast<const unsigned int*>(current);
    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            bmp::Color r(*ref);
            bmp::Color c(*cur);

            auto dif = (static_cast<float>(r.red()) / 255.0f) - (static_cast<float>(c.red()) / 255.0f);
            rms += dif * dif;

            dif = (static_cast<float>(r.green()) / 255.0f) - (static_cast<float>(c.green()) / 255.0f);
            rms += dif * dif;

            dif = (static_cast<float>(r.blue()) / 255.0f) - (static_cast<float>(c.blue()) / 255.0f);
            rms += dif * dif;

            ++ref;
            ++cur;
        }
    }
    return rms / (width * height * 3);
}

void GlobalEnvironment::saveRTVToFile(const std::string& filePath, TextureRTV& rtv)
{
    // capture current frame
    TextureSRV srv = device().createTextureSRV(rtv.texture());
    BufferSRV frame = device().createBufferSRV(engine::BufferDescription()
        .format(engine::Format::R8G8B8A8_UINT)
        .elementSize(engine::formatBytes(engine::Format::R8G8B8A8_UINT))
        .elements(rtv.width() * rtv.height())
        .name("Frame capture")
        .usage(engine::ResourceUsage::GpuToCpu)
    );

    CommandList cmd = device().createCommandList();
    cmd.copyTexture(srv, frame);
    submit(std::move(cmd));

    auto ptr = frame.buffer().map(device());

    auto failed = image::Image::createImage(filePath, image::ImageType::BMP, engine::Format::R8G8B8A8_UINT, rtv.width(), rtv.height());
    failed->reserve();
    auto fail = failed->map(0, 0);
    memcpy(const_cast<uint8_t*>(fail.data), ptr, formatBytes(engine::Format::R8G8B8A8_UINT, rtv.width(), rtv.height()));
    failed->flipVertical();
    failed->convert();
    failed->save();
}
