#pragma once

#include "engine/graphics/Graphics.h"
#include "engine/graphics/Common.h"
#include "shaders/core/HLSLTest.h"
#include <memory>
#include <string>

namespace engine
{
    class Core
    {

    private:
        engine::SamplerDescription m_linearClamp;
        engine::SamplerDescription m_pointClamp;

    public:
        Core(Device& device, ShaderStorage& shaderStorage, const std::string& shaderRootPath);

        //engine::RootSignature rootSignature;
        engine::Pipeline<shaders::HLSLTest> mainPipeline;
        Buffer uniformBuffer;

        //std::shared_ptr<engine::DescriptorHeap> descriptorHeap;
        
    };

}
