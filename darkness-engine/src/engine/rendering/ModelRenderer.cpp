#include "engine/rendering/ModelRenderer.h"
#include "engine/rendering/ShadowRenderer.h"
#include "engine/rendering/DepthPyramid.h"
#include "engine/rendering/ImguiRenderer.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/Viewport.h"
#include "engine/graphics/Rect.h"
#include "engine/graphics/Pipeline.h"
#include "engine/graphics/Common.h"
#include "tools/Measure.h"
#include "tools/ToolsCommon.h"

#include <random>

using namespace tools;

namespace engine
{
    bool frustumCull(
        std::vector<Vector4f>& frustumPlanes,
        const BoundingBox& aabb,
        const Vector3f cameraPosition,
        const Matrix4f& transform)
    {
        Vector3f corner[8] =
        {
            transform * aabb.min,
            transform * Vector3f{ aabb.min.x, aabb.max.y, aabb.max.z },
            transform * Vector3f{ aabb.min.x, aabb.min.y, aabb.max.z },
            transform * Vector3f{ aabb.min.x, aabb.max.y, aabb.min.z },
            transform * aabb.max,
            transform * Vector3f{ aabb.max.x, aabb.min.y, aabb.min.z },
            transform * Vector3f{ aabb.max.x, aabb.max.y, aabb.min.z },
            transform * Vector3f{ aabb.max.x, aabb.min.y, aabb.max.z }
        };

        for (int i = 0; i < 6; ++i)
        {
            bool hit = false;
            for (auto&& c : corner)
            {
                if (frustumPlanes[i].xyz().normalize().dot(c - cameraPosition) >= 0)
                {
                    hit = true;
                    break;
                }
            }
            if (!hit)
                return false;
        }
        return true;
    }

    ModelRenderer::ModelRenderer(Device& device, ShaderStorage& shaderStorage, Vector2<int> virtualResolution)
        : m_device{ device }
        , m_shaderStorage{ shaderStorage }
        , m_virtualResolution{ virtualResolution }
        , m_culler{ m_device, shaderStorage }
        , m_clusterRenderer{ m_device, shaderStorage }
        , m_picker{ m_device, shaderStorage }
        //, m_particleTest{ device, shaderStorage }
#ifdef EARLYZ_ENABLED
        , m_earlyzPipeline{ device.createPipeline<shaders::EarlyZ>(shaderStorage) }
        , m_earlyzAlphaClipped{ device.createPipeline<shaders::EarlyZAlphaClipped>(shaderStorage) }
#endif
        , m_pipeline{ device.createPipeline<shaders::MeshRenderer>(shaderStorage) }
        , m_pbrPipeline{ device.createPipeline<shaders::MeshRendererPbr>(shaderStorage) }
        , m_pbrPipelineAlphaclipped{ device.createPipeline<shaders::MeshRendererPbr>(shaderStorage) }
        , m_wireframePipeline{ device.createPipeline<shaders::Wireframe>(shaderStorage) }
        , m_lightingPipelines{}
        , m_ssaoPipeline{ device.createPipeline<shaders::SSAO>(shaderStorage) }
        , m_ssaoBlurPipeline{ device.createPipeline<shaders::SSAOBlur>(shaderStorage) }
        , m_temporalResolve{ }
        
        , m_debugBoundingSpheres{ device.createPipeline<shaders::DebugBoundingSpheres>(shaderStorage) }

        , m_currentRenderMode{ 0 }

        , m_renderWidth{ m_virtualResolution.x }
        , m_renderHeight{ m_virtualResolution.y }

        , m_renderOutline{ std::make_unique<RenderOutline>(device, shaderStorage, m_virtualResolution) }

        , m_gbuffer{ std::make_shared<GBuffer>(device, m_renderWidth, m_renderHeight) }

