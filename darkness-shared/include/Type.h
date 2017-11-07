#pragma once

#include <string>
#include <functional>
#include <typeinfo>
#include "Serialization.h"
#include "Property.h"

namespace serialization
{
    template<typename T>
    std::string typeName()
    {
        return std::string(typeid(T).name());
    }

    /*template<typename T>
    class TypeProperty
    {
    public:
        TypeProperty(
            const std::string& typeId,
            const std::string& propertyName,
            std::function<void(T)> setValue,
            std::function<T()> getValue)
            : m_typeId{ typeId }
            , m_propertyName{ propertyName }
            , m_setValue{ setValue }
            , m_getValue{ getValue }
        {}
    private:
        const std::string m_typeId;
        const std::string m_propertyName;
        std::function<void(T)> m_setValue;
        std::function<T()> m_getValue;
    };*/

    class Type
    {
    public:
        Type() {};
        Type(
            const std::string& typeIdStr,
            std::function<void*()> construct,
            std::function<void(void*)> destroy,
            std::function<void(Stream&, void*)> serialize,
            std::function<void(Stream&, void*)> deserialize);

        const std::string& typeId() const;
        void* construct();
        void destroy(void* ptr);
        void serialize(Stream&, void* ptr);
        void deserialize(Stream&, void* ptr);

        void registerProperty(std::shared_ptr<Property> prop)
        {
            m_properties.emplace_back(prop);
        }

        std::vector<std::shared_ptr<Property>>& properties()
        {
            return m_properties;
        }

        /*template<typename T>
        void registerProperty(TypeProperty<T> typeProperty)
        {
            m_properties.emplace_back(typeProperty);
        }

        std::vector<TypeProperty>& properties()
        {
            return m_properties;
        }*/
    private:
        std::string m_typeId;
        std::function<void*()> m_construct;
        std::function<void(void*)> m_destroy;
        std::function<void(Stream&, void*)> m_serialize;
        std::function<void(Stream&, void*)> m_deserialize;
        //std::vector<TypeProperty> m_properties;
        std::vector<std::shared_ptr<Property>> m_properties;
    };
}
