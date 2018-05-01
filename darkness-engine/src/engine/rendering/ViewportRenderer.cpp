#include "engine/rendering/ViewportRenderer.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/primitives/Vector2.h"
#include "engine/graphics/Common.h"

#include "components/Camera.h"

namespace engine
{
    ViewportRenderer::ViewportRenderer(
        Device& device, 
        ShaderStorage& shaderStorage,
        int width,
        int height)
        : m_device{ device }
        , m_depthPyramid{ device, shaderStorage, width, height }
        , m_rtv{ device.createTextureRTV(TextureDescription()
            .width(width)
            .height(height)
            .format(engine::Format::R16G16B16A16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("main render target")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f })
        ) }
        , m_srv{ device.createTextureSRV(m_rtv.texture()) }

        , m_modelRenderer(device, shaderStorage, Vector2<int>{ width, height })
        , m_modelTransparentRenderer(device, shaderStorage)
        , m_shadowRenderer(device, shaderStorage)
        , m_renderCubemap(device, shaderStorage)
        , m_postProcess(device, shaderStorage)
    {
    }

    void ViewportRenderer::render(
        CommandList& cmd, 
        FlatScene& scene, 
        TextureRTV& destination,
        unsigned int mouseX,
        unsigned int mouseY)
    {
        if ((scene.selectedCamera == -1) ||
            (scene.selectedCamera > scene.cameras.size() - 1) ||
            (!scene.cameras[scene.selectedCamera]))
            return;

        auto& camera = *scene.cameras[scene.selectedCamera];

        cmd.clearRenderTargetView(m_rtv, { 0.0f, 0.0f, 0.0f, 1.0f });

        {
            CPU_MARKER("Render shadows");
            GPU_MARKER(cmd, "Render shadows");
            if (scene.lightData->changeHappened())
                m_shadowRenderer.refresh();
            m_shadowRenderer.render(cmd, scene);
        }

        {
            CPU_MARKER("Render environment cubemap");
            GPU_MARKER(cmd, "Render environment cubemap");
            m_renderCubemap.render(m_modelRenderer.lightingTargetRTV(), camera, cmd);
        }

        {
            CPU_MARKER("Render models");
            GPU_MARKER(cmd, "Render models");
            m_modelRenderer.render(
                m_device,
                m_rtv, m_depthPyramid,
                cmd,
                camera,
                scene,
                m_shadowRenderer.shadowMap(), m_shadowRenderer.shadowVP(),
                mouseX, mouseY);
        }

        {
            CPU_MARKER("Render transparent models");
            GPU_MARKER(cmd, "Render transparent models");
            //renderTransparentModels(device, cmd, flatScene);
        }

        if(scene.postprocess)
        {
            CPU_MARKER("Render postprocess");
            GPU_MARKER(cmd, "Render postprocess");
            m_postProcess.render(destination, m_modelRenderer.finalFrame(), cmd, *scene.postprocess);
        }

    }

    void ViewportRenderer::refresh(engine::Vector2<int> virtualResolution)
    {
        m_rtv = m_device.createTextureRTV(TextureDescription()
            .width(virtualResolution.x)
            .height(virtualResolution.y)
            .format(engine::Format::R16G16B16A16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("main render target")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f })
        );
        m_srv = m_device.createTextureSRV(m_rtv.texture());
        m_depthPyramid.refresh(virtualResolution);
        m_modelRenderer.resize(static_cast<uint32_t>(virtualResolution.x), static_cast<uint32_t>(virtualResolution.y));
    }

    unsigned int ViewportRenderer::pickedObject(Device& device)
    {
        return m_modelRenderer.pickedObject(device);
    }

    void ViewportRenderer::setSelectedObject(int64_t object)
    {
        m_modelRenderer.setSelectedObject(object);
    }

    void ViewportRenderer::clearResources()
    {
        m_modelRenderer.clearResources();
        m_modelTransparentRenderer.clearResources();
        m_shadowRenderer.clearResources();
        m_renderCubemap.clearResources();
        m_postProcess.clearResources();
    }
}