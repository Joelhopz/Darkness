#pragma once

#include "engine/primitives/Vector3.h"
#include "engine/primitives/Matrix4.h"
#include "engine/primitives/Quaternion.h"
#include "engine/EngineComponent.h"
#include "plugins/PluginManager.h"
#include "tools/Property.h"
#include "tools/Serialization.h"
#include "tools/StringTools.h"
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <map>
#include <stack>
#include <algorithm>
#include <sstream>

#include "components/Transform.h"
#include "components/MeshRendererComponent.h"
#include "components/MaterialComponent.h"
#include "components/LightComponent.h"
#include "components/Camera.h"
#include "components/PostprocessComponent.h"
#include "components/RigidBodyComponent.h"
#include "components/CollisionShapeComponent.h"
#include "components/ProbeComponent.h"

#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif
using namespace std;
using namespace rapidjson;

#undef max
#undef min
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

namespace engine
{
    struct FlatSceneNode
    {
        Matrix4f transform;
        Matrix4f previousTransform;
        std::shared_ptr<MeshRendererComponent> mesh;
        std::shared_ptr<MaterialComponent> material;
        std::shared_ptr<RigidBodyComponent> rigidBody;
        unsigned int objectId;
    };

    struct FlatSceneLightNode
    {
        Matrix4f transform;
        Vector3f position;
        Vector3f direction;
        float range;
        Quaternionf rotation;
        LightType type;
        float outerCone;
        float innerCone;
        bool shadowCaster;
        bool positionChanged;
        bool rotationChanged;
        std::shared_ptr<LightComponent> light;
        std::shared_ptr<SceneNode> node;
    };

    class LightData;
    struct FlatScene
    {
        std::vector<FlatSceneNode> nodes;
        std::vector<FlatSceneNode> transparentNodes;
        std::vector<FlatSceneNode> alphaclippedNodes;
        std::vector<std::shared_ptr<Camera>> cameras;
        std::vector<std::shared_ptr<SceneNode>> cameraNodes;
        std::vector<std::shared_ptr<ProbeComponent>> probes;
        std::vector<std::shared_ptr<SceneNode>> probeNodes;
        int selectedCamera;
        std::shared_ptr<PostprocessComponent> postprocess;
        std::vector<FlatSceneLightNode> lights;

        std::shared_ptr<LightData> lightData;

        bool refreshed = false;
        int64_t selectedObject;
        void clear()
        {
            nodes.clear();
            transparentNodes.clear();
            alphaclippedNodes.clear();
            cameras.clear();
            cameraNodes.clear();
            probes.clear();
            probeNodes.clear();
            postprocess = nullptr;
            lights.clear();
        }
    };

    class SceneNode : public std::enable_shared_from_this<SceneNode>
    {
    public:
        SceneNode(SceneNode* parent = nullptr);
        ~SceneNode();
        SceneNode(SceneNode&&) = default;
        SceneNode(const SceneNode&) = default;
        SceneNode& operator=(SceneNode&&) = default;
        SceneNode& operator=(const SceneNode&) = default;

        void addChild(std::shared_ptr<SceneNode> child);
        void removeChild(std::shared_ptr<SceneNode> child);

        const std::shared_ptr<SceneNode> child(size_t index) const;
        std::shared_ptr<SceneNode> child(size_t index);
        
        const std::shared_ptr<SceneNode> operator[](size_t index) const;
        std::shared_ptr<SceneNode> operator[](size_t index);

        void parent(SceneNode* parent);

        const SceneNode* parent() const;
        SceneNode* parent();

        void flatten(bool simulating, FlatScene& resultList, unsigned int& objectIndex);

        size_t indexInParent() const;
        size_t childCount() const;

        size_t componentCount() const;
        std::shared_ptr<EngineComponent> component(size_t index);
        
        void addComponent(std::shared_ptr<EngineComponent> component);
        void removeComponent(std::shared_ptr<EngineComponent> component);

        void addComponent(TypeInstance&& component);

        std::vector<TypeInstance>& components()
        {
            return m_componentinstances;
        }

        template <class T>
        std::shared_ptr<T> getComponent()
        {
            for(auto& component : m_components)
            {
                if (std::dynamic_pointer_cast<typename std::decay<T>::type>(component))
                {
                    return std::dynamic_pointer_cast<typename std::decay<T>::type>(component);
                }
            }
            return nullptr;
        }

