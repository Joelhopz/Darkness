#pragma once

#include "engine/graphics/Device.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/CommandList.h"
#include "shaders/core/lsao/LSAO.h"
#include "engine/graphics/ShaderStorage.h"

namespace engine
{
    class LineSweepAO
    {
    public:
        LineSweepAO(Device& device, ShaderStorage& shaderStorage);

        void render(
            Device& device,
            TextureRTV& currentRenderTarget,
            TextureSRV& depthBuffer,
            CommandList& cmd
        );

    private:
        Device& m_device;
        ShaderStorage& m_shaderStorage;
        engine::Pipeline<shaders::LSAO> m_pipeline;
    };
}