        , m_lightingTarget{ device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::R16G16B16A16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("FullRes Target 0")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f })) }
        , m_lightingTargetSRV{ device.createTextureSRV(m_lightingTarget.texture()) }

        , m_fullResTargetFrame{ 
            device.createTextureRTV(TextureDescription()
                .width(m_renderWidth)
                .height(m_renderHeight)
                .format(Format::R16G16B16A16_FLOAT)
                .usage(ResourceUsage::GpuRenderTargetReadWrite)
                .name("FullRes Target 0")
                .dimension(ResourceDimension::Texture2D)
                .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f })),
            device.createTextureRTV(TextureDescription()
                .width(m_renderWidth)
                .height(m_renderHeight)
                .format(Format::R16G16B16A16_FLOAT)
                .usage(ResourceUsage::GpuRenderTargetReadWrite)
                .name("FullRes Target 1")
                .dimension(ResourceDimension::Texture2D)
                .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f }))
        }
        , m_fullResTargetFrameSRV{
            device.createTextureSRV(m_fullResTargetFrame[0].texture()),
            device.createTextureSRV(m_fullResTargetFrame[1].texture())
        }
        , m_currentFullResIndex{ 0 }
        , m_lastResolvedIndex{ 0 }

        , m_ssaoRTV{ device.createTextureRTV(TextureDescription()
#ifdef SCALEAOSIZE
            .width(virtualResolution.x / SSAOresolutionScaleFactor)
            .height(virtualResolution.y / SSAOresolutionScaleFactor)
#else
            .width(SSAOWidth)
            .height(SSAOHeight)
#endif
            .format(Format::R16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("SSAO Render target")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 0.0f })
        ) }
        , m_ssaoSRV{ device.createTextureSRV(m_ssaoRTV.texture()) }

        /*, m_pickBufferUAV{ device.createBufferUAV(BufferDescription()
            .elements(1)
            .format(Format::R32_UINT)
            .usage(ResourceUsage::GpuReadWrite)
            .name("Pick buffer")
        ) }
        , m_pickBufferReadBack{ device.createBufferSRV(BufferDescription()
            .elements(1)
            .elementSize(sizeof(unsigned int))
            .format(Format::R32_UINT)
            .usage(ResourceUsage::GpuToCpu)
            .name("Pick buffer readback")
        ) }*/

        , m_dsv{ device.createTextureDSV(TextureDescription()
            .name("Depth Buffer")
            .format(Format::D32_FLOAT)
            .width(virtualResolution.x)
            .height(virtualResolution.y)
            .usage(ResourceUsage::DepthStencil)
            .optimizedDepthClearValue(0.0f)
            .dimension(ResourceDimension::Texture2D)
        ) }
        , m_dsvSRV{ device.createTextureSRV(m_dsv) }

        , m_selectedObject{ -1 }
    {
        createSSAOSampleKernel();
        createSSAOBlurKernel();
        m_ssaoNoiseTexture = device.createTextureSRV(TextureDescription()
            .width(4)
            .height(4)
            .format(Format::R32G32B32A32_FLOAT)
            .name("SSAO noise texture")
            .setInitialData(TextureDescription::InitialData(
                m_ssaoNoise, 
                4u * static_cast<uint32_t>(4 * sizeof(float)), 
                16u * static_cast<uint32_t>(4 * sizeof(float))))
        );
        m_ssaoKernelBuffer = device.createBufferSRV(BufferDescription()
            .elementSize(sizeof(Float4))
            .elements(m_ssaoKernel.size())
            .name("SSAO Kernel buffer")
            .setInitialData(m_ssaoKernel)
            .format(Format::R32G32B32A32_FLOAT)
        );
        m_ssaoBlurKernelBuffer = device.createBufferSRV(BufferDescription()
            .elementSize(sizeof(Float))
            .elements(m_ssaoBlurKernel.size())
            .name("SSAO Blur kernel buffer")
            .setInitialData(m_ssaoBlurKernel)
            .format(Format::R32_FLOAT)
        );

        m_ssaoPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
        m_ssaoPipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
        m_ssaoPipeline.setDepthStencilState(DepthStencilDescription().depthEnable(false));
        m_ssaoPipeline.ps.noiseTexture = m_ssaoNoiseTexture;
        m_ssaoPipeline.ps.ssaoSampler = device.createSampler(SamplerDescription()
            .textureAddressMode(TextureAddressMode::Wrap)
            .filter(Filter::Point)
        );
        m_ssaoPipeline.ps.depthSampler = device.createSampler(SamplerDescription().textureAddressMode(TextureAddressMode::Clamp).filter(Filter::Point));
        m_ssaoPipeline.ps.defaultSampler = device.createSampler(SamplerDescription().filter(Filter::Point));

        m_ssaoBlurPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
        m_ssaoBlurPipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
        m_ssaoBlurPipeline.setDepthStencilState(DepthStencilDescription().depthEnable(false));
        m_ssaoBlurPipeline.ps.imageSampler = device.createSampler(SamplerDescription().filter(engine::Filter::Point).textureAddressMode(TextureAddressMode::Clamp));

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

#ifdef EARLYZ_ENABLED
        m_earlyzPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_earlyzPipeline.setRasterizerState(RasterizerDescription());
        m_earlyzPipeline.setDepthStencilState(DepthStencilDescription()
            .depthEnable(true)
            .depthWriteMask(DepthWriteMask::All)
            .depthFunc(ComparisonFunction::Greater)
            .frontFace(front)
            .backFace(back));

        m_earlyzAlphaClipped.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_earlyzAlphaClipped.setRasterizerState(RasterizerDescription().cullMode(CullMode::None));
        m_earlyzAlphaClipped.ps.imageSampler = device.createSampler(SamplerDescription().filter(Filter::Point));
        m_earlyzAlphaClipped.setDepthStencilState(DepthStencilDescription()
            .depthEnable(true)
            .depthWriteMask(DepthWriteMask::All)
            .depthFunc(ComparisonFunction::Greater)
            .frontFace(front)
            .backFace(back));
#endif

        // basic
        m_pipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_pipeline.setRasterizerState(RasterizerDescription());
        m_pipeline.ps.tex_sampler = device.createSampler(SamplerDescription().filter(Filter::Bilinear));
        m_pipeline.ps.shadow_sampler = device.createSampler(SamplerDescription()
            .addressU(TextureAddressMode::Mirror)
            .addressV(TextureAddressMode::Mirror)
            .filter(Filter::Comparison));

        m_pipeline.setDepthStencilState(DepthStencilDescription()
            .depthEnable(true)
            .depthWriteMask(DepthWriteMask::Zero)
            .depthFunc(ComparisonFunction::Equal)
            .frontFace(front)
            .backFace(back));

        // pbr
        m_pbrPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_pbrPipeline.setRasterizerState(RasterizerDescription().cullMode(CullMode::Back));
        m_pbrPipeline.ps.tex_sampler = device.createSampler(SamplerDescription().filter(Filter::Bilinear));
        m_pbrPipeline.setDepthStencilState(DepthStencilDescription()
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


        m_pbrPipelineAlphaclipped.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_pbrPipelineAlphaclipped.setRasterizerState(RasterizerDescription().cullMode(CullMode::None));
        m_pbrPipelineAlphaclipped.ps.tex_sampler = device.createSampler(SamplerDescription().filter(Filter::Bilinear));
        m_pbrPipelineAlphaclipped.setDepthStencilState(DepthStencilDescription()
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

        // wireframe pipeline
        m_wireframePipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_wireframePipeline.setRasterizerState(RasterizerDescription()
            .cullMode(CullMode::Back)
            .depthBias(16384)
            .fillMode(FillMode::Wireframe));
        m_wireframePipeline.ps.tex_sampler = device.createSampler(SamplerDescription().filter(Filter::Bilinear));
        m_wireframePipeline.setDepthStencilState(DepthStencilDescription()
            .depthEnable(true)
#ifdef EARLYZ_ENABLED
            .depthWriteMask(DepthWriteMask::Zero)
            .depthFunc(ComparisonFunction::Greater)
#else
            .depthWriteMask(DepthWriteMask::All)
            .depthFunc(ComparisonFunction::Greater)
#endif
            .frontFace(front)
            .backFace(back));


        m_debugBoundingSpheres.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_debugBoundingSpheres.setRasterizerState(RasterizerDescription()
            .cullMode(CullMode::Back)
            .fillMode(FillMode::Wireframe));
        m_debugBoundingSpheres.setDepthStencilState(DepthStencilDescription()
            .depthEnable(true)
            .depthWriteMask(DepthWriteMask::All)
            .depthFunc(ComparisonFunction::Greater)
            .frontFace(front)
            .backFace(back));

        // lighting
        for (int i = 0; i < shaders::LightingPS::DrawmodeCount; ++i)
        {
            auto lightingPipe = device.createPipeline<shaders::Lighting>(shaderStorage);
            lightingPipe.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
            lightingPipe.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
            lightingPipe.setDepthStencilState(DepthStencilDescription().depthEnable(false));
            lightingPipe.ps.tex_sampler = device.createSampler(SamplerDescription().filter(Filter::Bilinear));
            lightingPipe.ps.tri_sampler = device.createSampler(SamplerDescription().filter(Filter::Trilinear));
            lightingPipe.ps.depth_sampler = device.createSampler(SamplerDescription().filter(Filter::Point));
            lightingPipe.ps.point_sampler = device.createSampler(SamplerDescription().filter(Filter::Point));
            lightingPipe.ps.gbufferNormals = m_gbuffer->srv(GBufferType::Normal);
            lightingPipe.ps.gbufferUV = m_gbuffer->srv(GBufferType::Uv);
            lightingPipe.ps.gbufferInstanceId = m_gbuffer->srv(GBufferType::InstanceId);
            lightingPipe.ps.frameSize.x = m_gbuffer->srv(GBufferType::Normal).width();
            lightingPipe.ps.frameSize.y = m_gbuffer->srv(GBufferType::Normal).height();
            lightingPipe.ps.instanceMaterials = device.modelResources().gpuBuffers().instanceMaterial();
            lightingPipe.ps.materialTextures = device.modelResources().textures();
            lightingPipe.ps.drawmode = static_cast<engine::shaders::LightingPS::Drawmode>(i);
            lightingPipe.ps.shadow_sampler = device.createSampler(SamplerDescription()
                .addressU(TextureAddressMode::Mirror)
                .addressV(TextureAddressMode::Mirror)
                .filter(Filter::Comparison));
            m_lightingPipelines.emplace_back(std::move(lightingPipe));
        }

        for (int i = 0; i < 2; ++i)
        {
            auto temporalPipe = device.createPipeline<shaders::TemporalResolve>(shaderStorage);
            temporalPipe.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
            temporalPipe.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
            temporalPipe.setDepthStencilState(DepthStencilDescription().depthEnable(false));
            temporalPipe.ps.pointSampler = device.createSampler(SamplerDescription().textureAddressMode(TextureAddressMode::Clamp).filter(Filter::Point));
            temporalPipe.ps.bilinearSampler = device.createSampler(SamplerDescription().filter(Filter::Bilinear));
            temporalPipe.ps.visualizeMotion = static_cast<bool>(i);
            m_temporalResolve.emplace_back(std::move(temporalPipe));
        }
    }

    void ModelRenderer::resize(uint32_t width, uint32_t height)
    {
        m_virtualResolution = { static_cast<int>(width), static_cast<int>(height) };
        m_renderWidth = m_virtualResolution.x;
        m_renderHeight = m_virtualResolution.y;

        m_renderOutline = std::make_unique<RenderOutline>(m_device, m_shaderStorage, m_virtualResolution);

        m_gbuffer = nullptr;
        m_gbuffer = std::make_shared<GBuffer>(m_device, m_renderWidth, m_renderHeight);

        m_dsv = m_device.createTextureDSV(TextureDescription()
            .name("Depth Buffer")
            .format(Format::D32_FLOAT)
            .width(m_renderWidth)
            .height(m_renderHeight)
            .usage(ResourceUsage::DepthStencil)
            .optimizedDepthClearValue(0.0f)
            .dimension(ResourceDimension::Texture2D)
        );
        m_dsvSRV = m_device.createTextureSRV(m_dsv);

        m_lightingTarget = m_device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::R16G16B16A16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("FullRes Target 0")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f }));

        m_lightingTargetSRV = m_device.createTextureSRV(m_lightingTarget.texture());

        m_fullResTargetFrame[0] = m_device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::R16G16B16A16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("FullRes Target 0")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f }));

        m_fullResTargetFrame[1] = m_device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::R16G16B16A16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("FullRes Target 1")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f }));
        
        m_fullResTargetFrameSRV[0] = m_device.createTextureSRV(m_fullResTargetFrame[0].texture());
        m_fullResTargetFrameSRV[1] = m_device.createTextureSRV(m_fullResTargetFrame[1].texture());
        
        m_currentFullResIndex = 0;
        m_lastResolvedIndex = 0;

        m_ssaoRTV = m_device.createTextureRTV(TextureDescription()
#ifdef SCALEAOSIZE
            .width(m_virtualResolution.x / SSAOresolutionScaleFactor)
            .height(m_virtualResolution.y / SSAOresolutionScaleFactor)
#else
            .width(SSAOWidth)
            .height(SSAOHeight)
#endif
            .format(Format::R16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("SSAO Render target")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 0.0f })
        );
        m_ssaoSRV = m_device.createTextureSRV(m_ssaoRTV.texture());

        for (auto&& lightingPipe : m_lightingPipelines)
        {
            lightingPipe.ps.gbufferNormals = m_gbuffer->srv(GBufferType::Normal);
            lightingPipe.ps.gbufferUV = m_gbuffer->srv(GBufferType::Uv);
            lightingPipe.ps.gbufferInstanceId = m_gbuffer->srv(GBufferType::InstanceId);
            lightingPipe.ps.instanceMaterials = m_device.modelResources().gpuBuffers().instanceMaterial();
            lightingPipe.ps.materialTextures = m_device.modelResources().textures();
        }
    }

    void ModelRenderer::clearResources()
    {
        m_pipeline.ps.lightWorldPosition = BufferSRV();
        m_pipeline.ps.lightDirection = BufferSRV();
        m_pipeline.ps.lightColor = BufferSRV();
        m_pipeline.ps.lightIntensity = BufferSRV();
        m_pipeline.ps.lightRange = BufferSRV();
        m_pipeline.ps.lightParameters = BufferSRV();

        m_pipeline.vs.vertices = BufferSRV();
        m_pipeline.vs.normals = BufferSRV();
        m_pipeline.vs.tangents = BufferSRV();
        m_pipeline.vs.uv = BufferSRV();

        m_pipeline.ps.albedo = TextureSRV();
        m_pipeline.ps.roughness = TextureSRV();
        m_pipeline.ps.normal = TextureSRV();
        m_pipeline.ps.metalness = TextureSRV();
        m_pipeline.ps.occlusion = TextureSRV();

        m_pbrPipeline.vs.vertices = BufferSRV();
        m_pbrPipeline.vs.normals = BufferSRV();
        m_pbrPipeline.vs.tangents = BufferSRV();
        m_pbrPipeline.vs.uv = BufferSRV();
        
        m_pbrPipelineAlphaclipped.vs.vertices = BufferSRV();
        m_pbrPipelineAlphaclipped.vs.normals = BufferSRV();
        m_pbrPipelineAlphaclipped.vs.tangents = BufferSRV();
        m_pbrPipelineAlphaclipped.vs.uv = BufferSRV();

        for (auto&& lightingPipe : m_lightingPipelines)
        {
            lightingPipe.ps.environmentIrradianceCubemap = TextureSRV();
            lightingPipe.ps.environmentIrradiance = TextureSRV();
            lightingPipe.ps.environmentSpecular = TextureSRV();
            lightingPipe.ps.environmentBrdfLut = TextureSRV();
            lightingPipe.ps.gbufferNormals = TextureSRV();
            lightingPipe.ps.gbufferUV = TextureSRV();
            lightingPipe.ps.gbufferInstanceId = TextureSRV();
            lightingPipe.ps.instanceMaterials = BufferSRV();
            lightingPipe.ps.materialTextures = TextureBindlessSRV();
            lightingPipe.ps.shadowMap = TextureSRV();
            lightingPipe.ps.shadowVP = BufferSRV();
            lightingPipe.ps.lightWorldPosition = BufferSRV();
            lightingPipe.ps.lightDirection = BufferSRV();
            lightingPipe.ps.lightColor = BufferSRV();
            lightingPipe.ps.lightParameters = BufferSRV();
            lightingPipe.ps.lightType = BufferSRV();
            lightingPipe.ps.lightIntensity = BufferSRV();
            lightingPipe.ps.lightRange = BufferSRV();
        }

#ifdef EARLYZ_ENABLED
        m_earlyzPipeline.vs.vertices = BufferSRV();
        m_earlyzAlphaClipped.vs.vertices = BufferSRV();
        m_earlyzAlphaClipped.vs.uv = BufferSRV();
        m_earlyzAlphaClipped.ps.image = TextureSRV();
#endif
    }

    void ModelRenderer::render(
        Device& device,
        TextureRTV& currentRenderTarget,
        DepthPyramid& depthPyramid,
        CommandList& cmd,
        Camera& camera,
        FlatScene& scene,
        TextureSRV& shadowMap,
        BufferSRV& shadowVP,
        unsigned int mouseX,
        unsigned int mouseY
        )
    {
        bool open{ true };

        float w = static_cast<float>(m_device.width());
        float h = static_cast<float>(m_device.height());

        float logWidth = 150.0f;
        float logHeight = 70.0f;

        ImGui::SetNextWindowSizeConstraints(ImVec2(10.0f, 10.0f), ImVec2(logWidth, logHeight));
        if (ImGui::Begin("DebugMenu", &open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
        {
            ImGui::SetWindowSize(ImVec2(logWidth, logHeight), ImGuiSetCond_Always);
            ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));

            if (ImGui::TreeNode("Rendering"))
            {
                ImGui::Combo("Mode", &m_currentRenderMode, "Full\0Clusters\0MipAlbedo\0MipRoughness\0MipMetalness\0MipAO\0Albedo\0Roughness\0Metalness\0Occlusion\0Uv\0Normal\0Motion\0");
                ImGui::TreePop();
            }
        }
        ImGui::End();

        if (scene.selectedCamera == -1 || scene.cameras.size() == 0 || scene.selectedCamera > scene.cameras.size() - 1 || !scene.cameras[scene.selectedCamera])
            return;


        //renderBoxes(scene, { m_clusterRenderer.selectedClusterBoundingBox(device) }, 0x881080FF);


        LightData& lights = *scene.lightData;

        auto frameNumber = device.frameNumber();

        auto viewMatrix = camera.viewMatrix();
        auto projectionMatrix = camera.projectionMatrix(m_virtualResolution);
        auto jitterMatrix = camera.jitterMatrix(frameNumber, m_virtualResolution);
        auto jitterValue = camera.jitterValue(frameNumber);

        // for debugging
#if 0
        {
            cmd.clearDepthStencilView(depthPyramid.dsv(), 0.0f);

            m_culler.frustumCull(cmd, 
                (scene.cameras.size() == 1) ? camera : *scene.cameras[1], 
                device.modelResources(), 
                frameNumber, 
                m_virtualResolution);

            m_culler.occlusionCull(cmd, 
                (scene.cameras.size() == 1) ? camera : *scene.cameras[1], 
                device.modelResources(), 
                &depthPyramid, 
                frameNumber, 
                m_virtualResolution,
                CullingMode::EmitAll);

            if (scene.cameras.size() > 1)
            {
                {
                    CPU_MARKER("Render PBR geometry");
                    GPU_MARKER(cmd, "Render PBR geometry");
                    m_clusterRenderer.render(
                        cmd,
                        camera,
                        device.modelResources(),
                        m_gbuffer.get(),
                        &m_dsv,
                        m_culler.occlusionCulledClusters(),
                        m_culler.occlusionCullingDrawArguments(),
                        device.frameNumber(),
                        m_virtualResolution,
                        &m_pickBufferUAV,
                        mouseX, mouseY,
                        m_previousCameraViewMatrix,
                        m_previousCameraProjectionMatrix);
                }

                {
                    CPU_MARKER("Render PBR geometry to secondary camera");
                    GPU_MARKER(cmd, "Render PBR geometry to secondary camera");
                    m_clusterRenderer.render(
                        cmd,
                        *scene.cameras[1],
                        device.modelResources(),
                        nullptr,
                        &depthPyramid.dsv(),
                        m_culler.occlusionCulledClusters(),
                        m_culler.occlusionCullingDrawArguments(),
                        device.frameNumber(),
                        m_virtualResolution,
                        &m_pickBufferUAV,
                        mouseX, mouseY,
                        m_previousCameraViewMatrix,
                        m_previousCameraProjectionMatrix);
                }
            }
            else
            {
                CPU_MARKER("Render PBR geometry");
                GPU_MARKER(cmd, "Render PBR geometry");
                m_clusterRenderer.render(
                    cmd,
                    camera,
                    device.modelResources(),
                    m_gbuffer.get(),
                    &depthPyramid.dsv(),
                    m_culler.occlusionCulledClusters(),
                    m_culler.occlusionCullingDrawArguments(),
                    device.frameNumber(),
                    m_virtualResolution,
                    &m_pickBufferUAV,
                    mouseX, mouseY,
                    m_previousCameraViewMatrix,
                    m_previousCameraProjectionMatrix);
            }
        }
#endif

        depthPyramid.performDownsample(cmd);

        // culling
        {
            m_culler.frustumCull(
                cmd,
                (scene.cameras.size() == 1) ? camera : *scene.cameras[1],
                device.modelResources(),
                frameNumber,
                m_virtualResolution);

            m_culler.occlusionCull(
                cmd,
                (scene.cameras.size() == 1) ? camera : *scene.cameras[1],
                device.modelResources(),
                &depthPyramid,
                frameNumber,
                m_virtualResolution,
                CullingMode::EmitVisible);
        }
        
        m_gbuffer->clear(cmd);
        
        cmd.clearDepthStencilView(depthPyramid.dsv(), 0.0f);

        TextureDSV* activeDSV;
        TextureSRV* activeDSVSRV;

        if (scene.cameras.size() > 1)
        {
            activeDSV = &m_dsv;
            activeDSVSRV = &m_dsvSRV;
        }
        else
        {
            activeDSV = &depthPyramid.dsv();
            activeDSVSRV = &depthPyramid.srv();
        }
        
        cmd.clearDepthStencilView(*activeDSV, 0.0f);

        // Early Z render. Device fast path.
        renderEarlyZ(device, depthPyramid.dsv(), cmd, scene, camera, projectionMatrix, viewMatrix, jitterMatrix);

        // render models and lighting
        if (!camera.pbrShadingModel())
        {
            // forward renderer
            renderGeometry(device, currentRenderTarget, depthPyramid.dsv(), cmd, scene, projectionMatrix, viewMatrix, camera, lights, shadowMap, shadowVP);
        }
        else
        {
            // deferred physically based renderer
            // render model data to G-Buffer (albedo, normal, roughness, metalness, occlusion)
            if (scene.cameras.size() > 1)
            {
                {
                    CPU_MARKER("Render PBR geometry");
                    GPU_MARKER(cmd, "Render PBR geometry");
                    m_clusterRenderer.render(
                        cmd,
                        camera,
                        device.modelResources(),
                        m_gbuffer.get(),
                        &m_dsv,
                        m_culler.occlusionCulledClusters(),
                        m_culler.occlusionCullingDrawArguments(),
                        device.frameNumber(),
                        m_virtualResolution,
                        m_previousCameraViewMatrix,
                        m_previousCameraProjectionMatrix,
                        m_currentRenderMode < shaders::LightingPS::DrawmodeCount ? m_currentRenderMode : 0);
                }
                
                {
                    CPU_MARKER("Render PBR geometry to secondary camera");
                    GPU_MARKER(cmd, "Render PBR geometry to secondary camera");
                    m_clusterRenderer.render(
                        cmd,
                        *scene.cameras[1],
                        device.modelResources(),
                        nullptr,
                        &depthPyramid.dsv(),
                        m_culler.occlusionCulledClusters(),
                        m_culler.occlusionCullingDrawArguments(),
                        device.frameNumber(),
                        m_virtualResolution,
                        m_previousCameraViewMatrix,
                        m_previousCameraProjectionMatrix,
                        m_currentRenderMode < shaders::LightingPS::DrawmodeCount ? m_currentRenderMode : 0);
                }
            }
            else
            {
                CPU_MARKER("Render PBR geometry");
                GPU_MARKER(cmd, "Render PBR geometry");
                m_clusterRenderer.render(
                    cmd,
                    camera,
                    device.modelResources(),
                    m_gbuffer.get(),
                    &depthPyramid.dsv(),
                    m_culler.occlusionCulledClusters(),
                    m_culler.occlusionCullingDrawArguments(),
                    device.frameNumber(),
                    m_virtualResolution,
                    m_previousCameraViewMatrix,
                    m_previousCameraProjectionMatrix,
                    m_currentRenderMode < shaders::LightingPS::DrawmodeCount ? m_currentRenderMode : 0);
            }
            
            m_picker.pick(m_gbuffer.get(), cmd, mouseX, mouseY);

            // SSAO
            renderSSAO(device, cmd, *activeDSVSRV, camera);
            
            // lighting
            Vector3f probePosition;
            float probeRange = 100.0f;
            if (scene.probes.size() > 0)
            {
                ProbeComponent& probe = *scene.probes[0];
                probePosition = probe.position();
                probeRange = probe.range();
            }

            renderLighting(device, cmd, currentRenderTarget, *activeDSVSRV, camera, shadowMap, shadowVP, projectionMatrix, viewMatrix, lights, probePosition, probeRange);

            // wireframe
            renderWireframe(device, currentRenderTarget, *activeDSV, cmd, scene, projectionMatrix, viewMatrix, jitterMatrix, mouseX, mouseY);

            // render particles
            //m_particleTest.render(device, cmd, m_lightingTarget, *activeDSVSRV, camera, projectionMatrix, viewMatrix, jitterMatrix, lights, shadowMap, shadowVP);

            // render outline
            if (m_selectedObject != -1)
            {
                CPU_MARKER("Outline");
                GPU_MARKER(cmd, "Outline");

                for (auto& node : scene.nodes)
                {
                    //if (node.objectId == m_selectedObject)
                    if(node.mesh->meshBuffer().modelAllocations && node.mesh->meshBuffer().modelAllocations->subMeshInstance->instanceData.modelResource.gpuIndex == m_selectedObject)
                    {
                        m_renderOutline->render(device,
                            m_lightingTarget,
                            *activeDSVSRV,
                            camera,
                            device.modelResources(),
                            cmd,
                            node
                        );
                    }
                }
                for (auto& node : scene.alphaclippedNodes)
                {
                    //if (node.objectId == m_selectedObject)
                    if (node.mesh->meshBuffer().modelAllocations && node.mesh->meshBuffer().modelAllocations->subMeshInstance->instanceData.modelResource.gpuIndex == m_selectedObject)
                    {
                        m_renderOutline->render(device,
                            m_lightingTarget,
                            *activeDSVSRV,
                            camera,
                            device.modelResources(),
                            cmd,
                            node
                        );
                    }
                }
            }

            // remporal resolve
            renderTemporalResolve(cmd, *activeDSVSRV, camera, jitterValue, m_previousJitter, jitterMatrix);
        }

        m_previousCameraViewMatrix = viewMatrix;
        m_previousCameraProjectionMatrix = projectionMatrix;
        m_previousJitterMatrix = jitterMatrix;
        m_previousJitter = jitterValue;
    }

    void ModelRenderer::renderBoundingSpheres(
        Device& /*device*/,
        TextureRTV& /*currentRenderTarget*/,
        TextureDSV& /*depthBuffer*/,
        CommandList& /*cmd*/,
        const Matrix4f& /*cameraProjectionMatrix*/,
        const Matrix4f& /*cameraViewMatrix*/,
        const Matrix4f& /*jitterMatrix*/)
    {
#if 0
        m_gbuffer->clear(cmd);

        cmd.setRenderTargets({
            m_gbuffer->rtv() }, depthBuffer);
            /*m_gbuffer->rtv(GBufferType::Albedo),
            m_gbuffer->rtv(GBufferType::Normal),
            m_gbuffer->rtv(GBufferType::MotionVector),
            m_gbuffer->rtv(GBufferType::Roughness),
            m_gbuffer->rtv(GBufferType::Metalness),
            m_gbuffer->rtv(GBufferType::Occlusion),
            m_gbuffer->rtv(GBufferType::ObjectId) }, depthBuffer);*/

        //m_debugBoundingSpheres.vs.vertices = device.modelResources().vertex();
        //m_debugBoundingSpheres.vs.indices = device.modelResources().indice();
        m_debugBoundingSpheres.vs.clusterBindings = device.modelResources().bindings();
        m_debugBoundingSpheres.vs.transformHistory = device.modelResources().transforms();
        m_debugBoundingSpheres.vs.boundingBoxes = m_device.modelResources().boundingBoxes();

        m_debugBoundingSpheres.vs.viewMatrix = fromMatrix(cameraViewMatrix);
        m_debugBoundingSpheres.vs.projectionMatrix = fromMatrix(cameraProjectionMatrix);
        m_debugBoundingSpheres.vs.jitterMatrix = fromMatrix(jitterMatrix);
        //m_debugBoundingSpheres.vs.clusterMap = m_occlusionCullingOutputSRV;

        //cmd.bindIndexBuffer(m_occlusionCullingIndexOutputIBV);
        cmd.bindPipe(m_debugBoundingSpheres);
        //cmd.drawIndexedIndirect(m_occlusionCullingDrawArgs, 0);
        cmd.draw(device.modelResources().clusterCount() * 6 * 3 * 2);
#endif
    }

    void ModelRenderer::renderEarlyZ(
#ifdef EARLYZ_ENABLED
        Device& device,
        TextureDSV& depthBuffer,
        CommandList& cmd,
        FlatScene& scene,
        Camera& camera,
        const Matrix4f& cameraProjectionMatrix,
        const Matrix4f& cameraViewMatrix,
        const Matrix4f& jitterMatrix)
