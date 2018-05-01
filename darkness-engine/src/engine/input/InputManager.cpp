#include "engine/input/InputManager.h"
#include "tools/Debug.h"
#include "components/Camera.h"
#include <gainput/gainput.h>

using namespace gainput;

namespace engine
{
    InputManager::InputManager(int width, int height)
        : m_inputManager{ std::make_shared<gainput::InputManager>() }
        , m_inputMap{ std::make_shared<InputMap>(*m_inputManager) }
        , m_camera{ nullptr }
        , m_pitch{ 0.0f }
        , m_yaw{ 0.0f }
    {
        m_inputManager->SetDisplaySize(width, height);
        const DeviceId keyboardId = m_inputManager->CreateDevice<InputDeviceKeyboard>();
        const DeviceId mouseId = m_inputManager->CreateDevice<InputDeviceMouse>();
        const DeviceId padId = m_inputManager->CreateDevice<InputDevicePad>();

        m_inputMap->MapBool(static_cast<UserButtonId>(InputActions::Confirm), keyboardId, KeyReturn);
        m_inputMap->MapBool(static_cast<UserButtonId>(InputActions::Confirm), padId, PadButtonA);

        m_inputMap->MapFloat(static_cast<UserButtonId>(InputActions::LeftStickX), padId, PadButtonLeftStickX);
        m_inputMap->MapFloat(static_cast<UserButtonId>(InputActions::LeftStickY), padId, PadButtonLeftStickY);
        m_inputMap->MapFloat(static_cast<UserButtonId>(InputActions::RightStickX), padId, PadButtonRightStickX);
        m_inputMap->MapFloat(static_cast<UserButtonId>(InputActions::RightStickY), padId, PadButtonRightStickY);
    }

    void InputManager::update()
    {
        m_inputManager->Update();

        if (m_inputMap->GetBoolWasDown(static_cast<UserButtonId>(InputActions::Confirm)))
        {
            LOG("Confirmed!");
        }

        updateCamera();
    }

    void InputManager::updateCamera()
    {
        auto leftStickX = m_inputMap->GetFloat(static_cast<UserButtonId>(InputActions::LeftStickX)) * 0.4f;
        auto leftStickY = m_inputMap->GetFloat(static_cast<UserButtonId>(InputActions::LeftStickY)) * 0.4f;
        auto rightStickX = m_inputMap->GetFloat(static_cast<UserButtonId>(InputActions::RightStickX));
        auto rightStickY = m_inputMap->GetFloat(static_cast<UserButtonId>(InputActions::RightStickY));

        if (m_camera && ((rightStickX != 0.0f) || (rightStickY) || (leftStickY != 0.0f) || (leftStickX != 0.0f)))
        {
            m_camera->position(m_camera->position() + (m_camera->forward() * -leftStickY) + (m_camera->right() * leftStickX));

            m_pitch += -rightStickY * 2.0f;
            m_yaw += -rightStickX * 2.0f;

            float sp = std::sinf(DEG_TO_RAD * m_pitch);
            float cp = std::cosf(DEG_TO_RAD * m_pitch);
            float sy = std::sinf(DEG_TO_RAD * m_yaw);
            float cy = std::cosf(DEG_TO_RAD * m_yaw);

            m_right = { cy, 0.0f, -sy };
            m_forward = { sy * cp, -sp, cy * cp };
            m_up = m_forward.cross(m_right);

            m_upMove = { 0.0f, 1.0f, 0.0f };

            //m_forwardMove = { sy, 0.0f, cy };
            m_forwardMove = m_forward;

            auto pos = m_camera->position();

            engine::Matrix4f modelMatrix{
                m_right.x,        m_right.y,        m_right.z,        0.0f,
                m_up.x,            m_up.y,            m_up.z,            0.0f,
                m_forward.x,    m_forward.y,    m_forward.z,    0.0f,
                pos.x,    pos.y,    pos.z,    1.0f };

            engine::Matrix4f viewMatrix{
                m_right.x,        m_up.x,            m_forward.x,    0.0f,
                m_right.y,        m_up.y,            m_forward.y,    0.0f,
                m_right.z,        m_up.z,            m_forward.z,    0.0f,
                pos.dot(m_right),    -pos.dot(m_up),    pos.dot(m_forward),    1.0f };

            m_camera->rotation(engine::Quaternionf::fromMatrix(viewMatrix));

            m_camera->forward(m_forward);
            m_camera->up(m_up);
            m_camera->right(m_right);
        }
    }

    void InputManager::setCamera(Camera* camera)
    {
        m_camera = camera;
    }
}
