#include "engine/rendering/ModelTransparentRenderer.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/Viewport.h"
#include "engine/graphics/Rect.h"
#include "engine/graphics/Pipeline.h"
#include "tools/Measure.h"

using namespace tools;

namespace engine
{
    ModelTransparentRenderer::ModelTransparentRenderer(Device& device, ShaderStorage& shaderStorage)
        : m_device{ device }
        , m_shaderStorage{ shaderStorage }
        , m_pipeline{ device.createPipeline<shaders::MeshRendererTransparent>(shaderStorage) }
        , m_compositePipeline{ device.createPipeline<shaders::MeshTransparentComposite>(shaderStorage) }
    {
        m_colorAccumulate = device.createTextureRTV(TextureDescription()
            .format(Format::Format_R16G16B16A16_FLOAT)
            .width(device.width())
            .height(device.height())
            .name("transparency color accumulate")
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .optimizedClearValue(Float4(0.0f, 0.0f, 0.0f, 0.0f)));

        m_transparencyAccumulate = device.createTextureRTV(TextureDescription()
            .format(Format::Format_R16_FLOAT)
            .width(device.width())
            .height(device.height())
            .name("transparency alpha accumulate")
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .optimizedClearValue(Float4(1.0f, 1.0f, 1.0f, 1.0f)));

        m_colorAccumulateSRV = device.createTextureSRV(m_colorAccumulate.texture());
        m_transparencyAccumulateSRV = device.createTextureSRV(m_transparencyAccumulate.texture());

        m_pipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_pipeline.setRasterizerState(RasterizerDescription().cullMode(CullMode::None));
        m_pipeline.setRenderTargetFormats(
            { Format::Format_R16G16B16A16_FLOAT, Format::Format_R16_FLOAT },
            Format::Format_D32_FLOAT);
        m_pipeline.ps.tex_sampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear));
        m_pipeline.ps.shadow_sampler = device.createSampler(SamplerDescription()
            .addressU(TextureAddressMode::Mirror)
            .addressV(TextureAddressMode::Mirror)
            .filter(engine::Filter::Comparison));

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

        m_pipeline.setDepthStencilState(DepthStencilDescription()
            .depthEnable(true)
            .depthWriteMask(DepthWriteMask::Zero)
            .depthFunc(ComparisonFunction::Greater)
            .frontFace(front)
            .backFace(back));

        // first pass
        m_pipeline.setBlendState(BlendDescription()
            .renderTarget(0, RenderTargetBlendDescription()
                .blendEnable(true)
                .blendOp(BlendOperation::Add)
                .blendOpAlpha(BlendOperation::Add)
                .srcBlend(Blend::One)
                .srcBlendAlpha(Blend::One)
                .dstBlend(Blend::One)
                .dstBlendAlpha(Blend::One)
                .renderTargetWriteMask(1 | 2 | 4 | 8))
            .renderTarget(1, RenderTargetBlendDescription()
                .blendEnable(true)
                .blendOp(BlendOperation::Add)
                .blendOpAlpha(BlendOperation::Add)
                .srcBlend(Blend::Zero)
                .srcBlendAlpha(Blend::Zero)
                .dstBlend(Blend::InvSrcAlpha) // DONT TOUCH!!
                .dstBlendAlpha(Blend::InvSrcAlpha) // DONT TOUCH!!
                .renderTargetWriteMask(1 | 2 | 4 | 8))
        );

        // compositing pass
        m_compositePipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_compositePipeline.setRasterizerState(RasterizerDescription().cullMode(CullMode::None));
        m_compositePipeline.setRenderTargetFormat(Format::Format_R16G16B16A16_FLOAT, Format::Format_D32_FLOAT);
        m_compositePipeline.ps.tex_sampler = device.createSampler(SamplerDescription().filter(engine::Filter::Point));
        m_compositePipeline.setBlendState(BlendDescription().renderTarget(0, RenderTargetBlendDescription()
            .blendEnable(true)
            .blendOp(BlendOperation::Add)
            .blendOpAlpha(BlendOperation::Add)
            .srcBlend(Blend::SrcAlpha)
            .srcBlendAlpha(Blend::SrcAlpha)
            .dstBlend(Blend::DestAlpha)
            .dstBlendAlpha(Blend::DestAlpha)

            /*.srcBlend(Blend::SrcColor)
            .srcBlendAlpha(Blend::One)
            .dstBlend(Blend::DestColor)
            .dstBlendAlpha(Blend::DestAlpha)*/

            .renderTargetWriteMask(1 | 2 | 4 | 8)
        ));

        /*m_pipeline.setBlendState(BlendDescription()
            .renderTarget(0, RenderTargetBlendDescription()
                .blendEnable(true)
                .blendOp(BlendOperation::Add)
                .blendOpAlpha(BlendOperation::Add)
                .srcBlend(Blend::SrcAlpha)
                .srcBlendAlpha(Blend::One)
                .dstBlend(Blend::InvSrcAlpha)
                .dstBlendAlpha(Blend::Zero)
                .renderTargetWriteMask(1 | 2 | 4 | 8))
            .renderTarget(1, RenderTargetBlendDescription()
                .blendEnable(true)
                .blendOp(BlendOperation::Add)
                .blendOpAlpha(BlendOperation::Add)
                .srcBlend(Blend::Zero)
                .srcBlendAlpha(Blend::Zero)
                .dstBlend(Blend::InvSrcAlpha) // DONT TOUCH!!
                .dstBlendAlpha(Blend::InvSrcAlpha) // DONT TOUCH!!
                .renderTargetWriteMask(1 | 2 | 4 | 8))
        );

        // compositing pass
        m_compositePipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_compositePipeline.setRasterizerState(RasterizerDescription().cullMode(CullMode::None));
        m_compositePipeline.setRenderTargetFormat(Format::Format_R8G8B8A8_UNORM, Format::Format_D32_FLOAT);
        m_compositePipeline.ps.tex_sampler = device.createSampler(SamplerDescription().filter(engine::Filter::Point));
        m_compositePipeline.setBlendState(BlendDescription().renderTarget(0, RenderTargetBlendDescription()
            .blendEnable(true)
            .blendOp(BlendOperation::Add)
            .blendOpAlpha(BlendOperation::Add)
            .srcBlend(Blend::SrcAlpha)
            .srcBlendAlpha(Blend::One)
            .dstBlend(Blend::InvSrcAlpha)
            .dstBlendAlpha(Blend::Zero)
            .renderTargetWriteMask(1 | 2 | 4 | 8)
        ));*/

        m_compositePipeline.setDepthStencilState(DepthStencilDescription()
            .depthEnable(false));


    }

    void ModelTransparentRenderer::clearResources()
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
    }

    void ModelTransparentRenderer::render(
        Device& device,
        TextureRTV& currentRenderTarget,
        TextureDSV& depthBuffer,
        CommandList& cmd,
        MaterialComponent& defaultMaterial,
        Camera& camera,
        LightData& lights,
        FlatScene& scene,
        TextureSRV& shadowMap,
        BufferSRV& /*shadowVP*/
    )
    {
        if (scene.transparentNodes.size() == 0)
            return;
        cmd.clearRenderTargetView(m_colorAccumulate, { 0.0f, 0.0f, 0.0f, 0.0f });
        cmd.clearRenderTargetView(m_transparencyAccumulate, Color4f(1.0f, 1.0f, 1.0f, 1.0f));

        {
            cmd.setRenderTargets({ m_colorAccumulate, m_transparencyAccumulate }, depthBuffer);
            cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(m_colorAccumulate.width()), static_cast<float>(m_colorAccumulate.height()), 0.0f, 1.0f } });
            cmd.setScissorRects({ Rectangle{ 0, 0, m_colorAccumulate.width(), m_colorAccumulate.height() } });

            auto viewMatrix = camera.viewMatrix();

            m_pipeline.ps.cameraWorldSpacePosition = engine::Float4(camera.position(), 1.0f);
            m_pipeline.ps.environmentIrradiance = camera.environmentIrradiance();
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

            for (auto& node : scene.transparentNodes)
            {
                for (auto&& subMesh : node.mesh->meshBuffers())
                {
                    auto transformMatrix = node.transform;

                    m_pipeline.vs.modelMatrix = fromMatrix(transformMatrix);
                    m_pipeline.vs.viewMatrix = fromMatrix(viewMatrix);
                    m_pipeline.vs.modelViewProjectionMatrix = fromMatrix(camera.projectionMatrix() * (viewMatrix * transformMatrix));
                    //m_pipeline.vs.shadowModelViewProjectionMatrix = fromMatrix(shadowProjectionMatrix * (shadowViewMatrix * transformMatrix));

                    m_pipeline.vs.vertices = subMesh.vertices;
                    m_pipeline.vs.normals = subMesh.normals;
                    m_pipeline.vs.tangents = subMesh.tangents;
                    m_pipeline.vs.uv = subMesh.uv;

                    m_pipeline.ps.albedo = (node.material && node.material->hasAlbedo()) ? node.material->albedo() : defaultMaterial.albedo();
                    m_pipeline.ps.roughness = (node.material && node.material->hasRoughness()) ? node.material->roughness() : defaultMaterial.roughness();
                    m_pipeline.ps.normal = (node.material && node.material->hasNormal()) ? node.material->normal() : defaultMaterial.normal();
                    m_pipeline.ps.metalness = (node.material && node.material->hasMetalness()) ? node.material->metalness() : defaultMaterial.metalness();

                    m_pipeline.ps.materialParameters = engine::Float4(
                        node.material ? node.material->roughnessStrength() : defaultMaterial.roughnessStrength(),
                        node.material ? node.material->metalnessStrength() : defaultMaterial.metalnessStrength(),
                        node.material ? node.material->materialScaleX() : defaultMaterial.materialScaleX(),
                        node.material ? node.material->materialScaleY() : defaultMaterial.materialScaleY());

                    m_pipeline.ps.materialParameters2 = engine::Float4(
                        camera.exposure(),
                        0.0f, 0.0f, 0.0f
                    );

                    cmd.bindPipe(m_pipeline);
                    cmd.bindIndexBuffer(subMesh.indices);
                    cmd.drawIndexed(subMesh.indices.desc().elements, 1, 0, 0, 0);
                }
            }
        }
        {
            cmd.setRenderTargets({ currentRenderTarget });
            cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(currentRenderTarget.width()), static_cast<float>(currentRenderTarget.height()), 0.0f, 1.0f } });
            cmd.setScissorRects({ Rectangle{ 0, 0, currentRenderTarget.width(), currentRenderTarget.height() } });

            m_compositePipeline.ps.width = static_cast<float>(currentRenderTarget.width());
            m_compositePipeline.ps.height = static_cast<float>(currentRenderTarget.height());

            m_compositePipeline.ps.color = m_colorAccumulateSRV;
            m_compositePipeline.ps.alpha = m_transparencyAccumulateSRV;

            cmd.bindPipe(m_compositePipeline);
            cmd.draw(4u);
        }
    }
}