#else
        Device&,
        TextureDSV&,
        CommandList&,
        FlatScene&,
        Camera&,
        const Matrix4f&,
        const Matrix4f&,
        const Matrix4f&)
#endif
    {
        
#ifdef EARLYZ_ENABLED
        CPU_MARKER("Early Z");
        GPU_MARKER(cmd, "Early Z");

        std::vector<Vector4f> frustumPlanes;
        Vector3f cameraPosition;
        Camera* camTemp;

        auto tempFrustumPlanes = extractFrustumPlanes(camera.projectionMatrix() * camera.viewMatrix());

        if (scene.cameras.size() == 1)
        {
            frustumPlanes = tempFrustumPlanes;
            cameraPosition = camera.position();
            camTemp = &camera;
        }
        else if (scene.cameras.size() == 2)
        {
            frustumPlanes = extractFrustumPlanes(scene.cameras[1]->projectionMatrix() * scene.cameras[1]->viewMatrix());
            cameraPosition = scene.cameras[1]->position();
            camTemp = scene.cameras[1].get();
        }

        {
            // early Z for opaque meshes
            cmd.setRenderTargets({}, depthBuffer);

            {
                m_earlyzPipeline.vs.vertices = device.modelResources().vertex();
                m_earlyzPipeline.vs.indices = device.modelResources().indice();
                m_earlyzPipeline.vs.clusterBindings = device.modelResources().bindings();
                m_earlyzPipeline.vs.transformHistory = device.modelResources().transforms();

                m_earlyzPipeline.vs.viewMatrix = fromMatrix(cameraViewMatrix);
                m_earlyzPipeline.vs.projectionMatrix = fromMatrix(cameraProjectionMatrix);
                m_earlyzPipeline.vs.jitterMatrix = fromMatrix(jitterMatrix);
                m_earlyzPipeline.vs.clusterMap = m_culler.occlusionCulledClusters();

                //cmd.bindIndexBuffer(m_occlusionCullingIndexOutputIBV);
                cmd.bindPipe(m_earlyzPipeline);
                //cmd.drawIndexedIndirect(m_occlusionCullingDrawArgs, 0);
                cmd.drawIndirect(m_culler.occlusionCullingDrawArguments(), 0);
            }

        }
#endif
    }

    void ModelRenderer::renderGeometry(
        Device& /*device*/,
        TextureRTV& /*currentRenderTarget*/,
        TextureDSV& /*depthBuffer*/,
        CommandList& /*cmd*/,
        FlatScene& /*scene*/,
        Matrix4f /*cameraProjectionMatrix*/,
        Matrix4f /*cameraViewMatrix*/,
        Camera& /*camera*/,
        LightData& /*lights*/,
        TextureSRV& /*shadowMap*/,
        BufferSRV& /*shadowVP*/)
    {
#if 0
        CPU_MARKER("Render forward geometry");
        GPU_MARKER(cmd, "Render forward geometry");

        cmd.setRenderTargets({ currentRenderTarget }, depthBuffer);

        m_pipeline.ps.cameraWorldSpacePosition = Float4(camera.position(), 1.0f);
        m_pipeline.ps.environmentIrradiance = camera.environmentIrradiance();
        m_pipeline.ps.environmentSpecular = camera.environmentSpecular();
        m_pipeline.ps.shadowMap = shadowMap;

        m_pipeline.ps.lightCount.x = static_cast<unsigned int>(lights.count());
        if (lights.count() > 0)
        {
            m_pipeline.ps.lightWorldPosition = lights.worldPositions();
            m_pipeline.ps.lightDirection = lights.directions();
            m_pipeline.ps.lightColor = lights.colors();
            m_pipeline.ps.lightIntensity = lights.intensities();
            m_pipeline.ps.lightRange = lights.ranges();
            m_pipeline.ps.lightType = lights.types();
            m_pipeline.ps.lightParameters = lights.parameters();
        }

        for (auto& node : scene.nodes)
        {
            auto& subMesh = node.mesh->meshBuffer();
            {
                auto transformMatrix = node.transform;

                m_pipeline.vs.modelMatrix = fromMatrix(transformMatrix);
                m_pipeline.vs.modelViewProjectionMatrix = fromMatrix(cameraProjectionMatrix * (cameraViewMatrix * transformMatrix));
                m_pipeline.vs.vertices = subMesh.vertices;
                m_pipeline.vs.normals = subMesh.normals;
                m_pipeline.vs.tangents = subMesh.tangents;
                m_pipeline.vs.uv = subMesh.uv;

                if (node.material)
                {
                    if (node.material->hasAlbedo())
                        m_pipeline.ps.albedo = node.material->albedo();

                    if (node.material->hasRoughness())
                        m_pipeline.ps.roughness = node.material->roughness();

                    if (node.material->hasNormal())
                        m_pipeline.ps.normal = node.material->normal();

                    if (node.material->hasMetalness())
                        m_pipeline.ps.metalness = node.material->metalness();

                    if (node.material->hasOcclusion())
                    m_pipeline.ps.occlusion = node.material->occlusion();
                }

                m_pipeline.ps.materialParameters = Float4(
                    node.material ? node.material->roughnessStrength() : 1.0f,
                    node.material ? node.material->metalnessStrength() : 0.0f,
                    node.material ? node.material->materialScaleX() : 1.0f,
                    node.material ? node.material->materialScaleY() : 1.0f);

                m_pipeline.ps.materialParameters2 = Float4(
                    node.material ? node.material->occlusionStrength() : 0.0f,
                    camera.exposure(),
                    0.0f,
                    0.0f
                );

                cmd.bindPipe(m_pipeline);
                cmd.drawIndexed(subMesh.indices, subMesh.indices.desc().elements, 1, 0, 0, 0);
            }
        }
#endif
    }

    unsigned int ModelRenderer::pickedObject(Device& device)
    {
        return m_picker.selectedInstanceId(device);
    }

    void ModelRenderer::renderGeometryPbr(
        Device& /*device*/,
        TextureDSV* /*depthBuffer*/,
        CommandList& /*cmd*/,
        FlatScene& /*scene*/,
        Camera& /*camera*/,
        const Matrix4f& /*cameraProjectionMatrix*/,
        const Matrix4f& /*cameraViewMatrix*/,
        const Matrix4f& /*jitterMatrix*/,
        unsigned int /*mouseX*/,
        unsigned int /*mouseY*/,
        const Vector2f& /*jitter*/)
    {
        

        

#if 0
        std::vector<Vector4f> frustumPlanes;
        Vector3f cameraPosition;
        Camera* camTemp;

        auto tempFrustumPlanes = extractFrustumPlanes(camera.projectionMatrix() * camera.viewMatrix());

        if (scene.cameras.size() == 1)
        {
            frustumPlanes = tempFrustumPlanes;
            cameraPosition = camera.position();
            camTemp = &camera;
        }
        else if (scene.cameras.size() == 2)
        {
            frustumPlanes = extractFrustumPlanes(scene.cameras[1]->projectionMatrix() * scene.cameras[1]->viewMatrix());
            cameraPosition = scene.cameras[1]->position();
            camTemp = scene.cameras[1].get();
        }

        //m_gbuffer->clear(cmd);

        cmd.setRenderTargets({ 
            m_gbuffer->rtv(GBufferType::Albedo),
            m_gbuffer->rtv(GBufferType::Normal),
            m_gbuffer->rtv(GBufferType::MotionVector),
            m_gbuffer->rtv(GBufferType::Roughness),
            m_gbuffer->rtv(GBufferType::Metalness),
            m_gbuffer->rtv(GBufferType::Occlusion),
            m_gbuffer->rtv(GBufferType::ObjectId) }, depthBuffer);

        m_pbrPipeline.ps.mouseX.x = mouseX;
        m_pbrPipeline.ps.mouseY.x = mouseY;
        m_pbrPipeline.ps.pick = m_pickBufferUAV;

        m_pbrPipelineAlphaclipped.ps.mouseX.x = mouseX;
        m_pbrPipelineAlphaclipped.ps.mouseY.x = mouseY;
        m_pbrPipelineAlphaclipped.ps.pick = m_pickBufferUAV;

        {
            CPU_MARKER("Update material textures");
            /*for (auto& node : scene.nodes)
            {
                if (node.material)
                {
                    // materialTextures is an array
                    // no texture occurs in the array twice as each texture has a unique id
                    // when some shader accesses the list, it has the material index to the array based on the key
                    if (node.material->hasAlbedo() && m_pbrPipeline.ps.materialTextures.id(node.material->albedoKey()) == -1)
                        m_pbrPipeline.ps.materialTextures.push(node.material->albedoKey(), node.material->albedo());
                    if (node.material->hasNormal() && m_pbrPipeline.ps.materialTextures.id(node.material->normalKey()) == -1)
                        m_pbrPipeline.ps.materialTextures.push(node.material->normalKey(), node.material->normal());
                    if (node.material->hasRoughness() && m_pbrPipeline.ps.materialTextures.id(node.material->roughnessKey()) == -1)
                        m_pbrPipeline.ps.materialTextures.push(node.material->roughnessKey(), node.material->roughness());
                    if (node.material->hasMetalness() && m_pbrPipeline.ps.materialTextures.id(node.material->metalnessKey()) == -1)
                        m_pbrPipeline.ps.materialTextures.push(node.material->metalnessKey(), node.material->metalness());
                    if (node.material->hasOcclusion() && m_pbrPipeline.ps.materialTextures.id(node.material->occlusionKey()) == -1)
                        m_pbrPipeline.ps.materialTextures.push(node.material->occlusionKey(), node.material->occlusion());
                }
            }*/
        }

        {
            CPU_MARKER("Render opaque models");
            GPU_MARKER(cmd, "Render opaque models");

            m_clusterRenderer.vs.vertices = device.modelResources().vertex();
            m_clusterRenderer.vs.normals = device.modelResources().normal();
            m_clusterRenderer.vs.tangents = device.modelResources().tangent();
            m_clusterRenderer.vs.indices = device.modelResources().indice();
            m_clusterRenderer.vs.uv = device.modelResources().uv();

            m_clusterRenderer.vs.clusterBindings = device.modelResources().bindings();
            m_clusterRenderer.vs.clusterMaterials = device.modelResources().material();
            m_clusterRenderer.vs.transformHistory = device.modelResources().transforms();

            m_clusterRenderer.vs.viewMatrix = fromMatrix(cameraViewMatrix);
            m_clusterRenderer.vs.projectionMatrix = fromMatrix(cameraProjectionMatrix);
            m_clusterRenderer.vs.previousViewMatrix = fromMatrix(m_previousCameraViewMatrix);
            m_clusterRenderer.vs.previousProjectionMatrix = fromMatrix(m_previousCameraProjectionMatrix);
            m_clusterRenderer.vs.jitterMatrix = fromMatrix(jitterMatrix);
            m_clusterRenderer.vs.clusterMap = m_culler.occlusionCulledClusters();

            m_clusterRenderer.ps.clusterBindings = device.modelResources().bindings();
            m_clusterRenderer.ps.clusterMaterials = device.modelResources().material();
            m_clusterRenderer.ps.materialTextures = device.modelResources().textures();
            
            m_clusterRenderer.ps.mouseX.x = mouseX;
            m_clusterRenderer.ps.mouseY.x = mouseY;
            m_clusterRenderer.ps.materialIdToObjectId = device.modelResources().materialToObjectId();
            m_clusterRenderer.ps.pick = m_pickBufferUAV;

            /*cmd.bindPipe(m_clusterRenderer);
            cmd.draw(device.modelResources().clusterCount() * ClusterMaxSize);*/
            //cmd.bindIndexBuffer(m_occlusionCullingIndexOutputIBV);
            cmd.bindPipe(m_clusterRenderer);
            //cmd.drawIndexedIndirect(m_occlusionCullingDrawArgs, 0);
            cmd.drawIndirect(m_culler.occlusionCullingDrawArguments(), 0);
        }


#if 0
        {
            CPU_MARKER("Render opaque models");
            GPU_MARKER(cmd, "Render opaque models");

            std::uniform_real_distribution<float> randomFloat(0.0f, 1.0f);
            std::default_random_engine gen;
            // model render pass
            for (auto& node : scene.nodes)
            {
                auto& subMesh = node.mesh->meshBuffer();
                {
                    auto objectBounds = node.mesh->subMesh().boundingBox;

                    if (frustumCull(*camTemp, frustumPlanes, objectBounds, camTemp->position(), node.transform))
                    {
                        const auto& transformMatrix = node.transform;
                        const auto& previousTransformMatrix = node.previousTransform;

                        const auto cameraTransform = cameraViewMatrix * transformMatrix;

                        m_pbrPipeline.vs.vertices = subMesh.vertices;
                        m_pbrPipeline.vs.normals = subMesh.normals;
                        m_pbrPipeline.vs.tangents = subMesh.tangents;
                        m_pbrPipeline.vs.uv = subMesh.uv;

                        m_pbrPipeline.vs.modelMatrix = fromMatrix(transformMatrix);
                        m_pbrPipeline.vs.modelViewProjectionMatrix = fromMatrix(cameraProjectionMatrix * cameraTransform);
                        m_pbrPipeline.vs.previousModelViewProjectionMatrix = fromMatrix(m_previousCameraProjectionMatrix * (m_previousCameraViewMatrix * previousTransformMatrix));
                        m_pbrPipeline.vs.jitterModelViewProjectionMatrix = fromMatrix(jitterMatrix * (cameraProjectionMatrix * cameraTransform));

                        node.transform = node.previousTransform;

                        if (node.material)
                        {
                            /*m_pbrPipeline.ps.albedoId.x = node.material->hasAlbedo() ? m_pbrPipeline.ps.materialTextures.id(node.material->albedoKey()) : 0;
                            m_pbrPipeline.ps.normalId.x = node.material->hasNormal() ? m_pbrPipeline.ps.materialTextures.id(node.material->normalKey()) : 0;
                            m_pbrPipeline.ps.roughnessId.x = node.material->hasRoughness() ? m_pbrPipeline.ps.materialTextures.id(node.material->roughnessKey()) : 0;
                            m_pbrPipeline.ps.metalnessId.x = node.material->hasMetalness() ? m_pbrPipeline.ps.materialTextures.id(node.material->metalnessKey()) : 0;
                            m_pbrPipeline.ps.occlusionId.x = node.material->hasOcclusion() ? m_pbrPipeline.ps.materialTextures.id(node.material->occlusionKey()) : 0;*/

                            m_pbrPipeline.ps.albedo = node.material->hasAlbedo() ? node.material->albedo() : TextureSRV();
                            m_pbrPipeline.ps.normal = node.material->hasNormal() ? node.material->normal() : TextureSRV();
                            m_pbrPipeline.ps.roughness = node.material->hasRoughness() ? node.material->roughness() : TextureSRV();
                            m_pbrPipeline.ps.metalness = node.material->hasMetalness() ? node.material->metalness() : TextureSRV();
                            m_pbrPipeline.ps.occlusion = node.material->hasOcclusion() ? node.material->occlusion() : TextureSRV();

                            auto color = node.material->color();
                            m_pbrPipeline.ps.color = Vector4f(color.x, color.y, color.z, 1.0f);
                        }

                        m_pbrPipeline.ps.roughnessStrength = node.material ? node.material->roughnessStrength() : 1.0f;
                        m_pbrPipeline.ps.metalnessStrength = node.material ? node.material->metalnessStrength() : 0.0f;
                        m_pbrPipeline.ps.occlusionStrength = node.material ? node.material->occlusionStrength() : 0.0f;

                        m_pbrPipeline.ps.hasAlbedo.x = static_cast<unsigned int>(node.material && node.material->hasAlbedo());
                        m_pbrPipeline.ps.hasNormal.x = static_cast<unsigned int>(node.material && node.material->hasNormal());
                        m_pbrPipeline.ps.hasRoughness.x = static_cast<unsigned int>(node.material && node.material->hasRoughness());
                        m_pbrPipeline.ps.hasMetalness.x = static_cast<unsigned int>(node.material && node.material->hasMetalness());
                        m_pbrPipeline.ps.hasOcclusion.x = static_cast<unsigned int>(node.material && node.material->hasOcclusion());

                        m_pbrPipeline.ps.materialScale = Float2(
                            node.material ? node.material->materialScaleX() : 1.0f,
                            node.material ? node.material->materialScaleY() : 1.0f);
                        m_pbrPipeline.ps.objectId.x = node.objectId;
                        m_pbrPipeline.ps.jitter = Float2{ jitter.x, jitter.y };


                        cmd.bindPipe(m_pbrPipeline);
                        cmd.drawIndexed(subMesh.indices, subMesh.indices.desc().elements, 1, 0, 0, 0);

                        /*int processedIndexes = 0;
                        while (processedIndexes < subMesh.indices.desc().elements)
                        {
                            int processNow = min(subMesh.indices.desc().elements - processedIndexes, 3 * 256);
                            m_pbrPipeline.ps.color = Vector4f(randomFloat(gen), randomFloat(gen), randomFloat(gen), 1.0f);
                            cmd.bindPipe(m_pbrPipeline);
                            cmd.bindIndexBuffer(subMesh.indices);
                            cmd.drawIndexed(processNow, 1, processedIndexes, 0, 0);
                            processedIndexes += processNow;
                        }*/

                        /*m_culling.cs.input = m_computeInput;
                        m_culling.cs.output = m_computeOutput;
                        cmd.bindPipe(m_culling);
                        cmd.dispatch(1, 0, 0);*/

#if 0
                        uint32_t facesProcessed = 0;
                        uint32_t clusterId = 0;
                        for (auto&& cluster : node.mesh->subMesh().clusterIndexCount)
                        {
                            auto clusterBounds = node.mesh->subMesh().clusterBounds[clusterId];

                            /*{
                                Vector3f corner[8] =
                                {
                                    clusterBounds.min,
                                    Vector3f{ clusterBounds.max.x, clusterBounds.min.y, clusterBounds.min.z },
                                    Vector3f{ clusterBounds.max.x, clusterBounds.min.y, clusterBounds.max.z },
                                    Vector3f{ clusterBounds.min.x, clusterBounds.min.y, clusterBounds.max.z },

                                    Vector3f{ clusterBounds.min.x, clusterBounds.max.y, clusterBounds.min.z },
                                    Vector3f{ clusterBounds.max.x, clusterBounds.max.y, clusterBounds.min.z },
                                    clusterBounds.max,
                                    Vector3f{ clusterBounds.min.x, clusterBounds.max.y, clusterBounds.max.z }
                                };

                                ImGuiIO& io = ImGui::GetIO();
                                ImGui::Begin("another hud", NULL, io.DisplaySize, 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);

                                ImDrawList* drawList = ImGui::GetWindowDrawList();

                                ImGui::End();


                                auto mvp = camera.projectionMatrix() * camera.viewMatrix();

                                drawLine3d(camera, tempFrustumPlanes, mvp, drawList, corner[0], corner[1], 0xffaa0000, 1.0f);
                                drawLine3d(camera, tempFrustumPlanes, mvp, drawList, corner[1], corner[2], 0xffaa0000, 1.0f);
                                drawLine3d(camera, tempFrustumPlanes, mvp, drawList, corner[2], corner[3], 0xffaa0000, 1.0f);
                                drawLine3d(camera, tempFrustumPlanes, mvp, drawList, corner[3], corner[0], 0xffaa0000, 1.0f);

                                drawLine3d(camera, tempFrustumPlanes, mvp, drawList, corner[4], corner[5], 0xffaa0000, 1.0f);
                                drawLine3d(camera, tempFrustumPlanes, mvp, drawList, corner[5], corner[6], 0xffaa0000, 1.0f);
                                drawLine3d(camera, tempFrustumPlanes, mvp, drawList, corner[6], corner[7], 0xffaa0000, 1.0f);
                                drawLine3d(camera, tempFrustumPlanes, mvp, drawList, corner[7], corner[4], 0xffaa0000, 1.0f);

                                drawLine3d(camera, tempFrustumPlanes, mvp, drawList, corner[0], corner[4], 0xffaa0000, 1.0f);
                                drawLine3d(camera, tempFrustumPlanes, mvp, drawList, corner[1], corner[5], 0xffaa0000, 1.0f);
                                drawLine3d(camera, tempFrustumPlanes, mvp, drawList, corner[2], corner[6], 0xffaa0000, 1.0f);
                                drawLine3d(camera, tempFrustumPlanes, mvp, drawList, corner[3], corner[7], 0xffaa0000, 1.0f);
                            }*/



                            if (frustumCull(
                                *camTemp,
                                frustumPlanes,
                                clusterBounds,
                                cameraPosition,
                                node.transform))
                            {
                                m_pbrPipeline.ps.color = Vector4f(randomFloat(gen), randomFloat(gen), randomFloat(gen), 1.0f);

                                cmd.bindPipe(m_pbrPipeline);
                                cmd.bindIndexBuffer(subMesh.indices);
                                cmd.drawIndexed(cluster, 1, facesProcessed, 0, 0);
                            }
                            ++clusterId;
                            facesProcessed += cluster;
                        }
#endif
                    }
                }
            }
        }


#if 1
        {
            CPU_MARKER("Render alphaclipped models");
            GPU_MARKER(cmd, "Render alphaclipped models");

            for (auto& node : scene.alphaclippedNodes)
            {
                auto& subMesh = node.mesh->meshBuffer();
                {
                    auto objectBounds = node.mesh->subMesh().boundingBox;

                    if (frustumCull(*camTemp, frustumPlanes, objectBounds, camTemp->position(), node.transform))
                    {
                        const auto& transformMatrix = node.transform;
                        const auto& previousTransformMatrix = node.previousTransform;

                        const auto cameraTransform = cameraViewMatrix * transformMatrix;

                        m_pbrPipelineAlphaclipped.vs.vertices = subMesh.vertices;
                        m_pbrPipelineAlphaclipped.vs.normals = subMesh.normals;
                        m_pbrPipelineAlphaclipped.vs.tangents = subMesh.tangents;
                        m_pbrPipelineAlphaclipped.vs.uv = subMesh.uv;
                        m_pbrPipelineAlphaclipped.vs.modelMatrix = fromMatrix(transformMatrix);
                        m_pbrPipelineAlphaclipped.vs.modelViewProjectionMatrix = fromMatrix(cameraProjectionMatrix * cameraTransform);
                        m_pbrPipelineAlphaclipped.vs.previousModelViewProjectionMatrix = fromMatrix(m_previousCameraProjectionMatrix * (m_previousCameraViewMatrix * previousTransformMatrix));
                        m_pbrPipelineAlphaclipped.vs.jitterModelViewProjectionMatrix = fromMatrix(jitterMatrix * (cameraProjectionMatrix * cameraTransform));

                        node.transform = node.previousTransform;

                        if (node.material)
                        {
                            /*m_pbrPipeline.ps.albedoId.x = node.material->hasAlbedo() ? m_pbrPipeline.ps.materialTextures.id(node.material->albedoKey()) : 0;
                            m_pbrPipeline.ps.normalId.x = node.material->hasNormal() ? m_pbrPipeline.ps.materialTextures.id(node.material->normalKey()) : 0;
                            m_pbrPipeline.ps.roughnessId.x = node.material->hasRoughness() ? m_pbrPipeline.ps.materialTextures.id(node.material->roughnessKey()) : 0;
                            m_pbrPipeline.ps.metalnessId.x = node.material->hasMetalness() ? m_pbrPipeline.ps.materialTextures.id(node.material->metalnessKey()) : 0;
                            m_pbrPipeline.ps.occlusionId.x = node.material->hasOcclusion() ? m_pbrPipeline.ps.materialTextures.id(node.material->occlusionKey()) : 0;*/

                            m_pbrPipelineAlphaclipped.ps.albedo = node.material->hasAlbedo() ? node.material->albedo() : TextureSRV();
                            m_pbrPipelineAlphaclipped.ps.normal = node.material->hasNormal() ? node.material->normal() : TextureSRV();
                            m_pbrPipelineAlphaclipped.ps.roughness = node.material->hasRoughness() ? node.material->roughness() : TextureSRV();
                            m_pbrPipelineAlphaclipped.ps.metalness = node.material->hasMetalness() ? node.material->metalness() : TextureSRV();
                            m_pbrPipelineAlphaclipped.ps.occlusion = node.material->hasOcclusion() ? node.material->occlusion() : TextureSRV();
                        }

                        m_pbrPipelineAlphaclipped.ps.roughnessStrength = node.material ? node.material->roughnessStrength() : 1.0f;
                        m_pbrPipelineAlphaclipped.ps.metalnessStrength = node.material ? node.material->metalnessStrength() : 0.0f;
                        m_pbrPipelineAlphaclipped.ps.occlusionStrength = node.material ? node.material->occlusionStrength() : 0.0f;

                        m_pbrPipelineAlphaclipped.ps.hasAlbedo.x = static_cast<unsigned int>(node.material && node.material->hasAlbedo());
                        m_pbrPipelineAlphaclipped.ps.hasNormal.x = static_cast<unsigned int>(node.material && node.material->hasNormal());
                        m_pbrPipelineAlphaclipped.ps.hasRoughness.x = static_cast<unsigned int>(node.material && node.material->hasRoughness());
                        m_pbrPipelineAlphaclipped.ps.hasMetalness.x = static_cast<unsigned int>(node.material && node.material->hasMetalness());
                        m_pbrPipelineAlphaclipped.ps.hasOcclusion.x = static_cast<unsigned int>(node.material && node.material->hasOcclusion());

                        m_pbrPipelineAlphaclipped.ps.materialScale = Float2(
                            node.material ? node.material->materialScaleX() : 1.0f,
                            node.material ? node.material->materialScaleY() : 1.0f);
                        m_pbrPipelineAlphaclipped.ps.objectId.x = node.objectId;

                        cmd.bindPipe(m_pbrPipelineAlphaclipped);
                        cmd.bindIndexBuffer(subMesh.indices);
                        cmd.drawIndexed(subMesh.indices.desc().elements, 1, 0, 0, 0);
                    }
                }
            }
        }
#endif

#endif

#endif
    }

    void ModelRenderer::renderWireframe(
        Device& /*device*/,
        TextureRTV& /*currentRenderTarget*/,
        TextureDSV& /*depthBuffer*/,
        CommandList& /*cmd*/,
        FlatScene& /*scene*/,
        const Matrix4f& /*cameraProjectionMatrix*/,
        const Matrix4f& /*cameraViewMatrix*/,
        const Matrix4f& /*jitterMatrix*/,
        unsigned int /*mouseX*/,
        unsigned int /*mouseY*/)
    {
        return;
#if 0
        CPU_MARKER("Wireframe");
        GPU_MARKER(cmd, "Wireframe");

        cmd.setRenderTargets({ m_lightingTarget }, depthBuffer);

        // model render pass
        for (auto& node : scene.nodes)
        {
            if (node.objectId == m_selectedObject)
            {
                auto& subMesh = node.mesh->meshBuffer();
                {
                    const auto& transformMatrix = node.transform;
                    const auto& previousTransformMatrix = node.previousTransform;

                    const auto cameraTransform = cameraViewMatrix * transformMatrix;

                    m_wireframePipeline.vs.vertices = subMesh.vertices;
                    m_wireframePipeline.vs.jitterModelViewProjectionMatrix = fromMatrix(jitterMatrix * (cameraProjectionMatrix * cameraTransform));

                    node.transform = node.previousTransform;

                    if (node.material)
                    {
                        auto color = node.material->color();
                        m_wireframePipeline.ps.color = Vector4f(color.x, color.y, color.z, 1.0f);
                    }

                    cmd.bindPipe(m_wireframePipeline);
                    cmd.bindIndexBuffer(subMesh.indices);
                    cmd.drawIndexed(subMesh.indices.desc().elements, 1, 0, 0, 0);
                }
            }
        }
        for (auto& node : scene.alphaclippedNodes)
        {
            if (node.objectId == m_selectedObject)
            {
                auto& subMesh = node.mesh->meshBuffer();
                {
                    const auto& transformMatrix = node.transform;
                    const auto& previousTransformMatrix = node.previousTransform;

                    const auto cameraTransform = cameraViewMatrix * transformMatrix;

                    m_pbrPipeline.vs.vertices = subMesh.vertices;
                    m_pbrPipeline.vs.jitterModelViewProjectionMatrix = fromMatrix(jitterMatrix * (cameraProjectionMatrix * cameraTransform));

                    node.transform = node.previousTransform;

                    if (node.material)
                    {
                        auto color = node.material->color();
                        m_wireframePipeline.ps.color = Vector4f(color.x, color.y, color.z, 1.0f);
                    }

                    cmd.bindPipe(m_wireframePipeline);
                    cmd.bindIndexBuffer(subMesh.indices);
                    cmd.drawIndexed(subMesh.indices.desc().elements, 1, 0, 0, 0);
                }
            }
        }
#endif
    }


    void ModelRenderer::renderLighting(
        Device& /*device*/,
        CommandList& cmd,
        TextureRTV& /*currentRenderTarget*/,
        TextureSRV& depthView,
        Camera& camera,
        TextureSRV& shadowMap,
        BufferSRV& shadowVP,
        Matrix4f cameraProjectionMatrix,
        Matrix4f cameraViewMatrix,
        LightData& lights,
        Vector3f probePosition,
        float probeRange)
    {
        CPU_MARKER("Lighting");
        GPU_MARKER(cmd, "Lighting");

        // lighting phase
        cmd.setRenderTargets({ m_lightingTarget });

        auto currentRenderMode = m_currentRenderMode < shaders::LightingPS::DrawmodeCount ? m_currentRenderMode : 0;
        engine::Pipeline<shaders::Lighting>* pipe = &m_lightingPipelines[currentRenderMode];

        pipe->ps.cameraWorldSpacePosition = camera.position();
        pipe->ps.environmentStrength = camera.environmentMapStrength();
        if (camera.environmentIrradiance().valid() && camera.environmentIrradiance().texture().arraySlices() == 1)
        {
            pipe->ps.environmentIrradiance = camera.environmentIrradiance();
            pipe->ps.environmentIrradianceCubemap = TextureSRV();
            pipe->ps.hasEnvironmentIrradianceCubemap.x = static_cast<unsigned int>(false);
            pipe->ps.hasEnvironmentIrradianceEquirect.x = static_cast<unsigned int>(true);
        }
        else
        {
            pipe->ps.environmentIrradiance = TextureSRV();
            pipe->ps.environmentIrradianceCubemap = camera.environmentIrradiance();
            pipe->ps.hasEnvironmentIrradianceCubemap.x = static_cast<unsigned int>(true);
            pipe->ps.hasEnvironmentIrradianceEquirect.x = static_cast<unsigned int>(false);
        }
        pipe->ps.environmentSpecular = camera.environmentSpecular();
        pipe->ps.environmentBrdfLut = camera.environmentBrdfLUT();
        pipe->ps.hasEnvironmentSpecular.x = camera.environmentSpecular().valid();
        pipe->ps.shadowSize = Float2{ 1.0f / static_cast<float>(ShadowMapWidth), 1.0f / static_cast<float>(ShadowMapHeight) };
        pipe->ps.shadowMap = shadowMap;
        pipe->ps.shadowVP = shadowVP;
        pipe->ps.depth = depthView;
        pipe->ps.cameraInverseProjectionMatrix = fromMatrix(cameraProjectionMatrix.inverse());
        pipe->ps.cameraInverseViewMatrix = fromMatrix(cameraViewMatrix.inverse());
        pipe->ps.ssao = m_blurTargetSRV;

        pipe->ps.frameSize.x = m_gbuffer->srv(GBufferType::Normal).width();
        pipe->ps.frameSize.y = m_gbuffer->srv(GBufferType::Normal).height();

        pipe->ps.gbufferNormals = m_gbuffer->srv(GBufferType::Normal);
        pipe->ps.gbufferUV = m_gbuffer->srv(GBufferType::Uv);
        pipe->ps.gbufferInstanceId = m_gbuffer->srv(GBufferType::InstanceId);

        pipe->ps.instanceMaterials = m_device.modelResources().gpuBuffers().instanceMaterial();
        //pipe->ps.objectIdToMaterialId = m_device.modelResources().gpuBuffers().objectIdToMaterial();
        pipe->ps.materialTextures = m_device.modelResources().textures();

        pipe->ps.probePositionRange = Float4{ probePosition.x, probePosition.y, probePosition.z, probeRange };
        pipe->ps.probeBBmin = Float4{ probePosition.x - probeRange, probePosition.y - probeRange, probePosition.z - probeRange, 0.0f };
        pipe->ps.probeBBmax = Float4{ probePosition.x + probeRange, probePosition.y + probeRange, probePosition.z + probeRange, 0.0f };

        pipe->ps.usingProbe.x = 0;

        pipe->ps.lightCount.x = static_cast<unsigned int>(lights.count());
        if (lights.count() > 0)
        {
            pipe->ps.lightWorldPosition = lights.worldPositions();
            pipe->ps.lightDirection = lights.directions();
            pipe->ps.lightColor = lights.colors();
            pipe->ps.lightIntensity = lights.intensities();
            pipe->ps.lightRange = lights.ranges();
            pipe->ps.lightType = lights.types();
            pipe->ps.lightParameters = lights.parameters();
        }
        pipe->ps.exposure = camera.exposure();

        cmd.bindPipe(m_lightingPipelines[currentRenderMode]);
        cmd.draw(4u);
    }

    void ModelRenderer::renderTemporalResolve(
        CommandList& cmd,
        TextureSRV& depthView,
        Camera& camera,
        const Vector2f& jitterValue,
        const Vector2f& previousJitterValue,
        const Matrix4f& jitterMatrix)
    {
        CPU_MARKER("Temporal resolve");
        GPU_MARKER(cmd, "Temporal resolve");

        m_lastResolvedIndex = m_currentFullResIndex;
        TextureRTV& target = m_fullResTargetFrame[m_currentFullResIndex];
        cmd.setRenderTargets({ target });

        auto temporalPipe = m_currentRenderMode >= shaders::LightingPS::DrawmodeCount ? &m_temporalResolve[1] : &m_temporalResolve[0];

        temporalPipe->ps.currentFrame = m_lightingTargetSRV;
        temporalPipe->ps.history = m_fullResTargetFrameSRV[previousFrameIndex()];
        temporalPipe->ps.depth = depthView;
        temporalPipe->ps.gbufferMotion = m_gbuffer->srv(GBufferType::Motion);
        temporalPipe->ps.textureSize = Float2(static_cast<float>(target.width()), static_cast<float>(target.height()));
        temporalPipe->ps.texelSize = Float2(1.0f / static_cast<float>(m_virtualResolution.x), 1.0f / static_cast<float>(m_virtualResolution.y));
        temporalPipe->ps.nearFar = Float2(camera.nearPlane(), camera.farPlane());
        temporalPipe->ps.jitter = jitterValue;
        temporalPipe->ps.previousJitter = previousJitterValue;
        temporalPipe->ps.inverseJitterMatrix = fromMatrix(jitterMatrix.inverse());

        cmd.bindPipe(*temporalPipe);
        cmd.draw(4u);

        ++m_currentFullResIndex;
        if (m_currentFullResIndex >= HistoryCount)
            m_currentFullResIndex = 0;
    }

    void ModelRenderer::renderSSAO(
        Device& device,
        CommandList& cmd,
        TextureSRV& depthView,
        Camera& camera)
    {
        CPU_MARKER("Render SSAO");
        GPU_MARKER(cmd, "Render SSAO");

        //cmd.clearRenderTargetView(m_ssaoRTV, Color4f{0.0f, 0.0f, 0.0f, 0.0f}); 
        cmd.setRenderTargets({ m_ssaoRTV });
#ifndef SCALEAOSIZE
        cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(SSAOWidth), static_cast<float>(SSAOHeight), 0.0f, 1.0f } });
        cmd.setScissorRects({ Rectangle{ 0, 0, static_cast<unsigned int>(SSAOWidth), static_cast<unsigned int>(SSAOHeight) } });
