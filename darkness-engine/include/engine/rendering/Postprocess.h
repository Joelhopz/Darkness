#pragma once

#include "engine/graphics/CommandList.h"
#include "components/Transform.h"
#include "components/MeshRendererComponent.h"
#include "components/MaterialComponent.h"
#include "components/LightComponent.h"
#include "components/Camera.h"
#include "components/PostprocessComponent.h"
#include "shaders/core/postprocess/Bloom.h"
#include "shaders/core/postprocess/BloomExtractBright.h"
#include "shaders/core/tools/GaussianBlur.h"
#include "shaders/core/postprocess/Tonemap.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/Scene.h"
#include "engine/rendering/LightData.h"
#include <memory>

namespace engine
{
    class Postprocess
    {
    public:
        Postprocess(Device& device, ShaderStorage& shaderStorage);
        void render(
            Device& device,
            TextureRTV& currentRenderTarget,
            TextureSRV& frame,
            CommandList& cmd,
            PostprocessComponent& postprocessComponent
        );

        void clearResources();

    private:
        Device& m_device;
        ShaderStorage& m_shaderStorage;
        engine::Pipeline<shaders::BloomExtractBright> m_bloomExtractBrightPipeline;
        engine::Pipeline<shaders::GaussianBlur> m_gaussianBlurPipeline;
        engine::Pipeline<shaders::Bloom> m_bloomPipeline;
        engine::Pipeline<shaders::Tonemap> m_tonemapPipeline;

        uint32_t m_lastFrameWidth;
        uint32_t m_lastFrameHeight;
        TextureRTV m_brightTarget;
        TextureSRV m_brightTargetSRV;
        std::vector<TextureRTV> m_blurTarget;
        std::vector<TextureSRV> m_blurTargetSRV;
    };
}
