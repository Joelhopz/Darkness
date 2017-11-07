#if 0
#pragma once

#include "engine/graphics/CommandList.h"
#include "components/Transform.h"
#include "components/MeshRendererComponent.h"
#include "components/MaterialComponent.h"
#include "components/LightComponent.h"
#include "components/Camera.h"
#include "shaders/core/SSAORender.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/Scene.h"
#include "engine/rendering/LightData.h"
#include <memory>

namespace engine
{
    class SSAORenderer
    {
    public:
        SSAORenderer(Device& device, ShaderStorage& shaderStorage);
        void render(
            Device& device,
            TextureRTV& currentRenderTarget,
            TextureSRV& depthBuffer,
            CommandList& cmd
        );

    private:
        Device& m_device;
        ShaderStorage& m_shaderStorage;
        engine::Pipeline<shaders::SSAORender> m_pipeline;
    };
}
#endif