#include "engine/rendering/GpuBuffers.h"
#include "engine/rendering/ModelResources.h"
#include "engine/rendering/SubMesh.h"
#include <algorithm>

namespace engine
{
    GpuBuffers::GpuBuffers(Device& device)
        // per vertex data
        : m_vertexDataAllocator{ 
            device, 
            MaxVertexCount,
            {
                Format::R32G32_UINT,     // vertex
                Format::R32G32_FLOAT,        // normal
                Format::R32G32_FLOAT,        // tangent
                Format::R8G8B8A8_UNORM       // color
            },
            "Vertex data" }

        // uv data
        , m_uvDataAllocator{ device, MaxUvCount,{ Format::R32G32_FLOAT }, "UV data" }

        // per triangle data
        , m_indexAllocator{ device, MaxIndexCount, { Format::R32_UINT }, "Triangle data" }

        // per triangle data
        , m_adjacencyAllocator{ device, MaxIndexCount*2,{ Format::R32_UINT }, "Adjacency data", true }

        // per cluster data
        , m_clusterDataAllocator{ 
            device, 
            MaxClusterCount,
            {
                sizeof(ClusterData),    // 12 bytes
                sizeof(BoundingBox),    // 24 bytes
                sizeof(BoundingSphere)  // 16 bytes
            },
            "Cluster data" }

        , m_subMeshDataAllocator{
            device, 
            MaxSubModels,
            {
                sizeof(SubMeshData),    // 8 bytes
                sizeof(BoundingBox),    // 24 bytes
                sizeof(BoundingSphere),  // 16 bytes
                sizeof(SubMeshAdjacency)
            },
            "SubMesh data" }

        , m_instanceDataAllocator{
            device, 
            MaxInstances,
            {
                { Format::R32_UINT },           // 4 bytes      // Format::R32_UINT
                { Format::R32_UINT }            // 4 bytes      // Format::R32_UINT
            },
            {
                { sizeof(TransformHistory), ResourceUsage::GpuReadWrite },   // 192 bytes
                { sizeof(InstanceMaterial) },   // 60 bytes
            },
            "Instance data" }
        , m_maxInstanceIndexInUse{ InvalidMaxInstanceIndex }
    {}

    void GpuBuffers::maxInstanceIndexInUse(uint32_t value)
    {
        m_maxInstanceIndexInUse = value;
    }

    uint32_t GpuBuffers::maxInstanceIndexInUse() const
    {
        return m_maxInstanceIndexInUse;
    }

    void GpuBuffers::addInstance(SubMeshInstance* instance)
    {
        m_trackedInstances.emplace_back(instance);
    }

    void GpuBuffers::removeInstance(SubMeshInstance* instance)
    {
        if (m_trackedInstances.back() == instance)
        {
            m_trackedInstances.pop_back();
            return;
        }
        auto i = std::find(m_trackedInstances.begin(), m_trackedInstances.end(), instance);
        if (i != m_trackedInstances.end())
        {
            m_trackedInstances.erase(i);
            LOG("Searched and Removed instance: %p", instance);
        }
        else
        {
            LOG("Tried to remove instance but failed: %p", instance);
        }
    }

    void GpuBuffers::relocateInstance(uint32_t oldIndex, uint32_t newIndex)
    {
        std::iter_swap(m_trackedInstances.begin() + oldIndex, m_trackedInstances.begin() + newIndex);
        m_trackedInstances[newIndex]->instanceData.modelResource = m_trackedInstances[oldIndex]->instanceData.modelResource;
    }
}