        std::string name() const;
        SceneNode& name(const std::string& name);

        void serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
        {
            writer.StartObject();

            std::string key = serialization::keyPrefix<serialization::KeyTypes, serialization::TypeId>(serialization::NodeName);
            writer.Key(key.data());
            writer.String(m_name.data());

            key = serialization::keyPrefix<serialization::KeyTypes, serialization::TypeId>(serialization::ComponentList);
            writer.Key(key.data());

            writer.StartArray();
            for (const auto& component : m_components)
            {
                component->writeJsonValue(writer);
            }
            writer.EndArray();

            key = serialization::keyPrefix<serialization::KeyTypes, serialization::TypeId>(serialization::NodeList);
            writer.Key(key.data());

            writer.StartArray();
            for (const auto& child : m_childs)
            {
                child->serialize(writer);
            }
            writer.EndArray();

            writer.EndObject();
        }

        bool invalid() const
        {
            return m_invalid;
        }
        void invalidate()
        {
            m_invalid = true;
            if (m_parent)
                m_parent->invalidate();
        }

        int64_t id() const
        {
            return m_id;
        }

        std::shared_ptr<SceneNode> find(int64_t id);
        std::vector<std::shared_ptr<SceneNode>> path(int64_t id);

        const Matrix4f& combinedTransform() const { return m_combinedTransform; }
    private:
        std::vector<std::shared_ptr<SceneNode>> m_childs;
        std::vector<std::shared_ptr<EngineComponent>> m_components;
        std::vector<TypeInstance> m_componentinstances;
        SceneNode* m_parent;
        std::string m_name;

        std::shared_ptr<Transform> m_transformComponent;
        Matrix4f m_combinedTransform;

        void recalculateCombinedTransform();

        bool m_invalid;
        int64_t m_id;
    };

    class Scene
    {
    public:
        Scene();

        FlatScene& flatten(bool simulating, std::shared_ptr<SceneNode> node = nullptr);

        template <class T>
        std::shared_ptr<T> getComponent(std::shared_ptr<SceneNode> node = nullptr)
        {
            if (!node)
                node = m_rootNode;

            auto component = node->getComponent<T>();
            if (component)
                return component;

            for (size_t i = 0; i < node->childCount(); ++i)
            {
                component = node->child(i)->getComponent<T>();
                if (component)
                    return component;
            }

            return nullptr;
        }

        const std::shared_ptr<SceneNode> root() const;
        std::shared_ptr<SceneNode>& root();

        void setRoot(std::shared_ptr<SceneNode> rootNode)
        {
            m_rootNode = rootNode;
        }

        void serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
        {
            m_rootNode->serialize(writer);
        }

        void saveTo(const std::string& filepath);
        void loadFrom(const std::string& filepath);

        void clear(bool full = false);

        std::shared_ptr<SceneNode> find(int64_t id);
        std::vector<std::shared_ptr<SceneNode>> path(int64_t id);

    private:
        FlatScene m_flatscene;
        std::shared_ptr<SceneNode> m_rootNode;
    };

    extern int ident;
    std::string identString();

    class SceneDeserialize : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, SceneDeserialize>
    {
    public:
        SceneDeserialize(std::shared_ptr<SceneNode> scene)
            : m_scene{ scene }
            //, m_nodeCurrent{ scene }
            , m_componentCurrent{ nullptr }
            , m_propertyTypeId{ serialization::TypeId::InvalidId }
        {
            m_nodeCurrent.push(scene);
        }

        bool Null()
        {
            m_keyStack.pop();

            //OutputDebugStringA((identString() + std::string("Null()\n")).data());
            return true;
        }

