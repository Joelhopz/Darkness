#include "components/MeshRendererComponent.h"
#include "tools/MeshTools.h"

namespace engine
{
    MeshBuffers convert(
        const std::string& path,
        Device& device, 
        engine::SubMesh& source)
    {
        MeshBuffers buffers;
        std::string key = path + "_vert";
        buffers.vertices = device.createBufferSRV(
            tools::hash(
                reinterpret_cast<const uint8_t*>(key.c_str()),
                static_cast<unsigned int>(key.length())),
            BufferDescription()
            .usage(ResourceUsage::CpuToGpu)
            .format(Format::Format_R32G32B32_FLOAT)
            .name("SubMesh Vertices")
            .setInitialData(BufferDescription::InitialData(source.position)));

        key = path + "_norm";
        buffers.normals = device.createBufferSRV(
            tools::hash(
                reinterpret_cast<const uint8_t*>(key.c_str()),
                static_cast<unsigned int>(key.length())),
            BufferDescription()
            .usage(ResourceUsage::CpuToGpu)
            .format(Format::Format_R32G32B32_FLOAT)
            .name("SubMesh Normals")
            .setInitialData(BufferDescription::InitialData(source.normal)));

        key = path + "_tang";
        buffers.tangents = device.createBufferSRV(
            tools::hash(
                reinterpret_cast<const uint8_t*>(key.c_str()),
                static_cast<unsigned int>(key.length())),
            BufferDescription()
            .usage(ResourceUsage::CpuToGpu)
            .format(Format::Format_R32G32B32_FLOAT)
            .name("SubMesh Tangents")
            .setInitialData(BufferDescription::InitialData(source.tangent)));

        key = path + "_uv";
        buffers.uv = device.createBufferSRV(
            tools::hash(
                reinterpret_cast<const uint8_t*>(key.c_str()),
                static_cast<unsigned int>(key.length())),
            BufferDescription()
            .usage(ResourceUsage::CpuToGpu)
            .format(Format::Format_R32G32_FLOAT)
            .name("SubMesh UV")
            .setInitialData(BufferDescription::InitialData(source.uv)));

        key = path + "_indices";
        buffers.indices = device.createBufferIBV(
            tools::hash(
                reinterpret_cast<const uint8_t*>(key.c_str()),
                static_cast<unsigned int>(key.length())),
            BufferDescription()
            .usage(ResourceUsage::CpuToGpu)
            .name("SubMesh Indices")
            .setInitialData(BufferDescription::InitialData(source.indices)));

        key = path + "_indicesAdj";
        if(device.cachedDataExists<BufferIBV>(tools::hash(
            reinterpret_cast<const uint8_t*>(key.c_str()),
            static_cast<unsigned int>(key.length()))))
        {
            buffers.indicesAdjacency = device.createBufferIBV(
                tools::hash(
                    reinterpret_cast<const uint8_t*>(key.c_str()),
                    static_cast<unsigned int>(key.length())),
                BufferDescription()
                .usage(ResourceUsage::CpuToGpu)
                .name("SubMesh Indices Adjacency"));
        }
        else
        {
            auto adj = meshGenerateAdjacency(source.indices, source.position);
            buffers.indicesAdjacency = device.createBufferIBV(
                tools::hash(
                    reinterpret_cast<const uint8_t*>(key.c_str()),
                    static_cast<unsigned int>(key.length())),
                BufferDescription()
                .usage(ResourceUsage::CpuToGpu)
                .name("SubMesh Indices Adjacency")
                .setInitialData(BufferDescription::InitialData(adj)));
        }
        
        key = path + "_clusterId";
        buffers.clusterId = device.createBufferSRV(
            tools::hash(
                reinterpret_cast<const uint8_t*>(key.c_str()),
                static_cast<unsigned int>(key.length())),
            BufferDescription()
            .usage(ResourceUsage::CpuToGpu)
            .format(Format::Format_R32_UINT)
            .name("SubMesh Cluster Ids")
            .setInitialData(BufferDescription::InitialData(source.clusterId)));

        key = path + "_clusterIndexCount";
        buffers.clusterIndexCount = device.createBufferSRV(
            tools::hash(
                reinterpret_cast<const uint8_t*>(key.c_str()),
                static_cast<unsigned int>(key.length())),
            BufferDescription()
            .usage(ResourceUsage::CpuToGpu)
            .format(Format::Format_R32_UINT)
            .name("SubMesh Cluster Index Count")
            .setInitialData(BufferDescription::InitialData(source.clusterIndexCount)));

        key = path + "_clusterBounds";
        buffers.clusterBounds = device.createBufferSRV(
            tools::hash(
                reinterpret_cast<const uint8_t*>(key.c_str()),
                static_cast<unsigned int>(key.length())),
            BufferDescription()
            .usage(ResourceUsage::CpuToGpu)
            .name("SubMesh ClusterBounds")
            .structured(true)
            .setInitialData(BufferDescription::InitialData(source.clusterBounds)));

        return buffers;
    }
}

