#pragma once

#include "engine/Drawer.h"
#include "engine/EngineComponent.h"
#include "tools/Serialization.h"
#include <memory>
#include <string>
#include <functional>

namespace engine
{

    using PropertyChanged = std::function<void()>;

    class Property
    {
    public:
        Property() {};

        Property(const Property&) = delete;
        Property& operator=(const Property&) = delete;
        Property(Property&&) = delete;
        Property& operator=(Property&&) = delete;

        template <class T>
        Property(EngineComponent* component, const std::string& name, T _value, PropertyChanged onChanged)
            : m_impl(std::make_shared<VariantImpl<typename std::decay<T>::type>>(_value))
            , m_name{ name }
            , m_component{ component }
        {
            m_onChanged[this] = { onChanged };
#ifndef GAME_BUILD
            createVariantDrawer = [&]()->std::shared_ptr<Drawer>
            {
                return createDrawer<typename std::decay<T>::type>(*this);
            };
#endif
            writeJson = [&](rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
            {
                writeJsonValue<typename std::decay<T>::type>(writer, m_name, dynamic_cast<VariantImpl<typename std::decay<T>::type>&>(*m_impl.get()).m_value);
            };
            if(component)
                component->registerProperty(name, this);
        }

        void swap(Property& property)
        {
            std::swap(m_impl, property.m_impl);
        }

        template <class T>
        Property(EngineComponent* component, const std::string& name, T _value)
            : m_impl(std::make_shared<VariantImpl<typename std::decay<T>::type>>(_value))
            , m_name{ name }
            , m_component{ component }
        {
#ifndef GAME_BUILD
            createVariantDrawer = [&]()->std::shared_ptr<Drawer>
            {
                return createDrawer<typename std::decay<T>::type>(*this);
            };
#endif
            writeJson = [&](rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
            {
                writeJsonValue<typename std::decay<T>::type>(writer, m_name, dynamic_cast<VariantImpl<typename std::decay<T>::type>&>(*m_impl.get()).m_value);
            };
            if(component)
                component->registerProperty(name, this);
        }

        ~Property()
        {
            for (auto&& client : m_onRemove)
            {
                for (auto&& onRemoved : client.second)
                {
                    onRemoved();
                }
            }

            if(m_component)
                m_component->unregisterProperty(m_name, this);
        }

        template<typename T>
        void setRangeCheck(std::function<bool(T, T&)> check)
        {
            auto im = dynamic_cast<VariantImpl<typename std::decay<T>::type>&>(*m_impl.get());
            im.m_rangeCheck = check;
        }

        template<typename T>
        bool rangeCheck(T value, T& closestPossible)
        {
            auto im = dynamic_cast<VariantImpl<typename std::decay<T>::type>&>(*m_impl.get());
            if (im.m_rangeCheck)
                return im.m_rangeCheck(value, closestPossible);
            closestPossible = value;
            return true;
        }

        void registerForRemovalNotification(void* ptr, PropertyChanged onRemove)
        {
            m_onRemove[ptr].emplace_back(onRemove);
        }

        void registerForChangeNotification(void* ptr, PropertyChanged onChanged)
        {
            m_onChanged[ptr].emplace_back(onChanged);
        }

        void unregisterForChangeNotification(void* ptr)
        {
            m_onChanged.erase(ptr);
        }

        template <class T>
        const T& value() const
        {
            return dynamic_cast<VariantImpl<typename std::decay<T>::type>&>(*m_impl.get()).m_value;
        }

        template <class T>
        void value(const T& _value)
        {
            auto im = dynamic_cast<VariantImpl<typename std::decay<T>::type>&>(*m_impl.get());
            T closest;
            if (im.m_rangeCheck && im.m_rangeCheck(_value, closest))
            {
                m_impl = std::make_shared<VariantImpl<typename std::decay<T>::type>>(closest);
            }
            else
            {
                m_impl = std::make_shared<VariantImpl<typename std::decay<T>::type>>(_value);
            }

            if(m_component)
                m_component->onValueChanged();
            for (auto&& client : m_onChanged)
            {
                for (auto&& onChanged : client.second)
                {
                    onChanged();
                }
            }
        }

        const std::string& name() const
        {
            return m_name;
        }

#ifndef GAME_BUILD
        std::function<std::shared_ptr<Drawer>()> createVariantDrawer;
#endif
        std::function<void(rapidjson::PrettyWriter<rapidjson::StringBuffer>&)> writeJson;

    private:
        std::string m_name;
        EngineComponent* m_component;
        std::unordered_map<void*, std::vector<PropertyChanged>> m_onChanged;
        std::unordered_map<void*, std::vector<PropertyChanged>> m_onRemove;

        struct AbstractVariantImpl { virtual ~AbstractVariantImpl() {} };

        template <class T>
        struct VariantImpl : public AbstractVariantImpl
        {
            VariantImpl(T value)
                : m_value{ value }
            {}

            T m_value;
            std::function<bool(T, T&)> m_rangeCheck;
        };

        std::shared_ptr<AbstractVariantImpl> m_impl;
    };
}
