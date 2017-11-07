#pragma once

#include "engine/Scene.h"
#include "components/Camera.h"
#include "rendering/Rendering.h"
#include "platform/window/Window.h"
#include "engine/graphics/Fence.h"
#include "engine/graphics/Semaphore.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/SwapChain.h"
#include "engine/graphics/Resources.h"
#include "engine/rendering/Mesh.h"
#include "engine/rendering/ModelRenderer.h"
#include "engine/rendering/ModelTransparentRenderer.h"
#include "engine/rendering/ShadowRenderer.h"
#include "engine/rendering/ImguiRenderer.h"
#include "engine/rendering/RenderCubemap.h"
#include "engine/rendering/DebugView.h"
#include "engine/rendering/LightData.h"
#include "engine/rendering/LogWindow.h"
#include "engine/rendering/Postprocess.h"
#include "engine/RenderSetup.h"
#include "engine/InputEvents.h"
#include "engine/primitives/Quaternion.h"
#include "engine/primitives/Vector2.h"
#include "engine/primitives/Vector3.h"
#include "engine/primitives/Matrix4.h"

#include "components/MeshRendererComponent.h"
#include "components/MaterialComponent.h"
#include "components/LightComponent.h"
#include "components/PostprocessComponent.h"

#include <memory>
#include <string>
#include <chrono>

#include "btBulletDynamicsCommon.h"

class AssetConvert
{
public:
    AssetConvert(const std::shared_ptr<engine::Mesh>& source);

    std::vector<engine::Vertex> target;
    std::vector<uint32_t>       targetIndices;
};

constexpr float MoveSpeed = 2.0f;
constexpr float FastMoveSpeed = 10.0f;

class CameraInput
{
public:
    CameraInput()
        : m_camera{ nullptr }
        , m_mouseDown{ false }
        , m_lastX{ 0 }
        , m_lastY{ 0 }
        , m_up{ 0.0f, 1.0f, 0.0f }
        , m_right{ 1.0f, 0.0f, 0.0f }
        , m_forward{ 0.0f, 0.0f, 1.0f }
        , m_position{ 0.0f, 0.0f, 0.0f }
        , m_positionCurrent{ 0.0f, 0.0f, 0.0f }
        , m_upMove{ 0.0f, 0.0f, 0.0f }
        , m_forwardMove{ 0.0f, 0.0f, 0.0f }
        , m_upDown{ false }
        , m_downDown{ false }
        , m_rightDown{ false }
        , m_leftDown{ false }
        , m_shiftDown{ false }
        , m_altDown{ false }
        , m_pitch{ 0.0f }
        , m_yaw{ 0.0f }
        , m_pitchCurrent{ 0.0f }
        , m_yawCurrent{ 0.0f }
    {}
    CameraInput(std::shared_ptr<engine::Camera> camera)
        : m_camera{ camera }
        , m_mouseDown{ false }
        , m_lastX{ 0 }
        , m_lastY{ 0 }
        , m_up{ 0.0f, 1.0f, 0.0f }
        , m_right{ 1.0f, 0.0f, 0.0f }
        , m_forward{ 0.0f, 0.0f, 1.0f }
        , m_position{ 0.0f, 0.0f, 0.0f }
        , m_positionCurrent{ 0.0f, 0.0f, 0.0f }
        , m_upMove{ 0.0f, 0.0f, 0.0f }
        , m_forwardMove{ 0.0f, 0.0f, 0.0f }
        , m_upDown{ false }
        , m_downDown{ false }
        , m_rightDown{ false }
        , m_leftDown{ false }
        , m_shiftDown{ false }
        , m_altDown{ false }
        , m_pitch{ 0.0f }
        , m_yaw{ 0.0f }
        , m_pitchCurrent{ 0.0f }
        , m_yawCurrent{ 0.0f }
    {}

    void setCamera(std::shared_ptr<engine::Camera>& camera)
    {
        m_camera = camera;
    }

    void onMouseMove(int x, int y);
    void onMouseDown(engine::MouseButton button, int x, int y);
    void onMouseUp(engine::MouseButton button, int x, int y);
    void onMouseDoubleClick(engine::MouseButton button, int x, int y);

