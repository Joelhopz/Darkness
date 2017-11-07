#include "engine/rendering/ShadowRenderer.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/Viewport.h"
#include "engine/graphics/Rect.h"
#include "engine/graphics/Barrier.h"
#include "tools/Measure.h"

using namespace tools;

namespace engine
{
    ShadowRenderer::ShadowRenderer(Device& device, ShaderStorage& shaderStorage)
        : m_device{ device }
        , m_shaderStorage{ shaderStorage }
        , m_pipeline{ device.createPipeline<shaders::ShadowRender>(shaderStorage) }
        , m_transparentPipeline{ device.createPipeline<shaders::ShadowRenderTransparent>(shaderStorage) }
        /*, m_shadowMap{ device.createTextureDSV(TextureDescription()
            .name("Shadow map")
            .format(Format::Format_D32_FLOAT)
            .width(ShadowMapWidth)
            .height(ShadowMapHeight)
            .usage(ResourceUsage::DepthStencil)
            .optimizedDepthClearValue(0.0f)
            .dimension(ResourceDimension::Texture2D)
        ) }
        , m_shadowMapSRV{ device.createTextureSRV(m_shadowMap) }*/
        , m_shadowCamera{ std::make_shared<Transform>() }
        , m_cubemapCamera{ std::make_shared<Transform>() }
        , m_refresh{ false }
    {
        m_cubemapCamera.nearPlane(0.1f);
        m_cubemapCamera.farPlane(1000.0f);
        m_cubemapCamera.fieldOfView(90.0f);
        m_cubemapCamera.projection(Projection::Perspective);

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

        //double depthBias = 0.005;
        m_pipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_pipeline.setRasterizerState(RasterizerDescription()
            .depthBias(-16384)
            .depthBiasClamp(1000000.0f)
            //.depthClipEnable(false)
            .slopeScaledDepthBias(-1.75f)
            .cullMode(CullMode::None));
        m_pipeline.setRenderTargetFormats({}, Format::Format_D32_FLOAT);
        m_pipeline.setDepthStencilState(DepthStencilDescription()
            .depthEnable(true)
            .depthWriteMask(DepthWriteMask::All)
            .depthFunc(ComparisonFunction::Greater)
            .frontFace(front)
            .backFace(back));

        m_transparentPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_transparentPipeline.setRasterizerState(RasterizerDescription().cullMode(CullMode::Back));
        m_transparentPipeline.setRenderTargetFormats({}, Format::Format_D32_FLOAT);
        m_transparentPipeline.ps.transparencySampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear));
        m_transparentPipeline.setDepthStencilState(DepthStencilDescription()
            .depthEnable(true)
            .depthWriteMask(DepthWriteMask::All)
            .depthFunc(ComparisonFunction::Greater)
            .frontFace(front)
            .backFace(back));

        m_shadowCamera.nearPlane(0.1f);
        m_shadowCamera.farPlane(1000.0f);
        m_shadowCamera.fieldOfView(60.0f);
        m_shadowCamera.width(ShadowMapWidth);
        m_shadowCamera.height(ShadowMapHeight);
        m_shadowCamera.projection(Projection::Perspective);
    }

    void ShadowRenderer::clearResources()
    {
        m_pipeline.vs.vertices = BufferSRV();

        m_transparentPipeline.vs.vertices = BufferSRV();
        m_transparentPipeline.vs.uv = BufferSRV();
        m_transparentPipeline.ps.transparencyMap = TextureSRV();
    }

    void ShadowRenderer::render(
        Device& device,
        TextureRTV& /*currentRenderTarget*/,
        TextureDSV& /*depthBuffer*/,
        CommandList& cmd,
        MaterialComponent& defaultMaterial,
        Camera& /*camera*/,
        LightData& lights,
        FlatScene& scene
    )
    {
        if (scene.selectedCamera == -1 || !scene.cameras[scene.selectedCamera])
            return;

        cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(ShadowMapWidth), static_cast<float>(ShadowMapHeight), 0.0f, 1.0f } });
        cmd.setScissorRects({ Rectangle{ 0u, 0u, static_cast<unsigned int>(ShadowMapWidth), static_cast<unsigned int>(ShadowMapHeight) } });

        uint32_t shadowCasterMapCount = 0;
        for (uint32_t i = 0; i < lights.count(); ++i)
        {
            if (lights.shadowCaster()[i])
            {
                LightType type = static_cast<LightType>(lights.engineTypes()[i]);
                if(type == LightType::Spot)
                    ++shadowCasterMapCount;
                else if (type == LightType::Directional)
                    ++shadowCasterMapCount;
                else if (type == LightType::Point)
                    shadowCasterMapCount += 6;
            }
        }

        if((!m_shadowMap.valid() || shadowCasterMapCount != m_shadowMap.texture().description().descriptor.arraySlices || m_refresh) && shadowCasterMapCount > 0)
        {
            m_refresh = false;
            m_shadowMap = device.createTextureDSV(TextureDescription()
                .name("Shadow map")
                .format(Format::Format_D32_FLOAT)
                .width(ShadowMapWidth)
                .height(ShadowMapHeight)
                .arraySlices(shadowCasterMapCount)
                .usage(ResourceUsage::DepthStencil)
                .optimizedDepthClearValue(0.0f)
                .dimension(ResourceDimension::Texture2DArray)
            );
            m_shadowMapSRV = device.createTextureSRV(m_shadowMap);

            m_shadowMapIndices.clear();
            for (uint32_t i = 0; i < shadowCasterMapCount; ++i)
            {
                m_shadowMapIndices.emplace_back(
                    std::move(device.createTextureDSV(
                        m_shadowMap.texture(), 
                        m_shadowMap.texture().description().dimension(ResourceDimension::Texture2DArray),
                        SubResource{ 0, 1, i, 1 })));
            }

            m_shadowVP = device.createBufferSRV(BufferDescription()
                .name("Shadow view projection matrixes")
                .elements(shadowCasterMapCount)
                .elementSize(sizeof(Float4x4))
                .structured(true)
                .usage(ResourceUsage::GpuReadWrite)
            );
        }
        if(m_shadowMap.valid())
            cmd.clearDepthStencilView(m_shadowMap, 0.0f);

        std::vector<Matrix4f> shadowViewProjectionMatrices;

        {
            // fast path for opaque and transparent meshes
            int shadowMapIndex = 0;
            for (uint32_t i = 0; i < lights.count(); ++i)
            {
                if (lights.shadowCaster()[i])
                {
                    LightType type = static_cast<LightType>(lights.engineTypes()[i]);

                    if (type == LightType::Point)
                    {
                        m_cubemapCamera.width(static_cast<int>(ShadowMapWidth));
                        m_cubemapCamera.height(static_cast<int>(ShadowMapHeight));

                        Vector3f pos = lights.positions()[i];
                        m_cubemapCamera.position(pos);

                        std::vector<Matrix4f> sides;
                        sides.emplace_back(Matrix4f{ m_cubemapCamera.lookAt(pos + Vector3f(0.0f, 0.0f, 0.0f), pos + Vector3f(-1.0f,  0.0f,  0.0f), Vector3f(0.0f, 1.0f,  0.0f)) });
                        sides.emplace_back(Matrix4f{ m_cubemapCamera.lookAt(pos + Vector3f(0.0f, 0.0f, 0.0f), pos + Vector3f( 1.0f,  0.0f,  0.0f), Vector3f(0.0f, 1.0f,  0.0f)) });
                        sides.emplace_back(Matrix4f{ m_cubemapCamera.lookAt(pos + Vector3f(0.0f, 0.0f, 0.0f), pos + Vector3f( 0.0f,  1.0f,  0.0f), Vector3f(0.0f, 0.0f, -1.0f)) });
                        sides.emplace_back(Matrix4f{ m_cubemapCamera.lookAt(pos + Vector3f(0.0f, 0.0f, 0.0f), pos + Vector3f( 0.0f, -1.0f,  0.0f), Vector3f(0.0f, 0.0f,  1.0f)) });
                        sides.emplace_back(Matrix4f{ m_cubemapCamera.lookAt(pos + Vector3f(0.0f, 0.0f, 0.0f), pos + Vector3f( 0.0f,  0.0f,  1.0f), Vector3f(0.0f, 1.0f,  0.0f)) });
                        sides.emplace_back(Matrix4f{ m_cubemapCamera.lookAt(pos + Vector3f(0.0f, 0.0f, 0.0f), pos + Vector3f( 0.0f,  0.0f, -1.0f), Vector3f(0.0f, 1.0f,  0.0f)) });

                        for(auto&& side : sides)
                        {
                            m_cubemapCamera.rotation(Quaternionf::fromMatrix(side));

                            //cmd.clearDepthStencilView(m_shadowMapIndices[shadowMapIndex], 0.0f);
                            cmd.setRenderTargets({}, m_shadowMapIndices[shadowMapIndex]);
                            
                            auto viewMatrix = m_cubemapCamera.viewMatrix();
                            auto shadowProjection = m_cubemapCamera.projectionMatrix();

                            shadowViewProjectionMatrices.emplace_back(shadowProjection * viewMatrix);

                            for (auto& node : scene.nodes)
                            {
                                for (auto&& subMesh : node.mesh->meshBuffers())
                                {
                                    auto transformMatrix = node.transform;

                                    m_pipeline.vs.modelViewProjectionMatrix = fromMatrix(shadowProjection * (viewMatrix * transformMatrix));
                                    m_pipeline.vs.vertices = subMesh.vertices;

                                    cmd.bindPipe(m_pipeline);
                                    cmd.bindIndexBuffer(subMesh.indices);
                                    cmd.drawIndexed(subMesh.indices.desc().elements, 1, 0, 0, 0);
                                }
                            }

                            for (auto& node : scene.transparentNodes)
                            {
                                for (auto&& subMesh : node.mesh->meshBuffers())
                                {
                                    auto transformMatrix = node.transform;

                                    m_pipeline.vs.modelViewProjectionMatrix = fromMatrix(shadowProjection * (viewMatrix * transformMatrix));
                                    m_pipeline.vs.vertices = subMesh.vertices;

                                    cmd.bindPipe(m_pipeline);
                                    cmd.bindIndexBuffer(subMesh.indices);
                                    cmd.drawIndexed(subMesh.indices.desc().elements, 1, 0, 0, 0);
                                }
                            }

                            ++shadowMapIndex;
                        }
                    }
                    else if (type == LightType::Spot)
                    {
                        //cmd.clearDepthStencilView(m_shadowMapIndices[shadowMapIndex], 0.0f);
                        cmd.setRenderTargets({}, m_shadowMapIndices[shadowMapIndex]);

                        m_shadowCamera.position(lights.positions()[i]);
                        m_shadowCamera.rotation(scene.lights[i].rotation);
                        m_shadowCamera.fieldOfView(scene.lights[i].outerCone * 2.0f);
                        auto viewMatrix = m_shadowCamera.viewMatrix();
                        auto shadowProjection = m_shadowCamera.projectionMatrix();
                        shadowViewProjectionMatrices.emplace_back(shadowProjection * viewMatrix);

                        for (auto& node : scene.nodes)
                        {
                            for (auto&& subMesh : node.mesh->meshBuffers())
                            {
                                auto transformMatrix = node.transform;

                                m_pipeline.vs.modelViewProjectionMatrix = fromMatrix(shadowProjection * (viewMatrix * transformMatrix));
                                m_pipeline.vs.vertices = subMesh.vertices;

                                cmd.bindPipe(m_pipeline);
                                cmd.bindIndexBuffer(subMesh.indices);
                                cmd.drawIndexed(subMesh.indices.desc().elements, 1, 0, 0, 0);
                            }
                        }

                        for (auto& node : scene.transparentNodes)
                        {
                            for (auto&& subMesh : node.mesh->meshBuffers())
                            {
                                auto transformMatrix = node.transform;

                                m_pipeline.vs.modelViewProjectionMatrix = fromMatrix(shadowProjection * (viewMatrix * transformMatrix));
                                m_pipeline.vs.vertices = subMesh.vertices;

                                cmd.bindPipe(m_pipeline);
                                cmd.bindIndexBuffer(subMesh.indices);
                                cmd.drawIndexed(subMesh.indices.desc().elements, 1, 0, 0, 0);
                            }
                        }

                        ++shadowMapIndex;
                    }
                }
            }
        }

        {
            // alpha discard for alphaclipped
            int shadowMapIndex = 0;
            for (uint32_t i = 0; i < lights.count(); ++i)
            {
                if (lights.shadowCaster()[i])
                {
                    LightType type = static_cast<LightType>(lights.engineTypes()[i]);

                    if (type == LightType::Point)
                    {
                        m_cubemapCamera.width(static_cast<int>(ShadowMapWidth));
                        m_cubemapCamera.height(static_cast<int>(ShadowMapHeight));

                        Vector3f pos = lights.positions()[i];
                        m_cubemapCamera.position(pos);

                        std::vector<Matrix4f> sides;
                        sides.emplace_back(Matrix4f{ m_cubemapCamera.lookAt(pos + Vector3f(0.0f, 0.0f, 0.0f), pos + Vector3f(-1.0f,  0.0f,  0.0f), Vector3f(0.0f, 1.0f,  0.0f)) });
                        sides.emplace_back(Matrix4f{ m_cubemapCamera.lookAt(pos + Vector3f(0.0f, 0.0f, 0.0f), pos + Vector3f(1.0f,  0.0f,  0.0f), Vector3f(0.0f, 1.0f,  0.0f)) });
                        sides.emplace_back(Matrix4f{ m_cubemapCamera.lookAt(pos + Vector3f(0.0f, 0.0f, 0.0f), pos + Vector3f(0.0f,  1.0f,  0.0f), Vector3f(0.0f, 0.0f, -1.0f)) });
                        sides.emplace_back(Matrix4f{ m_cubemapCamera.lookAt(pos + Vector3f(0.0f, 0.0f, 0.0f), pos + Vector3f(0.0f, -1.0f,  0.0f), Vector3f(0.0f, 0.0f,  1.0f)) });
                        sides.emplace_back(Matrix4f{ m_cubemapCamera.lookAt(pos + Vector3f(0.0f, 0.0f, 0.0f), pos + Vector3f(0.0f,  0.0f,  1.0f), Vector3f(0.0f, 1.0f,  0.0f)) });
                        sides.emplace_back(Matrix4f{ m_cubemapCamera.lookAt(pos + Vector3f(0.0f, 0.0f, 0.0f), pos + Vector3f(0.0f,  0.0f, -1.0f), Vector3f(0.0f, 1.0f,  0.0f)) });

                        for (auto&& side : sides)
                        {
                            m_cubemapCamera.rotation(Quaternionf::fromMatrix(side));

                            //cmd.clearDepthStencilView(m_shadowMapIndices[shadowMapIndex], 0.0f);
                            cmd.setRenderTargets({}, m_shadowMapIndices[shadowMapIndex]);

                            auto viewMatrix = m_cubemapCamera.viewMatrix();
                            auto shadowProjection = m_cubemapCamera.projectionMatrix();

                            for (auto& node : scene.alphaclippedNodes)
                            {
                                for (auto&& subMesh : node.mesh->meshBuffers())
                                {
                                    auto transformMatrix = node.transform;

                                    m_transparentPipeline.vs.modelViewProjectionMatrix = fromMatrix(shadowProjection * (viewMatrix * transformMatrix));
                                    m_transparentPipeline.vs.vertices = subMesh.vertices;
                                    m_transparentPipeline.vs.uv = subMesh.uv;
                                    m_transparentPipeline.ps.transparencyMap = (node.material && node.material->hasAlbedo()) ? node.material->albedo() : defaultMaterial.albedo();

                                    cmd.bindPipe(m_transparentPipeline);
                                    cmd.bindIndexBuffer(subMesh.indices);
                                    cmd.drawIndexed(subMesh.indices.desc().elements, 1, 0, 0, 0);
                                }
                            }
                            ++shadowMapIndex;
                        }
                    }
                    else if (type == LightType::Spot)
                    {
                        cmd.setRenderTargets({}, m_shadowMapIndices[shadowMapIndex]);

                        m_shadowCamera.position(lights.positions()[i]);
                        m_shadowCamera.rotation(scene.lights[i].rotation);
                        m_shadowCamera.fieldOfView(scene.lights[i].outerCone * 2.0f);
                        auto viewMatrix = m_shadowCamera.viewMatrix();

                        for (auto& node : scene.alphaclippedNodes)
                        {
                            for (auto&& subMesh : node.mesh->meshBuffers())
                            {
                                auto transformMatrix = node.transform;

                                m_transparentPipeline.vs.modelViewProjectionMatrix = fromMatrix(m_shadowCamera.projectionMatrix() * (viewMatrix * transformMatrix));
                                m_transparentPipeline.vs.vertices = subMesh.vertices;
                                m_transparentPipeline.vs.uv = subMesh.uv;
                                m_transparentPipeline.ps.transparencyMap = (node.material && node.material->hasAlbedo()) ? node.material->albedo() : defaultMaterial.albedo();

                                cmd.bindPipe(m_transparentPipeline);
                                cmd.bindIndexBuffer(subMesh.indices);
                                cmd.drawIndexed(subMesh.indices.desc().elements, 1, 0, 0, 0);
                            }
                        }

                        ++shadowMapIndex;
                    }
                }
            }
        }

        if(shadowCasterMapCount > 0)
            device.uploadBuffer(cmd, m_shadowVP, ByteRange(shadowViewProjectionMatrices));
    }
}
