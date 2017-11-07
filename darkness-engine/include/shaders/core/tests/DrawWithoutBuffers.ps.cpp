#include "DrawWithoutBuffers.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> DrawWithoutBuffersPS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tests/DrawWithoutBuffers.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tests/DrawWithoutBuffers.ps.support");
        }

        DrawWithoutBuffersPS::DrawWithoutBuffersPS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> DrawWithoutBuffersPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> DrawWithoutBuffersPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> DrawWithoutBuffersPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> DrawWithoutBuffersPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> DrawWithoutBuffersPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> DrawWithoutBuffersPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> DrawWithoutBuffersPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> DrawWithoutBuffersPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& DrawWithoutBuffersPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> DrawWithoutBuffersPS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool DrawWithoutBuffersPS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithoutBuffersPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithoutBuffersPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithoutBuffersPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithoutBuffersPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithoutBuffersPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithoutBuffersPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithoutBuffersPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& DrawWithoutBuffersPS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& DrawWithoutBuffersPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& DrawWithoutBuffersPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& DrawWithoutBuffersPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& DrawWithoutBuffersPS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& DrawWithoutBuffersPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& DrawWithoutBuffersPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& DrawWithoutBuffersPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& DrawWithoutBuffersPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t DrawWithoutBuffersPS::descriptorCount() const
        {
            return 0;
        }
    }
}