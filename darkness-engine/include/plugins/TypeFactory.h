#pragma once

#include <unordered_map>
#include "TypeFactoryIf.h"

namespace engine
{
    class TypeFactory : public TypeFactoryIf
    {
    public:
        void registerType(
            const std::string& typeName,
            CreateFunction create,
            DestroyFunction destroy) override;

        void unregisterType(
            const std::string& typeName
        ) override;

        void* createType(const std::string& typeName) const;
        void destroyType(const std::string& typeName, void* typeInstance) const;

        const std::unordered_map<std::string, TypeContainer>& types() const override;
    private:
        
        std::unordered_map<std::string, TypeContainer> m_types;
    };
}
