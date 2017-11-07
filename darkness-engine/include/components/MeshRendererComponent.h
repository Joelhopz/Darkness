#pragma once

#include "engine/EngineComponent.h"
#include "tools/Property.h"
#include "tools/hash/Hash.h"
#include "engine/primitives/Vector3.h"
#include "engine/primitives/Matrix4.h"
#include "engine/graphics/Common.h"
#include "engine/rendering/Mesh.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/Queue.h"
#include "engine/graphics/CommandList.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/Pipeline.h"
#include "platform/File.h"
#include <memory>
#include <string>

namespace engine
{
    struct MeshBuffers
    {
        engine::BufferSRV vertices;
        engine::BufferSRV normals;
        engine::BufferSRV tangents;
        engine::BufferSRV uv;
        engine::BufferIBV indices;
        engine::BufferIBV indicesAdjacency;

        engine::BufferSRV clusterId;
        engine::BufferSRV clusterIndexCount;
        engine::BufferSRV clusterBounds;
    };

    MeshBuffers convert(
        const std::string& path,
        Device& device, 
        engine::SubMesh& source);

    class MeshRendererComponent : public EngineComponent
    {
        Property m_meshPath;
        Property m_meshIndex;
        std::shared_ptr<engine::Mesh> m_mesh;
        size_t m_vertexBufferPosition;
        size_t m_indexBufferPosition;

        std::vector<MeshBuffers> m_meshBuffers;
        bool m_cpuDirty;
        bool m_gpuDirty;
        bool m_modelsChanged;
    protected:
        std::string m_oldMeshPath;
        void onValueChanged() override
        {
            if (m_oldMeshPath != m_meshPath.value<std::string>())
            {
                m_oldMeshPath = m_meshPath.value<std::string>();
                m_cpuDirty = true;
            }
        }
    public:
        MeshRendererComponent()
            : m_meshPath{ this, "MeshPath", std::string("") }
            , m_meshIndex{ this, "MeshIndex", static_cast<int>(0), [this]() { m_gpuDirty = true; } }
            , m_cpuDirty{ false }
            , m_gpuDirty{ false }
            , m_modelsChanged{ true }
        {
            m_name = "MeshRenderer";
        }

        MeshRendererComponent(const std::string& meshPath, int meshIndex)
            : m_meshPath{ this, "MeshPath", meshPath }
            , m_meshIndex{ this, "MeshIndex", meshIndex }
            , m_cpuDirty{ true }
            , m_gpuDirty{ false }
            , m_modelsChanged{ true }
        {
            m_name = "MeshRenderer";
        }

        std::shared_ptr<engine::EngineComponent> clone() const override
        {
            return std::make_shared<MeshRendererComponent>(m_meshPath.value<std::string>(), m_meshIndex.value<int>());
        }

        bool modelsChanged(bool clear = false)
        {
            bool res = m_modelsChanged; 
            if (clear) 
                m_modelsChanged = false; 
            return res;
        }

        std::string meshPath() const
        {
            return m_meshPath.value<std::string>();
        }

        void setMeshPath(const std::string& path)
        {
            m_cpuDirty = m_meshPath.value<std::string>() != path;
            m_gpuDirty = m_cpuDirty;
            m_meshPath.value<std::string>(path);
        }

        std::vector<MeshBuffers>& meshBuffers()
        {
            return m_meshBuffers;
        }

        const std::vector<SubMesh>& subMeshes() const
        {
            return m_mesh->subMeshes();
        }

    public:
        void invalidateGpu()
        {
            m_gpuDirty = true;
        }

    public:
        void cpuRefresh(Device& device)
        {
            if (m_cpuDirty)
            {
                m_cpuDirty = false;

                auto meshPathStr = m_meshPath.value<std::string>();
                if (meshPathStr != "")
                {
                    bool foundFile = fileExists(meshPathStr);
                    if (foundFile)
                    {
                        m_mesh = device.createMesh(
                            tools::hash(meshPathStr),
                            meshPathStr);
                        m_gpuDirty = true;
                    }
                    else
                        LOG_WARNING("Missing model file: %s", meshPathStr.c_str());
                }
            }
        }

        bool gpuRefresh(Device& device)
        {
            bool change = false;
            if(m_gpuDirty)
            {
                m_gpuDirty = false;
                change = true;
                m_meshBuffers.clear();
                if (m_mesh)
                {
                    //for (auto&& subMesh : m_mesh->subMeshes())
                    {
                        if (m_meshIndex.value<int>() < m_mesh->subMeshes().size())
                        {
                            m_meshBuffers.emplace_back(convert(
                                m_meshPath.value<std::string>() + std::to_string(m_meshIndex.value<int>()),
                                device,
                                m_mesh->subMeshes()[m_meshIndex.value<int>()]));
                        }
                    }
                }
                m_modelsChanged = true;
            }
            return change;
        }
    };
}
