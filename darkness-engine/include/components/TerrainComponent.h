#pragma once

#include "engine/EngineComponent.h"
#include "tools/Property.h"

namespace engine
{
    class Transform;

    class TerrainComponent : public EngineComponent
    {
        Property m_height;
    public:
        TerrainComponent();
        std::shared_ptr<engine::EngineComponent> clone() const override;
    private:
        std::shared_ptr<Transform> m_transform;
    };
}
