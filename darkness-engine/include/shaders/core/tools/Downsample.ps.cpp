#include "Downsample.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> DownsamplePS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/Downsample.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/Downsample.ps.support");
        }

        DownsamplePS::DownsamplePS()
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

        std::vector<TextureSRV> DownsamplePS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(image);
            
            return result;
        }

        std::vector<TextureUAV> DownsamplePS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> DownsamplePS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> DownsamplePS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> DownsamplePS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> DownsamplePS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> DownsamplePS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> DownsamplePS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& DownsamplePS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> DownsamplePS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(imageSampler);
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool DownsamplePS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("image")) return true;
            
            
            return false;
        }

        bool DownsamplePS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DownsamplePS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DownsamplePS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool DownsamplePS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DownsamplePS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DownsamplePS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DownsamplePS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& DownsamplePS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("image")) return image;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& DownsamplePS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& DownsamplePS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& DownsamplePS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& DownsamplePS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("imageSampler")) return imageSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& DownsamplePS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& DownsamplePS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& DownsamplePS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& DownsamplePS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t DownsamplePS::descriptorCount() const
        {
            return 2;
        }
    }
}