#pragma once

#include "plugins/TypeFactory.h"
#include "tools/Debug.h"
#include "Common.h"
#include <string>
#include <map>
#include <vector>
#include <memory>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#define CALLCONV
#endif

namespace engine
{
    struct Rect
    {
        int x;
        int y;
        int width;
        int height;
    };

    typedef void(*CBDrawText)(void*, Rect, const std::string&);

    struct PluginDrawer
    {
        void* callbacks;
        void* drawer;
    };

    typedef void*(CALLCONV *CreateTypeRegistry)();
    typedef void(CALLCONV *DestroyTypeRegistry)(void*);
    typedef void(CALLCONV *InitializeTypes)(void*);
    typedef uint32_t(CALLCONV *NumberOfTypes)(void*);
    typedef void(CALLCONV *TypeName)(void*, uint32_t, char*, uint32_t);
    typedef void*(CALLCONV *CreateTypeInstance)(void*, uint32_t);
    typedef void(CALLCONV *DestroyTypeInstance)(void*);
    typedef uint32_t(CALLCONV *NumberOfProperties)(void*);
    typedef void(CALLCONV *PropertyName)(void*, uint32_t, char*, uint32_t);

    typedef void*(CALLCONV *CreateDrawerCallbacks)(void*, CBDrawText);
    typedef void(CALLCONV *DestroyDrawerCallbacks)(void*);

    typedef void*(CALLCONV *CreatePropertyDrawer)(void*, uint32_t, void*);
    typedef void(CALLCONV *DestroyPropertyDrawer)(void*);
    typedef void(CALLCONV *DrawPropertyDrawer)(void*, int, int, int, int);

    typedef void*(CALLCONV *SerializeType)(void*);
    typedef uint32_t(CALLCONV *SerializeDataSize)(void*);
    typedef void(CALLCONV *SerializeFetch)(void*, void*);
    typedef void(CALLCONV *DeserializeType)(void*, void*, uint32_t);

    struct ApiPointers
    {
        CreateTypeRegistry m_createTypeRegistry;
        DestroyTypeRegistry m_destroyTypeRegistry;
        InitializeTypes m_initializeTypes;

        NumberOfTypes m_numberOfTypes;
        TypeName m_typeName;
        CreateTypeInstance m_createTypeInstance;
        DestroyTypeInstance m_destroyTypeInstance;
        NumberOfProperties m_numberOfProperties;
        PropertyName m_propertyName;

        CreateDrawerCallbacks m_createDrawerCallbacks;
        DestroyDrawerCallbacks m_destroyDrawerCallbacks;

        CreatePropertyDrawer m_createPropertyDrawer;
        DestroyPropertyDrawer m_destroyPropertyDrawer;
        DrawPropertyDrawer m_drawPropertyDrawer;

        SerializeType m_serializeType;
        SerializeDataSize m_serializeDataSize;
        SerializeFetch m_serializeFetch;
        DeserializeType m_deserializeType;

        void* registry;
    };

    class PluginProperty
    {
    public:
        PluginProperty(
            std::shared_ptr<ApiPointers> apiPointers,
            void* typeInstance,
            uint32_t propertyIndex)
            : m_apiPointers{ apiPointers }
            , m_typeInstance{ typeInstance }
            , m_propertyIndex{ propertyIndex }
        {
            char typeNameBuffer[1024] = { 0 };
            m_apiPointers->m_propertyName(m_typeInstance, propertyIndex, &typeNameBuffer[0], 1024);
            m_name = typeNameBuffer;
        }

        const std::string& name() const
        {
            return m_name;
        }

        PluginDrawer* createDrawer(void* propertyWidget, CBDrawText cbDrawText)
        {
            PluginDrawer* res = new PluginDrawer();
            res->callbacks = m_apiPointers->m_createDrawerCallbacks(propertyWidget, cbDrawText);
            res->drawer = m_apiPointers->m_createPropertyDrawer(m_typeInstance, m_propertyIndex, res->callbacks);
            return res;
        }
        void destroyDrawer(PluginDrawer* ptr)
        {
            m_apiPointers->m_destroyPropertyDrawer(ptr->drawer);
            m_apiPointers->m_destroyDrawerCallbacks(ptr->callbacks);
            delete ptr;
        }