        bool Bool(bool b)
        {
            switch (m_propertyTypeId)
            {
                case serialization::TypeId::Bool:
                {
                    if (m_componentCurrent->hasVariant(m_propertyName))
                        m_componentCurrent->variant(m_propertyName).value<bool>(readJsonValue_bool(b));
                    else
                        m_componentCurrent->insertLoadedValue<bool>(m_propertyName, readJsonValue_bool(b));
                }
                /*case serialization::TypeId::ButtonPush:
                {
                    if (m_componentCurrent->hasVariant(m_propertyName))
                        m_componentCurrent->variant(m_propertyName).value<engine::ButtonPush>(readJsonValue_buttonPush(b));
                    else
                        m_componentCurrent->insertLoadedValue<engine::ButtonPush>(m_propertyName, readJsonValue_buttonPush(b));
                }*/
                case serialization::TypeId::ButtonToggle:
                {
                    if (m_componentCurrent->hasVariant(m_propertyName))
                        m_componentCurrent->variant(m_propertyName).value<engine::ButtonToggle>(readJsonValue_buttonToggle(b));
                    else
                        m_componentCurrent->insertLoadedValue<engine::ButtonToggle>(m_propertyName, readJsonValue_buttonToggle(b));
                }
            }
            m_keyStack.pop();

            std::string res;
            res += "Bool(";
            res += b ? "true" : "false";
            res += ")\n";
            //OutputDebugStringA((identString() + res).data());
            return true;
        }

        bool Int(int i)
        {
            switch (m_propertyTypeId)
            {
                case serialization::TypeId::Char:
                {
                    if (m_componentCurrent->hasVariant(m_propertyName))
                        m_componentCurrent->variant(m_propertyName).value<char>(readJsonValue_char(i));
                    else
                        m_componentCurrent->insertLoadedValue<char>(m_propertyName, readJsonValue_char(i));
                    break;
                }
                case serialization::TypeId::Short:
                {
                    if (m_componentCurrent->hasVariant(m_propertyName))
                        m_componentCurrent->variant(m_propertyName).value<short>(readJsonValue_short(i));
                    else
                        m_componentCurrent->insertLoadedValue<short>(m_propertyName, readJsonValue_short(i));
                    break;
                }
                case serialization::TypeId::Int:
                {
                    if (m_componentCurrent->hasVariant(m_propertyName))
                        m_componentCurrent->variant(m_propertyName).value<int>(readJsonValue_int(i));
                    else
                        m_componentCurrent->insertLoadedValue<int>(m_propertyName, readJsonValue_int(i));
                    break;
                }
            }
            m_keyStack.pop();

            /*char buf[64]; memset(&buf[0], 0, 64);
            sprintf_s(buf, "Int(%i)\n", i);
            std::string res(buf);
            OutputDebugStringA((identString() + res).data());*/
            return true;
        }
        
        bool Uint(unsigned u)
        {
            switch (m_propertyTypeId)
            {
                case serialization::TypeId::Char:
                {
                    if (m_componentCurrent->hasVariant(m_propertyName))
                        m_componentCurrent->variant(m_propertyName).value<char>(readJsonValue_char((int)u));
                    else
                        m_componentCurrent->insertLoadedValue<char>(m_propertyName, readJsonValue_char((int)u));
                    break;
                }
                case serialization::TypeId::Short:
                {
                    if (m_componentCurrent->hasVariant(m_propertyName))
                        m_componentCurrent->variant(m_propertyName).value<short>(readJsonValue_short((int)u));
                    else
                        m_componentCurrent->insertLoadedValue<short>(m_propertyName, readJsonValue_short((int)u));
                    break;
                }
                case serialization::TypeId::Int:
                {
                    if (m_componentCurrent->hasVariant(m_propertyName))
                        m_componentCurrent->variant(m_propertyName).value<int>(readJsonValue_int((int)u));
                    else
                        m_componentCurrent->insertLoadedValue<int>(m_propertyName, readJsonValue_int((int)u));
                    break;
                }
                case serialization::TypeId::UnsignedChar:
                {
                    if (m_componentCurrent->hasVariant(m_propertyName))
                        m_componentCurrent->variant(m_propertyName).value<unsigned char>(readJsonValue_unsignedchar(u));
                    else
                        m_componentCurrent->insertLoadedValue<unsigned char>(m_propertyName, readJsonValue_unsignedchar(u));
                    break;
                }
                case serialization::TypeId::UnsignedShort:
                {
                    if (m_componentCurrent->hasVariant(m_propertyName))
                        m_componentCurrent->variant(m_propertyName).value<unsigned short>(readJsonValue_unsignedshort(u));
                    else
                        m_componentCurrent->insertLoadedValue<unsigned short>(m_propertyName, readJsonValue_unsignedshort(u));
                    break;
                }
                case serialization::TypeId::UnsignedInt:
                {
                    if (m_componentCurrent->hasVariant(m_propertyName))
                        m_componentCurrent->variant(m_propertyName).value<unsigned int>(readJsonValue_unsignedint(u));
                    else
                        m_componentCurrent->insertLoadedValue<unsigned int>(m_propertyName, readJsonValue_unsignedint(u));
                    break;
                }
            }
            m_keyStack.pop();

            /*char buf[64]; memset(&buf[0], 0, 64);
            sprintf_s(buf, "Uint(%u)\n", u);
            std::string res(buf);
            OutputDebugStringA((identString() + res).data());*/
            return true;
        }

