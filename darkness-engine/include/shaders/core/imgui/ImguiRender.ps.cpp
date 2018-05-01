#include "ImguiRender.ps.h"
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
        std::shared_ptr<const ShaderBinary> ImguiRenderPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/imgui/ImguiRender.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/imgui/ImguiRender.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        ImguiRenderPS::ImguiRenderPS()
            : m_constantRange{
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"pos", "SV_Position", "float4"}
            
            ,
            
            
            ShaderInputParameter{"color", "COLOR0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"uv", "TEXCOORD0", "float2"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        ImguiRenderPS::ImguiRenderPS(const ImguiRenderPS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            tex = cl.tex;
            

            

            

            

            

            

            

            

            
            pointSampler = cl.pointSampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ImguiRenderPS::ImguiRenderPS(ImguiRenderPS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            tex = std::move(cl.tex);
            

            

            

            

            

            

            

            

            
            pointSampler = std::move(cl.pointSampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ImguiRenderPS& ImguiRenderPS::operator=(const ImguiRenderPS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            tex = cl.tex;
            

            

            

            

            

            

            

            

            
            pointSampler = cl.pointSampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ImguiRenderPS& ImguiRenderPS::operator=(ImguiRenderPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            tex = std::move(cl.tex);
            

            

            

            

            

            

            

            

            
            pointSampler = std::move(cl.pointSampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> ImguiRenderPS::textureSrvNames() const
        {
            return {
                
                "tex"
                
                
            };
        }

        std::vector<std::string> ImguiRenderPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ImguiRenderPS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ImguiRenderPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ImguiRenderPS::samplerNames() const
        {
            return {
                
                "pointSampler"
                
                
            };
        }

        std::vector<std::string> ImguiRenderPS::srvNames() const
        {
            return {
                
                "tex"
                
                
            };
        }

        std::vector<std::string> ImguiRenderPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension ImguiRenderPS::textureDimension(const std::string& name) const
        {
            
            if("tex" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> ImguiRenderPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(tex);
            
            return result;
        }

        std::vector<TextureUAV> ImguiRenderPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> ImguiRenderPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> ImguiRenderPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> ImguiRenderPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> ImguiRenderPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> ImguiRenderPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> ImguiRenderPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& ImguiRenderPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> ImguiRenderPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(pointSampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& ImguiRenderPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool ImguiRenderPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("tex")) return true;
            
            
            return false;
        }

        bool ImguiRenderPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ImguiRenderPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ImguiRenderPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool ImguiRenderPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ImguiRenderPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ImguiRenderPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ImguiRenderPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& ImguiRenderPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("tex")) return tex;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& ImguiRenderPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& ImguiRenderPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& ImguiRenderPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void ImguiRenderPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("tex")) { tex = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ImguiRenderPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ImguiRenderPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ImguiRenderPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& ImguiRenderPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("pointSampler")) return pointSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& ImguiRenderPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& ImguiRenderPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& ImguiRenderPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& ImguiRenderPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t ImguiRenderPS::descriptorCount() const
        {
            return 1;
        }
    }
}