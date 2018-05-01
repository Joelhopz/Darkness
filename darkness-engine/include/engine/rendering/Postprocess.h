#pragma once

#include "engine/graphics/CommandList.h"
#include "components/Transform.h"
#include "components/MeshRendererComponent.h"
#include "components/MaterialComponent.h"
#include "components/LightComponent.h"
#include "components/Camera.h"
#include "components/PostprocessComponent.h"
#include "shaders/core/postprocess/Bloom.h"
#include "shaders/core/postprocess/Tonemap.h"
#include "shaders/core/tools/CopyTexture.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/Scene.h"
#include "engine/rendering/LightData.h"
#include "engine/rendering/tools/GaussianBlurTool.h"
#include "engine/rendering/tools/DownsampleTool.h"
#include "engine/rendering/tools/ExtractBrightness.h"
#include "engine/rendering/tools/CombineTextures.h"
#include <memory>

namespace engine
{
    constexpr uint32_t BlurCount = 4;

    class Postprocess
    {
    public:
        Postprocess(Device& device, ShaderStorage& shaderStorage);
        void render(
            TextureRTV& currentRenderTarget,
            TextureSRV& frame,
            CommandList& cmd,
            PostprocessComponent& postprocessComponent
        );

        void clearResources();

    private:
        Device& m_device;
        ShaderStorage& m_shaderStorage;
        GaussianBlurTool m_blur;
        DownsampleTool m_downsample;
        ExtractBrightness m_extractBrightness;
        CombineTextures m_combineTextures;
        
        void createResources(
            uint32_t width, uint32_t height,
            Format brightFormat,
            Format frameFormat);

        engine::Pipeline<shaders::Bloom> m_bloomPipeline;
        engine::Pipeline<shaders::Tonemap> m_tonemapPipeline;

        engine::Pipeline<shaders::Bloom> m_bloomPipelineHDR;
        engine::Pipeline<shaders::Tonemap> m_tonemapPipelineHDR;

        engine::Pipeline<shaders::CopyTexture> m_copyTexture;

        uint32_t m_lastFrameWidth;
        uint32_t m_lastFrameHeight;
        TextureRTV m_brightTarget;
        TextureSRV m_brightTargetSRV;

        TextureRTV m_downSampleTargetRTV;
        TextureSRV m_downSampleTargetSRV;
        TextureRTV m_downSampleTargetRTVHDR;
        TextureSRV m_downSampleTargetSRVHDR;


        std::vector<TextureRTV> m_mipsRTV;
        std::vector<TextureSRV> m_mipsSRV;
        std::vector<TextureUAV> m_mipsUAV;

        std::vector<TextureRTV> m_blurTargets;
        std::vector<TextureSRV> m_blurTargetsSRV;

        TextureRTV m_blurTarget;
        TextureSRV m_blurTargetSRV;

        std::vector<TextureRTV> m_combineTargets;
        std::vector<TextureSRV> m_combineTargetsSRV;

        TextureRTV m_frameAndBloom;
        TextureSRV m_frameAndBloomSRV;
    };
}
