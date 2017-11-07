#pragma once

#include <string>
#include <functional>

namespace engine
{
    using CreateFunction = std::function<void*()>;
    using DestroyFunction = std::function<void(void*)>;

    struct TypeContainer
    {
        std::string typeName;
        CreateFunction create;
        DestroyFunction destroy;
    };

    class TypeFactoryIf
    {
    public:
        virtual void registerType(
            const std::string& typeName,
            CreateFunction create,
            DestroyFunction destroy) = 0;

        virtual void unregisterType(const std::string& typeName) = 0;

        virtual const std::unordered_map<std::string, TypeContainer>& types() const = 0;
    };
}
