#pragma once

#include <memory>
#include <string>

class Drawer
{
public:
    virtual ~Drawer() {};
    virtual void* native() = 0;
    virtual void setParent(void* parent) = 0;
    virtual void setPaths(const std::string& /*contentPath*/, const std::string& /*processedContentPath*/) {};
};

namespace engine
{
    class Property;
    template <typename T>
    std::shared_ptr<Drawer> createDrawer(Property& value);
}
