#pragma once

#include "engine/graphics/Resources.h"
#include "engine/rendering/Mesh.h"
#include "tools/image/Image.h"
#include <functional>
#include <unordered_map>

#define RESOURCE_CACHE_TEXTURESRV
#define RESOURCE_CACHE_BUFFERSRV
#define RESOURCE_CACHE_BUFFERIBV
#define RESOURCE_CACHE_IMAGE
#define RESOURCE_CACHE_MESH

namespace engine
{
    using ResourceCreateTextureSRV = std::function<TextureSRV()>;
    using ResourceCreateBufferSRV = std::function<BufferSRV()>;
    using ResourceCreateBufferIBV = std::function<BufferIBV()>;
    class ResourceCache
    {
    public:
        TextureSRV createTextureSRV(ResourceKey key, ResourceCreateTextureSRV create);
        BufferSRV createBufferSRV(ResourceKey key, ResourceCreateBufferSRV create);
        BufferIBV createBufferIBV(ResourceKey key, ResourceCreateBufferIBV create);

        std::shared_ptr<image::ImageIf> createImage(
            ResourceKey key, 
            const std::string& filename,
            Format type = Format::Format_BC7_UNORM,
            int width = -1,
            int height = -1,
            int slices = -1,
            int mips = -1);

        std::shared_ptr<Mesh> createMesh(
            ResourceKey key,
            const std::string& filename);

        template<typename T>
        bool cachedDataExists(ResourceKey key) const;

        void clear();
    private:
        std::unordered_map<ResourceKey, TextureSRV> m_textureSRV;
        std::unordered_map<ResourceKey, BufferSRV> m_bufferSRV;
        std::unordered_map<ResourceKey, BufferIBV> m_bufferIBV;
        std::unordered_map<ResourceKey, std::shared_ptr<image::ImageIf>> m_images;
        std::unordered_map<ResourceKey, std::shared_ptr<Mesh>> m_meshes;
    };
}
