#include "engine/rendering/ModelRenderer.h"
#include "engine/rendering/ShadowRenderer.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/Viewport.h"
#include "engine/graphics/Rect.h"
#include "engine/graphics/Pipeline.h"
#include "engine/graphics/Common.h"
#include "tools/Measure.h"

#include <random>

using namespace tools;

namespace engine
{
    ModelRenderer::ModelRenderer(Device& device, ShaderStorage& shaderStorage, Vector2<int> virtualResolution)
        : m_device{ device }
        , m_shaderStorage{ shaderStorage }
        , m_virtualResolution{ virtualResolution }
#ifdef EARLYZ_ENABLED
        , m_earlyzPipeline{ device.createPipeline<shaders::EarlyZ>(shaderStorage) }
        , m_earlyzAlphaClipped{ device.createPipeline<shaders::EarlyZAlphaClipped>(shaderStorage) }
#endif
        , m_pipeline{ device.createPipeline<shaders::MeshRenderer>(shaderStorage) }
        , m_pbrPipeline{ device.createPipeline<shaders::MeshRendererPbr>(shaderStorage) }
        , m_wireframePipeline{ device.createPipeline<shaders::Wireframe>(shaderStorage) }
        , m_lightingPipeline{ device.createPipeline<shaders::Lighting>(shaderStorage) }
        , m_ssaoPipeline{ device.createPipeline<shaders::SSAO>(shaderStorage) }
        , m_ssaoBlurPipeline{ device.createPipeline<shaders::SSAOBlur>(shaderStorage) }
        , m_temporalResolve{ device.createPipeline<shaders::TemporalResolve>(shaderStorage) }
        , m_culling{ device.createPipeline<shaders::Culling>(shaderStorage) }

        , m_renderWidth{ m_virtualResolution.x }
        , m_renderHeight{ m_virtualResolution.y }

        , m_renderOutline{ std::make_unique<RenderOutline>(device, shaderStorage, m_virtualResolution) }

