#include "engine/rendering/Culler.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/Queue.h"
#include "engine/graphics/Fence.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/CommandList.h"
#include "engine/rendering/DepthPyramid.h"
#include "components/Camera.h"

#include "engine/graphics/HLSLTypeConversions.h"

namespace engine
{
    #include "shaders/core/shared_types/ClusterData.hlsli"
    #include "shaders/core/shared_types/ClusterInstanceData.hlsli"
    #include "shaders/core/shared_types/SubMeshData.hlsli"
    #include "shaders/core/shared_types/InstanceMaterial.hlsli"
    #include "shaders/core/shared_types/TransformHistory.hlsli"
    #include "shaders/core/shared_types/FrustumCullingOutput.hlsli"

    Culler::Culler(Device& device, ShaderStorage& shaderStorage)
        : m_device{ device }
        , m_prefixSumClusters{ device, shaderStorage }
        , m_frustumCulling{ device.createPipeline<shaders::FrustumCulling>(shaderStorage) }
        , m_occlusionCulling{ device.createPipeline<shaders::OcclusionCulling>(shaderStorage) }
        , m_occlusionCullingEmitAll{ device.createPipeline<shaders::OcclusionCulling>(shaderStorage) }
        , m_clusterGather{ device.createPipeline<shaders::ClusterGather>(shaderStorage) }
        , m_createOcclusionCullingArgs{ device.createPipeline<shaders::CreateOcclusionCullingArgs>(shaderStorage) }
        , m_createDrawArgs{ device.createPipeline<shaders::CreateDrawArguments>(shaderStorage) }
        , m_depthSampler{ device.createSampler(SamplerDescription().filter(Filter::Point)) }

        , m_frustumCullingOutputUAV{ 
            device.createBufferUAV(BufferDescription()
                .elementSize(sizeof(FrustumCullingOutput))
                .elements(MaxInstances)
                .structured(true)
                .usage(ResourceUsage::GpuReadWrite)
                .append(true)
                .name("Frustum culling cluster output (FrustumCullingOutput)")
            ) }

        , m_frustumCullingOutputSRV{ device.createBufferSRV(m_frustumCullingOutputUAV.buffer()) }

        , m_occlusionCullingOutputUAV{
            device.createBufferUAV(BufferDescription()
                .elementSize(sizeof(ClusterInstanceData))
                .elements(MaxClusterInstanceCount)
                .usage(ResourceUsage::GpuReadWrite)
                .structured(true)
                .name("Occlusion culling output clusters (ClusterInstanceData)")
            ) }
        , m_occlusionCullingOutputSRV{ device.createBufferSRV(m_occlusionCullingOutputUAV.buffer()) }

        , m_clusterCountBufferUAV{
            device.createBufferUAV(BufferDescription()
                .elementSize(sizeof(uint32_t))
                .elements(2)
                .format(Format::R32_UINT)
                .usage(ResourceUsage::GpuReadWrite)
                .name("Cluster and Instance count buffer (uint)")
            ) }
        , m_clusterCountBufferSRV{ device.createBufferSRV(m_clusterCountBufferUAV.buffer()) }

        , m_frustumCullingOutputAllocationSharedUAV{
            device.createBufferUAV(BufferDescription()
                .elementSize(sizeof(uint32_t))
                .elements(1)
                .format(Format::R32_UINT)
                .usage(ResourceUsage::GpuReadWrite)
                .name("Frustum culling shared output allocation (uint)")
            ) }

        , m_distributorUAV{
            device.createBufferUAV(BufferDescription()
                .elementSize(sizeof(uint32_t))
                .elements(1)
                .format(Format::R32_UINT)
                .usage(ResourceUsage::GpuReadWrite)
                .name("Occlusion culling output distributor buffer (uint)")
            ) }
        , m_distributorSRV{ device.createBufferSRV(m_distributorUAV.buffer()) }

        , m_clustersForOcclusionCullingUAV{
            device.createBufferUAV(BufferDescription()
                .elementSize(sizeof(ClusterInstanceData))
                .elements(MaxClusterInstanceCount)
                .usage(ResourceUsage::GpuReadWrite)
                .structured(true)
                .name("Clusters for occlusion culling (ClusterInstanceData)")
            ) }
        , m_clustersForOcclusionCullingSRV{ device.createBufferSRV(m_clustersForOcclusionCullingUAV.buffer()) }

        , m_gatherDispatchArgs{
            device.createBuffer(BufferDescription()
                .elementSize(sizeof(DispatchIndirectArgs))
                .elements(1)
                .usage(ResourceUsage::GpuReadWrite)
                .structured(true)
                .name("Gather dispatch args (DispatchIndirectArgs)")
            ) }

