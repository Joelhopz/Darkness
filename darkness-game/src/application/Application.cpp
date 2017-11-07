#include "Application.h"
#include <random>

using namespace engine;
using namespace platform;

namespace application
{
    Application::Application()
        : m_window{ std::make_shared<platform::Window>("Darkness", ScreenWidth, ScreenHeight) }
        , m_engine{ m_window, "C:\\work\\darkness\\darkness-engine\\bin\\x64\\Release Vulkan\\shaders\\" }
    {
        m_window->setResizeCallback([&](int /*width*/, int /*height*/) { m_engine.refresh(); });

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
			std::mt19937 gen(456); //Standard mersenne_twister_engine seeded with rd()
			std::uniform_real_distribution<> dis(-10.0f, 10.0f);
            std::uniform_real_distribution<> disz(0.0f, 10.0f);
			std::uniform_real_distribution<> disscale(0.01f, 0.3f);
            std::uniform_real_distribution<> colorrandom(0.0f, 1.0f);
			for (int i = 0; i < 40; ++i)
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
                    "C:\\Users\\Aleksi Jokinen\\Documents\\TestDarknessProject\\processed\\primitives\\uvsphere.fbx", 0));

                if (i % 3 == 0)
                    meshNode->addComponent(std::make_unique<MaterialComponent>(
                        "C:\\Users\\Aleksi Jokinen\\Documents\\TestDarknessProject\\processed\\pbr\\bamboo\\bamboo-wood-semigloss-albedo.png",
                        "C:\\Users\\Aleksi Jokinen\\Documents\\TestDarknessProject\\processed\\pbr\\bamboo\\bamboo-wood-semigloss-roughness.png",
                        "C:\\Users\\Aleksi Jokinen\\Documents\\TestDarknessProject\\processed\\pbr\\bamboo\\bamboo-wood-semigloss-normal.png",
                        "C:\\Users\\Aleksi Jokinen\\Documents\\TestDarknessProject\\processed\\pbr\\bamboo\\bamboo-wood-semigloss-metal.png",
                        "C:\\Users\\Aleksi Jokinen\\Documents\\TestDarknessProject\\processed\\pbr\\bamboo\\bamboo-wood-semigloss-ao.png"
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
    }
}