        , m_gbufferAlbedo{ device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R16G16B16A16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("GBuffer albedo")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 0.0f })
        ) }
        , m_gbufferNormal{ device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R16G16B16A16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("GBuffer normal")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f })
        ) }
        , m_gbufferMotion{ device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R16G16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("GBuffer motion")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 0.0f })
        ) }
        , m_gbufferRoughness{ device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("GBuffer roughness")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 0.0f })
        ) }
        , m_gbufferMetalness{ device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("GBuffer metalness")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 0.0f })
        ) }
        , m_gbufferOcclusion{ device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R8_UNORM)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("GBuffer occlusion")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 0.0f })
        ) }

        , m_objectId{ device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R32_UINT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("GBuffer object ID")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 0.0f })
        ) }

        , m_lightingTarget{ device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R16G16B16A16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("FullRes Target 0")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f })) }
        , m_lightingTargetSRV{ device.createTextureSRV(m_lightingTarget.texture()) }

        , m_fullResTargetFrame{ 
            device.createTextureRTV(TextureDescription()
                .width(m_renderWidth)
                .height(m_renderHeight)
                .format(Format::Format_R16G16B16A16_FLOAT)
                .usage(ResourceUsage::GpuRenderTargetReadWrite)
                .name("FullRes Target 0")
                .dimension(ResourceDimension::Texture2D)
                .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f })),
            device.createTextureRTV(TextureDescription()
                .width(m_renderWidth)
                .height(m_renderHeight)
                .format(Format::Format_R16G16B16A16_FLOAT)
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
            .format(Format::Format_R16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("SSAO Render target")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 0.0f })
        ) }
        , m_ssaoSRV{ device.createTextureSRV(m_ssaoRTV.texture()) }

        , m_gbufferAlbedoSRV{ device.createTextureSRV(m_gbufferAlbedo.texture()) }
        , m_gbufferNormalSRV{ device.createTextureSRV(m_gbufferNormal.texture()) }
        , m_gbufferMotionSRV{ device.createTextureSRV(m_gbufferMotion.texture()) }
        , m_gbufferRoughnessSRV{ device.createTextureSRV(m_gbufferRoughness.texture()) }
        , m_gbufferMetalnessSRV{ device.createTextureSRV(m_gbufferMetalness.texture()) }
        , m_gbufferOcclusionSRV{ device.createTextureSRV(m_gbufferOcclusion.texture()) }
        , m_objectIdSRV{ device.createTextureSRV(m_objectId.texture()) }

        , m_pickBufferUAV{ device.createBufferUAV(BufferDescription()
            .elements(1)
            .elementSize(sizeof(unsigned int))
            .usage(ResourceUsage::GpuReadWrite)
            .name("Pick buffer")
        ) }
        , m_pickBufferReadBack{ device.createBufferSRV(BufferDescription()
            .elements(1)
            .elementSize(sizeof(unsigned int))
            .format(Format::Format_R32_UINT)
            .usage(ResourceUsage::GpuToCpu)
            .name("Pick buffer readback")
        ) }
        , m_selectedObject{ -1 }
    {
        createSSAOSampleKernel();
        createSSAOBlurKernel();
        m_ssaoNoiseTexture = device.createTextureSRV(TextureDescription()
            .width(4)
            .height(4)
            .format(Format::Format_R32G32B32A32_FLOAT)
            .name("SSAO noise texture")
            .usage(ResourceUsage::CpuToGpu)
            .setInitialData(TextureDescription::InitialData(
                m_ssaoNoise, 
                4u * static_cast<uint32_t>(4 * sizeof(float)), 
                16u * static_cast<uint32_t>(4 * sizeof(float))))
        );
        m_ssaoKernelBuffer = device.createBufferSRV(BufferDescription()
            .elementSize(sizeof(Float4))
            .elements(m_ssaoKernel.size())
            .name("SSAO Kernel buffer")
            .usage(ResourceUsage::CpuToGpu)
            .setInitialData(m_ssaoKernel)
            .format(Format::Format_R32G32B32A32_FLOAT)
        );
        m_ssaoBlurKernelBuffer = device.createBufferSRV(BufferDescription()
            .elementSize(sizeof(Float))
            .elements(m_ssaoBlurKernel.size())
            .name("SSAO Blur kernel buffer")
            .usage(ResourceUsage::CpuToGpu)
            .setInitialData(m_ssaoBlurKernel)
            .format(Format::Format_R32_FLOAT)
        );

        m_ssaoPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
        m_ssaoPipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
        m_ssaoPipeline.setRenderTargetFormat(Format::Format_R16_FLOAT);
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
        m_ssaoBlurPipeline.setRenderTargetFormat(Format::Format_R16_FLOAT);
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
        m_earlyzPipeline.setRenderTargetFormats({}, Format::Format_D32_FLOAT);
        m_earlyzPipeline.setDepthStencilState(DepthStencilDescription()
            .depthEnable(true)
            .depthWriteMask(DepthWriteMask::All)
            .depthFunc(ComparisonFunction::Greater)
            .frontFace(front)
            .backFace(back));

        m_earlyzAlphaClipped.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_earlyzAlphaClipped.setRasterizerState(RasterizerDescription().cullMode(CullMode::None));
        m_earlyzAlphaClipped.setRenderTargetFormats({}, Format::Format_D32_FLOAT);
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
        m_pipeline.setRenderTargetFormat(Format::Format_R16G16B16A16_FLOAT, Format::Format_D32_FLOAT);
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
        m_pbrPipeline.setRenderTargetFormats({
            Format::Format_R16G16B16A16_FLOAT,
            Format::Format_R16G16B16A16_FLOAT,
            Format::Format_R16G16_FLOAT,
            Format::Format_R16_FLOAT,
            Format::Format_R16_FLOAT,
            Format::Format_R8_UNORM,
            Format::Format_R32_UINT
        }, Format::Format_D32_FLOAT);
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

        // wireframe pipeline
        m_wireframePipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_wireframePipeline.setRasterizerState(RasterizerDescription()
            .cullMode(CullMode::Back)
            .depthBias(16384)
            .fillMode(FillMode::Wireframe));
        m_wireframePipeline.setRenderTargetFormats({ Format::Format_R16G16B16A16_FLOAT }, Format::Format_D32_FLOAT);
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

        // lighting
        m_lightingPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
        m_lightingPipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
        m_lightingPipeline.setRenderTargetFormat(Format::Format_R16G16B16A16_FLOAT, Format::Format_UNKNOWN);
        m_lightingPipeline.setDepthStencilState(DepthStencilDescription().depthEnable(false));
        m_lightingPipeline.ps.tex_sampler = device.createSampler(SamplerDescription().filter(Filter::Point));
        m_lightingPipeline.ps.depth_sampler = device.createSampler(SamplerDescription().filter(Filter::Point));
        m_lightingPipeline.ps.point_sampler = device.createSampler(SamplerDescription().filter(Filter::Point));
        m_lightingPipeline.ps.shadow_sampler = device.createSampler(SamplerDescription()
            .addressU(TextureAddressMode::Mirror)
            .addressV(TextureAddressMode::Mirror)
            .filter(Filter::Comparison));
        m_lightingPipeline.ps.albedo = m_gbufferAlbedoSRV;
        m_lightingPipeline.ps.normal = m_gbufferNormalSRV;
        m_lightingPipeline.ps.roughness = m_gbufferRoughnessSRV;
        m_lightingPipeline.ps.metalness = m_gbufferMetalnessSRV;
        m_lightingPipeline.ps.occlusion = m_gbufferOcclusionSRV;
        
        m_temporalResolve.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
        m_temporalResolve.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
        m_temporalResolve.setRenderTargetFormat(Format::Format_R16G16B16A16_FLOAT, Format::Format_UNKNOWN);
        m_temporalResolve.setDepthStencilState(DepthStencilDescription().depthEnable(false));
        m_temporalResolve.ps.pointSampler = device.createSampler(SamplerDescription().textureAddressMode(TextureAddressMode::Clamp).filter(Filter::Point));
        m_temporalResolve.ps.bilinearSampler = device.createSampler(SamplerDescription().filter(Filter::Bilinear));
        m_temporalResolve.ps.trilinearSampler = device.createSampler(SamplerDescription().textureAddressMode(TextureAddressMode::Clamp).filter(Filter::Trilinear));
        m_temporalResolve.ps.anisotropicSampler = device.createSampler(SamplerDescription().textureAddressMode(TextureAddressMode::Clamp).filter(Filter::Anisotropic));

        /*m_lightingPipeline.setBlendState(BlendDescription().renderTarget(0, RenderTargetBlendDescription()
            .blendEnable(true)
            .blendOp(BlendOperation::Add)
            .blendOpAlpha(BlendOperation::Add)

            .srcBlend(Blend::SrcAlpha)
            .srcBlendAlpha(Blend::Zero)
            .dstBlend(Blend::InvSrcAlpha)
            .dstBlendAlpha(Blend::Zero)

            .renderTargetWriteMask(1 | 2 | 4 | 8)
        ));*/

        std::vector<InputData> inputData;
        inputData.emplace_back(InputData{ 1u, 2u });
        inputData.emplace_back(InputData{ 5u, 10u });
        inputData.emplace_back(InputData{ 11u, 15u });
        inputData.emplace_back(InputData{ 13u, 25u });
        inputData.emplace_back(InputData{ 30u, 54u });
        inputData.emplace_back(InputData{ 30u, 2u });
        inputData.emplace_back(InputData{ 12u, 5u });
        inputData.emplace_back(InputData{ 9u, 3u });
        inputData.emplace_back(InputData{ 212u, 225u });
        inputData.emplace_back(InputData{ 119u, 43u });

        inputData.emplace_back(InputData{ 1u, 2u });
        inputData.emplace_back(InputData{ 5u, 10u });
        inputData.emplace_back(InputData{ 11u, 15u });
        inputData.emplace_back(InputData{ 13u, 25u });
        inputData.emplace_back(InputData{ 30u, 54u });
        inputData.emplace_back(InputData{ 30u, 2u });
        inputData.emplace_back(InputData{ 12u, 5u });
        inputData.emplace_back(InputData{ 9u, 3u });
        inputData.emplace_back(InputData{ 212u, 225u });
        inputData.emplace_back(InputData{ 119u, 43u });

        inputData.emplace_back(InputData{ 1u, 2u });
        inputData.emplace_back(InputData{ 5u, 10u });
        inputData.emplace_back(InputData{ 11u, 15u });
        inputData.emplace_back(InputData{ 13u, 25u });
        inputData.emplace_back(InputData{ 30u, 54u });
        inputData.emplace_back(InputData{ 30u, 2u });
        inputData.emplace_back(InputData{ 12u, 5u });
        inputData.emplace_back(InputData{ 9u, 3u });
        inputData.emplace_back(InputData{ 212u, 225u });
        inputData.emplace_back(InputData{ 119u, 43u });

        inputData.emplace_back(InputData{ 1u, 2u });
        inputData.emplace_back(InputData{ 5u, 10u });
        inputData.emplace_back(InputData{ 11u, 15u });
        inputData.emplace_back(InputData{ 13u, 25u });
        inputData.emplace_back(InputData{ 30u, 54u });
        inputData.emplace_back(InputData{ 30u, 2u });
        inputData.emplace_back(InputData{ 12u, 5u });
        inputData.emplace_back(InputData{ 9u, 3u });
        inputData.emplace_back(InputData{ 212u, 225u });
        inputData.emplace_back(InputData{ 119u, 43u });

        inputData.emplace_back(InputData{ 1u, 2u });
        inputData.emplace_back(InputData{ 5u, 10u });
        inputData.emplace_back(InputData{ 11u, 15u });
        inputData.emplace_back(InputData{ 13u, 25u });
        inputData.emplace_back(InputData{ 30u, 54u });
        inputData.emplace_back(InputData{ 30u, 2u });
        inputData.emplace_back(InputData{ 12u, 5u });
        inputData.emplace_back(InputData{ 9u, 3u });
        inputData.emplace_back(InputData{ 212u, 225u });
        inputData.emplace_back(InputData{ 119u, 43u });

        inputData.emplace_back(InputData{ 1u, 2u });
        inputData.emplace_back(InputData{ 5u, 10u });
        inputData.emplace_back(InputData{ 11u, 15u });
        inputData.emplace_back(InputData{ 13u, 25u });
        inputData.emplace_back(InputData{ 30u, 54u });
        inputData.emplace_back(InputData{ 30u, 2u });
        inputData.emplace_back(InputData{ 12u, 5u });
        inputData.emplace_back(InputData{ 9u, 3u });
        inputData.emplace_back(InputData{ 212u, 225u });
        inputData.emplace_back(InputData{ 119u, 43u });

        inputData.emplace_back(InputData{ 1u, 2u });
        inputData.emplace_back(InputData{ 5u, 10u });
        inputData.emplace_back(InputData{ 11u, 15u });
        inputData.emplace_back(InputData{ 13u, 25u });
        inputData.emplace_back(InputData{ 30u, 54u });
        inputData.emplace_back(InputData{ 30u, 2u });
        inputData.emplace_back(InputData{ 12u, 5u });
        inputData.emplace_back(InputData{ 9u, 3u });
        inputData.emplace_back(InputData{ 212u, 225u });
        inputData.emplace_back(InputData{ 119u, 43u });

        inputData.emplace_back(InputData{ 1u, 2u });
        inputData.emplace_back(InputData{ 5u, 10u });
        inputData.emplace_back(InputData{ 11u, 15u });
        inputData.emplace_back(InputData{ 13u, 25u });
        inputData.emplace_back(InputData{ 30u, 54u });
        inputData.emplace_back(InputData{ 30u, 2u });
        inputData.emplace_back(InputData{ 12u, 5u });
        inputData.emplace_back(InputData{ 9u, 3u });
        inputData.emplace_back(InputData{ 212u, 225u });
        inputData.emplace_back(InputData{ 119u, 43u });

        inputData.emplace_back(InputData{ 1u, 2u });
        inputData.emplace_back(InputData{ 5u, 10u });
        inputData.emplace_back(InputData{ 11u, 15u });
        inputData.emplace_back(InputData{ 13u, 25u });
        inputData.emplace_back(InputData{ 30u, 54u });
        inputData.emplace_back(InputData{ 30u, 2u });
        inputData.emplace_back(InputData{ 12u, 5u });
        inputData.emplace_back(InputData{ 9u, 3u });
        inputData.emplace_back(InputData{ 212u, 225u });
        inputData.emplace_back(InputData{ 119u, 43u });

        m_computeInput = device.createBufferSRV(BufferDescription()
            .name("Compute input")
            .structured(true)
            .usage(ResourceUsage::CpuToGpu)
            .setInitialData(inputData)
        );

        m_computeOutput = device.createBufferUAV(BufferDescription()
            .name("Compute output")
            .append(true)
            .structured(true)
            .elements(inputData.size())
            .elementSize(sizeof(InputData))
            .usage(ResourceUsage::GpuReadWrite)
        );

        m_computeResult = device.createBuffer(BufferDescription()
            .elements(inputData.size())
            .elementSize(sizeof(InputData))
            .usage(ResourceUsage::GpuToCpu)
            //.format(Format::Format_R32_UINT)
            .name("Compute readback")
        );
    }

    void ModelRenderer::resize(uint32_t width, uint32_t height)
    {
        m_virtualResolution = { static_cast<int>(width), static_cast<int>(height) };
        m_renderWidth = m_virtualResolution.x;
        m_renderHeight = m_virtualResolution.y;

        m_renderOutline = std::make_unique<RenderOutline>(m_device, m_shaderStorage, m_virtualResolution);

        m_gbufferAlbedo = m_device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R16G16B16A16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("GBuffer albedo")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 0.0f })
        );

        m_gbufferNormal = m_device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R16G16B16A16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("GBuffer normal")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f })
        );

        m_gbufferMotion = m_device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R16G16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("GBuffer motion")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 0.0f })
        );

        m_gbufferRoughness = m_device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("GBuffer roughness")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 0.0f })
        );

        m_gbufferMetalness = m_device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("GBuffer metalness")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 0.0f })
        );

        m_gbufferOcclusion = m_device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R8_UNORM)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("GBuffer occlusion")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 0.0f })
        );

        m_objectId = m_device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R32_UINT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("GBuffer object ID")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 0.0f })
        );

        m_lightingTarget = m_device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R16G16B16A16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("FullRes Target 0")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f }));

        m_lightingTargetSRV = m_device.createTextureSRV(m_lightingTarget.texture());

        m_fullResTargetFrame[0] = m_device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R16G16B16A16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("FullRes Target 0")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f }));

        m_fullResTargetFrame[1] = m_device.createTextureRTV(TextureDescription()
            .width(m_renderWidth)
            .height(m_renderHeight)
            .format(Format::Format_R16G16B16A16_FLOAT)
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
            .format(Format::Format_R16_FLOAT)
            .usage(ResourceUsage::GpuRenderTargetReadWrite)
            .name("SSAO Render target")
            .dimension(ResourceDimension::Texture2D)
            .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 0.0f })
        );
        m_ssaoSRV = m_device.createTextureSRV(m_ssaoRTV.texture());

        m_gbufferAlbedoSRV = m_device.createTextureSRV(m_gbufferAlbedo.texture());
        m_gbufferNormalSRV = m_device.createTextureSRV(m_gbufferNormal.texture());
        m_gbufferMotionSRV = m_device.createTextureSRV(m_gbufferMotion.texture());
        m_gbufferRoughnessSRV = m_device.createTextureSRV(m_gbufferRoughness.texture());
        m_gbufferMetalnessSRV = m_device.createTextureSRV(m_gbufferMetalness.texture());
        m_gbufferOcclusionSRV = m_device.createTextureSRV(m_gbufferOcclusion.texture());
        m_objectIdSRV = m_device.createTextureSRV(m_objectId.texture());

        m_lightingPipeline.ps.albedo = m_gbufferAlbedoSRV;
        m_lightingPipeline.ps.normal = m_gbufferNormalSRV;
        m_lightingPipeline.ps.roughness = m_gbufferRoughnessSRV;
        m_lightingPipeline.ps.metalness = m_gbufferMetalnessSRV;
        m_lightingPipeline.ps.occlusion = m_gbufferOcclusionSRV;
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
        
        /*m_pbrPipeline.ps.albedo = TextureSRV();
        m_pbrPipeline.ps.roughness = TextureSRV();
        m_pbrPipeline.ps.normal = TextureSRV();
        m_pbrPipeline.ps.metalness = TextureSRV();
        m_pbrPipeline.ps.occlusion = TextureSRV();*/

        m_lightingPipeline.ps.environmentIrradianceCubemap = TextureSRV();
        m_lightingPipeline.ps.environmentIrradiance = TextureSRV();
        m_lightingPipeline.ps.environmentSpecular = TextureSRV();
        m_lightingPipeline.ps.environmentBrdfLut = TextureSRV();

        m_lightingPipeline.ps.albedo = TextureSRV();
        m_lightingPipeline.ps.normal = TextureSRV();
        m_lightingPipeline.ps.roughness = TextureSRV();
        m_lightingPipeline.ps.metalness = TextureSRV();
        m_lightingPipeline.ps.occlusion = TextureSRV();

        m_lightingPipeline.ps.shadowMap = TextureSRV();
        m_lightingPipeline.ps.shadowVP = BufferSRV();

        m_lightingPipeline.ps.lightWorldPosition = BufferSRV();
        m_lightingPipeline.ps.lightDirection = BufferSRV();
        m_lightingPipeline.ps.lightColor = BufferSRV();
        m_lightingPipeline.ps.lightParameters = BufferSRV();
        m_lightingPipeline.ps.lightType = BufferSRV();
        m_lightingPipeline.ps.lightIntensity = BufferSRV();
        m_lightingPipeline.ps.lightRange = BufferSRV();
        

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
        TextureDSV& depthBuffer,
        TextureSRV& depthView,
        CommandList& cmd,
        MaterialComponent& defaultMaterial,
        Camera& camera,
        LightData& lights,
        FlatScene& scene,
        TextureSRV& shadowMap,
        BufferSRV& shadowVP,
        unsigned int mouseX,
        unsigned int mouseY
        )
    {
        auto viewMatrix = camera.viewMatrix();
        auto cameraProjectionMatrix = camera.projectionMatrix(m_virtualResolution);
        auto jitterMatrix = camera.jitterMatrix(device.frameNumber(), m_virtualResolution);
        auto jitterValue = camera.jitterValue(device.frameNumber());

        // Early Z render. Device fast path.
        renderEarlyZ(device, depthBuffer, cmd, scene, cameraProjectionMatrix, viewMatrix, jitterMatrix);

        // render models and lighting
        if (!camera.pbrShadingModel())
        {
            // forward renderer
            renderGeometry(device, currentRenderTarget, depthBuffer, cmd, scene, cameraProjectionMatrix, viewMatrix, camera, lights, defaultMaterial, shadowMap, shadowVP);
        }
        else
        {
            // deferred physically based renderer
            // render model data to G-Buffer (albedo, normal, roughness, metalness, occlusion)
            renderGeometryPbr(
                device, 
                depthBuffer, 
                cmd, 
                scene, 
                camera,
                cameraProjectionMatrix, 
                viewMatrix, 
                jitterMatrix, 
                defaultMaterial, 
                mouseX, mouseY,
                jitterValue);
            
            // SSAO
            renderSSAO(device, cmd, depthView, camera);
            
            // lighting
            renderLighting(device, cmd, currentRenderTarget, depthView, camera, shadowMap, shadowVP, cameraProjectionMatrix, viewMatrix, lights);

            // wireframe
            renderWireframe(device, currentRenderTarget, depthBuffer, cmd, scene, cameraProjectionMatrix, viewMatrix, jitterMatrix, defaultMaterial, mouseX, mouseY);

            // render outline
            if (m_selectedObject != -1)
            {
                CPU_MARKER("Outline");
                GPU_MARKER(cmd, "Outline");

                for (auto& node : scene.nodes)
                {
                    if (node.objectId == m_selectedObject)
                    {
                        m_renderOutline->render(device,
                            m_lightingTarget,
                            depthBuffer,
                            camera,
                            cmd,
                            node
                        );
                    }
                }
                for (auto& node : scene.alphaclippedNodes)
                {
                    if (node.objectId == m_selectedObject)
                    {
                        m_renderOutline->render(device,
                            m_lightingTarget,
                            depthBuffer,
                            camera,
                            cmd,
                            node
                        );
                    }
                }
            }

            // remporal resolve
            renderTemporalResolve(cmd, depthView, camera, jitterValue, m_previousJitter, jitterMatrix);
        }

        {
            CPU_MARKER("Pick buffer update");
            GPU_MARKER(cmd, "Pick buffer update");
            cmd.copyBuffer(m_pickBufferUAV.buffer(), m_pickBufferReadBack.buffer(), 1);
        }

        m_previousCameraViewMatrix = viewMatrix;
        m_previousCameraProjectionMatrix = cameraProjectionMatrix;
        m_previousJitterMatrix = jitterMatrix;
        m_previousJitter = jitterValue;
    }

    void ModelRenderer::renderEarlyZ(
        Device& device,
        TextureDSV& depthBuffer,
        CommandList& cmd,
        FlatScene& scene,
        const Matrix4f& cameraProjectionMatrix,
        const Matrix4f& cameraViewMatrix,
        const Matrix4f& jitterMatrix)
    {
        
#ifdef EARLYZ_ENABLED
        CPU_MARKER("Early Z");
        GPU_MARKER(cmd, "Early Z");

        {
            // early Z for opaque meshes
            cmd.setRenderTargets({ TextureRTV() }, depthBuffer);
            cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(depthBuffer.texture().width()), static_cast<float>(depthBuffer.texture().height()), 0.0f, 1.0f } });
            cmd.setScissorRects({ Rectangle{ 0, 0, depthBuffer.texture().width(), depthBuffer.texture().height() } });

            for (auto& node : scene.nodes)
            {
                for (auto&& subMesh : node.mesh->meshBuffers())
                {
                    auto transformMatrix = node.transform;

                    m_earlyzPipeline.vs.jitterModelViewProjectionMatrix = fromMatrix(jitterMatrix * (cameraProjectionMatrix * (cameraViewMatrix * transformMatrix)));
                    m_earlyzPipeline.vs.vertices = subMesh.vertices;

                    cmd.bindPipe(m_earlyzPipeline);
                    cmd.bindIndexBuffer(subMesh.indices);
                    cmd.drawIndexed(subMesh.indices.desc().elements, 1, 0, 0, 0);
                }
            }

            for (auto& node : scene.alphaclippedNodes)
            {
                for (auto&& subMesh : node.mesh->meshBuffers())
                {
                    auto transformMatrix = node.transform;

                    m_earlyzAlphaClipped.vs.jitterModelViewProjectionMatrix = fromMatrix(jitterMatrix * (cameraProjectionMatrix * (cameraViewMatrix * transformMatrix)));
                    m_earlyzAlphaClipped.vs.vertices = subMesh.vertices;
                    m_earlyzAlphaClipped.vs.uv = subMesh.uv;

                    m_earlyzAlphaClipped.ps.image = (node.material && node.material->hasAlbedo()) ? node.material->albedo() : TextureSRV();

                    cmd.bindPipe(m_earlyzAlphaClipped);
                    cmd.bindIndexBuffer(subMesh.indices);
                    cmd.drawIndexed(subMesh.indices.desc().elements, 1, 0, 0, 0);
                }
            }
        }