        bool Int64(int64_t /*i*/)
        {
            m_keyStack.pop();

            /*char buf[64]; memset(&buf[0], 0, 64);
            sprintf_s(buf, "Int64(%lld)\n", i);
            std::string res(buf);
            OutputDebugStringA((identString() + res).data());*/
            return true;
        }

        bool Uint64(uint64_t /*u*/)
        {
            m_keyStack.pop();

            /*char buf[64]; memset(&buf[0], 0, 64);
            sprintf_s(buf, "Uint64(%llu)\n", u);
            std::string res(buf);
            OutputDebugStringA((identString() + res).data());*/
            return true;
        }

        bool Double(double d)
        {
            switch (m_propertyTypeId)
            {
                case serialization::TypeId::Float:
                {
                    if(m_componentCurrent->hasVariant(m_propertyName))
                        m_componentCurrent->variant(m_propertyName).value<float>(readJsonValue_float(d));
                    else
                        m_componentCurrent->insertLoadedValue<float>(m_propertyName, readJsonValue_float(d));
                    break;
                }
                case serialization::TypeId::Double:
                {
                    if (m_componentCurrent->hasVariant(m_propertyName))
                        m_componentCurrent->variant(m_propertyName).value<double>(readJsonValue_double(d));
                    else
                        m_componentCurrent->insertLoadedValue<double>(m_propertyName, readJsonValue_double(d));
                    break;
                }
            }
            m_keyStack.pop();

            /*char buf[64]; memset(&buf[0], 0, 64);
            sprintf_s(buf, "Double(%f)\n", d);
            std::string res(buf);
            OutputDebugStringA((identString() + res).data());*/
            return true;
        }

