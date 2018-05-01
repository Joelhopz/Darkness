#include "CopyTexture.ps.h"
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
        std::shared_ptr<const ShaderBinary> CopyTexturePS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/CopyTexture.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/CopyTexture.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        CopyTexturePS::CopyTexturePS()
            : m_constantRange{
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"position", "SV_Position0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"uv", "TEXCOORD0", "float2"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        CopyTexturePS::CopyTexturePS(const CopyTexturePS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            src = cl.src;
            

            

            

            

            

            

            

            

            
            samp = cl.samp;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CopyTexturePS::CopyTexturePS(CopyTexturePS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            src = std::move(cl.src);
            

            

            

            

            

            

            

            

            
            samp = std::move(cl.samp);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CopyTexturePS& CopyTexturePS::operator=(const CopyTexturePS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            src = cl.src;
            

            

            

            

            

            

            

            

            
            samp = cl.samp;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CopyTexturePS& CopyTexturePS::operator=(CopyTexturePS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            src = std::move(cl.src);
            

            

            

            

            

            

            

            

            
            samp = std::move(cl.samp);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> CopyTexturePS::textureSrvNames() const
        {
            return {
                
                "src"
                
                
            };
        }

        std::vector<std::string> CopyTexturePS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CopyTexturePS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CopyTexturePS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CopyTexturePS::samplerNames() const
        {
            return {
                
                "samp"
                
                
            };
        }

        std::vector<std::string> CopyTexturePS::srvNames() const
        {
            return {
                
                "src"
                
                
            };
        }

        std::vector<std::string> CopyTexturePS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension CopyTexturePS::textureDimension(const std::string& name) const
        {
            
            if("src" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> CopyTexturePS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(src);
            
            return result;
        }

        std::vector<TextureUAV> CopyTexturePS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> CopyTexturePS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> CopyTexturePS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> CopyTexturePS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> CopyTexturePS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> CopyTexturePS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> CopyTexturePS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& CopyTexturePS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> CopyTexturePS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(samp);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& CopyTexturePS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool CopyTexturePS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("src")) return true;
            
            
            return false;
        }

        bool CopyTexturePS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CopyTexturePS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CopyTexturePS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool CopyTexturePS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CopyTexturePS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CopyTexturePS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CopyTexturePS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& CopyTexturePS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("src")) return src;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& CopyTexturePS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& CopyTexturePS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& CopyTexturePS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void CopyTexturePS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("src")) { src = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CopyTexturePS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CopyTexturePS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CopyTexturePS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& CopyTexturePS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("samp")) return samp;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& CopyTexturePS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& CopyTexturePS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& CopyTexturePS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& CopyTexturePS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t CopyTexturePS::descriptorCount() const
        {
            return 1;
        }
    }
}