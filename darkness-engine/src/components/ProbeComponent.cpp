#include "components/ProbeComponent.h"
#include "components/Camera.h"
#include "components/PostprocessComponent.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/CommandList.h"
#include "engine/graphics/Viewport.h"
#include "engine/graphics/Rect.h"
#include "engine/rendering/ViewportRenderer.h"
#include <algorithm>

namespace engine
{
    ProbeComponent::ProbeComponent()
        : size{ this, "Size", 1024 }
        , m_range{ this, "Range", 5.0f }
        , m_updateNow{ false }
        , updateButton{ this, "Update", ButtonPush{ ButtonPush::NotPressed }, [this]()
        {
            if (this->updateButton.value<ButtonPush>() == ButtonPush::Pressed)
                this->m_updateNow = true;
        } }
    {
        m_name = "Probe";
    };

    void ProbeComponent::initialize()
    {
        //m_transform = std::make_shared<Transform>();
        m_transform = getComponent<Transform>();
        m_camera = std::make_shared<Camera>(m_transform);
        m_postprocess = std::make_shared<PostprocessComponent>();

        m_camera->nearPlane(0.1f);
        m_camera->farPlane(1000.0f);
        m_camera->fieldOfView(90.0f);
        m_camera->projection(Projection::Perspective);
        m_camera->pbrShadingModel(true);
        m_postprocess->bloomEnabled(true);
    }

    std::shared_ptr<engine::EngineComponent> ProbeComponent::clone() const
    {
        auto res = std::make_shared<ProbeComponent>();
        return res;
    };

    static const char* pass_msg[6] = { "Pass POSX", "Pass NEGX", "Pass POSY", "Pass NEGY", "Pass POSZ", "Pass NEGZ" };

    Vector3f ProbeComponent::position()
    {
        if (!m_transform)
            initialize();
        return m_transform->position();
    }

    float ProbeComponent::range() const
    {
        return m_range.value<float>();
    }

