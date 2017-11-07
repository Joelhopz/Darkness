#include "plugins/TypeFactory.h"
#include <algorithm>

namespace engine
{
    void TypeFactory::registerType(
        const std::string& typeName,
        CreateFunction create,
        DestroyFunction destroy)
    {
        if (m_types.find(typeName) == m_types.end())
        {
            m_types[typeName] = TypeContainer{ typeName, create, destroy };
        }
    }

    void TypeFactory::unregisterType(
        const std::string& typeName)
    {
        auto container = m_types.find(typeName);
        if(container != m_types.end())
        {
            m_types.erase(container);
        }
    }

    const std::unordered_map<std::string, TypeContainer>& TypeFactory::types() const
    {
        return m_types;
    }

    void* TypeFactory::createType(const std::string& typeName) const
    {
        auto container = m_types.find(typeName);
        if (container != m_types.end())
        {
            return container->second.create();
        }
        return nullptr;
    }

    void TypeFactory::destroyType(const std::string& typeName, void* typeInstance) const
    {
        auto container = m_types.find(typeName);
        if (container != m_types.end())
        {
            return container->second.destroy(typeInstance);
        }
    }
}
