#include "ShadowRender.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> ShadowRenderPS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/shadows/ShadowRender.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/shadows/ShadowRender.ps.support");
        }

        ShadowRenderPS::ShadowRenderPS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> ShadowRenderPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> ShadowRenderPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> ShadowRenderPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> ShadowRenderPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> ShadowRenderPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> ShadowRenderPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> ShadowRenderPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> ShadowRenderPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& ShadowRenderPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> ShadowRenderPS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool ShadowRenderPS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& ShadowRenderPS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& ShadowRenderPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& ShadowRenderPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& ShadowRenderPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& ShadowRenderPS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& ShadowRenderPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& ShadowRenderPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& ShadowRenderPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& ShadowRenderPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t ShadowRenderPS::descriptorCount() const
        {
            return 0;
        }
    }
}