#pragma once

#include "engine/graphics/Device.h"
#include "engine/graphics/Resources.h"
#include "shaders/core/tools/DownSample.h"

namespace engine
{
    class CommandList;
    class ShaderStorage;
    class DownsampleTool
    {
    public:
        DownsampleTool(Device& device, ShaderStorage& shaderStorage);
        void downsample(CommandList& cmd, TextureSRV& src, TextureRTV& dst);
    private:
        Device & m_device;
        engine::Pipeline<shaders::Downsample> m_downsamplePipeline;
    };
}
