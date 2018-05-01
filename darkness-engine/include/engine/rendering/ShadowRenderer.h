#pragma once

#include "engine/graphics/CommandList.h"
#include "components/Transform.h"
#include "components/MeshRendererComponent.h"
#include "components/MaterialComponent.h"
#include "components/LightComponent.h"
#include "components/Camera.h"
#include "engine/graphics/CommonNoDep.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/Scene.h"
#include "engine/rendering/LightData.h"
#include "engine/rendering/ClusterRenderer.h"
#include "engine/rendering/Culler.h"
#include <memory>

namespace engine
{
    class ShadowRenderer
    {
    public:
        ShadowRenderer(Device& device, ShaderStorage& shaderStorage);

        void render(
            CommandList& cmd,
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
        uint32_t shadowCasterCount(const engine::LightData& lightData) const;
        void updateShadowMaps(uint32_t shadowCasterMapCount);

    private:
        Device& m_device;
        ShaderStorage& m_shaderStorage;
        ClusterRenderer m_clusterRenderer;
        Culler m_culler;

        engine::Camera m_shadowCamera;
        TextureDSV m_shadowMap;
        TextureSRV m_shadowMapSRV;
        std::vector<TextureDSV> m_shadowMapIndices;
        BufferSRV m_shadowVP;

        engine::Camera m_cubemapCamera;
        bool m_refresh;
    };
}
