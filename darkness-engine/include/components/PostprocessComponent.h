#pragma once

#include "engine/EngineComponent.h"
#include "tools/Property.h"
#include "engine/primitives/Vector3.h"
#include "engine/primitives/Matrix4.h"

namespace engine
{
    class PostprocessComponent : public EngineComponent
    {
        Property m_bloomEnabled;
    public:
        PostprocessComponent()
            : m_bloomEnabled{ this, "bloom enabled", engine::ButtonToggle::NotPressed }
        {
            m_name = "PostprocessComponent";
        }

        bool bloomEnabled() const
        {
            return static_cast<bool>(m_bloomEnabled.value<ButtonToggle>());
        }

        void bloomEnabled(bool value)
        {
            m_bloomEnabled.value<ButtonToggle>(static_cast<ButtonToggle>(value));
        }

        std::shared_ptr<engine::EngineComponent> clone() const override
        {
            auto postProcessComponent = std::make_shared<engine::PostprocessComponent>();
            postProcessComponent->bloomEnabled(bloomEnabled());
            return postProcessComponent;
        }
    };
}
