#include "engine/rendering/RenderOutline.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/Viewport.h"
#include "engine/graphics/Rect.h"
#include "engine/graphics/Common.h"
#include "engine/graphics/Pipeline.h"
#include "engine/graphics/Fence.h"
#include "tools/Measure.h"
#include "tools/MeshTools.h"
#include <climits>

using namespace tools;

namespace engine
{
    RenderOutline::RenderOutline(
        Device& device, 
        ShaderStorage& shaderStorage,
        Vector2<int> virtualResolution)
        : m_device{ device }
        , m_shaderStorage{ shaderStorage }
        , m_outlinePipeline{ device.createPipeline<shaders::Outline>(shaderStorage) }
        , m_virtualResolution{ virtualResolution }
        , m_adjacencyId{ std::numeric_limits<unsigned int>().max() }

    {
        m_outlinePipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList, true);
        m_outlinePipeline.setRasterizerState(RasterizerDescription().cullMode(CullMode::None));
        m_outlinePipeline.setRenderTargetFormat(Format::Format_R16G16B16A16_FLOAT, Format::Format_UNKNOWN);
        m_outlinePipeline.setDepthStencilState(DepthStencilDescription().depthEnable(false));
    }

    void RenderOutline::rebuildAdjacencyData(FlatSceneNode& model)
    {
        m_adjacencySave.emplace_back(
            std::pair<uint64_t, std::vector<BufferIBV>>{ 
                m_device.frameNumber(), 
                    std::move(m_adjacencyBuffers) });
        m_adjacencyBuffers.clear();

        for (auto&& subMesh : model.mesh->subMeshes())
        {
            auto adjacencyData = meshGenerateAdjacency(subMesh.indices, subMesh.position);

            m_adjacencyBuffers.emplace_back(m_device.createBufferIBV(BufferDescription()
                .usage(ResourceUsage::CpuToGpu)
                .name("SubMesh Indices")
                .setInitialData(BufferDescription::InitialData(adjacencyData))
            ));
        }
    }

    void RenderOutline::clearSaves()
    {
        bool found = true;
        while (found)
        {
            found = false;
            for (auto save = m_adjacencySave.begin(); save != m_adjacencySave.end(); ++save)
            {
                if (m_device.frameNumber() - (*save).first > BackBufferCount)
                {
                    m_adjacencySave.erase(save);
                    found = true;
                    break;
                }
            }
        }
    }

    void RenderOutline::render(
        Device& device,
        TextureRTV& currentRenderTarget,
        TextureDSV& /*depthBuffer*/,
        Camera& camera,
        CommandList& cmd,
        FlatSceneNode& model)
    {
        /*clearSaves();
        if (model.objectId != m_adjacencyId)
        {
            m_adjacencyId = model.objectId;
            rebuildAdjacencyData(model);
        }*/

        auto viewMatrix = camera.viewMatrix();
        auto cameraProjectionMatrix = camera.projectionMatrix(m_virtualResolution);
        auto jitterMatrix = camera.jitterMatrix(device.frameNumber(), m_virtualResolution);
        auto jitterValue = camera.jitterValue(device.frameNumber());

        cmd.setRenderTargets({ currentRenderTarget });
        cmd.setViewPorts({ Viewport{ 0.0f, 0.0f, static_cast<float>(currentRenderTarget.width()), static_cast<float>(currentRenderTarget.height()), 0.0f, 1.0f } });
        cmd.setScissorRects({ Rectangle{ 0, 0, currentRenderTarget.width(), currentRenderTarget.height() } });
        m_outlinePipeline.ps.color = Float4{ 0.0f, 1.0f, 0.0f, 1.0f };

        const auto& transformMatrix = model.transform;
        const auto cameraTransform = viewMatrix * transformMatrix;
        
        m_outlinePipeline.gs.modelMatrix = fromMatrix(transformMatrix);
        m_outlinePipeline.gs.modelViewMatrix = fromMatrix(viewMatrix * transformMatrix);
        m_outlinePipeline.gs.jitterProjectionMatrix = fromMatrix(jitterMatrix * cameraProjectionMatrix);

        m_outlinePipeline.gs.jitterModelViewProjectionMatrix = fromMatrix(jitterMatrix * (cameraProjectionMatrix * (viewMatrix * transformMatrix)));
        m_outlinePipeline.gs.cameraWorldSpacePosition = camera.position();
        m_outlinePipeline.gs.lineThickness = 2.0f;
        m_outlinePipeline.gs.texelSize = Float2{ 
            1.0f / static_cast<float>(camera.width()),
            1.0f / static_cast<float>(camera.height()) };

        int index = 0;
        for (auto&& subMesh : model.mesh->meshBuffers())
        {
            m_outlinePipeline.vs.vertices = subMesh.vertices;
            m_outlinePipeline.vs.normals = subMesh.normals;
            cmd.bindPipe(m_outlinePipeline);
            cmd.bindIndexBuffer(subMesh.indicesAdjacency);
            //cmd.drawIndexed(m_adjacencyBuffers[index].desc().elements, 1, 0, 0, 0);
            cmd.drawIndexed(subMesh.indicesAdjacency.desc().elements, 1, 0, 0, 0);
            ++index;
        }
    }

}
