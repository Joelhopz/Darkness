#include "TemporalResolve.vs.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> TemporalResolveVS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/temporal/TemporalResolve.vs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/temporal/TemporalResolve.vs.support");
        }

        TemporalResolveVS::TemporalResolveVS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> TemporalResolveVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> TemporalResolveVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> TemporalResolveVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> TemporalResolveVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> TemporalResolveVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> TemporalResolveVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> TemporalResolveVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> TemporalResolveVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& TemporalResolveVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> TemporalResolveVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool TemporalResolveVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolveVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolveVS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolveVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolveVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolveVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolveVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolveVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& TemporalResolveVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& TemporalResolveVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& TemporalResolveVS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& TemporalResolveVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& TemporalResolveVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& TemporalResolveVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& TemporalResolveVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& TemporalResolveVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& TemporalResolveVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t TemporalResolveVS::descriptorCount() const
        {
            return 0;
        }
    }
}