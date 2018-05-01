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
        , m_clusterRenderer{ device, shaderStorage }
        , m_culler{ device, shaderStorage }
        /*, m_shadowMap{ device.createTextureDSV(TextureDescription()
            .name("Shadow map")
            .format(Format::D32_FLOAT)
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

        m_shadowCamera.nearPlane(0.1f);
        m_shadowCamera.farPlane(1000.0f);
        m_shadowCamera.fieldOfView(60.0f);
        m_shadowCamera.width(ShadowMapWidth);
        m_shadowCamera.height(ShadowMapHeight);
        m_shadowCamera.projection(Projection::Perspective);

#if 0
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
        m_pipeline.setDepthStencilState(DepthStencilDescription()
            .depthEnable(true)
            .depthWriteMask(DepthWriteMask::All)
            .depthFunc(ComparisonFunction::Greater)
            .frontFace(front)
            .backFace(back));

        m_transparentPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_transparentPipeline.setRasterizerState(RasterizerDescription().cullMode(CullMode::Back));
        m_transparentPipeline.ps.transparencySampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear));
        m_transparentPipeline.setDepthStencilState(DepthStencilDescription()
            .depthEnable(true)
            .depthWriteMask(DepthWriteMask::All)
            .depthFunc(ComparisonFunction::Greater)
            .frontFace(front)
            .backFace(back));
#endif

        
    }

    void ShadowRenderer::clearResources()
    {
        /*m_pipeline.vs.vertices = BufferSRV();

        m_transparentPipeline.vs.vertices = BufferSRV();
        m_transparentPipeline.vs.uv = BufferSRV();
        m_transparentPipeline.ps.transparencyMap = TextureSRV();*/
    }

    bool frustumCull(
        std::vector<Vector4f>& frustumPlanes,
        const BoundingBox& aabb,
        const Vector3f cameraPosition,
        const Matrix4f& transform,
        float range)
    {
        // Jussi Knuuttilas algorithm
        // works pretty well
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
                if (((c - cameraPosition).magnitude() < range) && frustumPlanes[i].xyz().normalize().dot(c - cameraPosition) >= 0)
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

    uint32_t ShadowRenderer::shadowCasterCount(const engine::LightData& lightData) const
    {
        uint32_t shadowCasterMapCount = 0;
        for (uint32_t i = 0; i < lightData.count(); ++i)
        {
            if (lightData.shadowCaster()[i])
            {
                LightType type = static_cast<LightType>(lightData.engineTypes()[i]);
                if (type == LightType::Spot)
                    ++shadowCasterMapCount;
                else if (type == LightType::Directional)
                    ++shadowCasterMapCount;
                else if (type == LightType::Point)
                    shadowCasterMapCount += 6;
            }
        }
        return shadowCasterMapCount;
    }

    void ShadowRenderer::updateShadowMaps(uint32_t shadowCasterMapCount)
    {
        if ((!m_shadowMap.valid() || shadowCasterMapCount != m_shadowMap.texture().description().descriptor.arraySlices || m_refresh) && shadowCasterMapCount > 0)
        {
            m_refresh = false;
            m_shadowMap = m_device.createTextureDSV(TextureDescription()
                .name("Shadow map")
                .format(Format::D32_FLOAT)
                .width(ShadowMapWidth)
                .height(ShadowMapHeight)
                .arraySlices(shadowCasterMapCount)
                .usage(ResourceUsage::DepthStencil)
                .optimizedDepthClearValue(0.0f)
                .dimension(ResourceDimension::Texture2DArray)
            );
            m_shadowMapSRV = m_device.createTextureSRV(m_shadowMap);

            m_shadowMapIndices.clear();
            for (uint32_t i = 0; i < shadowCasterMapCount; ++i)
            {
                m_shadowMapIndices.emplace_back(
                    std::move(m_device.createTextureDSV(
                        m_shadowMap.texture(),
                        m_shadowMap.texture().description().dimension(ResourceDimension::Texture2DArray),
                        SubResource{ 0, 1, i, 1 })));
            }

            m_shadowVP = m_device.createBufferSRV(BufferDescription()
                .name("Shadow view projection matrixes")
                .elements(shadowCasterMapCount)
                .elementSize(sizeof(Float4x4))
                .structured(true)
                .usage(ResourceUsage::GpuReadWrite)
            );
        }
    }

    void ShadowRenderer::render(
        CommandList& cmd,
        FlatScene& scene
    )
    {
        if (scene.selectedCamera == -1 || scene.cameras.size() == 0 || !scene.cameras[scene.selectedCamera])
            return;

        auto& lights = *scene.lightData;
        uint32_t shadowCasterMapCount = shadowCasterCount(*scene.lightData);
        updateShadowMaps(shadowCasterMapCount);
        
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
                            auto viewMatrix = m_cubemapCamera.viewMatrix();
                            auto shadowProjection = m_cubemapCamera.projectionMatrix();
                            shadowViewProjectionMatrices.emplace_back(shadowProjection * viewMatrix);
                            
                            auto frameNumber = m_device.frameNumber();
                            m_culler.frustumCull(cmd, m_cubemapCamera, m_device.modelResources(), frameNumber, { ShadowMapWidth, ShadowMapHeight });
                            //ASSERT(false, "fix the depth pyramid pass");
                            m_culler.occlusionCull(cmd, m_cubemapCamera, m_device.modelResources(), nullptr, frameNumber, { ShadowMapWidth, ShadowMapHeight }, CullingMode::EmitVisible);

                            m_clusterRenderer.render(
                                cmd,
                                m_cubemapCamera,
                                m_device.modelResources(),
                                nullptr,
                                &m_shadowMapIndices[shadowMapIndex],
                                m_culler.occlusionCulledClusters(),
                                m_culler.occlusionCullingDrawArguments(),
                                m_device.frameNumber(),
                                { ShadowMapWidth, ShadowMapHeight },
                                m_cubemapCamera.viewMatrix(),
                                m_cubemapCamera.projectionMatrix(),
                                0
                            );

                            ++shadowMapIndex;
                        }
                    }
                    else if (type == LightType::Spot)
                    {
                        m_shadowCamera.position(lights.positions()[i]);
                        m_shadowCamera.rotation(scene.lights[i].rotation);
                        m_shadowCamera.fieldOfView(scene.lights[i].outerCone * 2.0f);
                        shadowViewProjectionMatrices.emplace_back(m_shadowCamera.projectionMatrix() * m_shadowCamera.viewMatrix());

                        auto frameNumber = m_device.frameNumber();
                        m_culler.frustumCull(cmd, m_shadowCamera, m_device.modelResources(), frameNumber, { ShadowMapWidth, ShadowMapHeight });
                        //ASSERT(false, "fix the depth pyramid pass");
                        m_culler.occlusionCull(cmd, m_shadowCamera, m_device.modelResources(), nullptr, frameNumber, { ShadowMapWidth, ShadowMapHeight }, CullingMode::EmitVisible);

                        m_clusterRenderer.render(
                            cmd,
                            m_shadowCamera,
                            m_device.modelResources(),
                            nullptr,
                            &m_shadowMapIndices[shadowMapIndex],
                            m_culler.occlusionCulledClusters(),
                            m_culler.occlusionCullingDrawArguments(),
                            frameNumber,
                            { ShadowMapWidth, ShadowMapHeight },
                            m_shadowCamera.viewMatrix(),
                            m_shadowCamera.projectionMatrix(),
                            0
                        );
                        ++shadowMapIndex;
                    }
                }
            }
        }

        if(shadowCasterMapCount > 0)
            m_device.uploadBuffer(cmd, m_shadowVP, ByteRange(shadowViewProjectionMatrices));
    }
}
