#pragma once

#include "engine/graphics/CommonNoDep.h"
#include "engine/graphics/Pipeline.h"
#include "engine/graphics/Sampler.h"
#include "engine/graphics/Device.h"
#include "engine/rendering/PrefixSumClusters.h"
#include "shaders/core/culling/FrustumCulling.h"
#include "shaders/core/culling/OcclusionCulling.h"
#include "shaders/core/culling/ClusterGather.h"
#include "shaders/core/culling/CreateOcclusionCullingArgs.h"
#include "shaders/core/culling/CreateDrawArguments.h"

namespace engine
{
    class CommandList;
    class Camera;
    class ModelResources;
    class DepthPyramid;

    enum class CullingMode
    {
        EmitAll,
        EmitVisible
    };

    class Culler
    {
    public:
        Culler(Device& device, ShaderStorage& shaderStorage);

        void frustumCull(
            CommandList& cmd, 
            Camera& camera, 
            ModelResources& modelResources, 

            // jitter parameters
            uint64_t frameNumber,
            Vector2<int> virtualResolution);

        void occlusionCull(
            CommandList& cmd,
            Camera& camera,
            ModelResources& modelResources,

            DepthPyramid* depthPyramid,

            // jitter parameters
            uint64_t frameNumber,
            Vector2<int> virtualResolution,

            CullingMode mode
        );

        BufferSRV& occlusionCulledClusters();
        Buffer& occlusionCullingDrawArguments();

    private:
        Device& m_device;
        PrefixSumClusters m_prefixSumClusters;
        engine::Pipeline<shaders::FrustumCulling> m_frustumCulling;
        engine::Pipeline<shaders::OcclusionCulling> m_occlusionCulling;
        engine::Pipeline<shaders::OcclusionCulling> m_occlusionCullingEmitAll;
        engine::Pipeline<shaders::ClusterGather> m_clusterGather;
        engine::Pipeline<shaders::CreateOcclusionCullingArgs> m_createOcclusionCullingArgs;
        engine::Pipeline<shaders::CreateDrawArguments> m_createDrawArgs;
        Sampler m_depthSampler;

    private:
        BufferUAV m_frustumCullingOutputUAV;
        BufferSRV m_frustumCullingOutputSRV;

        BufferUAV m_clustersForOcclusionCullingUAV;
        BufferSRV m_clustersForOcclusionCullingSRV;
        BufferUAV m_occlusionCullingOutputUAV;
        BufferSRV m_occlusionCullingOutputSRV;

        BufferUAV m_clusterCountBufferUAV;
        BufferSRV m_clusterCountBufferSRV;

        BufferUAV m_frustumCullingOutputAllocationSharedUAV;

        BufferUAV m_distributorUAV;
        BufferSRV m_distributorSRV;

        Buffer m_gatherDispatchArgs;
        BufferUAV m_gatherDispatchArgsUAV;

        Buffer m_occlusionCullingDispatchArgs;
        BufferUAV m_occlusionCullingDispatchArgsUAV;

        Buffer m_occlusionCullingDrawArgs;
        BufferUAV m_occlusionCullingDrawArgsUAV;

        BufferUAV m_clusterPrefixSumsUAV;
        BufferSRV m_clusterPrefixSumsSRV;
    };
}
