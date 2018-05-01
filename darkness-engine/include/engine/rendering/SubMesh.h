#pragma once

#include "engine/primitives/BoundingBox.h"
#include "engine/primitives/BoundingSphere.h"
#include "engine/primitives/Vector2.h"
#include "engine/primitives/Vector3.h"
#include "engine/primitives/Vector4.h"
#include "engine/rendering/Material.h"
#include "tools/CompressedFile.h"
#include "engine/rendering/ResidencyManager.h"

#include <vector>
#include <string>

namespace engine
{
    class ModelResources;

    enum class MeshBlockType
    {
        Position,
        Normal,
        Tangent,
        Uv,
        Indice,
        Color,
        Material,
        ClusterId,
        ClusterIndexCount,
        ClusterBounds,
        BoundingBox,
        AdjacencyData
    };

    struct MeshBlockHeader
    {
        MeshBlockType type;
        size_t size_bytes;
    };
    
    class SubMeshInstance
    {
    public:
        ModelResource instanceData;
        std::vector<ModelResource*> uvData;
    };

    class SubMesh
    {
        typedef int Count;
    public:
        ModelResource vertexData;
        std::vector<ModelResource> uvData;
        ModelResource triangleData;
        ModelResource adjacencyData;
        ModelResource clusterData;
        ModelResource subMeshData;

        std::vector<Vector2<uint32_t>> out_position;
        std::vector<Vector2f> out_normal;
        std::vector<Vector2f> out_tangent;
        std::vector<std::vector<Vector2f>> out_uv;
        std::vector<uint32_t> out_indices;
        std::vector<uint32_t> out_adjacency;
        std::vector<Vector4<unsigned char>> out_colors;
        std::vector<BoundingBox> out_clusterBounds;
        std::vector<uint32_t> clusterId;
        std::vector<uint32_t> clusterIndexCount;
        BoundingBox boundingBox;
        BoundingSphere boundingSphere;
        Material out_material;

        //size_t sizeBytes() const;
        void save(CompressedFile& file) const;
        bool load(ModelResources& modelResources, CompressedFile& file);

        std::shared_ptr<SubMeshInstance> createInstance(ModelResources& modelResources);
        void freeInstance(ModelResources& modelResources, SubMeshInstance* instance);

        int instanceCount() const;
    private:
        void writeBlockHeader(CompressedFile& file, MeshBlockHeader header) const;
        MeshBlockHeader readBlockHeader(CompressedFile& file);

        int m_instanceCount = 0;

        Count elementCount() const;

        template <typename T>
        size_t blockSize(const std::vector<T>& data) const
        {
            if(data.size() > 0)
                return sizeof(MeshBlockHeader) + sizeof(Count) + (sizeof(T) * data.size());
            else
                return 0;
        }

        template <typename T>
        void writeBlock(CompressedFile& file, MeshBlockType type, const std::vector<T>& data) const
        {
            if (data.size() > 0)
            {
                writeBlockHeader(file, { type, static_cast<size_t>(sizeof(Count) + (sizeof(T) * data.size())) });
                Count count = static_cast<Count>(data.size());
                file.write(reinterpret_cast<char*>(&count), sizeof(Count));
                file.write(reinterpret_cast<const char*>(&data[0]), static_cast<std::streamsize>(sizeof(T) * count));
            }
        }

        template <typename T>
        void writeBlock(CompressedFile& file, MeshBlockType type, const T& data) const
        {
            writeBlockHeader(file, { type, sizeof(T) });
            file.write(reinterpret_cast<const char*>(&data), static_cast<std::streamsize>(sizeof(T)));
        }

        void allocateIfNecessary(ModelResourceAllocator& resourceAllocator, ModelResource& data, Count count)
        {
            if (!data.allocated)
            {
                data.modelResource = resourceAllocator.allocate(count);
                data.allocated = true;
            }
        };

        template <typename T>
        void readBlock(ResidencyManager& residency, ModelResourceAllocator& resourceAllocator, CompressedFile& file, ModelResource& data)
        {
            Count count;
            file.read(reinterpret_cast<char*>(&count), sizeof(Count));
            allocateIfNecessary(resourceAllocator, data, count);
            data.uploads.emplace_back(residency.createUpdateAllocation(count * sizeof(T)));
            data.uploads.back().gpuIndex = data.modelResource.gpuIndex;
            file.read(reinterpret_cast<char*>(data.uploads.back().ptr), static_cast<std::streamsize>(sizeof(T) * count));
        }

        template <typename T>
        void readBlock(CompressedFile& file, std::vector<T>& data)
        {
            Count count;
            file.read(reinterpret_cast<char*>(&count), sizeof(Count));
            data.resize(static_cast<typename std::vector<T>::size_type>(count));
            file.read(reinterpret_cast<char*>(&data[0]), static_cast<std::streamsize>(sizeof(T) * count));
        }

        template <typename T>
        void readBlock(CompressedFile& file, T& data)
        {
            file.read(reinterpret_cast<char*>(&data), static_cast<std::streamsize>(sizeof(T)));
        }
    };

}
