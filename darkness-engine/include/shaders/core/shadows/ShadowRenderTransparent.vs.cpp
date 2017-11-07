#include "ShadowRenderTransparent.vs.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> ShadowRenderTransparentVS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/shadows/ShadowRenderTransparent.vs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/shadows/ShadowRenderTransparent.vs.support");
        }

        ShadowRenderTransparentVS::ShadowRenderTransparentVS()
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

        std::vector<TextureSRV> ShadowRenderTransparentVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> ShadowRenderTransparentVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> ShadowRenderTransparentVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(vertices);
            
            result.emplace_back(uv);
            
            return result;
        }

        std::vector<BufferUAV> ShadowRenderTransparentVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> ShadowRenderTransparentVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> ShadowRenderTransparentVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> ShadowRenderTransparentVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> ShadowRenderTransparentVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& ShadowRenderTransparentVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> ShadowRenderTransparentVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool ShadowRenderTransparentVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderTransparentVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderTransparentVS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("vertices")) return true;
            
            if (name == std::string("uv")) return true;
            
            
            return false;
        }

        bool ShadowRenderTransparentVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderTransparentVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderTransparentVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderTransparentVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ShadowRenderTransparentVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& ShadowRenderTransparentVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& ShadowRenderTransparentVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& ShadowRenderTransparentVS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("vertices")) return vertices;
            
            if(name == std::string("uv")) return uv;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& ShadowRenderTransparentVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& ShadowRenderTransparentVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& ShadowRenderTransparentVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& ShadowRenderTransparentVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& ShadowRenderTransparentVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& ShadowRenderTransparentVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t ShadowRenderTransparentVS::descriptorCount() const
        {
            return 3;
        }
    }
}