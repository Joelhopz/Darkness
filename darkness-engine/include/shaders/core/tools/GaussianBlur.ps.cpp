#include "GaussianBlur.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> GaussianBlurPS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/GaussianBlur.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/GaussianBlur.ps.support");
        }

        GaussianBlurPS::GaussianBlurPS()
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

        std::vector<TextureSRV> GaussianBlurPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(image);
            
            return result;
        }

        std::vector<TextureUAV> GaussianBlurPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> GaussianBlurPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> GaussianBlurPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> GaussianBlurPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> GaussianBlurPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> GaussianBlurPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> GaussianBlurPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& GaussianBlurPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> GaussianBlurPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(imageSampler);
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool GaussianBlurPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("image")) return true;
            
            
            return false;
        }

        bool GaussianBlurPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool GaussianBlurPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool GaussianBlurPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool GaussianBlurPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool GaussianBlurPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool GaussianBlurPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool GaussianBlurPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& GaussianBlurPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("image")) return image;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& GaussianBlurPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& GaussianBlurPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& GaussianBlurPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& GaussianBlurPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("imageSampler")) return imageSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& GaussianBlurPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& GaussianBlurPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& GaussianBlurPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& GaussianBlurPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t GaussianBlurPS::descriptorCount() const
        {
            return 2;
        }
    }
}