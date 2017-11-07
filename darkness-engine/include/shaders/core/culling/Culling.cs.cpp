#include "Culling.cs.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> CullingCS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/culling/Culling.cs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/culling/Culling.cs.support");
        }

        CullingCS::CullingCS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const CullingConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const CullingConstants*>(this)) + sizeof(CullingConstants)),
                    nullptr,
                    "CullingConstants"
                }
                
            
            
            }
        {}

        std::vector<TextureSRV> CullingCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> CullingCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> CullingCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(clusterId);
            
            result.emplace_back(clusterIndexCount);
            
            result.emplace_back(clusterBounds);
            
            return result;
        }

        std::vector<BufferUAV> CullingCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            result.emplace_back(clusterIndexCountDraw);
            
            return result;
        }

        std::vector<TextureBindlessSRV> CullingCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            result.emplace_back(textures);
            
            return result;
        }

        std::vector<TextureBindlessUAV> CullingCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> CullingCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> CullingCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& CullingCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> CullingCS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool CullingCS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CullingCS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CullingCS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("clusterId")) return true;
            
            if (name == std::string("clusterIndexCount")) return true;
            
            if (name == std::string("clusterBounds")) return true;
            
            
            return false;
        }

        bool CullingCS::hasBufferUav(const std::string& name) const
        {
            
            
            if (name == std::string("clusterIndexCountDraw")) return true;
            
            
            return false;
        }

        bool CullingCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            
            if (name == std::string("textures")) return true;
            
            
            return false;
        }

        bool CullingCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CullingCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CullingCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& CullingCS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& CullingCS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& CullingCS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("clusterId")) return clusterId;
            
            if(name == std::string("clusterIndexCount")) return clusterIndexCount;
            
            if(name == std::string("clusterBounds")) return clusterBounds;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& CullingCS::bufferUav(const std::string& name) const
        {
            
            
            if(name == std::string("clusterIndexCountDraw")) return clusterIndexCountDraw;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& CullingCS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& CullingCS::bindlessTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("textures")) return textures;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& CullingCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& CullingCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& CullingCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t CullingCS::descriptorCount() const
        {
            return 6;
        }
    }
}