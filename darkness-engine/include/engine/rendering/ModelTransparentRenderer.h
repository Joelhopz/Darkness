#pragma once

#include "engine/graphics/CommandList.h"
#include "components/Transform.h"
#include "components/MeshRendererComponent.h"
#include "components/MaterialComponent.h"
#include "components/LightComponent.h"
#include "components/Camera.h"
#include "shaders/core/forward/MeshRendererTransparent.h"
#include "shaders/core/transparent/MeshTransparentComposite.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/Scene.h"
#include "engine/rendering/LightData.h"
#include <memory>

namespace engine
{
    class ModelTransparentRenderer
    {
    public:
        ModelTransparentRenderer(Device& device, ShaderStorage& shaderStorage);
        void render(
            Device& device,
            TextureRTV& currentRenderTarget,
            TextureDSV& depthBuffer,
            CommandList& cmd,
            MaterialComponent& defaultMaterial,
            Camera& camera,
            LightData& lights,
            FlatScene& scene,
            TextureSRV& shadowMap,
            BufferSRV& shadowVP
        );

        void clearResources();

        TextureSRV& colorAccumulateSRV() { return m_colorAccumulateSRV; };
        TextureSRV& transparencyAccumulateSRV() { return m_transparencyAccumulateSRV; };

    private:
        Device& m_device;
        ShaderStorage& m_shaderStorage;
        engine::Pipeline<shaders::MeshRendererTransparent> m_pipeline;
        engine::Pipeline<shaders::MeshTransparentComposite> m_compositePipeline;
        TextureRTV m_colorAccumulate;
        TextureRTV m_transparencyAccumulate;

        TextureSRV m_colorAccumulateSRV;
        TextureSRV m_transparencyAccumulateSRV;
    };
}
