#include "TypeRegistry.h"
#include <algorithm>

namespace serialization
{
    Type& TypeRegistry::getType(const std::string& typeId)
    {
        auto index = std::find_if(
            m_types.begin(), 
            m_types.end(), 
            [typeId](Type type) 
        { return type.typeId() == typeId; });
        return *index;
    }

    void TypeRegistry::registerType(Type type)
    {
        m_types.emplace_back(type);
    };

    void* TypeRegistry::newType(const std::string& type)
    {
        return getType(type).construct();
    }

    void TypeRegistry::deleteType(const std::string& type, void* ptr)
    {
        getType(type).destroy(ptr);
    }

    void TypeRegistry::serialize(const std::string& type, Stream& stream, void* ptr)
    {
        return getType(type).serialize(stream, ptr);
    }

    void TypeRegistry::deserialize(const std::string& type, Stream& stream, void* ptr)
    {
        getType(type).deserialize(stream, ptr);
    }

    void TypeRegistry::registerProperty(const std::string& type, std::shared_ptr<Property> typeProperty)
    {
        getType(type).registerProperty(typeProperty);
    }
}