        bool String(const char* str, SizeType length, bool /*copy*/)
        {
            /*if(copy)
                LOG_INFO("Scene serialization does not implement string copy");*/
            if (m_keyStack.top() == serialization::KeyTypes::NodeName)
            {
                m_nodeCurrent.top()->name(std::string(str, length));
            }
            else if (m_keyStack.top() == serialization::KeyTypes::ComponentName)
            {
                std::shared_ptr<EngineComponent> newComponent = nullptr;
                std::string componentName = std::string(str, length);
                if (componentName == "Transform")
                    newComponent = std::make_shared<Transform>();
                else if (componentName == "MeshRenderer")
                    newComponent = std::make_shared<MeshRendererComponent>();
                else if (componentName == "Camera")
                    newComponent = std::make_shared<Camera>();
                else if (componentName == "MaterialComponent")
                    newComponent = std::make_shared<MaterialComponent>();
                else if (componentName == "LightComponent")
                    newComponent = std::make_shared<LightComponent>();
                else if (componentName == "PostprocessComponent")
                    newComponent = std::make_shared<PostprocessComponent>();
                else if (componentName == "Probe")
                    newComponent = std::make_shared<ProbeComponent>();
                else
                    ASSERT(false, "Scene could not deserialize component. Unknown component: %s", componentName.c_str());
                                
                m_nodeCurrent.top()->addComponent(newComponent);
                m_componentCurrent = newComponent;
            }
            else if (m_keyStack.top() == serialization::KeyTypes::PropertyName)
            {
                std::string propertyValue(str, length);
                m_propertyName = std::string(str, length);
            }
            else if (m_keyStack.top() == serialization::KeyTypes::PropertyValue)
            {
                std::string propertyValue(str, length);
                switch (m_propertyTypeId)
                {
                    case serialization::TypeId::InvalidId:
                    {
                        break;
                    }
                    case serialization::TypeId::Vector2f:
                    {
                        if (m_componentCurrent->hasVariant(m_propertyName))
                            m_componentCurrent->variant(m_propertyName).value<Vector2f>(readJsonValue_vector2f(propertyValue));
                        else
                            m_componentCurrent->insertLoadedValue<Vector2f>(m_propertyName, readJsonValue_vector2f(propertyValue));
                        break;
                    }
                    case serialization::TypeId::Vector3f:
                    {
                        if (m_componentCurrent->hasVariant(m_propertyName))
                            m_componentCurrent->variant(m_propertyName).value<Vector3f>(readJsonValue_vector3f(propertyValue));
                        else
                            m_componentCurrent->insertLoadedValue<Vector3f>(m_propertyName, readJsonValue_vector3f(propertyValue));
                        break;
                    }
                    case serialization::TypeId::Vector4f:
                    {
                        if (m_componentCurrent->hasVariant(m_propertyName))
                            m_componentCurrent->variant(m_propertyName).value<Vector4f>(readJsonValue_vector4f(propertyValue));
                        else
                            m_componentCurrent->insertLoadedValue<Vector4f>(m_propertyName, readJsonValue_vector4f(propertyValue));
                        break;
                    }
                    case serialization::TypeId::Matrix3f:
                    {
                        if (m_componentCurrent->hasVariant(m_propertyName))
                            m_componentCurrent->variant(m_propertyName).value<Matrix3f>(readJsonValue_matrix3f(propertyValue));
                        else
                            m_componentCurrent->insertLoadedValue<Matrix3f>(m_propertyName, readJsonValue_matrix3f(propertyValue));
                        break;
                    }
                    case serialization::TypeId::Matrix4f:
                    {
                        if (m_componentCurrent->hasVariant(m_propertyName))
                            m_componentCurrent->variant(m_propertyName).value<Matrix4f>(readJsonValue_matrix4f(propertyValue));
                        else
                            m_componentCurrent->insertLoadedValue<Matrix4f>(m_propertyName, readJsonValue_matrix4f(propertyValue));
                        break;
                    }
                    case serialization::TypeId::Quaternionf:
                    {
                        if (m_componentCurrent->hasVariant(m_propertyName))
                            m_componentCurrent->variant(m_propertyName).value<Quaternionf>(readJsonValue_quaternionf(propertyValue));
                        else
                            m_componentCurrent->insertLoadedValue<Quaternionf>(m_propertyName, readJsonValue_quaternionf(propertyValue));
                        break;
                    }
                    case serialization::TypeId::String:
                    {
                        if (m_componentCurrent->hasVariant(m_propertyName))
                            m_componentCurrent->variant(m_propertyName).value<std::string>(readJsonValue_string(propertyValue));
                        else
                            m_componentCurrent->insertLoadedValue<std::string>(m_propertyName, readJsonValue_string(propertyValue));
                        break;
                    }
                    case serialization::TypeId::Projection:
                    {
                        if (m_componentCurrent->hasVariant(m_propertyName))
                            m_componentCurrent->variant(m_propertyName).value<engine::Projection>(readJsonValue_projection(propertyValue));
                        else
                            m_componentCurrent->insertLoadedValue<engine::Projection>(m_propertyName, readJsonValue_projection(propertyValue));
                        break;
                    }
                    case serialization::TypeId::LightType:
                    {
                        if (m_componentCurrent->hasVariant(m_propertyName))
                            m_componentCurrent->variant(m_propertyName).value<engine::LightType>(readJsonValue_lightType(propertyValue));
                        else
                            m_componentCurrent->insertLoadedValue<engine::LightType>(m_propertyName, readJsonValue_lightType(propertyValue));
                        break;
                    }
                    case serialization::TypeId::CollisionShape:
                    {
                        if (m_componentCurrent->hasVariant(m_propertyName))
                            m_componentCurrent->variant(m_propertyName).value<engine::CollisionShape>(readJsonValue_collisionShape(propertyValue));
                        else
                            m_componentCurrent->insertLoadedValue<engine::CollisionShape>(m_propertyName, readJsonValue_collisionShape(propertyValue));
                        break;
                    }
                }
            }
            m_keyStack.pop();

            //std::string temp(str, length);
            //OutputDebugStringA((identString() + std::string("String("+temp+")\n")).data());
            return true;
        }

