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
#pragma warning( push )
#pragma warning( disable : 4702 )
        std::shared_ptr<const ShaderBinary> ImguiRenderVS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/imgui/ImguiRender.vs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/imgui/ImguiRender.vs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

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
            , m_inputParameters
            {
            
            ShaderInputParameter{"pos", "POSITION0", "float2"}
            
            ,
            
            
            ShaderInputParameter{"uv", "TEXCOORD0", "float2"}
            
            ,
            
            
            ShaderInputParameter{"color", "COLOR0", "uint"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        ImguiRenderVS::ImguiRenderVS(const ImguiRenderVS& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const constants_struct*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const constants_struct*>(this)) + sizeof(constants_struct)),
                    nullptr,
                    "constants_struct"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ImguiRenderVS::ImguiRenderVS(ImguiRenderVS&& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const constants_struct*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const constants_struct*>(this)) + sizeof(constants_struct)),
                    nullptr,
                    "constants_struct"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ImguiRenderVS& ImguiRenderVS::operator=(const ImguiRenderVS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ImguiRenderVS& ImguiRenderVS::operator=(ImguiRenderVS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> ImguiRenderVS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ImguiRenderVS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ImguiRenderVS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ImguiRenderVS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ImguiRenderVS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ImguiRenderVS::srvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ImguiRenderVS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension ImguiRenderVS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

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

        const std::vector<ShaderInputParameter>& ImguiRenderVS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

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

        void ImguiRenderVS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ImguiRenderVS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ImguiRenderVS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ImguiRenderVS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
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