#include "BloomExtractBright.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> BloomExtractBrightPS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/postprocess/BloomExtractBright.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/postprocess/BloomExtractBright.ps.support");
        }

        BloomExtractBrightPS::BloomExtractBrightPS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> BloomExtractBrightPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(framebuffer);
            
            return result;
        }

        std::vector<TextureUAV> BloomExtractBrightPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> BloomExtractBrightPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> BloomExtractBrightPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> BloomExtractBrightPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> BloomExtractBrightPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> BloomExtractBrightPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> BloomExtractBrightPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& BloomExtractBrightPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> BloomExtractBrightPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(framebufferSampler);
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool BloomExtractBrightPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("framebuffer")) return true;
            
            
            return false;
        }

        bool BloomExtractBrightPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& BloomExtractBrightPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("framebuffer")) return framebuffer;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& BloomExtractBrightPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& BloomExtractBrightPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& BloomExtractBrightPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& BloomExtractBrightPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("framebufferSampler")) return framebufferSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& BloomExtractBrightPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& BloomExtractBrightPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& BloomExtractBrightPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& BloomExtractBrightPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t BloomExtractBrightPS::descriptorCount() const
        {
            return 1;
        }
    }
}