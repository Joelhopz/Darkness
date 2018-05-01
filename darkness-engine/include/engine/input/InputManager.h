#pragma once

#include "engine/primitives/Vector3.h"
#include <memory>

namespace gainput
{
    class InputManager;
    class InputMap;
}

namespace engine
{
    enum class InputActions
    {
        Confirm,
        LeftStickX,
        LeftStickY,
        RightStickX,
        RightStickY
    };

    class Camera;

    class InputManager
    {
    public:
        InputManager(int width, int height);

        void setCamera(Camera* camera);

        void update();
    private:
        std::shared_ptr<gainput::InputManager> m_inputManager;
        std::shared_ptr<gainput::InputMap> m_inputMap;
        
        Camera* m_camera;
        float m_pitch;
        float m_yaw;
        engine::Vector3f m_up;
        engine::Vector3f m_right;
        engine::Vector3f m_forward;
        engine::Vector3f m_upMove;
        engine::Vector3f m_forwardMove;

        void updateCamera();
    };
}
