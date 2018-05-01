#include "engine/rendering/ClusterRenderer.h"
#include "engine/rendering/GBuffer.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/CommandList.h"
#include "components/Camera.h"

#undef EARLYZ_ENABLED

namespace engine
{
    ClusterRenderer::ClusterRenderer(Device& device, ShaderStorage& shaderStorage)
        : m_fullRender{ device.createPipeline<shaders::ClusterRender>(shaderStorage) }
        , m_gbufferRender{ device.createPipeline<shaders::ClusterRender>(shaderStorage) }
        , m_depthRender{ device.createPipeline<shaders::ClusterShadowRenderer>(shaderStorage) }
        , m_clusterDebugRender{ device.createPipeline<shaders::ClusterRender>(shaderStorage) }
    {
        DepthStencilOpDescription front;
        front.StencilFailOp = StencilOp::Keep;
        front.StencilDepthFailOp = StencilOp::Incr;
        front.StencilPassOp = StencilOp::Keep;
        front.StencilFunc = ComparisonFunction::Always;

        DepthStencilOpDescription back;
        back.StencilFailOp = StencilOp::Keep;
        back.StencilDepthFailOp = StencilOp::Decr;
        back.StencilPassOp = StencilOp::Keep;
        back.StencilFunc = ComparisonFunction::Always;

        m_fullRender.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_fullRender.setRasterizerState(RasterizerDescription().cullMode(CullMode::Back));
        m_fullRender.ps.tex_sampler = device.createSampler(SamplerDescription().filter(Filter::Bilinear));
        m_fullRender.setDepthStencilState(DepthStencilDescription()
            .depthEnable(true)
#ifdef EARLYZ_ENABLED
            .depthWriteMask(DepthWriteMask::Zero)
            .depthFunc(ComparisonFunction::Equal)
#else
            .depthWriteMask(DepthWriteMask::All)
            .depthFunc(ComparisonFunction::Greater)
#endif
            .frontFace(front)
            .backFace(back));

        m_gbufferRender.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_gbufferRender.setRasterizerState(RasterizerDescription().cullMode(CullMode::Back));
        m_gbufferRender.ps.tex_sampler = device.createSampler(SamplerDescription().filter(Filter::Bilinear));
        m_gbufferRender.setDepthStencilState(DepthStencilDescription()
            .depthEnable(false)
#ifdef EARLYZ_ENABLED
            .depthWriteMask(DepthWriteMask::Zero)
            .depthFunc(ComparisonFunction::Equal)
#else
            .depthWriteMask(DepthWriteMask::All)
            .depthFunc(ComparisonFunction::Always)
#endif
            .frontFace(front)
            .backFace(back));

        m_depthRender.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_depthRender.setRasterizerState(RasterizerDescription()
            .depthBias(-16384)
            .depthBiasClamp(1000000.0f)
            //.depthClipEnable(false)
            .slopeScaledDepthBias(-1.75f)
            .cullMode(CullMode::None));
        m_depthRender.setDepthStencilState(DepthStencilDescription()
            .depthEnable(true)
            .depthWriteMask(DepthWriteMask::All)
            .depthFunc(ComparisonFunction::Greater)
            .frontFace(front)
            .backFace(back));


        m_clusterDebugRender.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_clusterDebugRender.setRasterizerState(RasterizerDescription().cullMode(CullMode::Back));
        m_clusterDebugRender.ps.tex_sampler = device.createSampler(SamplerDescription().filter(Filter::Bilinear));
        m_clusterDebugRender.setDepthStencilState(DepthStencilDescription()
            .depthEnable(true)
#ifdef EARLYZ_ENABLED
            .depthWriteMask(DepthWriteMask::Zero)
            .depthFunc(ComparisonFunction::Equal)
#else
            .depthWriteMask(DepthWriteMask::All)
            .depthFunc(ComparisonFunction::Greater)
#endif
            .frontFace(front)
            .backFace(back));
        m_clusterDebugRender.ps.debugClusters = true;
        
    }

    static int frameId = 0;

