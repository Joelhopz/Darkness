#include "engine/rendering/DebugView.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/Viewport.h"
#include "engine/graphics/Rect.h"
#include "tools/Measure.h"

using namespace tools;

namespace engine
{
    DebugView::DebugView(Device& device, ShaderStorage& shaderStorage)
        : m_device{ device }
        , m_shaderStorage{ shaderStorage }
        , m_pipeline{ device.createPipeline<shaders::DebugViewer>(shaderStorage) }
    {
        m_pipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
        m_pipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
        m_pipeline.ps.tex_sampler = device.createSampler(SamplerDescription().filter(engine::Filter::Bilinear));
    }

    void DebugView::render(
        Device& device,
        TextureRTV& currentRenderTarget,
        CommandList& cmd,
        TextureSRV& debugSRV)
    {
        if (debugSRV.valid())
        {
            cmd.setRenderTargets({ currentRenderTarget });
            cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(device.width()), static_cast<float>(device.height()), 0.0f, 1.0f } });
            cmd.setScissorRects({ Rectangle{ 0, 0, static_cast<unsigned int>(device.width()), static_cast<unsigned int>(device.height()) } });

            m_pipeline.ps.tex = debugSRV;
            m_pipeline.vs.width = static_cast<float>(device.width());
            m_pipeline.vs.height = static_cast<float>(device.height());

            cmd.bindPipe(m_pipeline);
            cmd.draw(4u);
        }
    }
}
