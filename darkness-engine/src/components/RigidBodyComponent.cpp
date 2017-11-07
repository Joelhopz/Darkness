#include "components/RigidBodyComponent.h"
#include "components/CollisionShapeComponent.h"
#include "components/Transform.h"
#include "btBulletDynamicsCommon.h"

namespace engine
{
    RigidBodyComponent::RigidBodyComponent()
        : m_mass{ this, "Mass", 1.0f }
    {
        m_name = "RigidBody";
        
        /*btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);*/
    }

    std::shared_ptr<btRigidBody> RigidBodyComponent::body()
    {
        if(!m_transform)
            m_transform = getComponent<Transform>();

        if (!m_collisionShape)
            m_collisionShape = getComponent<CollisionShapeComponent>();

        if (m_transform && m_collisionShape)
        {
            if (!m_rigidBody)
            {
                auto pos = m_transform->position();
                btTransform transform;
                transform.setIdentity();
                transform.setOrigin(btVector3{ pos.x, pos.y, pos.z });

                auto shape = m_collisionShape->shape();

                btVector3 localInertia(0, 0, 0);
                bool dynamic = m_mass.value<float>() != 0.0f;
                if (dynamic)
                    shape->calculateLocalInertia(m_mass.value<float>(), localInertia);

                btDefaultMotionState* motionState = new btDefaultMotionState(transform);
                btRigidBody::btRigidBodyConstructionInfo rbInfo(
                    m_mass.value<float>(), 
                    motionState, 
                    shape.get(), 
                    localInertia);

                m_rigidBody = std::make_shared<btRigidBody>(rbInfo);
            }
        }

        return m_rigidBody;
    }

    std::shared_ptr<engine::EngineComponent> RigidBodyComponent::clone() const
    {
        return std::make_shared<engine::RigidBodyComponent>();
    }
}
