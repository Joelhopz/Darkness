#include "engine/Scene.h"
#include "platform/File.h"
#include <algorithm>
#include "tools/Debug.h"
#include "tools/StringTools.h"
#include "btBulletDynamicsCommon.h"
#include "components/ProbeComponent.h"

namespace engine
{
    SceneNode::SceneNode(SceneNode* parent)
        : m_parent{ parent }
        , m_transformComponent{ nullptr }
    {
    }

    void SceneNode::addChild(std::shared_ptr<SceneNode> child)
    {
        m_childs.emplace_back(child);
        child->parent(this);
        child->recalculateCombinedTransform();
        invalidate();
    }

    void SceneNode::removeChild(std::shared_ptr<SceneNode> child)
    {
        auto foundchild = std::find(m_childs.begin(), m_childs.end(), child);
        if (foundchild != m_childs.end())
        {
            m_childs.erase(foundchild);
        }
        invalidate();
    }

    const std::shared_ptr<SceneNode> SceneNode::child(size_t index) const
    {
        if(index < m_childs.size())
            return m_childs[index];
        return{};
    }

    std::shared_ptr<SceneNode> SceneNode::child(size_t index)
    {
        if(index < m_childs.size())
            return m_childs[index];
        return{};
    }

    const std::shared_ptr<SceneNode> SceneNode::operator[](size_t index) const
    {
        if(index < m_childs.size())
            return m_childs[index];
        return{};
    }

    std::shared_ptr<SceneNode> SceneNode::operator[](size_t index)
    {
        if(index < m_childs.size())
            return m_childs[index];
        return{};
    }

    void SceneNode::parent(SceneNode* parent)
    {
        m_parent = parent;
    }

    const SceneNode* SceneNode::parent() const
    {
        return m_parent;
    }

    SceneNode* SceneNode::parent()
    {
        return m_parent;
    }

    size_t SceneNode::indexInParent() const
    {
        size_t index = 0;
        for (auto& sceneNode : m_parent->m_childs)
        {
            if (sceneNode.get() == this)
            {
                break;
            }
            ++index;
        }
        return index;
    }

    size_t SceneNode::childCount() const
    {
        return m_childs.size();
    }

    size_t SceneNode::componentCount() const
    {
        return m_components.size();
    }

