#include "Application.h"
#include <chrono>
#include <random>

using namespace engine;
using namespace platform;

namespace application
{
    Application::Application()
        : m_window{ std::make_shared<platform::Window>("Darkness", ScreenWidth, ScreenHeight) }
        , m_engine{ m_window, "" }
        //, m_socketServer{ 12345, [](std::shared_ptr<Socket> socket) {}, [](std::shared_ptr<Socket> socket) {} }
    {
        m_window->setResizeCallback([&](int /*width*/, int /*height*/) { m_engine.refresh(); });

#if 1
        m_engine.scene().loadFrom("C:\\Users\\Aleksi Jokinen\\Documents\\TestDarknessProject\\content\\scenes2\\substance_test2");

        float x = 0.0f;
        float y = .0f;
        float z = .0f;

        std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();
        bool res = true;
        while (res)
        {
            res = m_window->processMessages();
            m_engine.update();

            /*m_engine.scene().root()->child(1)->getComponent<Transform>()->rotation(Quaternionf::fromEulerAngles(x, y, z));

            auto now = std::chrono::high_resolution_clock::now();
            auto duration = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(now - last).count()) / 1000000.0;
            last = now;

            y += static_cast<float>(duration / 30.0);
            if (y > 360.0f)
                y -= 360.0f;*/
        }
        m_engine.shutdown();
#else
        {
            auto cameraNode = std::make_shared<engine::SceneNode>();
            auto cameraTransform = std::make_shared<engine::Transform>();
            cameraNode->name("Camera");
            cameraNode->addComponent(cameraTransform);
            cameraNode->addComponent(std::make_shared<engine::Camera>());
            cameraNode->addComponent(std::make_shared<engine::PostprocessComponent>());
            cameraNode->getComponent<Camera>()->pbrShadingModel(true);
            cameraNode->getComponent<Camera>()->exposure(1.0f);
            m_engine.scene().root()->addChild(cameraNode);
            cameraTransform->position({ 0.0f, 0.0f, 20.0f });
        }

        auto lightNode = std::make_shared<engine::SceneNode>();
        {
            
            lightNode->name("Point Light");
            lightNode->addComponent(std::make_shared<engine::Transform>());
            lightNode->getComponent<Transform>()->position({ 0.0f, 12.0f, 0.0f });
            lightNode->addComponent(std::make_shared<engine::LightComponent>());
            lightNode->getComponent<LightComponent>()->shadowCaster(true);
            lightNode->getComponent<LightComponent>()->intensity(10.0f);
            m_engine.scene().root()->addChild(lightNode);
        }

        std::vector<std::shared_ptr<engine::SceneNode>> meshNodes;
        {
            std::mt19937 gen(456);
            std::uniform_real_distribution<> dis(-10.0f, 10.0f);
            std::uniform_real_distribution<> disz(0.0f, 10.0f);
            std::uniform_real_distribution<> disscale(0.01f, 0.3f);
            std::uniform_real_distribution<> colorrandom(0.0f, 1.0f);
            for (int i = 0; i < 100; ++i)
            {
                auto meshNode = std::make_shared<engine::SceneNode>();
                auto transform = std::make_shared<engine::Transform>();

                transform->position({ (float)dis(gen), (float)dis(gen), (float)disz(gen) });
                float scale = (float)disscale(gen);
                transform->scale({ scale, scale, scale });

                meshNode->name("Mesh Renderer");
                meshNode->addComponent(transform);
                if(i % 2 == 0)
                    meshNode->addComponent(std::make_shared<engine::MeshRendererComponent>(
                        "C:\\Users\\Aleksi Jokinen\\Documents\\TestDarknessProject\\processed\\primitives\\cube.fbx", 0));
                else
                    meshNode->addComponent(std::make_shared<engine::MeshRendererComponent>(
                    "C:\\Users\\Aleksi Jokinen\\Documents\\TestDarknessProject\\processed\\primitives\\uv_sphere_highpoly.fbx", 0));

                if (i % 3 == 0)
                    meshNode->addComponent(std::make_unique<MaterialComponent>(
                        "C:\\Users\\Aleksi Jokinen\\Documents\\TestDarknessProject\\processed\\pbr\\bamboo\\bamboo-wood-semigloss-albedo.png",
                        "C:\\Users\\Aleksi Jokinen\\Documents\\TestDarknessProject\\processed\\pbr\\bamboo\\bamboo-wood-semigloss-roughness.png",
                        "C:\\Users\\Aleksi Jokinen\\Documents\\TestDarknessProject\\processed\\pbr\\bamboo\\bamboo-wood-semigloss-normal.png",
                        "C:\\Users\\Aleksi Jokinen\\Documents\\TestDarknessProject\\processed\\pbr\\bamboo\\bamboo-wood-semigloss-metal.png",
                        "C:\\Users\\Aleksi Jokinen\\Documents\\TestDarknessProject\\processed\\pbr\\bamboo\\bamboo-wood-semigloss-ao.png",
                        0, 0, 0, 0, 0
                        ));
                else
                    meshNode->addComponent(std::make_unique<MaterialComponent>());
                meshNode->getComponent<MaterialComponent>()->color({ (float)colorrandom(gen), (float)colorrandom(gen), (float)colorrandom(gen) });
                m_engine.scene().root()->addChild(meshNode);
                meshNodes.emplace_back(meshNode);
            }

        }

        bool res = true;
        float x = -90.0f;
        float y = .0f;
        float z = .0f;
        while (res)
        {
            res = m_window->processMessages();
            m_engine.update();

            for (int i = 0; i < meshNodes.size(); ++i)
            {
                meshNodes[i]->getComponent<Transform>()->rotation(Quaternionf::fromEulerAngles(x, y, z));
            }
            z += 0.6f;
            if (z > 360.0f)
                z -= 360.0f;

            y += 0.8f;
            if (y > 360.0f)
                y -= 360.0f;

            lightNode->getComponent<Transform>()->position({ y, 12.0f, 0.0f + z });
        }
        m_engine.shutdown();
        meshNodes.clear();
#endif
    }
}
