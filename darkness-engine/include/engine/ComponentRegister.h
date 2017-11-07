#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>

namespace engine
{
    class Property;
    class ComponentRegister
    {
    public:
        virtual ~ComponentRegister() {};
        int propertyCount() const;
        std::vector<std::string> propertyNames() const;

        bool hasVariant(const std::string& propertyName) const;
        const Property& variant(const std::string& propertyName) const;
        Property& variant(const std::string& propertyName);

        template<typename T>
        void insertLoadedValue(const std::string& propertyName, T value)
        {
            m_loadedButNotExistingProperties[propertyName] = std::make_shared<Property>(nullptr, "temporary", value);
        }

        void registerProperty(const std::string& propertyName, Property* variant);
        void unregisterProperty(const std::string& propertyName, Property* variant);

        void registerForChanges(void* client, std::function<void()> change)
        {
            m_changeNotifications[client] = change;
        }
        void unregisterForChanges(void* client)
        {
            m_changeNotifications.erase(client);
        }

    protected:
        mutable std::unordered_map<std::string, Property*> m_properties;
        mutable std::unordered_map<std::string, std::shared_ptr<Property>> m_loadedButNotExistingProperties;
        std::unordered_map<void*, std::function<void()>> m_changeNotifications;
    };
}
