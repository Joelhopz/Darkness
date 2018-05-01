#pragma once

#include "engine/rendering/ModelRenderer.h"
#include "engine/rendering/ModelTransparentRenderer.h"
#include "engine/rendering/ShadowRenderer.h"
#include "engine/rendering/RenderCubemap.h"
#include "engine/rendering/Postprocess.h"
#include "engine/rendering/DepthPyramid.h"

#include "engine/rendering/LightData.h"

namespace engine
{
    class Device;
    class ShaderStorage;
    class Camera;
    class CommandList;
    struct FlatScene;

    class ViewportRenderer
    {
    public:
        ViewportRenderer(
            Device& device, 
            ShaderStorage& shaderStorage,
            int width,
            int height);

        void render(
            CommandList& cmd, 
            FlatScene& scene, 
            TextureRTV& destination,
            unsigned int mouseX,
            unsigned int mouseY);

        unsigned int pickedObject(Device& device);
        void setSelectedObject(int64_t object);

        TextureDSV& dsv()
        {
            return m_depthPyramid.dsv();
        }

        void clearResources();

        RenderCubemap& cubemapRenderer()
        {
            return m_renderCubemap;
        }

        void refresh(engine::Vector2<int> virtualResolution);
    private:
        Device& m_device;

        TextureRTV m_rtv;
        TextureSRV m_srv;

        DepthPyramid m_depthPyramid;
        ModelRenderer m_modelRenderer;
        ModelTransparentRenderer m_modelTransparentRenderer;
        ShadowRenderer m_shadowRenderer;
        RenderCubemap m_renderCubemap;
        Postprocess m_postProcess;
    };
}