#endif

        auto viewCornerRays = camera.viewRays();
        m_ssaoPipeline.vs.viewMatrix = fromMatrix(camera.viewMatrix());
        m_ssaoPipeline.vs.topLeftViewRay = Float4{ viewCornerRays.topLeft, 0.0f };
        m_ssaoPipeline.vs.topRightViewRay = Float4{ viewCornerRays.topRight, 0.0f };
        m_ssaoPipeline.vs.bottomLeftViewRay = Float4{ viewCornerRays.bottomLeft, 0.0f };
        m_ssaoPipeline.vs.bottomRightViewRay = Float4{ viewCornerRays.bottomRight, 0.0f };

        m_ssaoPipeline.ps.depthTexture = depthView;
        m_ssaoPipeline.ps.normalTexture = m_gbuffer->srv(GBufferType::Normal);
        m_ssaoPipeline.ps.noiseTexture = m_ssaoNoiseTexture;
        m_ssaoPipeline.ps.samples = m_ssaoKernelBuffer;

        m_ssaoPipeline.ps.cameraProjectionMatrix = fromMatrix(camera.projectionMatrix(Vector2<int>{ static_cast<int>(m_ssaoRTV.width()), static_cast<int>(m_ssaoRTV.height()) }));
        m_ssaoPipeline.ps.cameraViewMatrix = fromMatrix(camera.viewMatrix());
        m_ssaoPipeline.ps.nearFar = Float2{ camera.nearPlane(), camera.farPlane() };
