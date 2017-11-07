#pragma once

#include "tools/Debug.h"
#include "engine/ComponentRegister.h"
#include "tools/Serialization.h"
#include <string>
#include <memory>
#include <functional>

namespace engine
{
    enum class ButtonPush
    {
        NotPressed,
        Pressed
    };
    enum class ButtonToggle
    {
        NotPressed,
        Pressed
    };

    class SceneNode;
    class Property;
    class EngineComponent : public ComponentRegister
    {
    protected:
        std::string m_name;

        template <class T>
        std::shared_ptr<T> getComponent()
        {
            auto components = getComponents();
            for (auto& component : components)
            {
                if (std::dynamic_pointer_cast<typename std::decay<T>::type>(component))
                {
                    return std::dynamic_pointer_cast<typename std::decay<T>::type>(component);
                }
            }
            return nullptr;
        }

        std::vector<std::shared_ptr<EngineComponent>> getComponents();

        friend class Property;
        virtual void onValueChanged() {};

    public:
        virtual ~EngineComponent() {};

        std::string name() const;
        void name(const std::string& name);

        void parentNode(std::shared_ptr<SceneNode> node);
        std::shared_ptr<SceneNode> parentNode() const;

        virtual void start() {};

        void writeJsonValue(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

        virtual std::shared_ptr<engine::EngineComponent> clone() const = 0;
    private:
        std::shared_ptr<SceneNode> m_parentNode;
    };
}
