#include "DummyDeclarations.h"
#include "engine/primitives/Quaternion.h"
#include "engine/primitives/Vector3.h"
#include "engine/primitives/Vector4.h"
#include "engine/EngineComponent.h"

#include <memory>

namespace engine
{
    template <>
    std::shared_ptr<Drawer> createDrawer<int>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<float>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<double>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<Vector2f>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<Vector3f>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<Vector4f>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<Matrix3f>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<Matrix4f>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<Projection>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<CollisionShape>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<std::string>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<engine::Quaternionf>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<LightType>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<engine::ButtonToggle>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<engine::ButtonPush>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<bool>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<char>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<short>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<unsigned char>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<unsigned short>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }

    template <>
    std::shared_ptr<Drawer> createDrawer<unsigned int>(Property& /*value*/) { return std::make_shared<DrawerAbs>(); }
}
