#include "Outline.vs.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> OutlineVS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/outline/Outline.vs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/outline/Outline.vs.support");
        }

        OutlineVS::OutlineVS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> OutlineVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> OutlineVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> OutlineVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(vertices);
            
            result.emplace_back(normals);
            
            return result;
        }

        std::vector<BufferUAV> OutlineVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> OutlineVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> OutlineVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> OutlineVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> OutlineVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& OutlineVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> OutlineVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool OutlineVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool OutlineVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool OutlineVS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("vertices")) return true;
            
            if (name == std::string("normals")) return true;
            
            
            return false;
        }

        bool OutlineVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool OutlineVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool OutlineVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool OutlineVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool OutlineVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& OutlineVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& OutlineVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& OutlineVS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("vertices")) return vertices;
            
            if(name == std::string("normals")) return normals;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& OutlineVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& OutlineVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& OutlineVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& OutlineVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& OutlineVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& OutlineVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t OutlineVS::descriptorCount() const
        {
            return 2;
        }
    }
}