        void callDraw(PluginDrawer* drawer, Rect rect)
        {
            m_apiPointers->m_drawPropertyDrawer(drawer->drawer, rect.x, rect.y, rect.width, rect.height);
        }

        template <typename T>
        void value(T value)
        {

        }

        template <typename T>
        T value()
        {
            return{};
        }

    private:
        std::shared_ptr<ApiPointers> m_apiPointers;
        void* m_typeInstance;
        uint32_t m_propertyIndex;
        std::string m_name;
    };

    class PluginType
    {
    public:
        PluginType(
            std::shared_ptr<ApiPointers> apiPointers,
            uint32_t typeIndex)
            : m_apiPointers{ apiPointers }
            , m_typeIndex{ typeIndex }
        {
            char nameBuffer[1024] = { 0 };
            m_apiPointers->m_typeName(m_apiPointers->registry, typeIndex, &nameBuffer[0], 1024);
            m_name = nameBuffer;
        }
        std::string& name() { return m_name; }
        const std::string& name() const { return m_name; }

        void* createInstance()
        {
            return m_apiPointers->m_createTypeInstance(m_apiPointers->registry, m_typeIndex);
        }

        void destroyInstance(void* ptr)
        {
            m_apiPointers->m_destroyTypeInstance(ptr);
        }

        std::vector<PluginProperty> properties(void* typeInstance)
        {
            std::vector<PluginProperty> res;
            auto propertyCount = m_apiPointers->m_numberOfProperties(typeInstance);
            for (uint32_t a = 0; a < propertyCount; ++a)
            {
                res.emplace_back(m_apiPointers, typeInstance, a);
            }
            return res;
        }

    private:
        std::string m_name;
        std::shared_ptr<ApiPointers> m_apiPointers;
        uint32_t m_typeIndex;
    };

    class TypeInstance
    {
    public:
        TypeInstance()
            : m_instance{ nullptr }
            , m_name{ "" }
        {}

        TypeInstance(
            std::shared_ptr<void> instance, 
            const std::string& name,
            PluginType* typePtr)
            : m_instance{ instance }
            , m_name{ name }
            , m_typePtr{ typePtr }
        {}

        const std::string& name() const
        {
            return m_name;
        }

        std::vector<PluginProperty> properties()
        {
            return m_typePtr->properties(m_instance.get());
        }

        /*PluginProperty& property(const std::string& name)
        {
            return {};
        }*/

    private:
        std::shared_ptr<void> m_instance;
        std::string m_name;
        PluginType* m_typePtr;
    };

#ifdef _WIN32
#define InstanceHandle HINSTANCE
#else
#define InstanceHandle void*
#endif
    
    class PluginManager
    {
    public:
        void addFolder(const std::string& folder);
        TypeFactory& factory();

        TypeInstance createType(const std::string& typeName)
        {
            for (auto&& pluginInstance : m_instances)
            {
                for (auto&& typeInstance : pluginInstance.types())
                {
                    if (typeInstanceNameEquals(typeInstance->name(), typeName))
                    {
                        std::shared_ptr<void> sptr = std::shared_ptr<void>(typeInstance->createInstance(),
                            [&](void* ptr) {
                            typeInstance->destroyInstance(ptr);
                        });

                        TypeInstance res(sptr, typeName, typeInstance.get());
                        return res;
                    }
                }
            }
            ASSERT(true);
            return TypeInstance();
        }

    private:

        class PluginInstance
        {
        public:
            PluginInstance(InstanceHandle _instance);

            std::vector<std::shared_ptr<PluginType>>& types() { return m_pluginTypes; }
        private:
            InstanceHandle instance;

            std::shared_ptr<ApiPointers> m_apiPointers;
            std::vector<std::shared_ptr<PluginType>> m_pluginTypes;
        };

        bool typeInstanceNameEquals(const std::string& generatedName, const std::string& name)
        {
            return (generatedName.find("class") != std::string::npos) && (generatedName.find(name) != std::string::npos);
        }

        TypeFactory m_typeFactory;
        std::vector<PluginInstance> m_instances;
    };
}
