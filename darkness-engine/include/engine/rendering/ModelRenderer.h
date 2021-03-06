#pragma once

#include "engine/graphics/CommandList.h"
#include "components/Transform.h"
#include "components/MeshRendererComponent.h"
#include "components/MaterialComponent.h"
#include "components/LightComponent.h"
#include "components/Camera.h"
#include "shaders/core/tools/EarlyZ.h"
#include "shaders/core/tools/EarlyZAlphaClipped.h"
#include "shaders/core/forward/MeshRenderer.h"
#include "shaders/core/model/MeshRendererPbr.h"
#include "shaders/core/lighting/Lighting.h"
#include "shaders/core/ssao/SSAO.h"
#include "shaders/core/ssao/SSAOBlur.h"
#include "shaders/core/temporal/TemporalResolve.h"
#include "shaders/core/tools/Wireframe.h"
#include "shaders/core/tools/DebugBoundingSpheres.h"
#include "engine/rendering/RenderOutline.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/Scene.h"
#include "engine/rendering/LightData.h"
#include "engine/rendering/GBuffer.h"
#include "engine/rendering/ModelResources.h"
#include "engine/rendering/Culler.h"
#include "engine/rendering/ClusterRenderer.h"
#include "engine/rendering/ParticleTest.h"
#include "engine/rendering/Picker.h"
#include <memory>

#undef EARLYZ_ENABLED
#define SCALEAOSIZE

namespace engine
{
#ifdef SCALEAOSIZE
    constexpr int SSAOresolutionScaleFactor = 1;
#else
    constexpr int SSAOWidth = 1600;
    constexpr int SSAOHeight = 1200;
#endif
    constexpr int HistoryCount = 2;
    constexpr int BilateralBlurSize = 15;

    class ModelRenderer
    {
    public:
        ModelRenderer(Device& device, ShaderStorage& shaderStorage, Vector2<int> virtualResolution);
        void render(
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
            );

        void clearResources();

        void resize(uint32_t width, uint32_t height);

        /*TextureRTV& gbufferAlbedo()
        {
            return m_gbuffer->rtv(GBufferType::Albedo);
        }*/

        TextureSRV& ssaoSRV()
        {
            return m_ssaoSRV;
        }

        TextureSRV& finalFrame()
        {
            return m_fullResTargetFrameSRV[m_lastResolvedIndex];
        }

        TextureRTV& finalFrameRTV()
        {
            return m_fullResTargetFrame[m_lastResolvedIndex];
        }

        TextureRTV& lightingTargetRTV()
        {
            return m_lightingTarget;
        }

        /*TextureSRV& motionVectors()
        {
            return m_gbuffer->srv(GBufferType::MotionVector);
        }*/

        unsigned int pickedObject(Device& device);
        void setSelectedObject(int64_t object)
        {
            m_selectedObject = object;
        }
    private:
        void renderBoundingSpheres(
            Device& device,
            TextureRTV& currentRenderTarget,
            TextureDSV& depthBuffer,
            CommandList& cmd,
            const Matrix4f& cameraProjectionMatrix,
            const Matrix4f& cameraViewMatrix,
            const Matrix4f& jitterMatrix);

        void renderEarlyZ(
            Device& device,
            TextureDSV& depthBuffer,
            CommandList& cmd,
            FlatScene& scene,
            Camera& camera,
            const Matrix4f& cameraProjectionMatrix,
            const Matrix4f& cameraViewMatrix,
            const Matrix4f& jitterMatrix);

        void renderGeometry(
            Device& device,
            TextureRTV& currentRenderTarget,
            TextureDSV& depthBuffer,
            CommandList& cmd,
            FlatScene& scene,
            Matrix4f cameraProjectionMatrix,
            Matrix4f cameraViewMatrix,
            Camera& camera,
            LightData& lights,
            TextureSRV& shadowMap,
            BufferSRV& shadowVP);

        void renderGeometryPbr(
            Device& device,
            TextureDSV* depthBuffer,
            CommandList& cmd,
            FlatScene& scene,
            Camera& camera,
            const Matrix4f& cameraProjectionMatrix,
            const Matrix4f& cameraViewMatrix,
            const Matrix4f& jitterMatrix,
            unsigned int mouseX,
            unsigned int mouseY,
            const Vector2f& jitter);

