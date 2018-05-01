#include "SSAO.ps.h"
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
        std::shared_ptr<const ShaderBinary> SSAOPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/ssao/SSAO.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/ssao/SSAO.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        SSAOPS::SSAOPS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)) + sizeof(Constants)),
                    nullptr,
                    "Constants"
                }
                
            
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"position", "SV_Position0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"viewRay", "TEXCOORD0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"uv", "TEXCOORD1", "float2"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        SSAOPS::SSAOPS(const SSAOPS& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)) + sizeof(Constants)),
                    nullptr,
                    "Constants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            depthTexture = cl.depthTexture;
            
            normalTexture = cl.normalTexture;
            
            noiseTexture = cl.noiseTexture;
            

            

            
            samples = cl.samples;
            

            

            

            

            

            

            
            ssaoSampler = cl.ssaoSampler;
            
            depthSampler = cl.depthSampler;
            
            defaultSampler = cl.defaultSampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        SSAOPS::SSAOPS(SSAOPS&& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)) + sizeof(Constants)),
                    nullptr,
                    "Constants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            depthTexture = std::move(cl.depthTexture);
            
            normalTexture = std::move(cl.normalTexture);
            
            noiseTexture = std::move(cl.noiseTexture);
            

            

            
            samples = std::move(cl.samples);
            

            

            

            

            

            

            
            ssaoSampler = std::move(cl.ssaoSampler);
            
            depthSampler = std::move(cl.depthSampler);
            
            defaultSampler = std::move(cl.defaultSampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        SSAOPS& SSAOPS::operator=(const SSAOPS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            depthTexture = cl.depthTexture;
            
            normalTexture = cl.normalTexture;
            
            noiseTexture = cl.noiseTexture;
            

            

            
            samples = cl.samples;
            

            

            

            

            

            

            
            ssaoSampler = cl.ssaoSampler;
            
            depthSampler = cl.depthSampler;
            
            defaultSampler = cl.defaultSampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        SSAOPS& SSAOPS::operator=(SSAOPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            depthTexture = std::move(cl.depthTexture);
            
            normalTexture = std::move(cl.normalTexture);
            
            noiseTexture = std::move(cl.noiseTexture);
            

            

            
            samples = std::move(cl.samples);
            

            

            

            

            

            

            
            ssaoSampler = std::move(cl.ssaoSampler);
            
            depthSampler = std::move(cl.depthSampler);
            
            defaultSampler = std::move(cl.defaultSampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> SSAOPS::textureSrvNames() const
        {
            return {
                
                "depthTexture"
                
                ,
                
                
                "normalTexture"
                
                ,
                
                
                "noiseTexture"
                
                
            };
        }

        std::vector<std::string> SSAOPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> SSAOPS::bufferSrvNames() const
        {
            return {
                
                "samples"
                
                
            };
        }

        std::vector<std::string> SSAOPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> SSAOPS::samplerNames() const
        {
            return {
                
                "ssaoSampler"
                
                ,
                
                
                "depthSampler"
                
                ,
                
                
                "defaultSampler"
                
                
            };
        }

        std::vector<std::string> SSAOPS::srvNames() const
        {
            return {
                
                "depthTexture"
                
                ,
                
                
                "normalTexture"
                
                ,
                
                
                "noiseTexture"
                
                ,
                
                
                "samples"
                
                
            };
        }

        std::vector<std::string> SSAOPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension SSAOPS::textureDimension(const std::string& name) const
        {
            
            if("depthTexture" == name) return engine::ResourceDimension::Texture2D;
            
            if("normalTexture" == name) return engine::ResourceDimension::Texture2D;
            
            if("noiseTexture" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> SSAOPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(depthTexture);
            
            result.emplace_back(normalTexture);
            
            result.emplace_back(noiseTexture);
            
            return result;
        }

        std::vector<TextureUAV> SSAOPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> SSAOPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(samples);
            
            return result;
        }

        std::vector<BufferUAV> SSAOPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> SSAOPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> SSAOPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> SSAOPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> SSAOPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& SSAOPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> SSAOPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(ssaoSampler);
            
            result.emplace_back(depthSampler);
            
            result.emplace_back(defaultSampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& SSAOPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool SSAOPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("depthTexture")) return true;
            
            if(name == std::string("normalTexture")) return true;
            
            if(name == std::string("noiseTexture")) return true;
            
            
            return false;
        }

        bool SSAOPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOPS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("samples")) return true;
            
            
            return false;
        }

        bool SSAOPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& SSAOPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("depthTexture")) return depthTexture;
            
            if(name == std::string("normalTexture")) return normalTexture;
            
            if(name == std::string("noiseTexture")) return noiseTexture;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& SSAOPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& SSAOPS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("samples")) return samples;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& SSAOPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void SSAOPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("depthTexture")) { depthTexture = texture; return; }
            
            if(name == std::string("normalTexture")) { normalTexture = texture; return; }
            
            if(name == std::string("noiseTexture")) { noiseTexture = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void SSAOPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void SSAOPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("samples")) { samples = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void SSAOPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& SSAOPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("ssaoSampler")) return ssaoSampler;
            
            if(name == std::string("depthSampler")) return depthSampler;
            
            if(name == std::string("defaultSampler")) return defaultSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& SSAOPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& SSAOPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& SSAOPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& SSAOPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t SSAOPS::descriptorCount() const
        {
            return 5;
        }
    }
}