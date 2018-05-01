#pragma once

#include "engine/graphics/CommonNoDep.h"
#include "engine/graphics/Pipeline.h"
#include "engine/graphics/Resources.h"
#include "engine/rendering/GBuffer.h"
#include "shaders/core/tools/Pick.h"

namespace engine
{
    class CommandList;
    class Device;

    class Picker
    {
    public:
        Picker(
            Device& device, 
            ShaderStorage& shaderStorage);

        void pick(
            GBuffer* gBuffer,
            CommandList& cmd, 
            uint32_t mouseX,
            uint32_t mouseY);

        uint32_t selectedInstanceId(Device& device);

    private:
        engine::Pipeline<shaders::Pick> m_pickPipeline;

    private:
        BufferUAV m_pickUAV;
        BufferSRV m_pickReadBack;
    };
}
