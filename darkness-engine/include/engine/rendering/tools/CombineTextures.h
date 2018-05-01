#pragma once

#include "engine/graphics/Device.h"
#include "engine/graphics/Resources.h"
#include "shaders/core/tools/AddTextures.h"

namespace engine
{
    class CommandList;
    class ShaderStorage;
    class CombineTextures
    {
    public:
        CombineTextures(Device& device, ShaderStorage& shaderStorage);
        void combine(CommandList& cmd, TextureSRV& textureA, TextureSRV& textureB, TextureRTV& dst);
    private:
        Device & m_device;
        engine::Pipeline<shaders::AddTextures> m_addTexturesPipeline;
    };
}
