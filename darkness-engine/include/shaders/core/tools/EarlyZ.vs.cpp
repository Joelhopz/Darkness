#include "EarlyZ.vs.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> EarlyZVS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/EarlyZ.vs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/EarlyZ.vs.support");
        }

        EarlyZVS::EarlyZVS()
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

        std::vector<TextureSRV> EarlyZVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> EarlyZVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> EarlyZVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(vertices);
            
            return result;
        }

        std::vector<BufferUAV> EarlyZVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> EarlyZVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> EarlyZVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> EarlyZVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> EarlyZVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& EarlyZVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> EarlyZVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool EarlyZVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZVS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("vertices")) return true;
            
            
            return false;
        }

        bool EarlyZVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& EarlyZVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& EarlyZVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& EarlyZVS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("vertices")) return vertices;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& EarlyZVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& EarlyZVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& EarlyZVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& EarlyZVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& EarlyZVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& EarlyZVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t EarlyZVS::descriptorCount() const
        {
            return 2;
        }
    }
}