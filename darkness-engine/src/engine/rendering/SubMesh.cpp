#include "engine/rendering/SubMesh.h"
#include "engine/primitives/Vector3.h"
#include "tools/Debug.h"

using namespace std;

namespace engine
{

    size_t SubMesh::sizeBytes() const
    {
        return blockSize<Vector3f>(position);
    }

    void SubMesh::writeBlockHeader(CompressedFile& file, MeshBlockHeader header) const
    {
        file.write(reinterpret_cast<char*>(&header), sizeof(MeshBlockHeader));
    }

    MeshBlockHeader SubMesh::readBlockHeader(CompressedFile& file)
    {
        MeshBlockHeader header;
        file.read(reinterpret_cast<char*>(&header), sizeof(MeshBlockHeader));
        return header;
    }

    bool SubMesh::load(CompressedFile& file)
    {
        Count elements;
        file.read(reinterpret_cast<char*>(&elements), sizeof(Count));

        if (file.eof())
            return false;

        while (elements)
        {
            auto blockHeader = readBlockHeader(file);
            switch (blockHeader.type)
            {
                case MeshBlockType::Position: 
                {
                    readBlock<Vector3f>(file, position);
                    break;
                }
                case MeshBlockType::Normal:
                {
                    readBlock<Vector3f>(file, normal);
                    break;
                }
                case MeshBlockType::Tangent: 
                {
                    readBlock<Vector3f>(file, tangent);
                    break;
                }
                case MeshBlockType::Uv: 
                {
                    readBlock<Vector2f>(file, uv);
                    break;
                }
                case MeshBlockType::Indice: 
                {
                    readBlock<uint32_t>(file, indices);
                    break;
                }
                case MeshBlockType::Color: 
                {
                    readBlock<Vector4f>(file, colors);
                    break;
                }
                case MeshBlockType::Material:
                {
                    if (blockHeader.size_bytes > 0)
                    {
                        std::vector<uint8_t> data(blockHeader.size_bytes);
                        file.read(reinterpret_cast<char*>(&data[0]), blockHeader.size_bytes);
                        material.load(data);
                    }
                    else
                    {
                        LOG("found null material in the model file");
                    }
                    break;
                }
                case MeshBlockType::BoundingBox:
                {
                    readBlock<SubMesh::BoundingBox>(file, boundingBox);
                    break;
                }
                case MeshBlockType::ClusterId:
                {
                    readBlock<uint32_t>(file, clusterId);
                    break;
                }
                case MeshBlockType::ClusterIndexCount:
                {
                    readBlock<uint32_t>(file, clusterIndexCount);
                    break;
                }
                case MeshBlockType::ClusterBounds:
                {
                    readBlock<SubMesh::BoundingBox>(file, clusterBounds);
                    break;
                }
                default: file.seekg(static_cast<std::streamoff>(blockHeader.size_bytes), ios::cur);
            }
            --elements;
        }
        return true;
    }

    SubMesh::Count SubMesh::elementCount() const
    {
        Count count{ 0 };
        if (position.size() > 0) ++count;
        if (normal.size() > 0) ++count;
        if (tangent.size() > 0) ++count;
        if (uv.size() > 0) ++count;
        if (indices.size() > 0) ++count;
        if (colors.size() > 0) ++count;
        if (material.data().size() > 0) ++count;
        if (position.size() > 0) ++count; // bounding box
        if (clusterId.size() > 0) ++count;
        if (clusterIndexCount.size() > 0) ++count;
        if (clusterBounds.size() > 0) ++count;
        return count;
    }

    void SubMesh::save(CompressedFile& file) const
    {
        Count elements = elementCount();
        file.write(reinterpret_cast<char*>(&elements), sizeof(Count));

        if (position.size() > 0)    writeBlock<Vector3f>(file, MeshBlockType::Position, position);
        if (normal.size() > 0)      writeBlock<Vector3f>(file, MeshBlockType::Normal, normal);
        if (tangent.size() > 0)     writeBlock<Vector3f>(file, MeshBlockType::Tangent, tangent);
        if (uv.size() > 0)          writeBlock<Vector2f>(file, MeshBlockType::Uv, uv);
        if (indices.size() > 0)     writeBlock<uint32_t>(file, MeshBlockType::Indice, indices);
        if (colors.size() > 0)      writeBlock<Vector4f>(file, MeshBlockType::Color, colors);
        
        auto materialData = material.data();
        if (materialData.size() > 0)
        {
            writeBlockHeader(file, { MeshBlockType::Material, materialData.size() });
            file.write(reinterpret_cast<char*>(materialData.data()), materialData.size());
        }
        if (position.size() > 0)            writeBlock<SubMesh::BoundingBox>(file, MeshBlockType::BoundingBox, boundingBox);
        if (clusterId.size() > 0)           writeBlock<uint32_t>(file, MeshBlockType::ClusterId, clusterId);
        if (clusterIndexCount.size() > 0)   writeBlock<uint32_t>(file, MeshBlockType::ClusterIndexCount, clusterIndexCount);
        if (clusterBounds.size() > 0)       writeBlock<SubMesh::BoundingBox>(file, MeshBlockType::ClusterBounds, clusterBounds);
    }

}
