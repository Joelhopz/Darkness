#pragma once

#include "engine/EngineComponent.h"
#include "tools/Property.h"

#include "engine/graphics/Resources.h"
#include "engine/primitives/Matrix4.h"

namespace engine
{
    class Transform;
    class Camera;
    class Device;
    class CommandList;
    class ShaderStorage;
    struct FlatScene;
    class ViewportRenderer;
    class PostprocessComponent;

    class CubePass
    {
    public:
        CubePass(Device& device, int width, int height);
        void render(
            CommandList& cmd,
            ViewportRenderer& viewportRenderer,
            Camera& camera,
            FlatScene& scene);
        TextureSRV& cubemap()
        {
            return m_cubemap;
        }
    private:
        TextureSRV m_cubemap;
        TextureRTV m_rtvs[6];
    };

    class ProbeComponent : public EngineComponent
    {
        Property size;
        Property m_range;
        Property updateButton;
    public:
        ProbeComponent();
        //ProbeComponent(std::shared_ptr<engine::Transform> transform);
        std::shared_ptr<engine::EngineComponent> clone() const override;

        bool update(Device& device, ShaderStorage& shaderStorage, CommandList& cmd, FlatScene& scene);

        TextureSRV& cubemap();
        TextureSRV& brdf();
        TextureSRV& irradiance();
        TextureSRV& specular();

        Vector3f position();
        float range() const;
    private:
        void initialize();
        bool m_updateNow;
        std::shared_ptr<Transform> m_transform;
        std::shared_ptr<Camera> m_camera;

        int m_lastWidth;
        int m_lastHeight;
        std::unique_ptr<CubePass> m_firstPass;
        std::unique_ptr<CubePass> m_secondPass;

        std::shared_ptr<ViewportRenderer> m_viewportRenderer;
        std::shared_ptr<PostprocessComponent> m_postprocess;
    };
}