#endif
    }

    void ModelRenderer::renderGeometry(
        Device& device,
        TextureRTV& currentRenderTarget,
        TextureDSV& depthBuffer,
        CommandList& cmd,
        FlatScene& scene,
        Matrix4f cameraProjectionMatrix,
        Matrix4f cameraViewMatrix,
        Camera& camera,
        LightData& lights,
        MaterialComponent& defaultMaterial,
        TextureSRV& shadowMap,
        BufferSRV& /*shadowVP*/)
    {
        CPU_MARKER("Render forward geometry");
        GPU_MARKER(cmd, "Render forward geometry");

        cmd.setRenderTargets({ currentRenderTarget }, depthBuffer);
        cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(currentRenderTarget.width()), static_cast<float>(currentRenderTarget.height()), 0.0f, 1.0f } });
        cmd.setScissorRects({ Rectangle{ 0, 0, currentRenderTarget.width(), currentRenderTarget.height() } });

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
            for (auto&& subMesh : node.mesh->meshBuffers())
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
                    m_pipeline.ps.albedo = (node.material->hasAlbedo()) ? node.material->albedo() : defaultMaterial.albedo();
                    m_pipeline.ps.roughness = (node.material->hasRoughness()) ? node.material->roughness() : defaultMaterial.roughness();
                    m_pipeline.ps.normal = (node.material->hasNormal()) ? node.material->normal() : defaultMaterial.normal();
                    m_pipeline.ps.metalness = (node.material->hasMetalness()) ? node.material->metalness() : defaultMaterial.metalness();
                    m_pipeline.ps.occlusion = (node.material->hasOcclusion()) ? node.material->occlusion() : defaultMaterial.occlusion();
                }

                m_pipeline.ps.materialParameters = Float4(
                    node.material ? node.material->roughnessStrength() : defaultMaterial.roughnessStrength(),
                    node.material ? node.material->metalnessStrength() : defaultMaterial.metalnessStrength(),
                    node.material ? node.material->materialScaleX() : defaultMaterial.materialScaleX(),
                    node.material ? node.material->materialScaleY() : defaultMaterial.materialScaleY());

                m_pipeline.ps.materialParameters2 = Float4(
                    node.material ? node.material->occlusionStrength() : defaultMaterial.occlusionStrength(),
                    camera.exposure(),
                    0.0f,
                    0.0f
                );

                cmd.bindPipe(m_pipeline);
                cmd.bindIndexBuffer(subMesh.indices);
                cmd.drawIndexed(subMesh.indices.desc().elements, 1, 0, 0, 0);
            }
        }
    }

    unsigned int ModelRenderer::pickedObject(Device& device)
    {
        unsigned int res = 0;
        uint32_t* bufferPtr = reinterpret_cast<uint32_t*>(m_pickBufferReadBack.buffer().map(device));
        res = *bufferPtr;
        m_pickBufferReadBack.buffer().unmap(device);
        return res;
    }

    bool frustumCull(
        Camera& camera,
        std::vector<Vector4f>& frustumPlanes,
        const SubMesh::BoundingBox& aabb,
        const Vector3f cameraPosition)
    {
#if 0
        bool allOut = true;
        for (auto&& plane : frustumPlanes)
        {
            if ((plane.xyz().normalize().dot(aabb.min - cameraPosition) >= 0) ||
                (plane.xyz().normalize().dot(aabb.max - cameraPosition) >= 0))
            {
                allOut = false;
                break;
            }
        }
        return !allOut;
#endif

#if 1
        // Jussi Knuuttilas algorithm
        // works pretty well
        Vector3f corner[8] =
        {
            aabb.min,
            Vector3f{ aabb.min.x, aabb.max.y, aabb.max.z },
            Vector3f{ aabb.min.x, aabb.min.y, aabb.max.z },
            Vector3f{ aabb.min.x, aabb.max.y, aabb.min.z },
            aabb.max,
            Vector3f{ aabb.max.x, aabb.min.y, aabb.min.z },
            Vector3f{ aabb.max.x, aabb.max.y, aabb.min.z },
            Vector3f{ aabb.max.x, aabb.min.y, aabb.max.z }
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
#endif

#if 0
        enum
        {
            INSIDE,
            OUTSIDE,
            INTERSECT
        };
        float m, n; int i, result = INSIDE;

        //for (i = 0; i < 6; i++)
        for(auto&& plane : frustumPlanes)
        {
            //PLANE *p = f->plane + i;

            m = (p->a * b->v[p->nx].x) + (p->b * b->v[p->ny].y) + (p->c * b->v[p->nz].z);
            if (m > -p->d) return OUTSIDE;

            n = (p->a * b->v[p->px].x) + (p->b * b->v[p->py].y) + (p->c * b->v[p->pz].z);
            if (n > -p->d) result = INTERSECT;

        } return result;
#endif

#if 0
        // Indexed for the 'index trick' later
        Vector3f box[] = { 
            { aabb.min.x, aabb.min.y, aabb.min.z }, 
            { aabb.max.x, aabb.max.y, aabb.max.z } };

        // We have 6 planes defining the frustum
        /*static const int NUM_PLANES = 6;
        const plane3 *planes[NUM_PLANES] =
        { &f.n, &f.l, &f.r, &f.b, &f.t, &f.f };*/

        // We only need to do 6 point-plane tests
        for (int i = 0; i < frustumPlanes.size(); ++i)
        {
            // This is the current plane
            const Vector4f& p = frustumPlanes[i];

            // p-vertex selection (with the index trick)
            // According to the plane normal we can know the
            // indices of the positive vertex
            const int px = static_cast<int>(p.x > 0.0f);
            const int py = static_cast<int>(p.y > 0.0f);
            const int pz = static_cast<int>(p.z > 0.0f);

            // Dot product
            // project p-vertex on plane normal
            // (How far is p-vertex from the origin)
            const float dp =
                (p.x*box[px].x) +
                (p.y*box[py].y) +
                (p.z*box[pz].z);

            // Doesn't intersect if it is behind the plane
            if (dp < -p.w) { return false; }
        }
        return true;
#endif
    }

    void ModelRenderer::renderGeometryPbr(
        Device& device,
        TextureDSV& depthBuffer,
        CommandList& cmd,
        FlatScene& scene,
        Camera& camera,
        const Matrix4f& cameraProjectionMatrix,
        const Matrix4f& cameraViewMatrix,
        const Matrix4f& jitterMatrix,
        MaterialComponent& defaultMaterial,
        unsigned int mouseX,
        unsigned int mouseY,
        const Vector2f& jitter)
    {
        CPU_MARKER("Render PBR geometry");
        GPU_MARKER(cmd, "Render PBR geometry");

        // perform compute test
        //m_computeOutput.setCounterValue(0);
        /*m_culling.cs.input = m_computeInput;
        m_culling.cs.output = m_computeOutput;
        cmd.bindPipe(m_culling);
        cmd.dispatch(1, 0, 0);*/
        
        /*uint32_t counter = m_computeOutput.getCounterValue();
        cmd.copyBuffer(m_computeOutput.buffer(), m_computeResult, counter);
        OutputData* data = reinterpret_cast<OutputData*>(m_computeResult.map(device));
        for (int i = 0; i < counter; ++i)
        {
            LOG("res: %u, %u", data[i].value, data[i].anotherValue);
        }
        m_computeResult.unmap(device);*/

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

        //
        {
            GPU_MARKER(cmd, "Clear G-buffer");
            cmd.clearRenderTargetView(m_gbufferAlbedo, { 0.0f, 0.0f, 0.0f, 0.0f });
            cmd.clearRenderTargetView(m_gbufferNormal, { 0.0f, 0.0f, 0.0f, 1.0f });
            cmd.clearRenderTargetView(m_gbufferMotion, { 0.0f, 0.0f, 0.0f, 0.0f });
            cmd.clearRenderTargetView(m_gbufferRoughness, { 0.0f, 0.0f, 0.0f, 0.0f });
            cmd.clearRenderTargetView(m_gbufferMetalness, { 0.0f, 0.0f, 0.0f, 0.0f });
            cmd.clearRenderTargetView(m_gbufferOcclusion, { 0.0f, 0.0f, 0.0f, 0.0f });
        }

        cmd.setRenderTargets({ m_gbufferAlbedo, m_gbufferNormal, m_gbufferMotion, m_gbufferRoughness, m_gbufferMetalness, m_gbufferOcclusion, m_objectId }, depthBuffer);
        cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(m_gbufferAlbedo.width()), static_cast<float>(m_gbufferAlbedo.height()), 0.0f, 1.0f } });
        cmd.setScissorRects({ Rectangle{ 0, 0, m_gbufferAlbedo.width(), m_gbufferAlbedo.height() } });

        m_pbrPipeline.ps.mouseX.x = mouseX;
        m_pbrPipeline.ps.mouseY.x = mouseY;
        m_pbrPipeline.ps.pick = m_pickBufferUAV;

        {
            CPU_MARKER("Update material textures");
            for (auto& node : scene.nodes)
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
            }
        }

        {
            CPU_MARKER("Render opaque models");
            GPU_MARKER(cmd, "Render opaque models");

            std::uniform_real_distribution<float> randomFloat(0.0f, 1.0f);
            std::default_random_engine gen;
            // model render pass
            for (auto& node : scene.nodes)
            {
                int subMeshIndex = 0;
                for (auto&& subMesh : node.mesh->meshBuffers())
                {
                    auto objectBounds = node.mesh->subMeshes()[subMeshIndex].boundingBox;
                    objectBounds.max = node.transform * objectBounds.max;
                    objectBounds.min = node.transform * objectBounds.min;

                    if (frustumCull(*camTemp, frustumPlanes, objectBounds, camTemp->position()))
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
                            m_pbrPipeline.ps.albedoId.x = node.material->hasAlbedo() ? m_pbrPipeline.ps.materialTextures.id(node.material->albedoKey()) : 0;
                            m_pbrPipeline.ps.normalId.x = node.material->hasNormal() ? m_pbrPipeline.ps.materialTextures.id(node.material->normalKey()) : 0;
                            m_pbrPipeline.ps.roughnessId.x = node.material->hasRoughness() ? m_pbrPipeline.ps.materialTextures.id(node.material->roughnessKey()) : 0;
                            m_pbrPipeline.ps.metalnessId.x = node.material->hasMetalness() ? m_pbrPipeline.ps.materialTextures.id(node.material->metalnessKey()) : 0;
                            m_pbrPipeline.ps.occlusionId.x = node.material->hasOcclusion() ? m_pbrPipeline.ps.materialTextures.id(node.material->occlusionKey()) : 0;

                            /*m_pbrPipeline.ps.albedo = node.material->hasAlbedo() ? node.material->albedo() : TextureSRV();
                            m_pbrPipeline.ps.normal = node.material->hasNormal() ? node.material->normal() : TextureSRV();
                            m_pbrPipeline.ps.roughness = node.material->hasRoughness() ? node.material->roughness() : TextureSRV();
                            m_pbrPipeline.ps.metalness = node.material->hasMetalness() ? node.material->metalness() : TextureSRV();
                            m_pbrPipeline.ps.occlusion = node.material->hasOcclusion() ? node.material->occlusion() : TextureSRV();*/

                            auto color = node.material->color();
                            m_pbrPipeline.ps.color = Vector4f(color.x, color.y, color.z, 1.0f);
                        }

                        m_pbrPipeline.ps.roughnessStrength = node.material ? node.material->roughnessStrength() : defaultMaterial.roughnessStrength();
                        m_pbrPipeline.ps.metalnessStrength = node.material ? node.material->metalnessStrength() : defaultMaterial.metalnessStrength();
                        m_pbrPipeline.ps.occlusionStrength = node.material ? node.material->occlusionStrength() : defaultMaterial.occlusionStrength();

                        m_pbrPipeline.ps.hasAlbedo.x = static_cast<unsigned int>(node.material && node.material->hasAlbedo());
                        m_pbrPipeline.ps.hasNormal.x = static_cast<unsigned int>(node.material && node.material->hasNormal());
                        m_pbrPipeline.ps.hasRoughness.x = static_cast<unsigned int>(node.material && node.material->hasRoughness());
                        m_pbrPipeline.ps.hasMetalness.x = static_cast<unsigned int>(node.material && node.material->hasMetalness());
                        m_pbrPipeline.ps.hasOcclusion.x = static_cast<unsigned int>(node.material && node.material->hasOcclusion());

                        m_pbrPipeline.ps.materialScale = Float2(
                            node.material ? node.material->materialScaleX() : defaultMaterial.materialScaleX(),
                            node.material ? node.material->materialScaleY() : defaultMaterial.materialScaleY());
                        m_pbrPipeline.ps.objectId.x = node.objectId;
                        m_pbrPipeline.ps.jitter = Float2{ jitter.x, jitter.y };


                        cmd.bindPipe(m_pbrPipeline);
                        cmd.bindIndexBuffer(subMesh.indices);
                        cmd.drawIndexed(subMesh.indices.desc().elements, 1, 0, 0, 0);

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
                        for (auto&& cluster : node.mesh->subMeshes()[subMeshIndex].clusterIndexCount)
                        {
                            auto clusterBounds = node.mesh->subMeshes()[subMeshIndex].clusterBounds[clusterId];
                            clusterBounds.max = node.transform * clusterBounds.max;
                            clusterBounds.min = node.transform * clusterBounds.min;

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
                                cameraPosition))
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
                    ++subMeshIndex;
                }
            }
        }
