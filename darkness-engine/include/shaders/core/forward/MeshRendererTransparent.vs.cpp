#include "MeshRendererTransparent.vs.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> MeshRendererTransparentVS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/forward/MeshRendererTransparent.vs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/forward/MeshRendererTransparent.vs.support");
        }

        MeshRendererTransparentVS::MeshRendererTransparentVS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)) + sizeof(ConstData)),
                    nullptr,
                    "ConstData"
                }
                
            
            
            }
        {}

        std::vector<TextureSRV> MeshRendererTransparentVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> MeshRendererTransparentVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> MeshRendererTransparentVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(vertices);
            
            result.emplace_back(normals);
            
            result.emplace_back(tangents);
            
            result.emplace_back(uv);
            
            return result;
        }

        std::vector<BufferUAV> MeshRendererTransparentVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> MeshRendererTransparentVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> MeshRendererTransparentVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> MeshRendererTransparentVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> MeshRendererTransparentVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& MeshRendererTransparentVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> MeshRendererTransparentVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool MeshRendererTransparentVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererTransparentVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererTransparentVS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("vertices")) return true;
            
            if (name == std::string("normals")) return true;
            
            if (name == std::string("tangents")) return true;
            
            if (name == std::string("uv")) return true;
            
            
            return false;
        }

        bool MeshRendererTransparentVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererTransparentVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererTransparentVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererTransparentVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererTransparentVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& MeshRendererTransparentVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& MeshRendererTransparentVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& MeshRendererTransparentVS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("vertices")) return vertices;
            
            if(name == std::string("normals")) return normals;
            
            if(name == std::string("tangents")) return tangents;
            
            if(name == std::string("uv")) return uv;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& MeshRendererTransparentVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& MeshRendererTransparentVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& MeshRendererTransparentVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& MeshRendererTransparentVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& MeshRendererTransparentVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& MeshRendererTransparentVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t MeshRendererTransparentVS::descriptorCount() const
        {
            return 5;
        }
    }
}