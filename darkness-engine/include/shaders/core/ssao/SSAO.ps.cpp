#include "SSAO.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> SSAOPS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/ssao/SSAO.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/ssao/SSAO.ps.support");
        }

        SSAOPS::SSAOPS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)) + sizeof(Constants)),
                    nullptr,
                    "Constants"
                }
                
            
            
            }
        {}

        std::vector<TextureSRV> SSAOPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(depthTexture);
            
            result.emplace_back(normalTexture);
            
            result.emplace_back(noiseTexture);
            
            return result;
        }

        std::vector<TextureUAV> SSAOPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> SSAOPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(samples);
            
            return result;
        }

        std::vector<BufferUAV> SSAOPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> SSAOPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> SSAOPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> SSAOPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> SSAOPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& SSAOPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> SSAOPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(ssaoSampler);
            
            result.emplace_back(depthSampler);
            
            result.emplace_back(defaultSampler);
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool SSAOPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("depthTexture")) return true;
            
            if(name == std::string("normalTexture")) return true;
            
            if(name == std::string("noiseTexture")) return true;
            
            
            return false;
        }

        bool SSAOPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOPS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("samples")) return true;
            
            
            return false;
        }

        bool SSAOPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& SSAOPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("depthTexture")) return depthTexture;
            
            if(name == std::string("normalTexture")) return normalTexture;
            
            if(name == std::string("noiseTexture")) return noiseTexture;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& SSAOPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& SSAOPS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("samples")) return samples;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& SSAOPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& SSAOPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("ssaoSampler")) return ssaoSampler;
            
            if(name == std::string("depthSampler")) return depthSampler;
            
            if(name == std::string("defaultSampler")) return defaultSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& SSAOPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& SSAOPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& SSAOPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& SSAOPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t SSAOPS::descriptorCount() const
        {
            return 5;
        }
    }
}