    void onKeyDown(engine::Key key, const engine::ModifierState& modifierState)
    {
        m_shiftDown = modifierState[engine::KeyModifier::ShiftLeft] || modifierState[engine::KeyModifier::ShiftRight];
        m_altDown = modifierState[engine::KeyModifier::AltLeft] || modifierState[engine::KeyModifier::AltRight];
        switch (key)
        {
        case engine::Key::ArrowUp: { m_upDown = true; break; }
        case engine::Key::ArrowDown: { m_downDown = true; break; }
        case engine::Key::ArrowRight: { m_rightDown = true; break; }
        case engine::Key::ArrowLeft: { m_leftDown = true; break; }

        case engine::Key::W: { m_upDown = true; break; }
        case engine::Key::S: { m_downDown = true; break; }
        case engine::Key::D: { m_rightDown = true; break; }
        case engine::Key::A: { m_leftDown = true; break; }
        default: break;
        }
    }

    void onKeyUp(engine::Key key, const engine::ModifierState& modifierState)
    {
        m_shiftDown = modifierState[engine::KeyModifier::ShiftLeft] || modifierState[engine::KeyModifier::ShiftRight];
        m_altDown = modifierState[engine::KeyModifier::AltLeft] || modifierState[engine::KeyModifier::AltRight];
        switch (key)
        {
        case engine::Key::ArrowUp: { m_upDown = false; break; }
        case engine::Key::ArrowDown: { m_downDown = false; break; }
        case engine::Key::ArrowRight: { m_rightDown = false; break; }
        case engine::Key::ArrowLeft: { m_leftDown = false; break; }

        case engine::Key::W: { m_upDown = false; break; }
        case engine::Key::S: { m_downDown = false; break; }
        case engine::Key::D: { m_rightDown = false; break; }
        case engine::Key::A: { m_leftDown = false; break; }
        default: break;
        }
    }

    bool hasCamera() const
    {
        return (bool)m_camera;
    }

    void position(const engine::Vector3f& position)
    {
        m_position = position;
        m_positionCurrent = m_position;
    }

    void rotation(const engine::Vector3f& rotation)
    {
        m_pitch = rotation.x;
        m_yaw = rotation.y;
        if (m_yaw < 0)
            m_yaw += 360.0f;
        m_pitchCurrent = m_pitch;
        m_yawCurrent = m_yaw;
    }

    engine::Vector3f position() const
    {
        return m_position;
    }

    engine::Vector3f rotation() const
    {
        return engine::Vector3f{ m_pitch, m_yaw, 0.0f };
    }

    void update(float delta)
    {
        float move = delta * MoveSpeed;
        if (m_shiftDown)
            move = delta * FastMoveSpeed;

        if(m_upDown)
            m_position -= m_forwardMove * move;
        if(m_downDown)
            m_position += m_forwardMove * move;

        if (m_rightDown)
            m_position += m_right * move;
        if (m_leftDown)
            m_position -= m_right * move;

        if(m_camera)
            apply();
    }

    std::pair<int, int> mousePosition() const
    {
        return { m_lastX, m_lastY };
    }
private:
    std::shared_ptr<engine::Camera> m_camera;
    bool m_mouseDown;
    int m_lastX;
    int m_lastY;
    std::pair<int, int> delta(int x, int y) const;

    engine::Vector3f m_up;
    engine::Vector3f m_right;
    engine::Vector3f m_forward;
    
    engine::Vector3f m_position;
    engine::Vector3f m_upMove;
    engine::Vector3f m_forwardMove;

    engine::Vector3f m_positionCurrent;

    bool m_upDown;
    bool m_downDown;
    bool m_rightDown;
    bool m_leftDown;
    bool m_shiftDown;
    bool m_altDown;

    float m_pitch;
    float m_yaw;

    float m_pitchCurrent;
    float m_yawCurrent;

