#pragma once

#include "shaders/core/model/ClusterRender.h"
#include "shaders/core/model/ClusterShadowRenderer.h"
#include "engine/graphics/Pipeline.h"

namespace engine
{
    class Device;
    class ShaderStorage;
    class CommandList;
    class ModelResources;
    class GBuffer;
    class TextureDSV;

    class ClusterRenderer
    {
    public:
        ClusterRenderer(Device& device, ShaderStorage& shaderStorage);

        void render(
            CommandList& cmd, 
            Camera& camera, 
            ModelResources& modelResources,

            GBuffer* gBuffer,
            TextureDSV* depthBuffer,

            // culling results
            BufferSRV& clusterList,
            Buffer& drawArguments,

            // jitter parameters
            uint64_t frameNumber,
            Vector2<int> virtualResolution,
            
            // remporal stuff
            const Matrix4f& previousViewMatrix,
            const Matrix4f& previousProjectionMatrix,

            int currentRenderMode
            );

        //BoundingBox selectedClusterBoundingBox(Device& device);
    private:
        engine::Pipeline<shaders::ClusterRender> m_fullRender;
        engine::Pipeline<shaders::ClusterRender> m_gbufferRender;
        engine::Pipeline<shaders::ClusterShadowRenderer> m_depthRender;
        engine::Pipeline<shaders::ClusterRender> m_clusterDebugRender;

        engine::Pipeline<shaders::ClusterRender>* m_currentPipeline;

        /*BufferUAV m_selectedClusterBBBufferUAV;
        BufferSRV m_selectedClusterBBBufferReadBack;*/
    };
}
