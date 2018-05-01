#pragma once

#include "engine/graphics/Resources.h"
#include "engine/rendering/ModelResourceAllocator.h"

namespace engine
{
    constexpr uint32_t InvalidMaxInstanceIndex = 0xFFFFFFFF;

    class SubMeshInstance;
    class Device;
    class GpuBuffers
    {
    public:
        GpuBuffers(Device& device);

        // per vertex data
        enum class VertexDataIndex
        {
            Vertex,
            Normal,
            Tangent,
            Color
        };

        enum class ClusterDataIndex
        {
            Binding,
            BoundingBox,
            BoundingSphere
        };

        enum class SubMeshDataIndex
        {
            SubMeshData,
            BoundingBox,
            BoundingSphere,
            Adjacency
        };

        enum class InstanceDataIndex
        {
            SubMeshBinding,
            Uv,
            Transform,
            Material
        };

        // per vertex data
        ModelResourceAllocator& vertexDataAllocator() { return m_vertexDataAllocator; };
        BufferSRV& vertex() { return m_vertexDataAllocator.gpuBuffers()[static_cast<int>(VertexDataIndex::Vertex)]; };
        BufferSRV& normal() { return m_vertexDataAllocator.gpuBuffers()[static_cast<int>(VertexDataIndex::Normal)]; };
        BufferSRV& tangent() { return m_vertexDataAllocator.gpuBuffers()[static_cast<int>(VertexDataIndex::Tangent)]; };
        BufferSRV& color() { return m_vertexDataAllocator.gpuBuffers()[static_cast<int>(VertexDataIndex::Color)]; };

        // uv data
        ModelResourceAllocator& uvDataAllocator() { return m_uvDataAllocator; };
        BufferSRV& uv() { return m_uvDataAllocator.gpuBuffers()[0]; };
        
        // per triangle data
        ModelResourceAllocator& indexAllocator() { return m_indexAllocator; };
        BufferSRV& index() { return m_indexAllocator.gpuBuffers()[0]; }

        // adjacency data
        ModelResourceAllocator& adjacencyAllocator() { return m_adjacencyAllocator; };
        BufferIBV& adjacency() { return m_adjacencyAllocator.gpuIndexBuffers()[0]; }

        // per cluster data
        ModelResourceAllocator& clusterDataAllocator() { return m_clusterDataAllocator; };
        BufferSRV& clusterBinding() { return m_clusterDataAllocator.gpuBuffers()[static_cast<int>(ClusterDataIndex::Binding)]; };
        BufferSRV& clusterBoundingBox() { return m_clusterDataAllocator.gpuBuffers()[static_cast<int>(ClusterDataIndex::BoundingBox)]; };
        BufferSRV& clusterBoundingSphere() { return m_clusterDataAllocator.gpuBuffers()[static_cast<int>(ClusterDataIndex::BoundingSphere)]; };
        
        // per submesh data
        ModelResourceAllocator& subMeshDataAllocator() { return m_subMeshDataAllocator; };
        BufferSRV& subMeshData() { return m_subMeshDataAllocator.gpuBuffers()[static_cast<int>(SubMeshDataIndex::SubMeshData)]; };
        BufferSRV& subMeshBoundingBox() { return m_subMeshDataAllocator.gpuBuffers()[static_cast<int>(SubMeshDataIndex::BoundingBox)]; };
        BufferSRV& subMeshBoundingSphere() { return m_subMeshDataAllocator.gpuBuffers()[static_cast<int>(SubMeshDataIndex::BoundingSphere)]; };
        BufferSRV& subMeshAdjacency() { return m_subMeshDataAllocator.gpuBuffers()[static_cast<int>(SubMeshDataIndex::Adjacency)]; };

        // per instance data
        ModelResourceLinearAllocator& instanceDataAllocator() { return m_instanceDataAllocator; };
        BufferSRV& instanceSubMeshBinding() { return m_instanceDataAllocator.gpuBuffers()[static_cast<int>(InstanceDataIndex::SubMeshBinding)]; };
        BufferSRV& instanceUv() { return m_instanceDataAllocator.gpuBuffers()[static_cast<int>(InstanceDataIndex::Uv)]; };
        BufferSRV& instanceTransform() { return m_instanceDataAllocator.gpuBuffers()[static_cast<int>(InstanceDataIndex::Transform)]; };
        BufferSRV& instanceMaterial() { return m_instanceDataAllocator.gpuBuffers()[static_cast<int>(InstanceDataIndex::Material)]; };

        void maxInstanceIndexInUse(uint32_t value);
        uint32_t maxInstanceIndexInUse() const;

        void addInstance(SubMeshInstance* instance);
        void removeInstance(SubMeshInstance* instance);
        void relocateInstance(uint32_t oldIndex, uint32_t newIndex);
    private:
        // model storage
        ModelResourceAllocator m_vertexDataAllocator;
        ModelResourceAllocator m_uvDataAllocator;
        ModelResourceAllocator m_indexAllocator;
        ModelResourceAllocator m_adjacencyAllocator;
        ModelResourceAllocator m_clusterDataAllocator;
        ModelResourceAllocator m_subMeshDataAllocator;
        ModelResourceLinearAllocator m_instanceDataAllocator;
        uint32_t m_maxInstanceIndexInUse;
        std::vector<SubMeshInstance*> m_trackedInstances;
    };
}
