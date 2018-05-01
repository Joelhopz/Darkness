#pragma once

#include "engine/primitives/BoundingSphere.h"
#include "engine/graphics/Resources.h"
#include "engine/rendering/SubMesh.h"
#include "engine/rendering/ModelResourceAllocator.h"
#include "engine/rendering/GpuBuffers.h"
#include "engine/rendering/ResidencyManager.h"
#include "tools/ByteRange.h"
#include "tools/MemoryAllocator.h"

#include "engine/graphics/HLSLTypeConversions.h"

namespace engine
{
    namespace image
    {
        class ImageIf;
    }
    class Device;
    class CommandList;
    class SubMesh;

    
    #include "shaders/core/shared_types/ClusterData.hlsli"
    #include "shaders/core/shared_types/SubMeshData.hlsli"
    #include "shaders/core/shared_types/InstanceMaterial.hlsli"
    #include "shaders/core/shared_types/TransformHistory.hlsli"
    #include "shaders/core/shared_types/SubMeshAdjacency.hlsli"

    /*struct ClusterData
    {
        uint32_t indexPointer;
        uint32_t indexCount;
        uint32_t vertexPointer;
    };

    struct SubMeshData
    {
        uint32_t clusterPointer;
        uint32_t clusterCount;
    };

    struct InstanceMaterial
    {
        uint32_t albedo;
        uint32_t metalness;
        uint32_t roughness;
        uint32_t normal;

        uint32_t ao;
        uint32_t materialSet;
        float roughnessStrength;
        float metalnessStrength;

        float occlusionStrength;
        float scaleX;
        float scaleY;
        float padding;

        Vector3f color;
    };

    struct TransformHistory
    {
        Matrix4f transformMat;
        Matrix4f prevTransformMat;
        Matrix4f inverseTransform;
    };*/

    // ~40 MB
    constexpr uint32_t MaxIndexCount = 30000000;

    // ~80 MB
    constexpr uint32_t MaxUvCount = 30000000;

    // ~600 MB
    // single vertex takes 60 bytes
    // it includes vertex, normal, tangent, uv, color
    // TODO: will make this a lot smaller
    constexpr uint32_t MaxVertexCount = MaxIndexCount;
    
    constexpr uint32_t ClusterMaxSize = 3 * 64;

    // ~2.6 MB
    // MaxClusterCount ~52083 clusters
    // single cluster takes 52 bytes
    // it includes cluster indexPtr, indexCount, vertexPointer, bb, bs
    constexpr uint32_t MaxClusterCount = MaxIndexCount / ClusterMaxSize;
    constexpr uint32_t MaxClusterInstanceCount = (100ull * 1024ull * 1024ull) / 4u;

    // ~45.7 MB
    // single sub model takes 48 bytes
    constexpr uint32_t MaxSubModels = 10000;

    // ~244 MB
    // single instance takes 256 bytes
    // it includes submesh binding, transforms, material
    constexpr uint32_t MaxInstances = 500000;

    struct ModelResourceAllocationState
    {
        uint32_t activeInstanceCount;
    };

    class ModelResources
    {
    public:
        ModelResources(Device& device);

        //BufferSRV& clusterData();
        //TextureBindlessSRV& textureData();

        struct ModelAllocation
        {
            std::shared_ptr<engine::SubMeshInstance> subMeshInstance;

            TransformHistory transforms;
        };

        std::shared_ptr<ModelAllocation> addSubmesh(std::shared_ptr<engine::SubMeshInstance>& subMesh);

        uint32_t addMaterial(const ResourceKey& key, image::ImageIf* image, bool srgb = false);

        void updateSubmeshTransform(ModelAllocation& model, const Matrix4f& transform);
        void updateSubmeshMaterial(ModelAllocation& model, const InstanceMaterial& material);
        void updateSubmeshObjectId(ModelAllocation& model, uint32_t objectId);
        void updateSubmeshUV(ModelAllocation& model, uint32_t uv);

        void streamResources(CommandList& cmd);

        uint32_t instanceCount() const { return m_allocationState->activeInstanceCount; }

        GpuBuffers& gpuBuffers();
        ResidencyManager& residency();

        TextureBindlessSRV& textures() { return m_textures; }
    private:
        Device& m_device;
        GpuBuffers m_gpuBuffers;
        ResidencyManager m_residencyManager;

        // texture storage
        TextureBindlessSRV m_textures;

        void removeSubmesh(const ModelResources::ModelAllocation* instance);

        // pending uploads
        struct PendingUpload
        {
            tools::ByteRange source;
            tools::ByteRange destination;
        };
        /*std::vector<PendingUpload> m_pendingVertex;
        std::vector<PendingUpload> m_pendingNormal;
        std::vector<PendingUpload> m_pendingTangent;
        std::vector<PendingUpload> m_pendingIndice;
        std::vector<PendingUpload> m_pendingUv;

        std::vector<PendingUpload> m_pendingBoundingSphere;
        std::vector<PendingUpload> m_pendingBoundingBox;
        std::vector<PendingUpload> m_pendingBinding;*/
        std::vector<PendingUpload> m_pendingMaterial;
        std::vector<PendingUpload> m_pendingMaterialIdToObjectId;
        std::vector<PendingUpload> m_pendingObjectIdToMaterialId;
        std::vector<PendingUpload> m_pendingUV;

        std::vector<PendingUpload> m_pendingTransform;

        /*std::vector<std::unique_ptr<std::vector<uint32_t>>> m_temporaryIndexes;
        std::vector<std::unique_ptr<ClusterBinding>> m_temporaryBindings;*/
        std::vector<std::unique_ptr<InstanceMaterial>> m_temporaryMaterials;
        std::vector<std::unique_ptr<TransformHistory>> m_temporaryTransform;
        //std::vector<std::unique_ptr<BoundingSphere>> m_temporaryBoundingSpheres;
        std::vector<std::unique_ptr<uint32_t>> m_temporaryMaterialIdToObjectId;
        std::vector<std::unique_ptr<uint32_t>> m_temporaryObjectIdToMaterialId;
        std::vector<std::unique_ptr<uint32_t>> m_temporaryUV;

        /*std::vector<std::unique_ptr<GpuBoundingBox>> m_temporaryBoundingBoxes;*/
        
        // 
        std::shared_ptr<ModelResourceAllocationState> m_allocationState;
        
    };
}
