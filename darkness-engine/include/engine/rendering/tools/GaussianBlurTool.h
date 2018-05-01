#pragma once

#include "engine/graphics/Device.h"
#include "engine/graphics/Resources.h"
#include "shaders/core/tools/GaussianBlur.h"

namespace engine
{
    class CommandList;
    class ShaderStorage;
    class GaussianBlurTool
    {
    public:
        GaussianBlurTool(Device& device, ShaderStorage& shaderStorage);
        void blur(CommandList& cmd, TextureSRV& src, TextureSRV& target);
    private:
        Device& m_device;
        engine::Pipeline<shaders::GaussianBlur> m_gaussianBlurPipeline;
        struct BlurContainer
        {
            uint32_t width;
            uint32_t height;
            TextureRTV m_blurTargetRTV[2];
            TextureSRV m_blurTargetSRV[2];
        };
        std::vector<BlurContainer> m_blurTextures;
        int refreshInternalTextures(TextureSRV& src);
    };
}
