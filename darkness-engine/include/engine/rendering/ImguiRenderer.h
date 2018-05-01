#pragma once

#include "shaders/core/imgui/ImguiRender.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Pipeline.h"
#include "engine/graphics/Resources.h"

#include <map>

namespace engine
{
    class CommandList;
    struct FlatScene;
    enum class MouseButton;
    struct BoundingSphere;

    void renderBoxes(const FlatScene& scene, const std::vector<BoundingBox>& boxes, uint32_t color);

    class ImguiRenderer
    {
    public:
        ImguiRenderer(Device& device, ShaderStorage& shaderStorage);
        void startFrame(
            Device& device,
            float delta);

        void endFrame(
            Device& device,
            TextureRTV& currentRenderTarget,
            TextureDSV& currentDepthTarget,
            CommandList& cmd);

        void render(Device& device, const FlatScene& scene);

        
        void renderSpheres(const FlatScene& scene, const std::vector<BoundingSphere>& spheres, uint32_t color);

        void onMouseMove(int x, int y);
        void onMouseDown(MouseButton button, int x, int y);
        void onMouseUp(MouseButton button, int x, int y);
        void onMouseDoubleClick(MouseButton button, int x, int y);

        bool usesMouse() const;

    private:
        ShaderStorage& m_shaderStorage;
        engine::Pipeline<shaders::ImguiRender> m_pipeline;
        TextureSRV m_fontAtlas;
        BufferVBV m_vbv;
        BufferIBV m_ibv;

        void recreateBuffersIfNeeded(Device& device, CommandList& cmd, ImVector<ImDrawVert>& vbv, ImVector<ImDrawIdx>& ibv);

        std::vector<BufferVBV> m_vbvs;
        std::vector<BufferIBV> m_ibvs;

        std::map<MouseButton, bool> m_mouseButtonStatus;
        std::map<MouseButton, bool> m_mouseButtonWasDownAtleastOnce;
        std::pair<int, int> m_lastKnownMousePosition;
        std::pair<int, int> m_lastKnownMousePositionPrev;
        std::map<MouseButton, std::pair<int, int>> m_lastKnownClickPosition;
    };
}
