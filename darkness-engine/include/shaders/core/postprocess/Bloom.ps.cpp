#include "Bloom.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> BloomPS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/postprocess/Bloom.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/postprocess/Bloom.ps.support");
        }

        BloomPS::BloomPS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> BloomPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(framebuffer);
            
            result.emplace_back(blur);
            
            return result;
        }

        std::vector<TextureUAV> BloomPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> BloomPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> BloomPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> BloomPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> BloomPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> BloomPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> BloomPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& BloomPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> BloomPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(framebufferSampler);
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool BloomPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("framebuffer")) return true;
            
            if(name == std::string("blur")) return true;
            
            
            return false;
        }

        bool BloomPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool BloomPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BloomPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool BloomPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BloomPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool BloomPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BloomPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& BloomPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("framebuffer")) return framebuffer;
            
            if(name == std::string("blur")) return blur;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& BloomPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& BloomPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& BloomPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& BloomPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("framebufferSampler")) return framebufferSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& BloomPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& BloomPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& BloomPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& BloomPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t BloomPS::descriptorCount() const
        {
            return 2;
        }
    }
}