        , m_gatherDispatchArgsUAV{
            device.createBufferUAV(m_gatherDispatchArgs) }

        , m_occlusionCullingDispatchArgs{
            device.createBuffer(BufferDescription()
                .elementSize(sizeof(DispatchIndirectArgs))
                .elements(1)
                .usage(ResourceUsage::GpuReadWrite)
                .structured(true)
                .name("Occlusion culling dispatch args (DispatchIndirectArgs)")
            ) }

        , m_occlusionCullingDispatchArgsUAV{
            device.createBufferUAV(m_occlusionCullingDispatchArgs) }

        , m_occlusionCullingDrawArgs{ device.createBuffer(BufferDescription()
            .format(Format::R32_UINT)
            //.elementSize(sizeof(uint32_t))
            .elements(4)
            .usage(ResourceUsage::GpuReadWrite)
            .name("Occlusion culling draw args (uint)")
        ) }
        , m_occlusionCullingDrawArgsUAV{ device.createBufferUAV(m_occlusionCullingDrawArgs) }

        , m_clusterPrefixSumsUAV{
            device.createBufferUAV(BufferDescription()
                .elementSize(sizeof(uint32_t))
                .elements(MaxClusterInstanceCount)
                .format(Format::R32_UINT)
                .usage(ResourceUsage::GpuReadWrite)
                .name("Clusters prefix sums (uint)")
            ) }
        , m_clusterPrefixSumsSRV{ device.createBufferSRV(m_clusterPrefixSumsUAV.buffer()) }
    {
        m_occlusionCulling.cs.depthSampler = m_depthSampler;
        m_occlusionCullingEmitAll.cs.depthSampler = m_depthSampler;
        m_occlusionCullingEmitAll.cs.emitAll = true;
    }

    void Culler::frustumCull(
        CommandList& cmd, 
        Camera& camera, 
        ModelResources& modelResources, 
        uint64_t frameNumber,
        Vector2<int> virtualResolution)
    {
        CPU_MARKER("Frustum culling");
        GPU_MARKER(cmd, "Frustum culling");

        std::vector<Vector4f> frustumPlanes = extractFrustumPlanes(camera.projectionMatrix() * camera.viewMatrix());

        cmd.setStructureCounter(m_frustumCullingOutputUAV, 0);
        cmd.clearBuffer(m_clusterCountBufferUAV.buffer(), 0, 0, m_clusterCountBufferUAV.buffer().description().descriptor.elements);
        cmd.clearBuffer(m_frustumCullingOutputUAV.buffer(), 0, 0, m_frustumCullingOutputUAV.desc().elements);
        cmd.clearBuffer(m_frustumCullingOutputAllocationSharedUAV.buffer(), 0, 0, m_frustumCullingOutputAllocationSharedUAV.desc().elements);

        // buffers
        m_frustumCulling.cs.subMeshBoundingBoxes = modelResources.gpuBuffers().subMeshBoundingBox();
        m_frustumCulling.cs.instanceSubMeshBinding = modelResources.gpuBuffers().instanceSubMeshBinding();
        m_frustumCulling.cs.subMeshData = modelResources.gpuBuffers().subMeshData();
        m_frustumCulling.cs.transformHistory = modelResources.gpuBuffers().instanceTransform();
        m_frustumCulling.cs.cullingOutput = m_frustumCullingOutputUAV;
        m_frustumCulling.cs.clusterCountBuffer = m_clusterCountBufferUAV;
        m_frustumCulling.cs.outputAllocationShared = m_frustumCullingOutputAllocationSharedUAV;

        // constants
        m_frustumCulling.cs.plane0 = frustumPlanes[0];
        m_frustumCulling.cs.plane1 = frustumPlanes[1];
        m_frustumCulling.cs.plane2 = frustumPlanes[2];
        m_frustumCulling.cs.plane3 = frustumPlanes[3];
        m_frustumCulling.cs.plane4 = frustumPlanes[4];
        m_frustumCulling.cs.plane5 = frustumPlanes[5];
        m_frustumCulling.cs.cameraPosition = camera.position();
        m_frustumCulling.cs.instanceCount.x = modelResources.instanceCount();

        cmd.bindPipe(m_frustumCulling);
        cmd.dispatch(roundUpToMultiple(modelResources.instanceCount(), 64) / 64, 1, 1);
    }