    std::shared_ptr<SceneNode> SceneNode::find(int64_t id)
    {
        if (m_id == id)
            return shared_from_this();
        for (auto&& child : m_childs)
        {
            auto&& temp = child->find(id);
            if (temp)
                return temp;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<SceneNode>> SceneNode::path(int64_t id)
    {
        std::vector<std::shared_ptr<SceneNode>> res;
        if (m_id == id)
        {
            res.emplace_back(shared_from_this());
            return res;
        }

        for (auto&& child : m_childs)
        {
            auto temp = child->path(id);
            if (temp.size() > 0)
            {
                res.emplace_back(shared_from_this());
                res.insert(res.end(), temp.begin(), temp.end());
                return res;
            }
        }
        return res;
    }

    void SceneNode::flatten(bool simulating, FlatScene& resultList, unsigned int& objectIndex)
    {
        m_id = objectIndex;

        auto transform = getComponent<Transform>();
        if (!transform)
            return;

        if (!m_transformComponent)
        {
            m_transformComponent = transform;
            m_transformComponent->variant("position").registerForChangeNotification(this, [this]() { recalculateCombinedTransform(); this->invalidate(); });
            m_transformComponent->variant("rotation").registerForChangeNotification(this, [this]() { recalculateCombinedTransform(); this->invalidate(); });
            m_transformComponent->variant("scale").registerForChangeNotification(this, [this]() { recalculateCombinedTransform(); this->invalidate(); });
        }

        /*if (m_invalid)
            recalculateCombinedTransform();*/

        m_invalid = false;

        auto mesh = getComponent<MeshRendererComponent>();
        if (mesh)
        {
            if (!getComponent<MaterialComponent>())
                this->addComponent(std::make_shared<MaterialComponent>());

            auto rigidBody = getComponent<RigidBodyComponent>();
            if (simulating && rigidBody)
            {
                btTransform trans;
                if (rigidBody->body() && rigidBody->body()->getMotionState())
                {
                    rigidBody->body()->getMotionState()->getWorldTransform(trans);

                    m_transformComponent->position(Vector3f(
                        static_cast<float>(trans.getOrigin().getX()),
                        static_cast<float>(trans.getOrigin().getY()),
                        static_cast<float>(trans.getOrigin().getZ())));
                }
            }

            auto material = getComponent<MaterialComponent>();
            if (material && material->transparent())
                resultList.transparentNodes.emplace_back(FlatSceneNode{ m_combinedTransform, m_combinedTransform, mesh, material, rigidBody, objectIndex });
            else if (material->alphaclipped())
                resultList.alphaclippedNodes.emplace_back(FlatSceneNode{ m_combinedTransform, m_combinedTransform, mesh, material, rigidBody, objectIndex });
            else
                resultList.nodes.emplace_back(FlatSceneNode{ m_combinedTransform, m_combinedTransform, mesh, material, rigidBody, objectIndex });

            mesh->updateObjectId(objectIndex);

        }

        ++objectIndex;
        
        auto camera = getComponent<Camera>();
        if (camera)
        {
            if (!getComponent<PostprocessComponent>())
                this->addComponent(std::make_shared<PostprocessComponent>());
            resultList.cameras.emplace_back(camera);
            resultList.cameraNodes.emplace_back(shared_from_this());
            resultList.postprocess = getComponent<PostprocessComponent>();
        }

        auto light = getComponent<LightComponent>();
        if (light)
        {
            auto lightDir = transform->rotation() * Vector3f(0.0f, 0.0f, 1.0f);
            resultList.lights.emplace_back(FlatSceneLightNode{ 
                m_combinedTransform,
                transform->position(),
                lightDir,
                light->range(),
                transform->rotation(),
                light->lightType(),
                light->outerConeAngle(),
                light->innerConeAngle(),
                light->shadowCaster(),
                transform->positionChanged(true),
                transform->rotationChanged(true),
                light,
                shared_from_this() });
        }

        auto probe = getComponent<ProbeComponent>();
        if (probe)
        {
            resultList.probes.emplace_back(probe);
            resultList.probeNodes.emplace_back(shared_from_this());
        }

        for (int i = 0; i < childCount(); ++i)
        {
            child(i)->flatten(simulating, resultList, objectIndex);
        }
    }

    std::shared_ptr<EngineComponent> SceneNode::component(size_t index)
    {
        return m_components[index];
    }

    SceneNode::~SceneNode()
    {
        if (m_transformComponent)
        {
            m_transformComponent->variant("position").unregisterForChangeNotification(this);
            m_transformComponent->variant("rotation").unregisterForChangeNotification(this);
            m_transformComponent->variant("scale").unregisterForChangeNotification(this);
        }
    }

    void SceneNode::recalculateCombinedTransform()
    {
        if (!m_transformComponent)
        {
            auto transform = getComponent<Transform>();
            if (transform)
            {
                m_transformComponent = transform;
                m_transformComponent->variant("position").registerForChangeNotification(this, [this]() { recalculateCombinedTransform(); this->invalidate(); });
                m_transformComponent->variant("rotation").registerForChangeNotification(this, [this]() { recalculateCombinedTransform(); this->invalidate(); });
                m_transformComponent->variant("scale").registerForChangeNotification(this, [this]() { recalculateCombinedTransform(); this->invalidate(); });
            }
        }

        if (m_parent && m_transformComponent)
        {
            m_combinedTransform = m_parent->combinedTransform() * m_transformComponent->transformMatrix();
        }
        else if (m_transformComponent)
        {
            m_combinedTransform = m_transformComponent->transformMatrix();
        }
        for (auto&& child : m_childs)
        {
            child->recalculateCombinedTransform();
        }

        auto mesh = getComponent<MeshRendererComponent>();
        if (mesh)
        {
            mesh->updateTransform(m_combinedTransform);
        }
    }

    void SceneNode::addComponent(std::shared_ptr<EngineComponent> component)
    {
        component->parentNode(this);
        m_components.emplace_back(component);
        component->start();
        recalculateCombinedTransform();
        invalidate();
    }

    void SceneNode::addComponent(TypeInstance&& component)
    {
        m_componentinstances.emplace_back(std::move(component));
        recalculateCombinedTransform();
        invalidate();
    }

    void SceneNode::removeComponent(std::shared_ptr<EngineComponent> component)
    {
        component->parentNode(nullptr);
        auto com = std::find(m_components.begin(), m_components.end(), component);
        if(com != m_components.end())
            m_components.erase(com);
        invalidate();
    }

    std::string SceneNode::name() const
    {
        return m_name;
    }

    SceneNode& SceneNode::name(const std::string& name)
    {
        m_name = name;
        return *this;
    }

    Scene::Scene()
        : m_rootNode{ std::make_shared<SceneNode>() }
    {
        m_rootNode->addComponent(std::make_shared<Transform>());
        m_flatscene.selectedCamera = 0;
    }

    FlatScene& Scene::flatten(bool simulating, std::shared_ptr<SceneNode> node)
    {
        if (m_rootNode->invalid())
        {
            m_flatscene.refreshed = true;
            m_flatscene.clear();

            /*Matrix4f position = Matrix4f::translate(0.0f, 0.0f, 0.0f);
            Matrix4f scale = Matrix4f::scale(1.0f, 1.0f, 1.0f);
            Matrix4f rotation = Matrix4f::rotation(0.0f, 0.0f, 0.0f);
            auto modelTransform = position * rotation * scale;*/

            unsigned int objectIdStart = 1u;
            m_rootNode->flatten(simulating, m_flatscene, objectIdStart);
            return m_flatscene;
        }
        else
        {
            m_flatscene.refreshed = false;
            return m_flatscene;
        }
    }

    std::shared_ptr<SceneNode> Scene::find(int64_t id)
    {
        return m_rootNode->find(id);
    }

    std::vector<std::shared_ptr<SceneNode>> Scene::path(int64_t id)
    {
        return m_rootNode->path(id);
    }

    const std::shared_ptr<SceneNode> Scene::root() const
    {
        return m_rootNode;
    }

    std::shared_ptr<SceneNode>& Scene::root()
    {
        return m_rootNode;
    }

    void Scene::saveTo(const std::string& filepath)
    {
        if (m_rootNode)
        {
            rapidjson::StringBuffer strBuffer;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strBuffer);
            m_rootNode->serialize(writer);

            ofstream outFile;
            outFile.open(filepath);
            outFile.write(strBuffer.GetString(), strBuffer.GetSize());
            outFile.close();
        }
    }

    void Scene::clear(bool full)
    {
        m_rootNode = std::make_shared<SceneNode>();
        if(!full)
            m_rootNode->addComponent(std::make_shared<Transform>());
        m_flatscene = FlatScene();
    }

    void Scene::loadFrom(const std::string& filepath)
    {
        if (fileExists(filepath))
        {
            ifstream inFile;
            inFile.open(filepath);
            std::vector<char> buffer;
            inFile.seekg(0, ios::end);
            buffer.resize(inFile.tellg());
            inFile.seekg(0, ios::beg);
            inFile.read(buffer.data(), buffer.size());
            inFile.close();

            shared_ptr<SceneNode> scene = make_shared<SceneNode>();
            {
                rapidjson::Reader reader;
                rapidjson::StringStream ss(buffer.data());
                SceneDeserialize handler(scene);
                reader.Parse(ss, handler);
            }
            if (!scene->getComponent<Transform>())
                scene->addComponent(std::make_shared<Transform>());

            setRoot(scene);
        }
    }

    /*std::vector<std::shared_ptr<SceneNode>> m_childs;
    std::vector<EngineComponent> m_components;
    std::shared_ptr<SceneNode> m_parent;*/

    int ident = 0;

    std::string identString()
    {
        std::string res = "";
        for (int i = 0; i < ident * 4; ++i)
        {
            res += " ";
        }
        return res;
    }
}
