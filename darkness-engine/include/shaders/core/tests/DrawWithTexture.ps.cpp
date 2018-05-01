#include "DrawWithTexture.ps.h"
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
        std::shared_ptr<const ShaderBinary> DrawWithTexturePS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tests/DrawWithTexture.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tests/DrawWithTexture.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        DrawWithTexturePS::DrawWithTexturePS()
            : m_constantRange{
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"position", "SV_Position", "float4"}
            
            ,
            
            
            ShaderInputParameter{"uv", "TEXCOORD0", "float4"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        DrawWithTexturePS::DrawWithTexturePS(const DrawWithTexturePS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            tex = cl.tex;
            

            

            

            

            

            

            

            

            
            tex_sampler = cl.tex_sampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DrawWithTexturePS::DrawWithTexturePS(DrawWithTexturePS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            tex = std::move(cl.tex);
            

            

            

            

            

            

            

            

            
            tex_sampler = std::move(cl.tex_sampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DrawWithTexturePS& DrawWithTexturePS::operator=(const DrawWithTexturePS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            tex = cl.tex;
            

            

            

            

            

            

            

            

            
            tex_sampler = cl.tex_sampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DrawWithTexturePS& DrawWithTexturePS::operator=(DrawWithTexturePS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            tex = std::move(cl.tex);
            

            

            

            

            

            

            

            

            
            tex_sampler = std::move(cl.tex_sampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> DrawWithTexturePS::textureSrvNames() const
        {
            return {
                
                "tex"
                
                
            };
        }

        std::vector<std::string> DrawWithTexturePS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DrawWithTexturePS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DrawWithTexturePS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DrawWithTexturePS::samplerNames() const
        {
            return {
                
                "tex_sampler"
                
                
            };
        }

        std::vector<std::string> DrawWithTexturePS::srvNames() const
        {
            return {
                
                "tex"
                
                
            };
        }

        std::vector<std::string> DrawWithTexturePS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension DrawWithTexturePS::textureDimension(const std::string& name) const
        {
            
            if("tex" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> DrawWithTexturePS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(tex);
            
            return result;
        }

        std::vector<TextureUAV> DrawWithTexturePS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> DrawWithTexturePS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> DrawWithTexturePS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> DrawWithTexturePS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> DrawWithTexturePS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> DrawWithTexturePS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> DrawWithTexturePS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& DrawWithTexturePS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> DrawWithTexturePS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(tex_sampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& DrawWithTexturePS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool DrawWithTexturePS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("tex")) return true;
            
            
            return false;
        }

        bool DrawWithTexturePS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithTexturePS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithTexturePS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithTexturePS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithTexturePS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithTexturePS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithTexturePS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& DrawWithTexturePS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("tex")) return tex;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& DrawWithTexturePS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& DrawWithTexturePS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& DrawWithTexturePS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void DrawWithTexturePS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("tex")) { tex = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DrawWithTexturePS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DrawWithTexturePS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DrawWithTexturePS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& DrawWithTexturePS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("tex_sampler")) return tex_sampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& DrawWithTexturePS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& DrawWithTexturePS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& DrawWithTexturePS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& DrawWithTexturePS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t DrawWithTexturePS::descriptorCount() const
        {
            return 1;
        }
    }
}