    void Culler::occlusionCull(
        CommandList& cmd,
        Camera& camera,
        ModelResources& modelResources,
        
        DepthPyramid* depthPyramid,

        uint64_t frameNumber,
        Vector2<int> virtualResolution,
        
        CullingMode mode)
    {
        {
            CPU_MARKER("Create occlusion culling args");
            GPU_MARKER(cmd, "Create occlusion culling args");

            // create indirect arguments
            cmd.clearBuffer(m_occlusionCullingDispatchArgs, 0, 0, m_occlusionCullingDispatchArgs.description().descriptor.elements);
            //cmd.copyStructureCounter(m_frustumCullingOutputUAV, m_frustumCullingStructureCount, 0);
            //m_createOcclusionCullingArgs.cs.structureCounter = m_frustumCullingStructureCountSRV;
            m_createOcclusionCullingArgs.cs.clusterCountBuffer = m_clusterCountBufferSRV;
            m_createOcclusionCullingArgs.cs.gatherDispatchArgs = m_gatherDispatchArgsUAV;
            m_createOcclusionCullingArgs.cs.occlusionDispatchArgs = m_occlusionCullingDispatchArgsUAV;
            cmd.bindPipe(m_createOcclusionCullingArgs);
            cmd.dispatch(1, 1, 1);
        }

        /*{
            CPU_MARKER("Prefix sum clusters");
            GPU_MARKER(cmd, "Prefix sum clusters");

            m_prefixSumClusters.prefixSum(
                m_device, cmd, 
                m_frustumCullingOutputSRV, 
                m_clusterCountBufferSRV, 
                m_gatherDispatchArgs, 
                m_clusterPrefixSumsUAV);
        }*/

        {
            CPU_MARKER("Gather clusters");
            GPU_MARKER(cmd, "Gather clusters");

            m_clusterGather.cs.clusterCountBuffer = m_clusterCountBufferSRV;
            m_clusterGather.cs.frustumCullingOutput = m_frustumCullingOutputSRV;
            m_clusterGather.cs.clustersForOcclusionCulling = m_clustersForOcclusionCullingUAV;
            cmd.bindPipe(m_clusterGather);
            cmd.dispatchIndirect(
                m_gatherDispatchArgs,
                0);
        }

        cmd.clearBuffer(m_occlusionCullingDrawArgsUAV.buffer(), 0, 0, m_occlusionCullingDrawArgsUAV.buffer().description().descriptor.elements);
        cmd.clearBuffer(m_distributorUAV.buffer(), 0, 0, m_distributorUAV.buffer().description().descriptor.elements);

        std::vector<Vector4f> frustumPlanes = extractFrustumPlanes(camera.projectionMatrix() * camera.viewMatrix());

        if (mode == CullingMode::EmitAll)
        {
            CPU_MARKER("Occlusion culling");
            GPU_MARKER(cmd, "Occlusion culling");

            // constants
            m_occlusionCullingEmitAll.cs.viewMatrix = fromMatrix(camera.viewMatrix());
            m_occlusionCullingEmitAll.cs.projectionMatrix = fromMatrix(camera.projectionMatrix());
            m_occlusionCullingEmitAll.cs.cameraPosition = Vector4f(camera.position(), 1.0f);
            m_occlusionCullingEmitAll.cs.size = Vector2f{ static_cast<float>(virtualResolution.x), static_cast<float>(virtualResolution.y) };
            m_occlusionCullingEmitAll.cs.inverseSize = Vector2f{ 1.0f / static_cast<float>(virtualResolution.x), 1.0f / static_cast<float>(virtualResolution.y) };
            m_occlusionCullingEmitAll.cs.pow2size = Vector2f{
                static_cast<float>(roundUpToPow2(virtualResolution.x)),
                static_cast<float>(roundUpToPow2(virtualResolution.y))
            };
            m_occlusionCullingEmitAll.cs.nearFar = Vector2f(camera.nearPlane(), camera.farPlane());
            m_occlusionCullingEmitAll.cs.mipLevels.x = static_cast<float>(depthPyramid ? depthPyramid->depth().texture().mipLevels() : 1);
            m_occlusionCullingEmitAll.cs.mipLevels.y = m_occlusionCulling.cs.mipLevels.x;
            m_occlusionCullingEmitAll.cs.mipLevels.z = m_occlusionCulling.cs.mipLevels.x;
            m_occlusionCullingEmitAll.cs.mipLevels.w = m_occlusionCulling.cs.mipLevels.x;
            m_occlusionCullingEmitAll.cs.plane0 = frustumPlanes[0];
            m_occlusionCullingEmitAll.cs.plane1 = frustumPlanes[1];
            m_occlusionCullingEmitAll.cs.plane2 = frustumPlanes[2];
            m_occlusionCullingEmitAll.cs.plane3 = frustumPlanes[3];
            m_occlusionCullingEmitAll.cs.plane4 = frustumPlanes[4];
            m_occlusionCullingEmitAll.cs.plane5 = frustumPlanes[5];

            // buffers
            //m_occlusionCullingEmitAll.cs.clustersForOcclusionCulling = m_clustersForOcclusionCullingSRV;
            m_occlusionCullingEmitAll.cs.frustumCullingOutput = m_clustersForOcclusionCullingSRV;
            m_occlusionCullingEmitAll.cs.boundingBoxes = modelResources.gpuBuffers().clusterBoundingBox();
            m_occlusionCullingEmitAll.cs.transformHistory = modelResources.gpuBuffers().instanceTransform();
            m_occlusionCullingEmitAll.cs.cullingOutput = m_occlusionCullingOutputUAV;
            m_occlusionCullingEmitAll.cs.clusterCount = m_clusterCountBufferSRV;
            m_occlusionCullingEmitAll.cs.occlusionDrawArgs = m_occlusionCullingDrawArgsUAV;
            m_occlusionCullingEmitAll.cs.distributor = m_distributorUAV;
            //m_occlusionCullingEmitAll.cs.clusterData = modelResources.gpuBuffers().clusterBinding();
            m_occlusionCullingEmitAll.cs.depthPyramid = depthPyramid != nullptr ? depthPyramid->depth() : TextureSRV();

            cmd.bindPipe(m_occlusionCullingEmitAll);
            cmd.dispatchIndirect(m_occlusionCullingDispatchArgs, 0);
        }
        else if (mode == CullingMode::EmitVisible)
        {
            CPU_MARKER("Occlusion culling");
            GPU_MARKER(cmd, "Occlusion culling");

            // constants
            m_occlusionCulling.cs.viewMatrix = fromMatrix(camera.viewMatrix());
            m_occlusionCulling.cs.projectionMatrix = fromMatrix(camera.projectionMatrix());
            m_occlusionCulling.cs.cameraPosition = Vector4f(camera.position(), 1.0f);
            m_occlusionCulling.cs.size = Vector2f{ static_cast<float>(virtualResolution.x), static_cast<float>(virtualResolution.y) };
            m_occlusionCulling.cs.inverseSize = Vector2f{ 1.0f / static_cast<float>(virtualResolution.x), 1.0f / static_cast<float>(virtualResolution.y) };
            m_occlusionCulling.cs.pow2size = Vector2f{
                static_cast<float>(roundUpToPow2(virtualResolution.x)),
                static_cast<float>(roundUpToPow2(virtualResolution.y))
            };
            m_occlusionCulling.cs.nearFar = Vector2f(camera.nearPlane(), camera.farPlane());
            m_occlusionCulling.cs.mipLevels.x = static_cast<float>(depthPyramid ? depthPyramid->depth().texture().mipLevels() : 1);
            m_occlusionCulling.cs.mipLevels.y = m_occlusionCulling.cs.mipLevels.x;
            m_occlusionCulling.cs.mipLevels.z = m_occlusionCulling.cs.mipLevels.x;
            m_occlusionCulling.cs.mipLevels.w = m_occlusionCulling.cs.mipLevels.x;
            m_occlusionCulling.cs.plane0 = frustumPlanes[0];
            m_occlusionCulling.cs.plane1 = frustumPlanes[1];
            m_occlusionCulling.cs.plane2 = frustumPlanes[2];
            m_occlusionCulling.cs.plane3 = frustumPlanes[3];
            m_occlusionCulling.cs.plane4 = frustumPlanes[4];
            m_occlusionCulling.cs.plane5 = frustumPlanes[5];

            // buffers
            //m_occlusionCulling.cs.clustersForOcclusionCulling = m_clustersForOcclusionCullingSRV;
            m_occlusionCulling.cs.frustumCullingOutput = m_clustersForOcclusionCullingSRV;
            m_occlusionCulling.cs.boundingBoxes = modelResources.gpuBuffers().clusterBoundingBox();
            m_occlusionCulling.cs.transformHistory = modelResources.gpuBuffers().instanceTransform();
            m_occlusionCulling.cs.cullingOutput = m_occlusionCullingOutputUAV;
            m_occlusionCulling.cs.clusterCount = m_clusterCountBufferSRV;
            m_occlusionCulling.cs.occlusionDrawArgs = m_occlusionCullingDrawArgsUAV;
            m_occlusionCulling.cs.distributor = m_distributorUAV;
            //m_occlusionCulling.cs.clusterData = modelResources.gpuBuffers().clusterBinding();
            m_occlusionCulling.cs.depthPyramid = depthPyramid != nullptr ? depthPyramid->depth() : TextureSRV();

            cmd.bindPipe(m_occlusionCulling);
            cmd.dispatchIndirect(m_occlusionCullingDispatchArgs, 0);
        }
    }

    BufferSRV& Culler::occlusionCulledClusters()
    {
        return m_occlusionCullingOutputSRV;
    }

    Buffer& Culler::occlusionCullingDrawArguments()
    {
        return m_occlusionCullingDrawArgs;
    }
}
