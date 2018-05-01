#include "engine/rendering/SubMesh.h"
#include "engine/rendering/ModelResources.h"
#include "engine/primitives/Vector3.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/CommandList.h"
#include "engine/graphics/Fence.h"
#include "engine/graphics/Queue.h"
#include "tools/MeshTools.h"
#include "tools/Debug.h"

#include <inttypes.h>

using namespace std;

namespace engine
{

    /*size_t SubMesh::sizeBytes() const
    {
        return blockSize<Vector3f>(position);
    }*/

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

    bool SubMesh::load(ModelResources& modelResources, CompressedFile& file)
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
                    readBlock<Vector2<uint32_t>>(modelResources.residency(), modelResources.gpuBuffers().vertexDataAllocator(), file, vertexData);
                    vertexData.uploads.back().buffer = modelResources.gpuBuffers().vertex();
                    modelResources.residency().makeResident(vertexData.uploads.back());
                    modelResources.residency().freeUpdateAllocation(vertexData.uploads.back());
                    vertexData.uploads.pop_back();
                    break;
                }
                case MeshBlockType::Normal:
                {
                    readBlock<Vector2f>(modelResources.residency(), modelResources.gpuBuffers().vertexDataAllocator(), file, vertexData);
                    vertexData.uploads.back().buffer = modelResources.gpuBuffers().normal();
                    modelResources.residency().makeResident(vertexData.uploads.back());
                    modelResources.residency().freeUpdateAllocation(vertexData.uploads.back());
                    vertexData.uploads.pop_back();
                    // we don't clear the uploads because of adjacency calculation
                    break;
                }
                case MeshBlockType::Tangent: 
                {
                    readBlock<Vector2f>(modelResources.residency(), modelResources.gpuBuffers().vertexDataAllocator(), file, vertexData);
                    vertexData.uploads.back().buffer = modelResources.gpuBuffers().tangent();
                    modelResources.residency().makeResident(vertexData.uploads.back());
                    modelResources.residency().freeUpdateAllocation(vertexData.uploads.back());
                    vertexData.uploads.pop_back();
                    break;
                }
                case MeshBlockType::Uv: 
                {
                    ModelResource u;
                    readBlock<Vector2f>(modelResources.residency(), modelResources.gpuBuffers().uvDataAllocator(), file, u);
                    u.uploads.back().buffer = modelResources.gpuBuffers().uv();
                    modelResources.residency().makeResident(u.uploads.back());
                    modelResources.residency().freeUpdateAllocation(u.uploads.back());
                    u.uploads.pop_back();
                    uvData.emplace_back(std::move(u));
                    break;
                }
                case MeshBlockType::Indice: 
                {
                    readBlock<uint32_t>(modelResources.residency(), modelResources.gpuBuffers().indexAllocator(), file, triangleData);
                    triangleData.uploads.back().buffer = modelResources.gpuBuffers().index();
                    modelResources.residency().makeResident(triangleData.uploads.back());
                    modelResources.residency().freeUpdateAllocation(triangleData.uploads.back());
                    triangleData.uploads.pop_back();
                    break;
                }
                case MeshBlockType::AdjacencyData:
                {
                    readBlock<uint32_t>(modelResources.residency(), modelResources.gpuBuffers().adjacencyAllocator(), file, adjacencyData);
                    adjacencyData.uploads.back().indexBuffer = modelResources.gpuBuffers().adjacency();
                    auto ptr = reinterpret_cast<uint32_t*>(adjacencyData.uploads.back().ptr);
                    for (uint32_t i = 0; i < adjacencyData.modelResource.elements; ++i)
                    {
                        *ptr += vertexData.modelResource.gpuIndex;
                        ++ptr;
                    }
                    modelResources.residency().makeResident(adjacencyData.uploads.back());
                    modelResources.residency().freeUpdateAllocation(adjacencyData.uploads.back());
                    adjacencyData.uploads.pop_back();
                    break;
                }
                case MeshBlockType::Color: 
                {
                    readBlock<Vector4<unsigned char>>(modelResources.residency(), modelResources.gpuBuffers().vertexDataAllocator(), file, vertexData);
                    vertexData.uploads.back().buffer = modelResources.gpuBuffers().color();
                    modelResources.residency().makeResident(vertexData.uploads.back());
                    modelResources.residency().freeUpdateAllocation(vertexData.uploads.back());
                    vertexData.uploads.pop_back();
                    // we don't clear the uploads because of adjacency calculation
                    break;
                }
                case MeshBlockType::Material:
                {
                    if (blockHeader.size_bytes > 0)
                    {
                        std::vector<uint8_t> data(blockHeader.size_bytes);
                        file.read(reinterpret_cast<char*>(&data[0]), blockHeader.size_bytes);
                        out_material.load(data);
                    }
                    else
                    {
                        LOG("found null material in the model file");
                    }
                    break;
                }
                case MeshBlockType::BoundingBox:
                {
                    readBlock<BoundingBox>(file, boundingBox);
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
                    //readBlock<BoundingBox>(modelResources.residency(), modelResources.gpuBuffers().boundingBoxAllocator(), file, clusterBounds);
                    readBlock<BoundingBox>(file, out_clusterBounds);
                    break;
                }
                default: file.seekg(static_cast<std::streamoff>(blockHeader.size_bytes), ios::cur);
            }
            --elements;
        }

        // generate adjacency data
        /*adjacencyData.modelResource = modelResources.gpuBuffers().adjacencyAllocator().allocate(triangleData.modelResource.elements * 2);
        adjacencyData.uploads.emplace_back(modelResources.residency().createUpdateAllocation(triangleData.modelResource.elements * 2 * sizeof(uint32_t)));
        adjacencyData.uploads.back().gpuIndex = adjacencyData.modelResource.gpuIndex;
        adjacencyData.uploads.back().indexBuffer = modelResources.gpuBuffers().adjacency();
        adjacencyData.allocated = true;
        meshGenerateAdjacency(
            reinterpret_cast<uint32_t*>(triangleData.uploads[0].ptr), 
            triangleData.modelResource.elements, 
            reinterpret_cast<Vector3f*>(vertexData.uploads[0].ptr),
            vertexData.modelResource.elements, 
            reinterpret_cast<uint32_t*>(adjacencyData.uploads[0].ptr),
            triangleData.modelResource.elements * 2,
            vertexData.modelResource.gpuIndex);

        modelResources.residency().makeResident(adjacencyData.uploads.back());
        modelResources.residency().freeUpdateAllocation(adjacencyData.uploads.back());
        for(auto&& vertexDataUpload : vertexData.uploads)
        { 
            modelResources.residency().freeUpdateAllocation(vertexDataUpload);
        }
        modelResources.residency().freeUpdateAllocation(triangleData.uploads[0]);
        vertexData.uploads.clear();
        triangleData.uploads.clear();
        adjacencyData.uploads.clear();*/

        // vertex, uv and triangle data has already been handled above
        // now we need to handle cluster and submesh data

        // cluster data
        clusterData.modelResource = modelResources.gpuBuffers().clusterDataAllocator().allocate(static_cast<uint32_t>(clusterId.size()));
        clusterData.uploads.emplace_back(modelResources.residency().createUpdateAllocation(static_cast<uint32_t>(sizeof(ClusterData) * clusterId.size())));
        clusterData.uploads.emplace_back(modelResources.residency().createUpdateAllocation(static_cast<uint32_t>(sizeof(BoundingBox) * clusterId.size())));
        clusterData.uploads.emplace_back(modelResources.residency().createUpdateAllocation(static_cast<uint32_t>(sizeof(BoundingSphere) * clusterId.size())));

        clusterData.uploads[0].gpuIndex = clusterData.modelResource.gpuIndex;
        clusterData.uploads[1].gpuIndex = clusterData.modelResource.gpuIndex;
        clusterData.uploads[2].gpuIndex = clusterData.modelResource.gpuIndex;

        clusterData.uploads[0].buffer = modelResources.gpuBuffers().clusterBinding();
        clusterData.uploads[1].buffer = modelResources.gpuBuffers().clusterBoundingBox();
        clusterData.uploads[2].buffer = modelResources.gpuBuffers().clusterBoundingSphere();

        ClusterData* cData =     reinterpret_cast<ClusterData*>(clusterData.uploads[0].ptr);
        BoundingBox* bbData =    reinterpret_cast<BoundingBox*>(clusterData.uploads[1].ptr);
        BoundingSphere* bsData = reinterpret_cast<BoundingSphere*>(clusterData.uploads[2].ptr);
        uint32_t indexStart = triangleData.modelResource.gpuIndex;
        for (int i = 0; i < clusterId.size(); ++i)
        {
            cData->indexCount = clusterIndexCount[i];
            cData->indexPointer = indexStart;
            cData->vertexPointer = vertexData.modelResource.gpuIndex;
            *bbData = out_clusterBounds[i];
            *bsData = boundingSphere; // TODO: there actually isn't any bounding sphere data currently

            indexStart += clusterIndexCount[i];
            cData++;
            bbData++;
            bsData++;
            
        }

        modelResources.residency().makeResident(clusterData.uploads[0]);
        modelResources.residency().makeResident(clusterData.uploads[1]);
        modelResources.residency().makeResident(clusterData.uploads[2]);
        modelResources.residency().freeUpdateAllocation(clusterData.uploads[0]);
        modelResources.residency().freeUpdateAllocation(clusterData.uploads[1]);
        modelResources.residency().freeUpdateAllocation(clusterData.uploads[2]);
        clusterData.uploads.clear();

        // submesh data
        subMeshData.modelResource = modelResources.gpuBuffers().subMeshDataAllocator().allocate(1);
        subMeshData.uploads.emplace_back(modelResources.residency().createUpdateAllocation(sizeof(SubMeshAdjacency)));
        subMeshData.uploads.emplace_back(modelResources.residency().createUpdateAllocation(sizeof(SubMeshData)));
        subMeshData.uploads.emplace_back(modelResources.residency().createUpdateAllocation(sizeof(BoundingBox)));
        subMeshData.uploads.emplace_back(modelResources.residency().createUpdateAllocation(sizeof(BoundingSphere)));

        subMeshData.uploads[0].gpuIndex = subMeshData.modelResource.gpuIndex;
        subMeshData.uploads[1].gpuIndex = subMeshData.modelResource.gpuIndex;
        subMeshData.uploads[2].gpuIndex = subMeshData.modelResource.gpuIndex;
        subMeshData.uploads[3].gpuIndex = subMeshData.modelResource.gpuIndex;

        subMeshData.uploads[0].buffer = modelResources.gpuBuffers().subMeshAdjacency();
        subMeshData.uploads[1].buffer = modelResources.gpuBuffers().subMeshData();
        subMeshData.uploads[2].buffer = modelResources.gpuBuffers().subMeshBoundingBox();
        subMeshData.uploads[3].buffer = modelResources.gpuBuffers().subMeshBoundingSphere();

        SubMeshAdjacency*   sAdjacency = reinterpret_cast<SubMeshAdjacency*>(subMeshData.uploads[0].ptr);
        SubMeshData*        sMeshData = reinterpret_cast<SubMeshData*>(subMeshData.uploads[1].ptr);
        BoundingBox*        sbbData =   reinterpret_cast<BoundingBox*>(subMeshData.uploads[2].ptr);
        BoundingSphere*     sbsData =   reinterpret_cast<BoundingSphere*>(subMeshData.uploads[3].ptr);

        sAdjacency->adjacencyPointer = adjacencyData.modelResource.gpuIndex;
        sAdjacency->adjacencyCount = adjacencyData.modelResource.elements;
        sAdjacency->baseVertexPointer = vertexData.modelResource.gpuIndex;
        sMeshData->clusterCount = static_cast<uint>(clusterId.size());
        sMeshData->clusterPointer = clusterData.modelResource.gpuIndex;
        *sbbData = boundingBox;
        *sbsData = boundingSphere;

        modelResources.residency().makeResident(subMeshData.uploads[0]);
        modelResources.residency().makeResident(subMeshData.uploads[1]);
        modelResources.residency().makeResident(subMeshData.uploads[2]);
        modelResources.residency().makeResident(subMeshData.uploads[3]);
        modelResources.residency().freeUpdateAllocation(subMeshData.uploads[0]);
        modelResources.residency().freeUpdateAllocation(subMeshData.uploads[1]);
        modelResources.residency().freeUpdateAllocation(subMeshData.uploads[2]);
        modelResources.residency().freeUpdateAllocation(subMeshData.uploads[3]);
        subMeshData.uploads.clear();

        return true;
    }

    static uint64_t InstanceCount = 0;

    std::shared_ptr<SubMeshInstance> SubMesh::createInstance(ModelResources& modelResources)
    {
        ++InstanceCount;
        //LOG("InstanceCount: %" PRIu64 "", InstanceCount);

        std::shared_ptr<SubMeshInstance> instance = std::shared_ptr<SubMeshInstance>(new SubMeshInstance(), [this, &modelResources](SubMeshInstance* ptr) { this->freeInstance(modelResources, ptr); delete ptr; });
        ModelResource& modres = instance->instanceData;

        for (auto&& uv : uvData)
        {
            instance->uvData.emplace_back(&uv);
        }

        modres.modelResource = modelResources.gpuBuffers().instanceDataAllocator().allocate(1);

        modelResources.gpuBuffers().addInstance(instance.get());

        auto maxInstanceIndex = modelResources.gpuBuffers().maxInstanceIndexInUse();
        if (maxInstanceIndex == InvalidMaxInstanceIndex)
            modelResources.gpuBuffers().maxInstanceIndexInUse(modres.modelResource.gpuIndex);
        else if (maxInstanceIndex < modres.modelResource.gpuIndex)
            modelResources.gpuBuffers().maxInstanceIndexInUse(modres.modelResource.gpuIndex);

        modres.uploads.emplace_back(modelResources.residency().createUpdateAllocation(sizeof(uint32_t)));
        modres.uploads.emplace_back(modelResources.residency().createUpdateAllocation(sizeof(uint32_t)));
        modres.uploads.emplace_back(modelResources.residency().createUpdateAllocation(sizeof(TransformHistory)));
        modres.uploads.emplace_back(modelResources.residency().createUpdateAllocation(sizeof(InstanceMaterial)));
        
        modres.uploads[0].gpuIndex = modres.modelResource.gpuIndex;
        modres.uploads[1].gpuIndex = modres.modelResource.gpuIndex;
        modres.uploads[2].gpuIndex = modres.modelResource.gpuIndex;
        modres.uploads[3].gpuIndex = modres.modelResource.gpuIndex;
        
        modres.uploads[0].buffer = modelResources.gpuBuffers().instanceSubMeshBinding();
        modres.uploads[1].buffer = modelResources.gpuBuffers().instanceUv();
        modres.uploads[2].buffer = modelResources.gpuBuffers().instanceTransform();
        modres.uploads[3].buffer = modelResources.gpuBuffers().instanceMaterial();

        uint32_t*           subMeshBinding  = reinterpret_cast<uint32_t*>(modres.uploads[0].ptr);
        uint32_t*           uv              = reinterpret_cast<uint32_t*>(modres.uploads[1].ptr);
        TransformHistory*   transform       = reinterpret_cast<TransformHistory*>(modres.uploads[2].ptr);
        InstanceMaterial*   material        = reinterpret_cast<InstanceMaterial*>(modres.uploads[3].ptr);

        *subMeshBinding = subMeshData.modelResource.gpuIndex;
        if(uvData.size() > 0)
            *uv = uvData[0].modelResource.gpuIndex;
        *transform = TransformHistory{};

        material->albedo = 0;
        material->metalness = 0;
        material->roughness = 0;
        material->normal = 0;
        material->ao = 0;
        material->materialSet = 0;
        material->roughnessStrength = 0.0f;
        material->metalnessStrength = 0.0f;
        material->occlusionStrength = 0.0f;
        material->scaleX = 0.0f;
        material->scaleY = 0.0f;
        material->padding = 0;
        material->color = {};

        modelResources.residency().makeResident(modres.uploads[0]);
        modelResources.residency().makeResident(modres.uploads[1]);
        modelResources.residency().makeResident(modres.uploads[2]);
        modelResources.residency().makeResident(modres.uploads[3]);
        modelResources.residency().freeUpdateAllocation(modres.uploads[0]);
        modelResources.residency().freeUpdateAllocation(modres.uploads[1]);
        modelResources.residency().freeUpdateAllocation(modres.uploads[2]);
        modelResources.residency().freeUpdateAllocation(modres.uploads[3]);
        modres.uploads.clear();

        ++m_instanceCount;
        return instance;
    }

    void SubMesh::freeInstance(ModelResources& modelResources, SubMeshInstance* instance)
    {
        auto maxInstanceIndex = modelResources.gpuBuffers().maxInstanceIndexInUse();
        if (instance->instanceData.modelResource.gpuIndex < maxInstanceIndex)
        {
            std::vector<Buffer> gpuInstanceBuffers = {
                modelResources.gpuBuffers().instanceSubMeshBinding().buffer(),
                modelResources.gpuBuffers().instanceUv().buffer(),
                modelResources.gpuBuffers().instanceTransform().buffer(),
                modelResources.gpuBuffers().instanceMaterial().buffer()};

            std::vector<int32_t> bufferElementSizes = {
                gpuInstanceBuffers[0].description().descriptor.elementSize,
                gpuInstanceBuffers[1].description().descriptor.elementSize,
                gpuInstanceBuffers[2].description().descriptor.elementSize,
                gpuInstanceBuffers[3].description().descriptor.elementSize
            };

            uint32_t allBytes = 0;
            for (auto&& bufElementSize : bufferElementSizes)
                allBytes += bufElementSize;

            auto upload = modelResources.residency().createUpdateAllocation(allBytes);

            {
                auto cmd = modelResources.residency().device().createCommandList(CommandListType::Direct);
                auto currentUploadBytePosition = 0;
                for (int i = 0; i < gpuInstanceBuffers.size(); ++i)
                {
                    cmd.copyBuffer(gpuInstanceBuffers[i], modelResources.residency().uploadBuffer(), 1, maxInstanceIndex, currentUploadBytePosition);
                    currentUploadBytePosition += bufferElementSizes[i];
                }
                currentUploadBytePosition = 0;
                for (int i = 0; i < gpuInstanceBuffers.size(); ++i)
                {
                    cmd.copyBuffer(modelResources.residency().uploadBuffer(), gpuInstanceBuffers[i], bufferElementSizes[i], currentUploadBytePosition, instance->instanceData.modelResource.gpuIndex);
                    currentUploadBytePosition += bufferElementSizes[i];
                }

                auto fence = modelResources.residency().device().createFence();
                modelResources.residency().device().queue().submit(cmd, fence);
                fence.blockUntilSignaled();
            }

            modelResources.gpuBuffers().relocateInstance(maxInstanceIndex, instance->instanceData.modelResource.gpuIndex);
            modelResources.gpuBuffers().removeInstance(instance);

            modelResources.gpuBuffers().instanceDataAllocator().free(maxInstanceIndex);
            --maxInstanceIndex;
            modelResources.gpuBuffers().maxInstanceIndexInUse(maxInstanceIndex);

            
        }
        else
        {
            modelResources.gpuBuffers().removeInstance(instance);
            modelResources.gpuBuffers().instanceDataAllocator().free(instance->instanceData.modelResource);
            --maxInstanceIndex;
            modelResources.gpuBuffers().maxInstanceIndexInUse(maxInstanceIndex);
        }
        
        --m_instanceCount;
    }

    int SubMesh::instanceCount() const 
    {
        return m_instanceCount;
    }

    SubMesh::Count SubMesh::elementCount() const
    {
        Count count{ 0 };
        if (out_position.size() > 0) ++count;
        if (out_normal.size() > 0) ++count;
        if (out_tangent.size() > 0) ++count;
        if (out_uv.size() > 0) count += static_cast<engine::SubMesh::Count>(out_uv.size());
        if (out_indices.size() > 0) ++count;
        if (out_adjacency.size() > 0) ++count;
        if (out_colors.size() > 0) ++count;
        if (out_material.data().size() > 0) ++count;
        if (out_position.size() > 0) ++count; // bounding box
        if (clusterId.size() > 0) ++count;
        if (clusterIndexCount.size() > 0) ++count;
        if (out_clusterBounds.size() > 0) ++count;
        return count;
    }

    void SubMesh::save(CompressedFile& file) const
    {
        Count elements = elementCount();
        file.write(reinterpret_cast<char*>(&elements), sizeof(Count));

        if (out_position.size() > 0)    writeBlock<Vector2<uint32_t>>(file, MeshBlockType::Position, out_position);
        if (out_normal.size() > 0)      writeBlock<Vector2f>(file, MeshBlockType::Normal, out_normal);
        if (out_tangent.size() > 0)     writeBlock<Vector2f>(file, MeshBlockType::Tangent, out_tangent);
        for (auto&& u : out_uv)
        {
            if (u.size() > 0)          writeBlock<Vector2f>(file, MeshBlockType::Uv, u);
        }
        if (out_indices.size() > 0)     writeBlock<uint32_t>(file, MeshBlockType::Indice, out_indices);
        if (out_adjacency.size() > 0)     writeBlock<uint32_t>(file, MeshBlockType::AdjacencyData, out_adjacency);
        if (out_colors.size() > 0)      writeBlock<Vector4<unsigned char>>(file, MeshBlockType::Color, out_colors);
        
        auto materialData = out_material.data();
        if (materialData.size() > 0)
        {
            writeBlockHeader(file, { MeshBlockType::Material, materialData.size() });
            file.write(reinterpret_cast<char*>(materialData.data()), materialData.size());
        }
        if (out_position.size() > 0)            writeBlock<BoundingBox>(file, MeshBlockType::BoundingBox, boundingBox);
        if (clusterId.size() > 0)           writeBlock<uint32_t>(file, MeshBlockType::ClusterId, clusterId);
        if (clusterIndexCount.size() > 0)   writeBlock<uint32_t>(file, MeshBlockType::ClusterIndexCount, clusterIndexCount);
        if (out_clusterBounds.size() > 0)       writeBlock<BoundingBox>(file, MeshBlockType::ClusterBounds, out_clusterBounds);
    }

}