    void closeInOnYaw(const float& target, float& current, const float& range)
    {
        constexpr float steps = 3.3f;
        float halfRange = range / 2.0f;

        float right = target + halfRange;
        bool onTheRight = false;
        float distance = 0.0f;
        if (right > range)
        {
            right -= range;
            if (current >= 0 && current < right)
            {
                // current is on the right of target
                // looped around 0
                distance = (range - target) + current;
                onTheRight = true;
            }
            else if(current >= target && current < range)
            {
                // current is on the right of target
                distance = current - target;
                onTheRight = true;
            }
            else
            {
                // current is on left from target
                if (current <= target)
                {
                    distance = target - current;
                    onTheRight = false;
                }
                else
                {
                    distance = target + (range - current);
                    onTheRight = false;
                }
            }
        }
        else
        {
            // current is on the right of target
            if (current >= target && current < right)
            {
                distance = current - target;
                onTheRight = true;
            }
            else
            {
                // current on left
                if (current < target)
                {
                    distance = target - current;
                    onTheRight = false;
                }
                else
                {
                    distance = target + (range - current);
                    onTheRight = false;
                }
            }
        }
        
        if (distance > range)
        {
            LOG("distance: %f", distance);
        }

        float dd = distance / steps;
        if (onTheRight)
        {
            current -= dd;
            if (current < 0)
                current += range;
        }
        else
        {
            current += dd;
            if (current >= range)
                current -= range;
        }

        if (current < 0)
        {
            LOG("the hell");
        }
    }

    void closeInOnPitch(const float& target, float& current)
    {
        float distance = 0.0f;
        constexpr float steps = 4.0f;
        if (target > current)
        {
            distance = target - current;
            float dd = distance / steps;
            current += dd;
        }
        else
        {
            distance = current - target;
            float dd = distance / steps;
            current -= dd;
        }
    }

    void applyYaw(float angle)
    {
        m_yaw += angle * 0.3f;
        if (m_yaw < 0.0f)
            m_yaw += 360.0f;
        if (m_yaw >= 360.0f)
            m_yaw -= 360.0f;
    };

    void applyPitch(float angle)
    {
        m_pitch += angle * 0.3f;
        if (m_pitch < -90.0f)
            m_pitch = -90.0f;
        else if (m_pitch > 90.0f)
            m_pitch = 90.0f;
    };

public:
    void apply(bool updateCamera = true)
    {
        closeInOnPitch(m_pitch, m_pitchCurrent);
        closeInOnYaw(m_yaw, m_yawCurrent, 360.0f);

        float sp = std::sinf(DEG_TO_RAD * m_pitchCurrent);
        float cp = std::cosf(DEG_TO_RAD * m_pitchCurrent);
        float sy = std::sinf(DEG_TO_RAD * m_yawCurrent);
        float cy = std::cosf(DEG_TO_RAD * m_yawCurrent);

        m_right = { cy, 0.0f, -sy };
        m_forward = { sy * cp, -sp, cy * cp };
        m_up = m_forward.cross(m_right);

        m_upMove = { 0.0f, 1.0f, 0.0f };

        //m_forwardMove = { sy, 0.0f, cy };
        m_forwardMove = m_forward;

        engine::Vector3f distance = m_position - m_positionCurrent;
        m_positionCurrent += distance / 3.3f;


        engine::Matrix4f modelMatrix{
            m_right.x,        m_right.y,        m_right.z,        0.0f,
            m_up.x,            m_up.y,            m_up.z,            0.0f,
            m_forward.x,    m_forward.y,    m_forward.z,    0.0f,
            m_positionCurrent.x,    m_positionCurrent.y,    m_positionCurrent.z,    1.0f};

        engine::Matrix4f viewMatrix{
            m_right.x,        m_up.x,            m_forward.x,    0.0f,
            m_right.y,        m_up.y,            m_forward.y,    0.0f,
            m_right.z,        m_up.z,            m_forward.z,    0.0f,
            m_positionCurrent.dot(m_right),    -m_positionCurrent.dot(m_up),    m_positionCurrent.dot(m_forward),    1.0f };

        if (updateCamera)
        {
            m_camera->rotation(engine::Quaternionf::fromMatrix(viewMatrix));
            m_camera->position(m_positionCurrent);
        }
        
        m_camera->forward(m_forward);
        m_camera->up(m_up);
        m_camera->right(m_right);
    }
private:
    void moveAlongSideAxis(float t)
    {
        m_position += m_right * t;
    }
    // --------------------------------------------------------------------------------------
    void moveAlongUpAxis(float t)
    {
        m_position += m_upMove * t;
    }
    // --------------------------------------------------------------------------------------
    void moveAlongForwardAxis(float t)
    {
        m_position += m_forwardMove * t;
    }

