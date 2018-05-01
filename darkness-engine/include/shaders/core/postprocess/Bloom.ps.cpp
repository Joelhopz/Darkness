#include "Bloom.ps.h"
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
        std::shared_ptr<const ShaderBinary> BloomPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/postprocess/Bloom.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/postprocess/Bloom.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        BloomPS::BloomPS()
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
        BloomPS::BloomPS(const BloomPS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            framebuffer = cl.framebuffer;
            
            blur = cl.blur;
            

            

            

            

            

            

            

            

            
            framebufferSampler = cl.framebufferSampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        BloomPS::BloomPS(BloomPS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            framebuffer = std::move(cl.framebuffer);
            
            blur = std::move(cl.blur);
            

            

            

            

            

            

            

            

            
            framebufferSampler = std::move(cl.framebufferSampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        BloomPS& BloomPS::operator=(const BloomPS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            framebuffer = cl.framebuffer;
            
            blur = cl.blur;
            

            

            

            

            

            

            

            

            
            framebufferSampler = cl.framebufferSampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        BloomPS& BloomPS::operator=(BloomPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            framebuffer = std::move(cl.framebuffer);
            
            blur = std::move(cl.blur);
            

            

            

            

            

            

            

            

            
            framebufferSampler = std::move(cl.framebufferSampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> BloomPS::textureSrvNames() const
        {
            return {
                
                "framebuffer"
                
                ,
                
                
                "blur"
                
                
            };
        }

        std::vector<std::string> BloomPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> BloomPS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> BloomPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> BloomPS::samplerNames() const
        {
            return {
                
                "framebufferSampler"
                
                
            };
        }

        std::vector<std::string> BloomPS::srvNames() const
        {
            return {
                
                "framebuffer"
                
                ,
                
                
                "blur"
                
                
            };
        }

        std::vector<std::string> BloomPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension BloomPS::textureDimension(const std::string& name) const
        {
            
            if("framebuffer" == name) return engine::ResourceDimension::Texture2D;
            
            if("blur" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> BloomPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(framebuffer);
            
            result.emplace_back(blur);
            
            return result;
        }

        std::vector<TextureUAV> BloomPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> BloomPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> BloomPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> BloomPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> BloomPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> BloomPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> BloomPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& BloomPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> BloomPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(framebufferSampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& BloomPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool BloomPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("framebuffer")) return true;
            
            if(name == std::string("blur")) return true;
            
            
            return false;
        }

        bool BloomPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool BloomPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BloomPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool BloomPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BloomPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool BloomPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BloomPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& BloomPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("framebuffer")) return framebuffer;
            
            if(name == std::string("blur")) return blur;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& BloomPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& BloomPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& BloomPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void BloomPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("framebuffer")) { framebuffer = texture; return; }
            
            if(name == std::string("blur")) { blur = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void BloomPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void BloomPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void BloomPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& BloomPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("framebufferSampler")) return framebufferSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& BloomPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& BloomPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& BloomPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& BloomPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t BloomPS::descriptorCount() const
        {
            return 2;
        }
    }
}