#include "EquirectToCubemap.vs.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> EquirectToCubemapVS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/EquirectToCubemap.vs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/EquirectToCubemap.vs.support");
        }

        EquirectToCubemapVS::EquirectToCubemapVS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)) + sizeof(ConstData)),
                    nullptr,
                    "ConstData"
                }
                
            
            
            }
        {}

        std::vector<TextureSRV> EquirectToCubemapVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> EquirectToCubemapVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> EquirectToCubemapVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> EquirectToCubemapVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> EquirectToCubemapVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> EquirectToCubemapVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> EquirectToCubemapVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> EquirectToCubemapVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& EquirectToCubemapVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> EquirectToCubemapVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool EquirectToCubemapVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapVS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& EquirectToCubemapVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& EquirectToCubemapVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& EquirectToCubemapVS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& EquirectToCubemapVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& EquirectToCubemapVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& EquirectToCubemapVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& EquirectToCubemapVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& EquirectToCubemapVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& EquirectToCubemapVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t EquirectToCubemapVS::descriptorCount() const
        {
            return 1;
        }
    }
}