#if 1
        {
            CPU_MARKER("Render alphaclipped models");
            GPU_MARKER(cmd, "Render alphaclipped models");

            for (auto& node : scene.alphaclippedNodes)
            {
                for (auto&& subMesh : node.mesh->meshBuffers())
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
                        m_pbrPipeline.ps.albedoId.x = node.material->hasAlbedo() ? m_pbrPipeline.ps.materialTextures.id(node.material->albedoKey()) : 0;
                        m_pbrPipeline.ps.normalId.x = node.material->hasNormal() ? m_pbrPipeline.ps.materialTextures.id(node.material->normalKey()) : 0;
                        m_pbrPipeline.ps.roughnessId.x = node.material->hasRoughness() ? m_pbrPipeline.ps.materialTextures.id(node.material->roughnessKey()) : 0;
                        m_pbrPipeline.ps.metalnessId.x = node.material->hasMetalness() ? m_pbrPipeline.ps.materialTextures.id(node.material->metalnessKey()) : 0;
                        m_pbrPipeline.ps.occlusionId.x = node.material->hasOcclusion() ? m_pbrPipeline.ps.materialTextures.id(node.material->occlusionKey()) : 0;

                        /*m_pbrPipeline.ps.albedo = node.material->hasAlbedo() ? node.material->albedo() : TextureSRV();
                        m_pbrPipeline.ps.normal = node.material->hasNormal() ? node.material->normal() : TextureSRV();
                        m_pbrPipeline.ps.roughness = node.material->hasRoughness() ? node.material->roughness() : TextureSRV();
                        m_pbrPipeline.ps.metalness = node.material->hasMetalness() ? node.material->metalness() : TextureSRV();
                        m_pbrPipeline.ps.occlusion = node.material->hasOcclusion() ? node.material->occlusion() : TextureSRV();*/
                    }

                    m_pbrPipeline.ps.roughnessStrength = node.material ? node.material->roughnessStrength() : defaultMaterial.roughnessStrength();
                    m_pbrPipeline.ps.metalnessStrength = node.material ? node.material->metalnessStrength() : defaultMaterial.metalnessStrength();
                    m_pbrPipeline.ps.occlusionStrength = node.material ? node.material->occlusionStrength() : defaultMaterial.occlusionStrength();

                    m_pbrPipeline.ps.hasAlbedo.x = static_cast<unsigned int>(node.material && node.material->hasAlbedo());
                    m_pbrPipeline.ps.hasNormal.x = static_cast<unsigned int>(node.material && node.material->hasNormal());
                    m_pbrPipeline.ps.hasRoughness.x = static_cast<unsigned int>(node.material && node.material->hasRoughness());
                    m_pbrPipeline.ps.hasMetalness.x = static_cast<unsigned int>(node.material && node.material->hasMetalness());
                    m_pbrPipeline.ps.hasOcclusion.x = static_cast<unsigned int>(node.material && node.material->hasOcclusion());

                    m_pbrPipeline.ps.materialScale = Float2(
                        node.material ? node.material->materialScaleX() : defaultMaterial.materialScaleX(),
                        node.material ? node.material->materialScaleY() : defaultMaterial.materialScaleY());
                    m_pbrPipeline.ps.objectId.x = node.objectId;

                    cmd.bindPipe(m_pbrPipeline);
                    cmd.bindIndexBuffer(subMesh.indices);
                    cmd.drawIndexed(subMesh.indices.desc().elements, 1, 0, 0, 0);
                }
            }
        }
