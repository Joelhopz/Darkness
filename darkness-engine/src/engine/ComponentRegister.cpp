#include "engine/ComponentRegister.h"
#include "tools/Property.h"

namespace engine
{
    int ComponentRegister::propertyCount() const { return static_cast<int>(m_properties.size()); }

    std::vector<std::string> ComponentRegister::propertyNames() const
    {
        std::vector<std::string> names;
        for (const auto& prop : m_properties)
        {
            names.emplace_back(prop.first);
        }
        return names;
    }

    bool ComponentRegister::hasVariant(const std::string& propertyName) const
    {
        return m_properties.find(propertyName) != m_properties.end();
    }
    
    const Property& ComponentRegister::variant(const std::string& propertyName) const
    {
        return *m_properties[propertyName];
    }

    Property& ComponentRegister::variant(const std::string& propertyName)
    {
        return *m_properties[propertyName];
    }

    void ComponentRegister::registerProperty(const std::string& propertyName, Property* variant)
    {
        m_properties[propertyName] = variant;

        auto loaded = m_loadedButNotExistingProperties.find(propertyName);
        if (loaded != m_loadedButNotExistingProperties.end())
        {
            variant->swap(*loaded->second);
            m_loadedButNotExistingProperties.erase(propertyName);
        }

        for (auto&& notify : m_changeNotifications)
        {
            notify.second();
        }
    }

    void ComponentRegister::unregisterProperty(const std::string& propertyName, Property* /*variant*/)
    {
        for (auto&& notify : m_changeNotifications)
        {
            notify.second();
        }

        m_properties.erase(propertyName);
    }
}
