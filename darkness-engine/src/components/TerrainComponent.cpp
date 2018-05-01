#include "components/TerrainComponent.h"
#include "components/Transform.h"

namespace engine
{
    TerrainComponent::TerrainComponent()
        : m_height{ this, "height", float(10.0f), [this]() { 
            //this->m_rangeChanged = true;
        }}
    {
        m_name = "Terrain";
    }

    /*std::shared_ptr<btRigidBody> TerrainComponent::body()
    {
        if (!m_transform)
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
    }*/

    std::shared_ptr<engine::EngineComponent> TerrainComponent::clone() const
    {
        return std::make_shared<engine::TerrainComponent>();
    }
}
