#include "ImguiRender.vs.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> ImguiRenderVS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/imgui/ImguiRender.vs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/imgui/ImguiRender.vs.support");
        }

        ImguiRenderVS::ImguiRenderVS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const constants_struct*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const constants_struct*>(this)) + sizeof(constants_struct)),
                    nullptr,
                    "constants_struct"
                }
                
            
            
            }
        {}

        std::vector<TextureSRV> ImguiRenderVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> ImguiRenderVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> ImguiRenderVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> ImguiRenderVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> ImguiRenderVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> ImguiRenderVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> ImguiRenderVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> ImguiRenderVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& ImguiRenderVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> ImguiRenderVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool ImguiRenderVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ImguiRenderVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ImguiRenderVS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ImguiRenderVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool ImguiRenderVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ImguiRenderVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ImguiRenderVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ImguiRenderVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& ImguiRenderVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& ImguiRenderVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& ImguiRenderVS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& ImguiRenderVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& ImguiRenderVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& ImguiRenderVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& ImguiRenderVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& ImguiRenderVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& ImguiRenderVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t ImguiRenderVS::descriptorCount() const
        {
            return 1;
        }
    }
}