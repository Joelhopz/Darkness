#pragma once

#include "engine/graphics/Device.h"
#include "engine/graphics/Resources.h"
#include "shaders/core/postprocess/BloomExtractBright.h"

namespace engine
{
    class CommandList;
    class ShaderStorage;
    class ExtractBrightness
    {
    public:
        ExtractBrightness(Device& device, ShaderStorage& shaderStorage);
        TextureSRV& extract(CommandList& cmd, TextureSRV& src);
    private:
        Device & m_device;
        engine::Pipeline<shaders::BloomExtractBright> m_bloomExtractBrightPipeline;
        void refreshInternalTextures(TextureSRV& src);
        TextureRTV m_brightTargetRTV;
        TextureSRV m_brightTargetSRV;
    };
}