#ifdef SCALEAOSIZE
        m_ssaoPipeline.ps.frameSize = Float2{ static_cast<float>(m_ssaoRTV.width()), static_cast<float>(m_ssaoRTV.height()) };
#else
        m_ssaoPipeline.ps.frameSize = Float2{ static_cast<float>(SSAOWidth), static_cast<float>(SSAOHeight) };
#endif

        cmd.bindPipe(m_ssaoPipeline);
        cmd.draw(4u);

#ifdef SCALEAOSIZE 
        if (!m_blurTarget.valid() || m_blurTarget.width() != m_ssaoRTV.width() || m_blurTarget.height() != m_ssaoRTV.height())
#else
        if (!m_blurTarget.valid() || m_blurTarget.width() != SSAOWidth || m_blurTarget.height() != SSAOHeight)
#endif
        {
            m_blurTarget = device.createTextureRTV(TextureDescription()
#ifdef SCALEAOSIZE
                .width(m_ssaoRTV.width())
                .height(m_ssaoRTV.height())
#else
                .width(SSAOWidth)
                .height(SSAOHeight)
#endif
                .format(engine::Format::R16_FLOAT)
                .usage(ResourceUsage::GpuRenderTargetReadWrite)
                .name("ssao blur target")
                .dimension(ResourceDimension::Texture2D)
                .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f }));

            m_blurTargetSRV = device.createTextureSRV(m_blurTarget.texture());
        }

        {
            // blur the SSAO
            m_ssaoBlurPipeline.ps.image = m_ssaoSRV;
            m_ssaoBlurPipeline.ps.blurKernel = m_ssaoBlurKernelBuffer;
            m_ssaoBlurPipeline.ps.kernelSize.x = static_cast<int32_t>((m_ssaoBlurKernel.size() - 1) / 2);
#ifdef SCALEAOSIZE
            m_ssaoBlurPipeline.ps.texelSize = Float2(1.0f / static_cast<float>(m_ssaoRTV.width()), 1.0f / static_cast<float>(m_ssaoRTV.height()));
#else
            m_ssaoBlurPipeline.ps.texelSize = Float2(1.0f / static_cast<float>(SSAOWidth), 1.0f / static_cast<float>(SSAOHeight));
#endif

            cmd.setRenderTargets({ m_blurTarget });
#ifndef SCALEAOSIZE
            cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(SSAOWidth), static_cast<float>(SSAOHeight), 0.0f, 1.0f } });
            cmd.setScissorRects({ Rectangle{ 0, 0, static_cast<unsigned int>(SSAOWidth), static_cast<unsigned int>(SSAOHeight) } });
