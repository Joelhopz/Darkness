#include "EquirectToCubemap.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> EquirectToCubemapPS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/EquirectToCubemap.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/EquirectToCubemap.ps.support");
        }

        EquirectToCubemapPS::EquirectToCubemapPS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> EquirectToCubemapPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(equirectangularMap);
            
            return result;
        }

        std::vector<TextureUAV> EquirectToCubemapPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> EquirectToCubemapPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> EquirectToCubemapPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> EquirectToCubemapPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> EquirectToCubemapPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> EquirectToCubemapPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> EquirectToCubemapPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& EquirectToCubemapPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> EquirectToCubemapPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(equirectangularSampler);
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool EquirectToCubemapPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("equirectangularMap")) return true;
            
            
            return false;
        }

        bool EquirectToCubemapPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& EquirectToCubemapPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("equirectangularMap")) return equirectangularMap;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& EquirectToCubemapPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& EquirectToCubemapPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& EquirectToCubemapPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& EquirectToCubemapPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("equirectangularSampler")) return equirectangularSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& EquirectToCubemapPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& EquirectToCubemapPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& EquirectToCubemapPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& EquirectToCubemapPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t EquirectToCubemapPS::descriptorCount() const
        {
            return 1;
        }
    }
}