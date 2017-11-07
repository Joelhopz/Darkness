#pragma once

#include "engine/primitives/Rect.h"

#include <functional>
#include <string>
#include <memory>

class Visual
{
public:
    virtual void draw(engine::Rect) = 0;
};

struct DrawerCallbacks
{
    void* propertyWidget;
    std::function<void(void* propertyWidget, engine::Rect, const std::string&)> drawString;
};

template<typename T>
std::shared_ptr<Visual> createDrawer(const T& item, DrawerCallbacks callbacks);
