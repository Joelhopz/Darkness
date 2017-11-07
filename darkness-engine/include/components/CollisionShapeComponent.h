#pragma once

#include "engine/EngineComponent.h"
#include "tools/Property.h"

class btCollisionShape;

namespace engine
{
    enum class CollisionShape
    {
        Plane,
        Box,
        Capsule,
        Sphere,
        Cylinder,
        Cone,
        BvhTriangleMesh,
        ConvexHull,
        ConvexTriangleMesh
    };

    std::string collisionShapeToString(const CollisionShape& shape);
    CollisionShape stringToCollisionShape(const std::string& shape);

    class CollisionShapeComponent : public EngineComponent
    {
        Property m_shape;
    public:
        CollisionShapeComponent();
        std::shared_ptr<engine::EngineComponent> clone() const override;

        std::shared_ptr<btCollisionShape> shape()
        {
            return m_collisionShape;
        }
        
    private:
        std::shared_ptr<btCollisionShape> m_collisionShape;

        void refreshShape();
    };
}