    void ClusterRenderer::render(
        CommandList& cmd, 
        Camera& camera,
        ModelResources& modelResources,
        
        GBuffer* gBuffer,
        TextureDSV* depthBuffer,

        BufferSRV& clusterList,
        Buffer& drawArguments,

        uint64_t frameNumber,
        Vector2<int> virtualResolution,
        
        const Matrix4f& previousViewMatrix,
        const Matrix4f& previousProjectionMatrix,
        int currentRenderMode)
    {
        CPU_MARKER("Render opaque models");
        GPU_MARKER(cmd, "Render opaque models");

        bool justDepth = false;
        if (gBuffer && depthBuffer)
        {
            cmd.setRenderTargets({
                gBuffer->rtv(GBufferType::Normal),
                gBuffer->rtv(GBufferType::Uv),
                gBuffer->rtv(GBufferType::Motion),
                gBuffer->rtv(GBufferType::InstanceId) }, *depthBuffer);
            if (currentRenderMode == 1)
                m_currentPipeline = &m_clusterDebugRender;
            else
                m_currentPipeline = &m_fullRender;
        }
        else if (gBuffer)
        {
            cmd.setRenderTargets({
                gBuffer->rtv(GBufferType::Normal),
                gBuffer->rtv(GBufferType::Uv),
                gBuffer->rtv(GBufferType::Motion),
                gBuffer->rtv(GBufferType::InstanceId) });
            if (currentRenderMode == 1)
                m_currentPipeline = &m_clusterDebugRender;
            else
                m_currentPipeline = &m_gbufferRender;
        }
        else if (depthBuffer)
        {
            cmd.setRenderTargets({}, *depthBuffer);
            justDepth = true;
        }
        
        if (!justDepth)
        {
            m_currentPipeline->vs.vertices = modelResources.gpuBuffers().vertex();
            m_currentPipeline->vs.normals = modelResources.gpuBuffers().normal();
            m_currentPipeline->vs.tangents = modelResources.gpuBuffers().tangent();
            m_currentPipeline->vs.uv = modelResources.gpuBuffers().uv();
            m_currentPipeline->vs.indexes = modelResources.gpuBuffers().index();
            
            m_currentPipeline->vs.clusters = clusterList;
            m_currentPipeline->vs.clusterData = modelResources.gpuBuffers().clusterBinding();
            m_currentPipeline->vs.transformHistory = modelResources.gpuBuffers().instanceTransform();
            m_currentPipeline->vs.instanceUV = modelResources.gpuBuffers().instanceUv();
            
            m_currentPipeline->vs.viewProjectionMatrix = fromMatrix(camera.projectionMatrix() * camera.viewMatrix());
            m_currentPipeline->vs.previousViewProjectionMatrix = fromMatrix(previousProjectionMatrix * previousViewMatrix);
            m_currentPipeline->vs.jitterViewProjectionMatrix = fromMatrix(camera.jitterMatrix(frameNumber, virtualResolution) * camera.projectionMatrix() * camera.viewMatrix());
            
            m_currentPipeline->ps.instanceMaterials = modelResources.gpuBuffers().instanceMaterial();
            m_currentPipeline->ps.materialTextures = modelResources.textures();
            
            cmd.bindPipe(*m_currentPipeline);
            cmd.drawIndirect(drawArguments, 0);
        }
        else
        {
            m_depthRender.vs.vertices = modelResources.gpuBuffers().vertex();
            m_depthRender.vs.indexes = modelResources.gpuBuffers().index();
            
            m_depthRender.vs.clusters = clusterList;
            m_depthRender.vs.clusterData = modelResources.gpuBuffers().clusterBinding();
            m_depthRender.vs.transformHistory = modelResources.gpuBuffers().instanceTransform();
            
            m_depthRender.vs.viewProjectionMatrix = fromMatrix(camera.projectionMatrix() * camera.viewMatrix());
            m_depthRender.vs.previousViewProjectionMatrix = fromMatrix(previousProjectionMatrix * previousViewMatrix);
            m_depthRender.vs.jitterViewProjectionMatrix = fromMatrix(camera.jitterMatrix(frameNumber, virtualResolution) * camera.projectionMatrix() * camera.viewMatrix());
            
            cmd.bindPipe(m_depthRender);
            cmd.drawIndirect(drawArguments, 0);
        }
    }
}
