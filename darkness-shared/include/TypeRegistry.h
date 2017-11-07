#pragma once

#include <string>
#include <map>
#include "Type.h"
#include "Serialization.h"
#include "Property.h"

namespace serialization
{
    class TypeRegistry
    {
        std::vector<Type> m_types;
        Type& getType(const std::string& typeId);
    public:
        std::vector<Type>& types() { return m_types; }

        template<typename T>
        void registerType(
            std::function<void*()> construct,
            std::function<void(void*)> destroy,
            std::function<void(Stream&, void*)> serialize,
            std::function<void(Stream&, void*)> deserialize)
        {
            registerType(Type(typeName<T>(), construct, destroy, serialize, deserialize));
        }

        void registerType(Type type);

        template<typename T>
        void* newType(const std::string& type)
        {
            return newType(typeName<T>());
        }

        template<typename T>
        void deleteType(void* ptr)
        {
            deleteType(typeName<T>(), ptr);
        }

        template<typename T>
        void serialize(Stream& stream, void* ptr)
        {
            serialize(typeName<T>(), stream, ptr);
        }

        template<typename T>
        void deserialize(Stream& stream, void* ptr)
        {
            deserialize(typeName<T>(), stream, ptr);
        }

        template<typename T>
        void registerProperty(std::shared_ptr<Property> prop)
        {
            registerProperty(typeName<T>(), prop);
        }

        void* newType(const std::string& type);
        void deleteType(const std::string& type, void* ptr);

        void serialize(const std::string& type, Stream& stream, void* ptr);
        void deserialize(const std::string& type, Stream& stream, void* ptr);

        void registerProperty(const std::string& type, std::shared_ptr<Property> typeProperty);
    };
}
