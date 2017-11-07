#pragma once

#include "engine/EngineComponent.h"
#include "tools/Property.h"

class btRigidBody;

namespace engine
{
    class Transform;
    class CollisionShapeComponent;

    class RigidBodyComponent : public EngineComponent
    {
        Property m_mass;
    public:
        RigidBodyComponent();
        std::shared_ptr<engine::EngineComponent> clone() const override;

        std::shared_ptr<btRigidBody> body();
    private:
        std::shared_ptr<Transform> m_transform;
        std::shared_ptr<CollisionShapeComponent> m_collisionShape;

        std::shared_ptr<btRigidBody> m_rigidBody;
        
    };
}
