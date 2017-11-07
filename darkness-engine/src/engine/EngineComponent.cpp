#include "engine/EngineComponent.h"
#include "tools/Property.h"
#include "engine/Scene.h"
#include "tools/Serialization.h"

namespace engine
{
    std::string EngineComponent::name() const
    {
        return m_name;
    }

    void EngineComponent::name(const std::string& name) 
    {
        m_name = name;
    }

    void EngineComponent::parentNode(std::shared_ptr<SceneNode> node)
    {
        m_parentNode = node;
    }

    std::shared_ptr<SceneNode> EngineComponent::parentNode() const
    {
        return m_parentNode;
    }

    std::vector<std::shared_ptr<EngineComponent>> EngineComponent::getComponents()
    {
        std::vector<std::shared_ptr<EngineComponent>> result;
        for (size_t i = 0; i < m_parentNode->componentCount(); ++i)
        {
            result.emplace_back(m_parentNode->component(i));
        }
        return result;
    }

    void EngineComponent::writeJsonValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        std::string key = serialization::keyPrefix<serialization::KeyTypes, serialization::TypeId>(serialization::ComponentName);
        writer.Key(key.data());
        writer.String(m_name.data());

        key = serialization::keyPrefix<serialization::KeyTypes, serialization::TypeId>(serialization::PropertyList);
        writer.Key(key.data());

        writer.StartArray();
        for (const auto& prop : m_properties)
        {
            writer.StartObject();
            prop.second->writeJson(writer);
            writer.EndObject();
        }

        writer.EndArray();
        writer.EndObject();
    }
}
