#if 0
#include "engine/rendering/SSAORenderer.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/Viewport.h"
#include "engine/graphics/Rect.h"
#include "tools/Measure.h"

using namespace tools;

namespace engine
{
    SSAORenderer::SSAORenderer(Device& device, ShaderStorage& shaderStorage)
        : m_device{ device }
        , m_shaderStorage{ shaderStorage }
        , m_pipeline{ device.createPipeline<shaders::SSAORender>(shaderStorage) }
    {
        m_pipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_pipeline.setRasterizerState(RasterizerDescription());
        m_pipeline.setRenderTargetFormat(Format::Format_R8G8B8A8_UNORM, Format::Format_D32_FLOAT);
        m_pipeline.ps.tex_sampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear));

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
            .depthWriteMask(DepthWriteMask::All)
            .depthFunc(ComparisonFunction::Greater)
            .frontFace(front)
            .backFace(back));
    }

    void SSAORenderer::render(
        Device& device,
        TextureRTV& currentRenderTarget,
        TextureSRV& depthBuffer,
        CommandList& cmd
    )
    {
        cmd.setRenderTargets({ currentRenderTarget });
        cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(device.width()), static_cast<float>(device.height()), 0.0f, 1.0f } });
        cmd.setScissorRects({ Rectangle{ 0, 0, static_cast<unsigned int>(device.width()), static_cast<unsigned int>(device.height()) } });

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

        for (auto& node : scene.nodes)
        {
            for (auto&& subMesh : node.mesh->meshBuffers())
            {
                auto transformMatrix = node.transform;

                m_pipeline.vs.modelMatrix = fromMatrix(transformMatrix);
                m_pipeline.vs.modelViewProjectionMatrix = fromMatrix(camera.projectionMatrix() * (viewMatrix * transformMatrix));
                m_pipeline.vs.shadowModelViewProjectionMatrix = fromMatrix(shadowProjectionMatrix * (shadowViewMatrix * transformMatrix));

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
}
#endif
