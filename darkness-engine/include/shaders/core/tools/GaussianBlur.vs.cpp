#include "GaussianBlur.vs.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> GaussianBlurVS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/GaussianBlur.vs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/GaussianBlur.vs.support");
        }

        GaussianBlurVS::GaussianBlurVS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> GaussianBlurVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> GaussianBlurVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> GaussianBlurVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> GaussianBlurVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> GaussianBlurVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> GaussianBlurVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> GaussianBlurVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> GaussianBlurVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& GaussianBlurVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> GaussianBlurVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool GaussianBlurVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool GaussianBlurVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool GaussianBlurVS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool GaussianBlurVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool GaussianBlurVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool GaussianBlurVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool GaussianBlurVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool GaussianBlurVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& GaussianBlurVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& GaussianBlurVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& GaussianBlurVS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& GaussianBlurVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& GaussianBlurVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& GaussianBlurVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& GaussianBlurVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& GaussianBlurVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& GaussianBlurVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t GaussianBlurVS::descriptorCount() const
        {
            return 0;
        }
    }
}