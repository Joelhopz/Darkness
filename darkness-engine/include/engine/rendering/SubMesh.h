#pragma once

#include "engine/primitives/Vector2.h"
#include "engine/primitives/Vector3.h"
#include "engine/primitives/Vector4.h"
#include "engine/rendering/Material.h"
#include "tools/CompressedFile.h"

#include <vector>
#include <string>

namespace engine
{
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
        BoundingBox
    };

    struct MeshBlockHeader
    {
        MeshBlockType type;
        size_t size_bytes;
    };

    class SubMesh
    {
        typedef int Count;
    public:
        std::vector<Vector3f> position;
        std::vector<Vector3f> normal;
        std::vector<Vector3f> tangent;
        std::vector<Vector2f> uv;
        std::vector<uint32_t> indices;
        std::vector<Vector4f> colors;

        std::vector<uint32_t> clusterId;
        std::vector<uint32_t> clusterIndexCount;
        struct BoundingBox
        {
            Vector3f min;
            Vector3f max;
        };
        std::vector<BoundingBox> clusterBounds;
        BoundingBox boundingBox;

        Material material;

        size_t sizeBytes() const;
        void save(CompressedFile& file) const;
        bool load(CompressedFile& file);
    private:
        void writeBlockHeader(CompressedFile& file, MeshBlockHeader header) const;
        MeshBlockHeader readBlockHeader(CompressedFile& file);

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
        void readBlock(CompressedFile& file, std::vector<T>& data)
        {
            Count count;
            file.read(reinterpret_cast<char*>(&count), sizeof(Count));
            data.resize(static_cast<typename std::vector<T>::size_type>(count));
            file.read(reinterpret_cast<char*>(&data[0]), static_cast<std::streamsize>(sizeof(T) * count));
        }

        template <typename T>
        void writeBlock(CompressedFile& file, MeshBlockType type, const T& data) const
        {
            writeBlockHeader(file, { type, sizeof(T) });
            file.write(reinterpret_cast<const char*>(&data), static_cast<std::streamsize>(sizeof(T)));
        }

        template <typename T>
        void readBlock(CompressedFile& file, T& data)
        {
            file.read(reinterpret_cast<char*>(&data), static_cast<std::streamsize>(sizeof(T)));
        }
    };

}
