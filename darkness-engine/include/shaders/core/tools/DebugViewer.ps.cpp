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
#pragma warning( push )
#pragma warning( disable : 4702 )
        std::shared_ptr<const ShaderBinary> DebugViewerPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/DebugViewer.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/DebugViewer.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        DebugViewerPS::DebugViewerPS()
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
        DebugViewerPS::DebugViewerPS(const DebugViewerPS& cl)
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
        DebugViewerPS::DebugViewerPS(DebugViewerPS&& cl)
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
        DebugViewerPS& DebugViewerPS::operator=(const DebugViewerPS& cl)
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
        DebugViewerPS& DebugViewerPS::operator=(DebugViewerPS&& cl)
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

        std::vector<std::string> DebugViewerPS::textureSrvNames() const
        {
            return {
                
                "tex"
                
                
            };
        }

        std::vector<std::string> DebugViewerPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DebugViewerPS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DebugViewerPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DebugViewerPS::samplerNames() const
        {
            return {
                
                "tex_sampler"
                
                
            };
        }

        std::vector<std::string> DebugViewerPS::srvNames() const
        {
            return {
                
                "tex"
                
                
            };
        }

        std::vector<std::string> DebugViewerPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension DebugViewerPS::textureDimension(const std::string& name) const
        {
            
            if("tex" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

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

        const std::vector<ShaderInputParameter>& DebugViewerPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

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

        void DebugViewerPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("tex")) { tex = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DebugViewerPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DebugViewerPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DebugViewerPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
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