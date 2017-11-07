#pragma once

#include "engine/graphics/Graphics.h"
#include "shaders/core/HLSLTest.h"
#include <memory>
#include <string>

namespace engine
{
    class MainRender
    {
    public:
        MainRender(
            Device& device, 
            ShaderStorage& shaderStorage,
            const std::string& shaderRootPath,
            int width, int height);
        MainRender(MainRender&&) = delete;
    private:
        engine::SamplerDescription m_anisoWrap;
        engine::SamplerDescription m_shadow;
        /*engine::Sampler m_samplerAnisoWrap;
        engine::Sampler m_samplerShadow;*/
        engine::RootSignature m_rootSignature;
        engine::Pipeline<shaders::HLSLTest> m_depthPipelineState;
        engine::Pipeline<shaders::HLSLTest> m_shadowPipelineState;
        engine::Pipeline<shaders::HLSLTest> m_modelPipelineState;
        
        /*engine::ColorBuffer m_sceneColorBuffer;
        engine::DepthBuffer m_sceneDepthBuffer;
        engine::ShadowBuffer m_shadowBuffer;*/

        Texture m_sceneColorBuffer;
        Texture m_sceneDepthBuffer;
        Texture m_shadowBuffer;
    };

}
