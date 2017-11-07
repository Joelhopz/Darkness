#include "HLSLTest.vs.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> HLSLTestVS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tests/HLSLTest.vs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tests/HLSLTest.vs.support");
        }

        HLSLTestVS::HLSLTestVS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> HLSLTestVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> HLSLTestVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> HLSLTestVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(position);
            
            result.emplace_back(color);
            
            return result;
        }

        std::vector<BufferUAV> HLSLTestVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> HLSLTestVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> HLSLTestVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> HLSLTestVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> HLSLTestVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& HLSLTestVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> HLSLTestVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool HLSLTestVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool HLSLTestVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool HLSLTestVS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("position")) return true;
            
            if (name == std::string("color")) return true;
            
            
            return false;
        }

        bool HLSLTestVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool HLSLTestVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool HLSLTestVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool HLSLTestVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool HLSLTestVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& HLSLTestVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& HLSLTestVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& HLSLTestVS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("position")) return position;
            
            if(name == std::string("color")) return color;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& HLSLTestVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& HLSLTestVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& HLSLTestVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& HLSLTestVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& HLSLTestVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& HLSLTestVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t HLSLTestVS::descriptorCount() const
        {
            return 2;
        }
    }
}