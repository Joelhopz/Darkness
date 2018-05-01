#include "BloomExtractBright.ps.h"
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
        std::shared_ptr<const ShaderBinary> BloomExtractBrightPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/postprocess/BloomExtractBright.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/postprocess/BloomExtractBright.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        BloomExtractBrightPS::BloomExtractBrightPS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const BloomExtractConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const BloomExtractConstants*>(this)) + sizeof(BloomExtractConstants)),
                    nullptr,
                    "BloomExtractConstants"
                }
                
            
            
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
        BloomExtractBrightPS::BloomExtractBrightPS(const BloomExtractBrightPS& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const BloomExtractConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const BloomExtractConstants*>(this)) + sizeof(BloomExtractConstants)),
                    nullptr,
                    "BloomExtractConstants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            framebuffer = cl.framebuffer;
            

            

            

            

            

            

            

            

            
            framebufferSampler = cl.framebufferSampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        BloomExtractBrightPS::BloomExtractBrightPS(BloomExtractBrightPS&& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const BloomExtractConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const BloomExtractConstants*>(this)) + sizeof(BloomExtractConstants)),
                    nullptr,
                    "BloomExtractConstants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            framebuffer = std::move(cl.framebuffer);
            

            

            

            

            

            

            

            

            
            framebufferSampler = std::move(cl.framebufferSampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        BloomExtractBrightPS& BloomExtractBrightPS::operator=(const BloomExtractBrightPS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            framebuffer = cl.framebuffer;
            

            

            

            

            

            

            

            

            
            framebufferSampler = cl.framebufferSampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        BloomExtractBrightPS& BloomExtractBrightPS::operator=(BloomExtractBrightPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            framebuffer = std::move(cl.framebuffer);
            

            

            

            

            

            

            

            

            
            framebufferSampler = std::move(cl.framebufferSampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> BloomExtractBrightPS::textureSrvNames() const
        {
            return {
                
                "framebuffer"
                
                
            };
        }

        std::vector<std::string> BloomExtractBrightPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> BloomExtractBrightPS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> BloomExtractBrightPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> BloomExtractBrightPS::samplerNames() const
        {
            return {
                
                "framebufferSampler"
                
                
            };
        }

        std::vector<std::string> BloomExtractBrightPS::srvNames() const
        {
            return {
                
                "framebuffer"
                
                
            };
        }

        std::vector<std::string> BloomExtractBrightPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension BloomExtractBrightPS::textureDimension(const std::string& name) const
        {
            
            if("framebuffer" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> BloomExtractBrightPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(framebuffer);
            
            return result;
        }

        std::vector<TextureUAV> BloomExtractBrightPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> BloomExtractBrightPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> BloomExtractBrightPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> BloomExtractBrightPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> BloomExtractBrightPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> BloomExtractBrightPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> BloomExtractBrightPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& BloomExtractBrightPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> BloomExtractBrightPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(framebufferSampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& BloomExtractBrightPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool BloomExtractBrightPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("framebuffer")) return true;
            
            
            return false;
        }

        bool BloomExtractBrightPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BloomExtractBrightPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& BloomExtractBrightPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("framebuffer")) return framebuffer;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& BloomExtractBrightPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& BloomExtractBrightPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& BloomExtractBrightPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void BloomExtractBrightPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("framebuffer")) { framebuffer = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void BloomExtractBrightPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void BloomExtractBrightPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void BloomExtractBrightPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& BloomExtractBrightPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("framebufferSampler")) return framebufferSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& BloomExtractBrightPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& BloomExtractBrightPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& BloomExtractBrightPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& BloomExtractBrightPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t BloomExtractBrightPS::descriptorCount() const
        {
            return 2;
        }
    }
}