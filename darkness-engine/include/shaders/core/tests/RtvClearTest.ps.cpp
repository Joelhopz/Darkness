#include "RtvClearTest.ps.h"
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
        std::shared_ptr<const ShaderBinary> RtvClearTestPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tests/RtvClearTest.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tests/RtvClearTest.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        RtvClearTestPS::RtvClearTestPS()
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
        RtvClearTestPS::RtvClearTestPS(const RtvClearTestPS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            sourceLod = cl.sourceLod;
            

            

            

            

            

            

            

            

            
            samp = cl.samp;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        RtvClearTestPS::RtvClearTestPS(RtvClearTestPS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            sourceLod = std::move(cl.sourceLod);
            

            

            

            

            

            

            

            

            
            samp = std::move(cl.samp);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        RtvClearTestPS& RtvClearTestPS::operator=(const RtvClearTestPS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            sourceLod = cl.sourceLod;
            

            

            

            

            

            

            

            

            
            samp = cl.samp;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        RtvClearTestPS& RtvClearTestPS::operator=(RtvClearTestPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            sourceLod = std::move(cl.sourceLod);
            

            

            

            

            

            

            

            

            
            samp = std::move(cl.samp);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> RtvClearTestPS::textureSrvNames() const
        {
            return {
                
                "sourceLod"
                
                
            };
        }

        std::vector<std::string> RtvClearTestPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> RtvClearTestPS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> RtvClearTestPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> RtvClearTestPS::samplerNames() const
        {
            return {
                
                "samp"
                
                
            };
        }

        std::vector<std::string> RtvClearTestPS::srvNames() const
        {
            return {
                
                "sourceLod"
                
                
            };
        }

        std::vector<std::string> RtvClearTestPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension RtvClearTestPS::textureDimension(const std::string& name) const
        {
            
            if("sourceLod" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> RtvClearTestPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(sourceLod);
            
            return result;
        }

        std::vector<TextureUAV> RtvClearTestPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> RtvClearTestPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> RtvClearTestPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> RtvClearTestPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> RtvClearTestPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> RtvClearTestPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> RtvClearTestPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& RtvClearTestPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> RtvClearTestPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(samp);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& RtvClearTestPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool RtvClearTestPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("sourceLod")) return true;
            
            
            return false;
        }

        bool RtvClearTestPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool RtvClearTestPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool RtvClearTestPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool RtvClearTestPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool RtvClearTestPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool RtvClearTestPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool RtvClearTestPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& RtvClearTestPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("sourceLod")) return sourceLod;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& RtvClearTestPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& RtvClearTestPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& RtvClearTestPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void RtvClearTestPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("sourceLod")) { sourceLod = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void RtvClearTestPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void RtvClearTestPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void RtvClearTestPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& RtvClearTestPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("samp")) return samp;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& RtvClearTestPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& RtvClearTestPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& RtvClearTestPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& RtvClearTestPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t RtvClearTestPS::descriptorCount() const
        {
            return 1;
        }
    }
}