        bool StartObject()
        {
            // we always have root node
            if(m_objectStack.size() == 0)
                m_objectStack.push(serialization::ObjectTypes::Node);
            else
            {
                if (m_keyStack.top() == serialization::KeyTypes::NodeList)
                {
                    auto newChild = std::make_shared<SceneNode>();
                    m_nodeCurrent.top()->addChild(newChild);
                    m_nodeCurrent.push(newChild);
                    m_objectStack.push(serialization::ObjectTypes::Node);
                }
                else if (m_keyStack.top() == serialization::KeyTypes::ComponentList)
                {
                    m_objectStack.push(serialization::ObjectTypes::Component);
                }
                else if (m_keyStack.top() == serialization::KeyTypes::PropertyList)
                {
                    m_objectStack.push(serialization::ObjectTypes::Property);
                }
                else
                {
                    ASSERT(false);
                }
            }

            //OutputDebugStringA((identString() + std::string("StartObject()\n")).data());
            ++ident;
            return true;
        }

        bool EndObject(SizeType /*memberCount*/)
        {
            //LOG_INFO("Scene serialization EndObject does not use memberCount");
            --ident;

            /*std::string temp;
            temp += "EndObject(";
            temp += std::to_string(memberCount);
            temp += ")\n";
            OutputDebugStringA((identString() + temp).data());*/

            if (m_objectStack.top() == serialization::ObjectTypes::Node)
                m_nodeCurrent.pop();
            m_objectStack.pop();

            return true;
        }
        
        bool Key(const char* str, SizeType length, bool /*copy*/)
        {
            //LOG_INFO("Scene serialization does not implement key copy");
            std::string stdStr(str, length);
            auto parts = tokenize(stdStr);
            serialization::KeyTypes keyType = static_cast<serialization::KeyTypes>(stringToNumber<int>(parts[0]));
            m_propertyTypeId = static_cast<serialization::TypeId>(stringToNumber<int>(parts[1]));
            m_keyStack.push(keyType);

            /*switch (keyType)
            {
                case serialization::NodeName:
                {
                    m_keyStack.push(serialization::KeyTypes::NodeName);
                    break;
                }
                case serialization::NodeList:
                {
                    m_stack.push(DeSerializeTask::NodeArray);
                    break;
                }
                case serialization::Property:
                {
                    int temp;
                    std::stringstream ss(parts[1]);
                    ss >> temp;
                    m_propertyTypeId = static_cast<serialization::TypeId>(temp);
                    m_propertyName = parts[2];
                    m_stack.push(DeSerializeTask::Property);
                    break;
                }
                case serialization::PropertyList:
                {
                    m_stack.push(DeSerializeTask::PropertyArray);
                    break;
                }
                case serialization::Component:
                {
                    m_stack.push(DeSerializeTask::Component);
                    break;
                }
                case serialization::ComponentList:
                {
                    m_stack.push(DeSerializeTask::ComponentArray);
                    break;
                }
            }*/

            /*std::string temp;
            temp += "Key(";
            temp += str;
            temp += ", ";
            temp += std::to_string(length);
            temp += ", ";
            temp += copy ? "true" : "false";
            temp += ")\n";
            OutputDebugStringA((identString() + temp).data());*/
            return true;
        }
        
        bool StartArray()
        {
            //OutputDebugStringA((identString() + std::string("StartArray()\n")).data());
            ++ident;

            return true;
        }

        bool EndArray(SizeType /*elementCount*/)
        {
            //LOG_INFO("Scene serialization EndArray does not use elementCount");
            --ident;

            /*std::string temp;
            temp += "EndArray(";
            temp += std::to_string(elementCount);
            temp += ")\n";
            OutputDebugStringA((identString() + temp).data());*/

            /*if (m_stack.top() == DeSerializeTask::ComponentArray)
            {
                m_currentComponent = nullptr;
            }*/
            m_keyStack.pop();
            
            return true;
        }
    private:
        std::shared_ptr<SceneNode> m_scene;

        std::stack<std::shared_ptr<SceneNode>>  m_nodeCurrent;
        std::shared_ptr<EngineComponent>        m_componentCurrent;

        serialization::TypeId m_propertyTypeId;
        std::string m_propertyName;

        std::stack<serialization::ObjectTypes> m_objectStack;
        std::stack<serialization::KeyTypes> m_keyStack;
    };
}
