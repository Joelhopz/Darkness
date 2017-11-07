#pragma once

#include "engine/graphics/CommandList.h"
#include "components/Transform.h"
#include "components/MeshRendererComponent.h"
#include "components/MaterialComponent.h"
#include "components/LightComponent.h"
#include "components/Camera.h"
#include "shaders/core/outline/Outline.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/Scene.h"
#include "engine/rendering/LightData.h"
#include <memory>
#include <map>

namespace engine
{
    class RenderOutline
    {
    public:
        RenderOutline(
            Device& device, 
            ShaderStorage& shaderStorage,
            Vector2<int> virtualResolution);

        void render(
            Device& device,
            TextureRTV& currentRenderTarget,
            TextureDSV& depthBuffer,
            Camera& camera,
            CommandList& cmd,
            FlatSceneNode& model
        );

    private:
        Device& m_device;
        ShaderStorage& m_shaderStorage;
        engine::Pipeline<shaders::Outline> m_outlinePipeline;
        Vector2<int> m_virtualResolution;

        unsigned int m_adjacencyId;
        std::vector<BufferIBV> m_adjacencyBuffers;
        void rebuildAdjacencyData(FlatSceneNode& model);

        void clearSaves();
        std::vector<std::pair<uint64_t, std::vector<BufferIBV>>> m_adjacencySave;
    };
}
