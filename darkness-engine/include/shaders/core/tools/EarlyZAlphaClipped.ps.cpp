#include "EarlyZAlphaClipped.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> EarlyZAlphaClippedPS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/EarlyZAlphaClipped.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/EarlyZAlphaClipped.ps.support");
        }

        EarlyZAlphaClippedPS::EarlyZAlphaClippedPS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> EarlyZAlphaClippedPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(image);
            
            return result;
        }

        std::vector<TextureUAV> EarlyZAlphaClippedPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> EarlyZAlphaClippedPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> EarlyZAlphaClippedPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> EarlyZAlphaClippedPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> EarlyZAlphaClippedPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> EarlyZAlphaClippedPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> EarlyZAlphaClippedPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& EarlyZAlphaClippedPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> EarlyZAlphaClippedPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(imageSampler);
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool EarlyZAlphaClippedPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("image")) return true;
            
            
            return false;
        }

        bool EarlyZAlphaClippedPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& EarlyZAlphaClippedPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("image")) return image;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& EarlyZAlphaClippedPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& EarlyZAlphaClippedPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& EarlyZAlphaClippedPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& EarlyZAlphaClippedPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("imageSampler")) return imageSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& EarlyZAlphaClippedPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& EarlyZAlphaClippedPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& EarlyZAlphaClippedPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& EarlyZAlphaClippedPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t EarlyZAlphaClippedPS::descriptorCount() const
        {
            return 1;
        }
    }
}