#endif
    }

    void ModelRenderer::renderWireframe(
        Device& device,
        TextureRTV& currentRenderTarget,
        TextureDSV& depthBuffer,
        CommandList& cmd,
        FlatScene& scene,
        const Matrix4f& cameraProjectionMatrix,
        const Matrix4f& cameraViewMatrix,
        const Matrix4f& jitterMatrix,
        MaterialComponent& defaultMaterial,
        unsigned int mouseX,
        unsigned int mouseY)
    {
        CPU_MARKER("Wireframe");
        GPU_MARKER(cmd, "Wireframe");

        cmd.setRenderTargets({ m_lightingTarget }, depthBuffer);
        cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(m_lightingTarget.width()), static_cast<float>(m_lightingTarget.height()), 0.0f, 1.0f } });
        cmd.setScissorRects({ Rectangle{ 0, 0, m_lightingTarget.width(), m_lightingTarget.height() } });

        // model render pass
        for (auto& node : scene.nodes)
        {
            if (node.objectId == m_selectedObject)
            {
                for (auto&& subMesh : node.mesh->meshBuffers())
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
                for (auto&& subMesh : node.mesh->meshBuffers())
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
        LightData& lights)
    {
        CPU_MARKER("Lighting");
        GPU_MARKER(cmd, "Lighting");

        // lighting phase
        cmd.setRenderTargets({ m_lightingTarget });
        cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(m_lightingTarget.width()), static_cast<float>(m_lightingTarget.height()), 0.0f, 1.0f } });
        cmd.setScissorRects({ Rectangle{ 0, 0, m_lightingTarget.width(), m_lightingTarget.height() } });

        m_lightingPipeline.ps.cameraWorldSpacePosition = camera.position();
        m_lightingPipeline.ps.environmentStrength = camera.environmentMapStrength();
        if (camera.environmentIrradiance().valid() && camera.environmentIrradiance().texture().arraySlices() == 1)
        {
            m_lightingPipeline.ps.environmentIrradiance = camera.environmentIrradiance();
            m_lightingPipeline.ps.environmentIrradianceCubemap = TextureSRV();
            m_lightingPipeline.ps.hasEnvironmentIrradianceCubemap.x = static_cast<unsigned int>(false);
            m_lightingPipeline.ps.hasEnvironmentIrradianceEquirect.x = static_cast<unsigned int>(true);
        }
        else
        {
            m_lightingPipeline.ps.environmentIrradiance = TextureSRV();
            m_lightingPipeline.ps.environmentIrradianceCubemap = camera.environmentIrradiance();
            m_lightingPipeline.ps.hasEnvironmentIrradianceCubemap.x = static_cast<unsigned int>(true);
            m_lightingPipeline.ps.hasEnvironmentIrradianceEquirect.x = static_cast<unsigned int>(false);
        }
        m_lightingPipeline.ps.environmentSpecular = camera.environmentSpecular();
        m_lightingPipeline.ps.environmentBrdfLut = camera.environmentBrdfLUT();
        m_lightingPipeline.ps.hasEnvironmentSpecular.x = camera.environmentSpecular().valid();
        m_lightingPipeline.ps.shadowSize = Float2{ 1.0f / static_cast<float>(ShadowMapWidth), 1.0f / static_cast<float>(ShadowMapHeight) };
        m_lightingPipeline.ps.shadowMap = shadowMap;
        m_lightingPipeline.ps.shadowVP = shadowVP;
        m_lightingPipeline.ps.depth = depthView;
        m_lightingPipeline.ps.cameraInverseProjectionMatrix = fromMatrix(cameraProjectionMatrix.inverse());
        m_lightingPipeline.ps.cameraInverseViewMatrix = fromMatrix(cameraViewMatrix.inverse());
        m_lightingPipeline.ps.ssao = m_blurTargetSRV;

        m_lightingPipeline.ps.lightCount.x = static_cast<unsigned int>(lights.count());
        if (lights.count() > 0)
        {
            m_lightingPipeline.ps.lightWorldPosition = lights.worldPositions();
            m_lightingPipeline.ps.lightDirection = lights.directions();
            m_lightingPipeline.ps.lightColor = lights.colors();
            m_lightingPipeline.ps.lightIntensity = lights.intensities();
            m_lightingPipeline.ps.lightRange = lights.ranges();
            m_lightingPipeline.ps.lightType = lights.types();
            m_lightingPipeline.ps.lightParameters = lights.parameters();
        }
        m_lightingPipeline.ps.exposure = camera.exposure();

        cmd.bindPipe(m_lightingPipeline);
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
        cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(target.width()), static_cast<float>(target.height()), 0.0f, 1.0f } });
        cmd.setScissorRects({ Rectangle{ 0, 0, target.width(), target.height() } });

        m_temporalResolve.ps.currentFrame = m_lightingTargetSRV;
        m_temporalResolve.ps.history = m_fullResTargetFrameSRV[previousFrameIndex()];
        m_temporalResolve.ps.depth = depthView;
        m_temporalResolve.ps.motion = m_gbufferMotionSRV;
        m_temporalResolve.ps.textureSize = Float2(static_cast<float>(target.width()), static_cast<float>(target.height()));
        m_temporalResolve.ps.texelSize = Float2(1.0f / static_cast<float>(m_virtualResolution.x), 1.0f / static_cast<float>(m_virtualResolution.y));
        m_temporalResolve.ps.nearFar = Float2(camera.nearPlane(), camera.farPlane());
        m_temporalResolve.ps.jitter = jitterValue;
        m_temporalResolve.ps.previousJitter = previousJitterValue;
        m_temporalResolve.ps.inverseJitterMatrix = fromMatrix(jitterMatrix.inverse());

        cmd.bindPipe(m_temporalResolve);
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
#ifdef SCALEAOSIZE
        cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(m_ssaoRTV.width()), static_cast<float>(m_ssaoRTV.height()), 0.0f, 1.0f } });
        cmd.setScissorRects({ Rectangle{ 0, 0, m_ssaoRTV.width(), m_ssaoRTV.height() } });
#else
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
        m_ssaoPipeline.ps.normalTexture = m_gbufferNormalSRV;
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
                .format(engine::Format::Format_R16_FLOAT)
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
#ifdef SCALEAOSIZE
            cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(m_ssaoRTV.width()), static_cast<float>(m_ssaoRTV.height()), 0.0f, 1.0f } });
            cmd.setScissorRects({ Rectangle{ 0, 0, m_ssaoRTV.width(), m_ssaoRTV.height() } });
#else
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
