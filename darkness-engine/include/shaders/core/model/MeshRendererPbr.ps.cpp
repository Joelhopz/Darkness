#include "MeshRendererPbr.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> MeshRendererPbrPS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/model/MeshRendererPbr.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/model/MeshRendererPbr.ps.support");
        }

        MeshRendererPbrPS::MeshRendererPbrPS()
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

        std::vector<TextureSRV> MeshRendererPbrPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> MeshRendererPbrPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> MeshRendererPbrPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> MeshRendererPbrPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            result.emplace_back(pick);
            
            return result;
        }

        std::vector<TextureBindlessSRV> MeshRendererPbrPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            result.emplace_back(materialTextures);
            
            return result;
        }

        std::vector<TextureBindlessUAV> MeshRendererPbrPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> MeshRendererPbrPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> MeshRendererPbrPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& MeshRendererPbrPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> MeshRendererPbrPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(tex_sampler);
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool MeshRendererPbrPS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererPbrPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererPbrPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererPbrPS::hasBufferUav(const std::string& name) const
        {
            
            
            if (name == std::string("pick")) return true;
            
            
            return false;
        }

        bool MeshRendererPbrPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            
            if (name == std::string("materialTextures")) return true;
            
            
            return false;
        }

        bool MeshRendererPbrPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererPbrPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererPbrPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& MeshRendererPbrPS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& MeshRendererPbrPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& MeshRendererPbrPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& MeshRendererPbrPS::bufferUav(const std::string& name) const
        {
            
            
            if(name == std::string("pick")) return pick;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& MeshRendererPbrPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("tex_sampler")) return tex_sampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& MeshRendererPbrPS::bindlessTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("materialTextures")) return materialTextures;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& MeshRendererPbrPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& MeshRendererPbrPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& MeshRendererPbrPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t MeshRendererPbrPS::descriptorCount() const
        {
            return 3;
        }
    }
}