#include "DrawWithPixelShaderResources.vs.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> DrawWithPixelShaderResourcesVS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tests/DrawWithPixelShaderResources.vs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tests/DrawWithPixelShaderResources.vs.support");
        }

        DrawWithPixelShaderResourcesVS::DrawWithPixelShaderResourcesVS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> DrawWithPixelShaderResourcesVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> DrawWithPixelShaderResourcesVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> DrawWithPixelShaderResourcesVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> DrawWithPixelShaderResourcesVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> DrawWithPixelShaderResourcesVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> DrawWithPixelShaderResourcesVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> DrawWithPixelShaderResourcesVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> DrawWithPixelShaderResourcesVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& DrawWithPixelShaderResourcesVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> DrawWithPixelShaderResourcesVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool DrawWithPixelShaderResourcesVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithPixelShaderResourcesVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithPixelShaderResourcesVS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithPixelShaderResourcesVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithPixelShaderResourcesVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithPixelShaderResourcesVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithPixelShaderResourcesVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithPixelShaderResourcesVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& DrawWithPixelShaderResourcesVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& DrawWithPixelShaderResourcesVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& DrawWithPixelShaderResourcesVS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& DrawWithPixelShaderResourcesVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& DrawWithPixelShaderResourcesVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& DrawWithPixelShaderResourcesVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& DrawWithPixelShaderResourcesVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& DrawWithPixelShaderResourcesVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& DrawWithPixelShaderResourcesVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t DrawWithPixelShaderResourcesVS::descriptorCount() const
        {
            return 0;
        }
    }
}