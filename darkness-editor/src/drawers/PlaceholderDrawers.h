#pragma once

#include "engine/Drawer.h"
#include <memory>

namespace engine
{
    class DrawerAbs : public Drawer
    {
    public:
        virtual ~DrawerAbs() {};
        virtual void* native()
        {
            return nullptr;
        };
        virtual void setParent(void* /*parent*/) {};
    };
}
