#include "BloomExtractBright.vs.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> BloomExtractBrightVS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/postprocess/BloomExtractBright.vs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/postprocess/BloomExtractBright.vs.support");
        }

        BloomExtractBrightVS::BloomExtractBrightVS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> BloomExtractBrightVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> BloomExtractBrightVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> BloomExtractBrightVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> BloomExtractBrightVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> BloomExtractBrightVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> BloomExtractBrightVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> BloomExtractBrightVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> BloomExtractBrightVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& BloomExtractBrightVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> BloomExtractBrightVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool BloomExtractBrightVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightVS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& BloomExtractBrightVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& BloomExtractBrightVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& BloomExtractBrightVS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& BloomExtractBrightVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& BloomExtractBrightVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& BloomExtractBrightVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& BloomExtractBrightVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& BloomExtractBrightVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& BloomExtractBrightVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t BloomExtractBrightVS::descriptorCount() const
        {
            return 0;
        }
    }
}