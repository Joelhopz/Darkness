#include "TemporalResolve.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> TemporalResolvePS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/temporal/TemporalResolve.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/temporal/TemporalResolve.ps.support");
        }

        TemporalResolvePS::TemporalResolvePS()
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

        std::vector<TextureSRV> TemporalResolvePS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(currentFrame);
            
            result.emplace_back(history);
            
            result.emplace_back(depth);
            
            result.emplace_back(motion);
            
            return result;
        }

        std::vector<TextureUAV> TemporalResolvePS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> TemporalResolvePS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> TemporalResolvePS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> TemporalResolvePS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> TemporalResolvePS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> TemporalResolvePS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> TemporalResolvePS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& TemporalResolvePS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> TemporalResolvePS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(pointSampler);
            
            result.emplace_back(bilinearSampler);
            
            result.emplace_back(trilinearSampler);
            
            result.emplace_back(anisotropicSampler);
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool TemporalResolvePS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("currentFrame")) return true;
            
            if(name == std::string("history")) return true;
            
            if(name == std::string("depth")) return true;
            
            if(name == std::string("motion")) return true;
            
            
            return false;
        }

        bool TemporalResolvePS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolvePS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolvePS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolvePS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolvePS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolvePS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolvePS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& TemporalResolvePS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("currentFrame")) return currentFrame;
            
            if(name == std::string("history")) return history;
            
            if(name == std::string("depth")) return depth;
            
            if(name == std::string("motion")) return motion;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& TemporalResolvePS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& TemporalResolvePS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& TemporalResolvePS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& TemporalResolvePS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("pointSampler")) return pointSampler;
            
            if(name == std::string("bilinearSampler")) return bilinearSampler;
            
            if(name == std::string("trilinearSampler")) return trilinearSampler;
            
            if(name == std::string("anisotropicSampler")) return anisotropicSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& TemporalResolvePS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& TemporalResolvePS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& TemporalResolvePS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& TemporalResolvePS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t TemporalResolvePS::descriptorCount() const
        {
            return 5;
        }
    }
}