#include "DebugViewer.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> DebugViewerPS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/DebugViewer.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tools/DebugViewer.ps.support");
        }

        DebugViewerPS::DebugViewerPS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> DebugViewerPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(tex);
            
            return result;
        }

        std::vector<TextureUAV> DebugViewerPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> DebugViewerPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> DebugViewerPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> DebugViewerPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> DebugViewerPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> DebugViewerPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> DebugViewerPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& DebugViewerPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> DebugViewerPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(tex_sampler);
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool DebugViewerPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("tex")) return true;
            
            
            return false;
        }

        bool DebugViewerPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DebugViewerPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DebugViewerPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool DebugViewerPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DebugViewerPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DebugViewerPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DebugViewerPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& DebugViewerPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("tex")) return tex;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& DebugViewerPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& DebugViewerPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& DebugViewerPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& DebugViewerPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("tex_sampler")) return tex_sampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& DebugViewerPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& DebugViewerPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& DebugViewerPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& DebugViewerPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t DebugViewerPS::descriptorCount() const
        {
            return 1;
        }
    }
}