#pragma once

#include "engine/graphics/CommandList.h"
#include "components/Transform.h"
#include "components/MeshRendererComponent.h"
#include "components/MaterialComponent.h"
#include "components/LightComponent.h"
#include "components/Camera.h"
#include "shaders/core/shadows/ShadowRender.h"
#include "shaders/core/shadows/ShadowRenderTransparent.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/Scene.h"
#include "engine/rendering/LightData.h"
#include <memory>

namespace engine
{
    constexpr int ShadowMapWidth = 1024;
    constexpr int ShadowMapHeight = 1024;

    class ShadowRenderer
    {
    public:
        ShadowRenderer(Device& device, ShaderStorage& shaderStorage);

        void render(
            Device& device,
            TextureRTV& currentRenderTarget,
            TextureDSV& depthBuffer,
            CommandList& cmd,
            MaterialComponent& defaultMaterial,
            Camera& camera,
            LightData& lights,
            FlatScene& scene
        );

        void clearResources();
        void refresh()
        {
            m_refresh = true;
        }

        engine::Camera& shadowCamera()
        {
            return m_shadowCamera;
        }

        TextureSRV& shadowMap()
        {
            return m_shadowMapSRV;
        }

        BufferSRV& shadowVP()
        {
            return m_shadowVP;
        }

    private:
        Device& m_device;
        ShaderStorage& m_shaderStorage;
        Pipeline<shaders::ShadowRender> m_pipeline;
        Pipeline<shaders::ShadowRenderTransparent> m_transparentPipeline;
        /*Matrix4f m_shadowView;
        Matrix4f m_shadowProjection;*/

        engine::Camera m_shadowCamera;
        TextureDSV m_shadowMap;
        TextureSRV m_shadowMapSRV;
        std::vector<TextureDSV> m_shadowMapIndices;
        BufferSRV m_shadowVP;

        engine::Camera m_cubemapCamera;
        bool m_refresh;
    };
}