    void orthonormalize()
    {
        m_forward.normalize();
        m_right = m_up.cross(m_forward);
        m_right.normalize();
        m_up = m_forward.cross(m_right);
    }
};

class Engine
{
public:
    Engine(
        std::shared_ptr<platform::Window> window,
        const std::string& shaderRootPath);
    void update();
    void shutdown();
    void refresh();
    engine::ShaderStorage& shaderStorage();
    engine::Scene& scene();

    void onMouseMove(int x, int y);
    void onMouseDown(engine::MouseButton button, int x, int y);
    void onMouseUp(engine::MouseButton button, int x, int y);
    void onMouseDoubleClick(engine::MouseButton button, int x, int y);

    void onKeyDown(engine::Key key, const engine::ModifierState& modifierState);
    void onKeyUp(engine::Key key, const engine::ModifierState& modifierState);

    void cameraInputActive(bool active);
    void resetCameraSize();
    void playClicked(bool value);

private:
    std::shared_ptr<platform::Window> m_window;
    std::unique_ptr<engine::RenderSetup> m_renderSetup;
    std::string m_shaderRootPath;
    engine::Scene m_scene;
    engine::Vector2<int> m_virtualResolution;
    std::unique_ptr<engine::ModelRenderer> m_modelRenderer;
    std::unique_ptr<engine::ModelTransparentRenderer> m_modelTransparentRenderer;
    std::unique_ptr<engine::ShadowRenderer> m_shadowRenderer;
    std::unique_ptr<engine::ImguiRenderer> m_imguiRenderer;
    std::unique_ptr<engine::RenderCubemap> m_renderCubemap;
    std::unique_ptr<engine::Postprocess> m_postProcess;
    std::unique_ptr<engine::DebugView> m_debugViewer;
    engine::LogWindow m_logWindow;

    engine::TextureRTV m_rtv;
    engine::TextureSRV m_srv;
    engine::TextureDSV m_dsv;
    engine::TextureSRV m_dsvSRV;

    bool m_cameraSizeRefresh;

    CameraInput m_cameraInput;
    bool m_cameraInputActive;

    std::unique_ptr<engine::MaterialComponent> m_defaultMaterial;
    std::unique_ptr<engine::LightData> m_lightData;

    void render();
    void clear(engine::CommandList& cmd);
    void updateEnvironmentCubemap(engine::Device& device, engine::Camera& camera);
    void updateLighting(engine::Device& device, engine::CommandList& cmd, engine::FlatScene& flatScene);
    void renderEnvironmentCubemap(engine::Device& device, engine::CommandList& cmd, engine::FlatScene& flatScene);
    void renderShadows(engine::Device& device, engine::CommandList& cmd, engine::FlatScene& flatScene);
    void renderModels(engine::Device& device, engine::CommandList& cmd, engine::FlatScene& flatScene);
    void renderTransparentModels(engine::Device& device, engine::CommandList& cmd, engine::FlatScene& flatScene);
    void renderPostprocess(engine::Device& device, engine::CommandList& cmd, engine::FlatScene& flatScene);
    void renderDebugView(engine::CommandList& cmd);
    void processShaderHotreload();

    float delta();
    std::chrono::high_resolution_clock::time_point m_lastUpdate;

    std::shared_ptr<engine::Camera> camera();
    std::string m_lastEnvironmentMap;
    std::shared_ptr<engine::Transform> m_cameraTransform;
    engine::Camera m_camera;

    // bullet
    std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> m_dispatcher;
    std::unique_ptr<btBroadphaseInterface> m_overlappingPairCache;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;

    std::unique_ptr<btAlignedObjectArray<btCollisionShape*>> m_collisionShapes;

    std::vector<void*> m_addedRigidBodies;



    bool m_simulating;
};