#endif

            cmd.bindPipe(m_ssaoBlurPipeline);
            cmd.draw(4u);
        }

    }

    void ModelRenderer::createSSAOSampleKernel()
    {
        std::uniform_real_distribution<float> randomFloat(0.0f, 1.0f);
        std::default_random_engine gen;

        auto lerp = [](float a, float b, float f)
        {
            return a + f * (b - a);
        };

        for (int i = 0; i < 64; ++i)
        {
            Vector3f sample{ 
                randomFloat(gen) * 2.0f - 1.0f,
                randomFloat(gen) * 2.0f - 1.0f,
                randomFloat(gen)
            };

            sample.normalize();
            sample *= randomFloat(gen);
            
            float scale = static_cast<float>(i) / 64.0f;
            scale = lerp(0.1f, 1.0f, scale * scale);
            sample *= scale;
            
            m_ssaoKernel.emplace_back(Vector4f(sample, 0.0f));
        }

        for (int i = 0; i < 16; ++i)
        {
            Vector3f temp{ 
                randomFloat(gen) * 2.0f - 1.0f,
                randomFloat(gen) * 2.0f - 1.0f,
                0.0f };
            temp.normalize();

            m_ssaoNoise.emplace_back(Vector4f{
                temp.x,
                temp.y,
                temp.z,
                0.0f
            });
        }
    }

    float normpdf(float x, float sigma)
    {
        return 0.39894f * exp(-0.5f * x * x / (sigma * sigma)) / sigma;
    }

    void ModelRenderer::createSSAOBlurKernel()
    {
        m_ssaoBlurKernel.resize(BilateralBlurSize);
        const int kSize = (BilateralBlurSize - 1) / 2;
        const float sigma = 10.0;
        for (int i = 0; i <= kSize; ++i)
        {
            m_ssaoBlurKernel[kSize + i] = m_ssaoBlurKernel[kSize - i] = normpdf(static_cast<float>(i), sigma);
        }
    }
}
