#include "ShadowRenderTransparent.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> ShadowRenderTransparentPS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/shadows/ShadowRenderTransparent.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/shadows/ShadowRenderTransparent.ps.support");
        }

        ShadowRenderTransparentPS::ShadowRenderTransparentPS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> ShadowRenderTransparentPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(transparencyMap);
            
            return result;
        }

        std::vector<TextureUAV> ShadowRenderTransparentPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> ShadowRenderTransparentPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> ShadowRenderTransparentPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> ShadowRenderTransparentPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> ShadowRenderTransparentPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> ShadowRenderTransparentPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> ShadowRenderTransparentPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& ShadowRenderTransparentPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> ShadowRenderTransparentPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(transparencySampler);
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool ShadowRenderTransparentPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("transparencyMap")) return true;
            
            
            return false;
        }

        bool ShadowRenderTransparentPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderTransparentPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderTransparentPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderTransparentPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderTransparentPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderTransparentPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderTransparentPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& ShadowRenderTransparentPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("transparencyMap")) return transparencyMap;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& ShadowRenderTransparentPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& ShadowRenderTransparentPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& ShadowRenderTransparentPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& ShadowRenderTransparentPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("transparencySampler")) return transparencySampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& ShadowRenderTransparentPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& ShadowRenderTransparentPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& ShadowRenderTransparentPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& ShadowRenderTransparentPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t ShadowRenderTransparentPS::descriptorCount() const
        {
            return 1;
        }
    }
}