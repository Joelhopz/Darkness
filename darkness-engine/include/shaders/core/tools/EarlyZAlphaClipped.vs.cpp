#include "EarlyZAlphaClipped.vs.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> EarlyZAlphaClippedVS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/EarlyZAlphaClipped.vs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/EarlyZAlphaClipped.vs.support");
        }

        EarlyZAlphaClippedVS::EarlyZAlphaClippedVS()
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

        std::vector<TextureSRV> EarlyZAlphaClippedVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> EarlyZAlphaClippedVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> EarlyZAlphaClippedVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(vertices);
            
            result.emplace_back(uv);
            
            return result;
        }

        std::vector<BufferUAV> EarlyZAlphaClippedVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> EarlyZAlphaClippedVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> EarlyZAlphaClippedVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> EarlyZAlphaClippedVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> EarlyZAlphaClippedVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& EarlyZAlphaClippedVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> EarlyZAlphaClippedVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool EarlyZAlphaClippedVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedVS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("vertices")) return true;
            
            if (name == std::string("uv")) return true;
            
            
            return false;
        }

        bool EarlyZAlphaClippedVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& EarlyZAlphaClippedVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& EarlyZAlphaClippedVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& EarlyZAlphaClippedVS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("vertices")) return vertices;
            
            if(name == std::string("uv")) return uv;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& EarlyZAlphaClippedVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& EarlyZAlphaClippedVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& EarlyZAlphaClippedVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& EarlyZAlphaClippedVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& EarlyZAlphaClippedVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& EarlyZAlphaClippedVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t EarlyZAlphaClippedVS::descriptorCount() const
        {
            return 3;
        }
    }
}