    CubePass::CubePass(Device& device, int width, int height)
    {
        m_cubemap = device.createTextureSRV(TextureDescription()
            .format(Format::R16G16B16A16_FLOAT)
            .width(width)
            .height(height)
            .arraySlices(6)
            .dimension(ResourceDimension::TextureCubemap)
            .name("Probe cubemap")
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .optimizedClearValue(Float4(0.0f, 0.0f, 0.0f, 0.0f)));

        // create render targets
        m_rtvs[0] = device.createTextureRTV(m_cubemap.texture(), m_cubemap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ 0, 1, 0, 1 });
        m_rtvs[1] = device.createTextureRTV(m_cubemap.texture(), m_cubemap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ 0, 1, 1, 1 });
        m_rtvs[2] = device.createTextureRTV(m_cubemap.texture(), m_cubemap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ 0, 1, 2, 1 });
        m_rtvs[3] = device.createTextureRTV(m_cubemap.texture(), m_cubemap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ 0, 1, 3, 1 });
        m_rtvs[4] = device.createTextureRTV(m_cubemap.texture(), m_cubemap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ 0, 1, 4, 1 });
        m_rtvs[5] = device.createTextureRTV(m_cubemap.texture(), m_cubemap.texture().description().dimension(ResourceDimension::Texture2DArray), SubResource{ 0, 1, 5, 1 });
    }

    void CubePass::render(
        CommandList& cmd, 
        ViewportRenderer& viewportRenderer,
        Camera& camera,
        FlatScene& scene)
    {
        cmd.clearTexture(m_cubemap.texture());
        Matrix4f m_cameraRotations[6];
        m_cameraRotations[0] = camera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f));
        m_cameraRotations[1] = camera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(-1.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f));
        m_cameraRotations[2] = camera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f));
        m_cameraRotations[3] = camera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, -1.0f, 0.0f), Vector3f(0.0f, 0.0f, -1.0f));
        m_cameraRotations[4] = camera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, -1.0f), Vector3f(0.0f, 1.0f, 0.0f));
        m_cameraRotations[5] = camera.lookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f), Vector3f(0.0f, 1.0f, 0.0f));

        // render cubemap
        for (int i = 0; i < 6; ++i)
        {
            CPU_MARKER(pass_msg[i]);
            GPU_MARKER(cmd, pass_msg[i]);
            //cmd.clearRenderTargetView(m_rtvs[i], { 0.0f, 0.0f, 0.0f, 0.0f });
            camera.rotation(Quaternionf::fromMatrix(m_cameraRotations[i]));
            viewportRenderer.render(cmd, scene, m_rtvs[i], 0, 0);
        }
    }

    TextureSRV& ProbeComponent::cubemap()
    {
        return m_secondPass->cubemap();
    }

    TextureSRV& ProbeComponent::brdf()
    {
        return m_camera->environmentBrdfLUT();
    }

    TextureSRV& ProbeComponent::irradiance()
    {
        return m_camera->environmentIrradiance();
    }

    TextureSRV& ProbeComponent::specular()
    {
        return m_camera->environmentSpecular();
    }

    bool ProbeComponent::update(Device& device, ShaderStorage& shaderStorage, CommandList& cmd, FlatScene& scene)
    {
        if (!m_updateNow)
            return false;
        m_updateNow = false;

        CPU_MARKER("Probe render");
        GPU_MARKER(cmd, "Probe render");

        auto width = size.value<int>();
        auto height = size.value<int>();

        if (!m_transform)
            initialize();

        m_camera->width(width);
        m_camera->height(height);

        if (!m_transform)
        {
            m_transform = getComponent<Transform>();
        }

        if (!m_viewportRenderer)
            m_viewportRenderer = std::make_shared<ViewportRenderer>(device, shaderStorage, width, height);

        // create passes
        if (m_lastWidth != width || m_lastHeight != height)
        {
            m_firstPass = std::make_unique<CubePass>(device, width, height);
            m_secondPass = std::make_unique<CubePass>(device, width, height);
            m_lastWidth = width;
            m_lastHeight = height;
        }

        auto prevSelectedCamera = scene.selectedCamera;
        auto prevPostprocess = scene.postprocess;
        scene.postprocess = m_postprocess;
        scene.cameras.emplace_back(m_camera);
        scene.selectedCamera = static_cast<int>(scene.cameras.size() - 1);
        m_camera->position(m_transform->position());
        
        LOG("probe camera pos: [%f, %f, %f]",
            m_camera->position().x,
            m_camera->position().y,
            m_camera->position().z);

        Camera& cam = *scene.cameras[scene.selectedCamera];
        cam.environmentMapPath("");
        cam.environmentMap(TextureSRV());
        m_firstPass->render(cmd, *m_viewportRenderer, *m_camera, scene);

        cam.environmentMap(m_firstPass->cubemap());
        m_viewportRenderer->cubemapRenderer().cubemap(cam.environmentMap());
        m_viewportRenderer->cubemapRenderer().createIrradianceCubemap(device, cam.environmentMap(), cmd);
        m_viewportRenderer->cubemapRenderer().prefilterConvolution(device, cam.environmentMap(), cmd);
        m_viewportRenderer->cubemapRenderer().brdfConvolution(cmd);

        /*m_secondPass->render(cmd, *m_viewportRenderer, *m_camera, scene);
        cam.environmentMap(m_secondPass->cubemap());
        m_viewportRenderer->cubemapRenderer().cubemap(cam.environmentMap());
        m_viewportRenderer->cubemapRenderer().createIrradianceCubemap(device, cam.environmentMap(), cmd);
        m_viewportRenderer->cubemapRenderer().prefilterConvolution(device, cam.environmentMap(), cmd);
        m_viewportRenderer->cubemapRenderer().brdfConvolution(cmd);*/
        
        cam.environmentBrdfLUT(m_viewportRenderer->cubemapRenderer().brdfConvolution());
        cam.environmentIrradiance(m_viewportRenderer->cubemapRenderer().irradiance());
        cam.environmentSpecular(m_viewportRenderer->cubemapRenderer().prefilteredEnvironmentMap());

        scene.selectedCamera = prevSelectedCamera;
        scene.cameras.erase(scene.cameras.end() - 1);
        scene.postprocess = prevPostprocess;
        return true;
    };
}
