#include "engine/rendering/LineSweepAO.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/Viewport.h"
#include "engine/graphics/Rect.h"
#include "tools/Measure.h"

using namespace tools;

namespace engine
{
    LineSweepAO::LineSweepAO(Device& device, ShaderStorage& shaderStorage)
        : m_device{ device }
        , m_shaderStorage{ shaderStorage }
        , m_pipeline{ device.createPipeline<shaders::LSAO>(shaderStorage) }
    {
        
    }

    void LineSweepAO::render(
        Device& /*device*/,
        TextureRTV& /*currentRenderTarget*/,
        TextureSRV& depthBuffer,
        CommandList& /*cmd*/
    )
    {
        m_pipeline.cs.depth = depthBuffer;
    }
}