        void renderLighting(
            Device& device, 
            CommandList& cmd,
            TextureRTV& currentRenderTarget,
            TextureSRV& depthView,
            Camera& camera,
            TextureSRV& shadowMap,
            BufferSRV& shadowVP,
            Matrix4f cameraProjectionMatrix,
            Matrix4f cameraViewMatrix,
            LightData& lights,
            Vector3f probePosition,
            float probeRange);

        void renderSSAO(
            Device& device,
            CommandList& cmd,
            TextureSRV& depthView,
            Camera& camera);

        void createSSAOSampleKernel();
        void createSSAOBlurKernel();

        void renderTemporalResolve(
            CommandList& cmd,
            TextureSRV& depthView,
            Camera& camera,
            const Vector2f& jitterValue,
            const Vector2f& previousJitterValue,
            const Matrix4f& jitterMatrix);

        void renderWireframe(
            Device& device,
            TextureRTV& currentRenderTarget,
            TextureDSV& depthBuffer,
            CommandList& cmd,
            FlatScene& scene,
            const Matrix4f& cameraProjectionMatrix,
            const Matrix4f& cameraViewMatrix,
            const Matrix4f& jitterMatrix,
            unsigned int mouseX,
            unsigned int mouseY);

    private:
        Device& m_device;
        ShaderStorage& m_shaderStorage;
        Vector2<int> m_virtualResolution;
        Culler m_culler;
        ClusterRenderer m_clusterRenderer;
        Picker m_picker;
        //ParticleTest m_particleTest;
        
#ifdef EARLYZ_ENABLED
        engine::Pipeline<shaders::EarlyZ> m_earlyzPipeline;
        engine::Pipeline<shaders::EarlyZAlphaClipped> m_earlyzAlphaClipped;
#endif
        engine::Pipeline<shaders::MeshRenderer> m_pipeline;
        engine::Pipeline<shaders::MeshRendererPbr> m_pbrPipeline;
        engine::Pipeline<shaders::MeshRendererPbr> m_pbrPipelineAlphaclipped;
        engine::Pipeline<shaders::Wireframe> m_wireframePipeline;

        std::vector<engine::Pipeline<shaders::Lighting>> m_lightingPipelines;
        std::vector<engine::Pipeline<shaders::TemporalResolve>> m_temporalResolve;
        engine::Pipeline<shaders::DebugBoundingSpheres> m_debugBoundingSpheres;

        int m_currentRenderMode;

        int m_renderWidth;
        int m_renderHeight;
        unsigned int m_frameNumber;

        std::unique_ptr<engine::RenderOutline> m_renderOutline;

        std::shared_ptr<GBuffer> m_gbuffer;

        BufferSRV m_shadowsSRV;

        /*BufferUAV m_pickBufferUAV;
        BufferSRV m_pickBufferReadBack;*/

        engine::Pipeline<shaders::SSAO> m_ssaoPipeline;
        std::vector<Vector4f> m_ssaoKernel;
        std::vector<Vector4f> m_ssaoNoise;
        TextureSRV m_ssaoNoiseTexture;
        BufferSRV m_ssaoKernelBuffer;
        TextureRTV m_ssaoRTV;
        TextureSRV m_ssaoSRV;
        std::vector<float> m_ssaoBlurKernel;
        BufferSRV m_ssaoBlurKernelBuffer;

        engine::Pipeline<shaders::SSAOBlur> m_ssaoBlurPipeline;
        TextureRTV m_blurTarget;
        TextureSRV m_blurTargetSRV;

        TextureRTV m_lightingTarget;
        TextureSRV m_lightingTargetSRV;
        TextureRTV m_fullResTargetFrame[HistoryCount];
        TextureSRV m_fullResTargetFrameSRV[HistoryCount];
        int m_currentFullResIndex;
        int m_lastResolvedIndex;

        TextureDSV m_dsv;
        TextureSRV m_dsvSRV;

        int previousFrameIndex()
        {
            auto prevIndex = m_currentFullResIndex - 1;
            if (prevIndex < 0)
                prevIndex = HistoryCount - 1;
            return prevIndex;
        }

        Matrix4f m_previousCameraProjectionMatrix;
        Matrix4f m_previousCameraViewMatrix;
        Matrix4f m_previousJitterMatrix;
        Vector2f m_previousJitter;

        int64_t m_selectedObject;
    };
}
