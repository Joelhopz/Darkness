#include "SSAOBlur.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> SSAOBlurPS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/ssao/SSAOBlur.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/ssao/SSAOBlur.ps.support");
        }

        SSAOBlurPS::SSAOBlurPS()
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

        std::vector<TextureSRV> SSAOBlurPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(image);
            
            return result;
        }

        std::vector<TextureUAV> SSAOBlurPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> SSAOBlurPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(blurKernel);
            
            return result;
        }

        std::vector<BufferUAV> SSAOBlurPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> SSAOBlurPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> SSAOBlurPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> SSAOBlurPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> SSAOBlurPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& SSAOBlurPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> SSAOBlurPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(imageSampler);
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool SSAOBlurPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("image")) return true;
            
            
            return false;
        }

        bool SSAOBlurPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurPS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("blurKernel")) return true;
            
            
            return false;
        }

        bool SSAOBlurPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& SSAOBlurPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("image")) return image;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& SSAOBlurPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& SSAOBlurPS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("blurKernel")) return blurKernel;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& SSAOBlurPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& SSAOBlurPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("imageSampler")) return imageSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& SSAOBlurPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& SSAOBlurPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& SSAOBlurPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& SSAOBlurPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t SSAOBlurPS::descriptorCount() const
        {
            return 3;
        }
    }
}