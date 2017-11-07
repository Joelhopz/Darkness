#include "SSAOBlur.vs.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> SSAOBlurVS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/ssao/SSAOBlur.vs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/ssao/SSAOBlur.vs.support");
        }

        SSAOBlurVS::SSAOBlurVS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> SSAOBlurVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> SSAOBlurVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> SSAOBlurVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> SSAOBlurVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> SSAOBlurVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> SSAOBlurVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> SSAOBlurVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> SSAOBlurVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& SSAOBlurVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> SSAOBlurVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool SSAOBlurVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurVS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& SSAOBlurVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& SSAOBlurVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& SSAOBlurVS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& SSAOBlurVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& SSAOBlurVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& SSAOBlurVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& SSAOBlurVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& SSAOBlurVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& SSAOBlurVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t SSAOBlurVS::descriptorCount() const
        {
            return 0;
        }
    }
}