#include "Irradiance.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> IrradiancePS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/ibl/Irradiance.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/ibl/Irradiance.ps.support");
        }

        IrradiancePS::IrradiancePS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> IrradiancePS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(environmentMap);
            
            return result;
        }

        std::vector<TextureUAV> IrradiancePS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> IrradiancePS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> IrradiancePS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> IrradiancePS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> IrradiancePS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> IrradiancePS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> IrradiancePS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& IrradiancePS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> IrradiancePS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(environmentSampler);
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool IrradiancePS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("environmentMap")) return true;
            
            
            return false;
        }

        bool IrradiancePS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool IrradiancePS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool IrradiancePS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool IrradiancePS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool IrradiancePS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool IrradiancePS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool IrradiancePS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& IrradiancePS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("environmentMap")) return environmentMap;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& IrradiancePS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& IrradiancePS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& IrradiancePS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& IrradiancePS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("environmentSampler")) return environmentSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& IrradiancePS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& IrradiancePS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& IrradiancePS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& IrradiancePS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t IrradiancePS::descriptorCount() const
        {
            return 1;
        }
    }
}