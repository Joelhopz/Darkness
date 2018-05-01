#include "Particle.ps.h"
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
        std::shared_ptr<const ShaderBinary> ParticlePS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/particle/Particle.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/particle/Particle.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        ParticlePS::ParticlePS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)) + sizeof(ConstData)),
                    nullptr,
                    "ConstData"
                }
                
            
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"position", "SV_Position0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"normal", "NORMAL0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"tangent", "NORMAL1", "float4"}
            
            ,
            
            
            ShaderInputParameter{"uv", "TEXCOORD0", "float2"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        ParticlePS::ParticlePS(const ParticlePS& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)) + sizeof(ConstData)),
                    nullptr,
                    "ConstData"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            environmentIrradianceCubemap = cl.environmentIrradianceCubemap;
            
            environmentIrradiance = cl.environmentIrradiance;
            
            environmentSpecular = cl.environmentSpecular;
            
            environmentBrdfLut = cl.environmentBrdfLut;
            
            shadowMap = cl.shadowMap;
            
            depth = cl.depth;
            

            

            
            lightWorldPosition = cl.lightWorldPosition;
            
            lightDirection = cl.lightDirection;
            
            lightColor = cl.lightColor;
            
            lightParameters = cl.lightParameters;
            
            lightType = cl.lightType;
            
            lightIntensity = cl.lightIntensity;
            
            lightRange = cl.lightRange;
            
            shadowVP = cl.shadowVP;
            

            

            
            diffuse = cl.diffuse;
            
            normal = cl.normal;
            
            alpha = cl.alpha;
            
            top = cl.top;
            
            left = cl.left;
            
            bottom = cl.bottom;
            
            right = cl.right;
            

            

            

            

            
            particleSampler = cl.particleSampler;
            
            triSampler = cl.triSampler;
            
            shadowSampler = cl.shadowSampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ParticlePS::ParticlePS(ParticlePS&& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)) + sizeof(ConstData)),
                    nullptr,
                    "ConstData"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            environmentIrradianceCubemap = std::move(cl.environmentIrradianceCubemap);
            
            environmentIrradiance = std::move(cl.environmentIrradiance);
            
            environmentSpecular = std::move(cl.environmentSpecular);
            
            environmentBrdfLut = std::move(cl.environmentBrdfLut);
            
            shadowMap = std::move(cl.shadowMap);
            
            depth = std::move(cl.depth);
            

            

            
            lightWorldPosition = std::move(cl.lightWorldPosition);
            
            lightDirection = std::move(cl.lightDirection);
            
            lightColor = std::move(cl.lightColor);
            
            lightParameters = std::move(cl.lightParameters);
            
            lightType = std::move(cl.lightType);
            
            lightIntensity = std::move(cl.lightIntensity);
            
            lightRange = std::move(cl.lightRange);
            
            shadowVP = std::move(cl.shadowVP);
            

            

            
            diffuse = std::move(cl.diffuse);
            
            normal = std::move(cl.normal);
            
            alpha = std::move(cl.alpha);
            
            top = std::move(cl.top);
            
            left = std::move(cl.left);
            
            bottom = std::move(cl.bottom);
            
            right = std::move(cl.right);
            

            

            

            

            
            particleSampler = std::move(cl.particleSampler);
            
            triSampler = std::move(cl.triSampler);
            
            shadowSampler = std::move(cl.shadowSampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ParticlePS& ParticlePS::operator=(const ParticlePS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            environmentIrradianceCubemap = cl.environmentIrradianceCubemap;
            
            environmentIrradiance = cl.environmentIrradiance;
            
            environmentSpecular = cl.environmentSpecular;
            
            environmentBrdfLut = cl.environmentBrdfLut;
            
            shadowMap = cl.shadowMap;
            
            depth = cl.depth;
            

            

            
            lightWorldPosition = cl.lightWorldPosition;
            
            lightDirection = cl.lightDirection;
            
            lightColor = cl.lightColor;
            
            lightParameters = cl.lightParameters;
            
            lightType = cl.lightType;
            
            lightIntensity = cl.lightIntensity;
            
            lightRange = cl.lightRange;
            
            shadowVP = cl.shadowVP;
            

            

            
            diffuse = cl.diffuse;
            
            normal = cl.normal;
            
            alpha = cl.alpha;
            
            top = cl.top;
            
            left = cl.left;
            
            bottom = cl.bottom;
            
            right = cl.right;
            

            

            

            

            
            particleSampler = cl.particleSampler;
            
            triSampler = cl.triSampler;
            
            shadowSampler = cl.shadowSampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ParticlePS& ParticlePS::operator=(ParticlePS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            environmentIrradianceCubemap = std::move(cl.environmentIrradianceCubemap);
            
            environmentIrradiance = std::move(cl.environmentIrradiance);
            
            environmentSpecular = std::move(cl.environmentSpecular);
            
            environmentBrdfLut = std::move(cl.environmentBrdfLut);
            
            shadowMap = std::move(cl.shadowMap);
            
            depth = std::move(cl.depth);
            

            

            
            lightWorldPosition = std::move(cl.lightWorldPosition);
            
            lightDirection = std::move(cl.lightDirection);
            
            lightColor = std::move(cl.lightColor);
            
            lightParameters = std::move(cl.lightParameters);
            
            lightType = std::move(cl.lightType);
            
            lightIntensity = std::move(cl.lightIntensity);
            
            lightRange = std::move(cl.lightRange);
            
            shadowVP = std::move(cl.shadowVP);
            

            

            
            diffuse = std::move(cl.diffuse);
            
            normal = std::move(cl.normal);
            
            alpha = std::move(cl.alpha);
            
            top = std::move(cl.top);
            
            left = std::move(cl.left);
            
            bottom = std::move(cl.bottom);
            
            right = std::move(cl.right);
            

            

            

            

            
            particleSampler = std::move(cl.particleSampler);
            
            triSampler = std::move(cl.triSampler);
            
            shadowSampler = std::move(cl.shadowSampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> ParticlePS::textureSrvNames() const
        {
            return {
                
                "environmentIrradianceCubemap"
                
                ,
                
                
                "environmentIrradiance"
                
                ,
                
                
                "environmentSpecular"
                
                ,
                
                
                "environmentBrdfLut"
                
                ,
                
                
                "shadowMap"
                
                ,
                
                
                "depth"
                
                
            };
        }

        std::vector<std::string> ParticlePS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ParticlePS::bufferSrvNames() const
        {
            return {
                
                "lightWorldPosition"
                
                ,
                
                
                "lightDirection"
                
                ,
                
                
                "lightColor"
                
                ,
                
                
                "lightParameters"
                
                ,
                
                
                "lightType"
                
                ,
                
                
                "lightIntensity"
                
                ,
                
                
                "lightRange"
                
                ,
                
                
                "shadowVP"
                
                
            };
        }

        std::vector<std::string> ParticlePS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ParticlePS::samplerNames() const
        {
            return {
                
                "particleSampler"
                
                ,
                
                
                "triSampler"
                
                ,
                
                
                "shadowSampler"
                
                
            };
        }

        std::vector<std::string> ParticlePS::srvNames() const
        {
            return {
                
                "environmentIrradianceCubemap"
                
                ,
                
                
                "environmentIrradiance"
                
                ,
                
                
                "environmentSpecular"
                
                ,
                
                
                "environmentBrdfLut"
                
                ,
                
                
                "lightWorldPosition"
                
                ,
                
                
                "lightDirection"
                
                ,
                
                
                "lightColor"
                
                ,
                
                
                "lightParameters"
                
                ,
                
                
                "lightType"
                
                ,
                
                
                "lightIntensity"
                
                ,
                
                
                "lightRange"
                
                ,
                
                
                "shadowMap"
                
                ,
                
                
                "shadowVP"
                
                ,
                
                
                "depth"
                
                ,
                
                
                "diffuse"
                
                ,
                
                
                "normal"
                
                ,
                
                
                "alpha"
                
                ,
                
                
                "top"
                
                ,
                
                
                "left"
                
                ,
                
                
                "bottom"
                
                ,
                
                
                "right"
                
                
            };
        }

        std::vector<std::string> ParticlePS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension ParticlePS::textureDimension(const std::string& name) const
        {
            
            if("environmentIrradianceCubemap" == name) return engine::ResourceDimension::TextureCubemap;
            
            if("environmentIrradiance" == name) return engine::ResourceDimension::Texture2D;
            
            if("environmentSpecular" == name) return engine::ResourceDimension::TextureCubemap;
            
            if("environmentBrdfLut" == name) return engine::ResourceDimension::Texture2D;
            
            if("shadowMap" == name) return engine::ResourceDimension::Texture2DArray;
            
            if("depth" == name) return engine::ResourceDimension::Texture2D;
            
            if("diffuse" == name) return engine::ResourceDimension::Texture2D;
            
            if("normal" == name) return engine::ResourceDimension::Texture2D;
            
            if("alpha" == name) return engine::ResourceDimension::Texture2D;
            
            if("top" == name) return engine::ResourceDimension::Texture2D;
            
            if("left" == name) return engine::ResourceDimension::Texture2D;
            
            if("bottom" == name) return engine::ResourceDimension::Texture2D;
            
            if("right" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> ParticlePS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(environmentIrradianceCubemap);
            
            result.emplace_back(environmentIrradiance);
            
            result.emplace_back(environmentSpecular);
            
            result.emplace_back(environmentBrdfLut);
            
            result.emplace_back(shadowMap);
            
            result.emplace_back(depth);
            
            return result;
        }

        std::vector<TextureUAV> ParticlePS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> ParticlePS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(lightWorldPosition);
            
            result.emplace_back(lightDirection);
            
            result.emplace_back(lightColor);
            
            result.emplace_back(lightParameters);
            
            result.emplace_back(lightType);
            
            result.emplace_back(lightIntensity);
            
            result.emplace_back(lightRange);
            
            result.emplace_back(shadowVP);
            
            return result;
        }

        std::vector<BufferUAV> ParticlePS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> ParticlePS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            result.emplace_back(diffuse);
            
            result.emplace_back(normal);
            
            result.emplace_back(alpha);
            
            result.emplace_back(top);
            
            result.emplace_back(left);
            
            result.emplace_back(bottom);
            
            result.emplace_back(right);
            
            return result;
        }

        std::vector<TextureBindlessUAV> ParticlePS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> ParticlePS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> ParticlePS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& ParticlePS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> ParticlePS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(particleSampler);
            
            result.emplace_back(triSampler);
            
            result.emplace_back(shadowSampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& ParticlePS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool ParticlePS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("environmentIrradianceCubemap")) return true;
            
            if(name == std::string("environmentIrradiance")) return true;
            
            if(name == std::string("environmentSpecular")) return true;
            
            if(name == std::string("environmentBrdfLut")) return true;
            
            if(name == std::string("shadowMap")) return true;
            
            if(name == std::string("depth")) return true;
            
            
            return false;
        }

        bool ParticlePS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ParticlePS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("lightWorldPosition")) return true;
            
            if (name == std::string("lightDirection")) return true;
            
            if (name == std::string("lightColor")) return true;
            
            if (name == std::string("lightParameters")) return true;
            
            if (name == std::string("lightType")) return true;
            
            if (name == std::string("lightIntensity")) return true;
            
            if (name == std::string("lightRange")) return true;
            
            if (name == std::string("shadowVP")) return true;
            
            
            return false;
        }

        bool ParticlePS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool ParticlePS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            
            if (name == std::string("diffuse")) return true;
            
            if (name == std::string("normal")) return true;
            
            if (name == std::string("alpha")) return true;
            
            if (name == std::string("top")) return true;
            
            if (name == std::string("left")) return true;
            
            if (name == std::string("bottom")) return true;
            
            if (name == std::string("right")) return true;
            
            
            return false;
        }

        bool ParticlePS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ParticlePS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ParticlePS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& ParticlePS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("environmentIrradianceCubemap")) return environmentIrradianceCubemap;
            
            if(name == std::string("environmentIrradiance")) return environmentIrradiance;
            
            if(name == std::string("environmentSpecular")) return environmentSpecular;
            
            if(name == std::string("environmentBrdfLut")) return environmentBrdfLut;
            
            if(name == std::string("shadowMap")) return shadowMap;
            
            if(name == std::string("depth")) return depth;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& ParticlePS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& ParticlePS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("lightWorldPosition")) return lightWorldPosition;
            
            if(name == std::string("lightDirection")) return lightDirection;
            
            if(name == std::string("lightColor")) return lightColor;
            
            if(name == std::string("lightParameters")) return lightParameters;
            
            if(name == std::string("lightType")) return lightType;
            
            if(name == std::string("lightIntensity")) return lightIntensity;
            
            if(name == std::string("lightRange")) return lightRange;
            
            if(name == std::string("shadowVP")) return shadowVP;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& ParticlePS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void ParticlePS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("environmentIrradianceCubemap")) { environmentIrradianceCubemap = texture; return; }
            
            if(name == std::string("environmentIrradiance")) { environmentIrradiance = texture; return; }
            
            if(name == std::string("environmentSpecular")) { environmentSpecular = texture; return; }
            
            if(name == std::string("environmentBrdfLut")) { environmentBrdfLut = texture; return; }
            
            if(name == std::string("shadowMap")) { shadowMap = texture; return; }
            
            if(name == std::string("depth")) { depth = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ParticlePS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ParticlePS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("lightWorldPosition")) { lightWorldPosition = buffer; return; }
            
            if(name == std::string("lightDirection")) { lightDirection = buffer; return; }
            
            if(name == std::string("lightColor")) { lightColor = buffer; return; }
            
            if(name == std::string("lightParameters")) { lightParameters = buffer; return; }
            
            if(name == std::string("lightType")) { lightType = buffer; return; }
            
            if(name == std::string("lightIntensity")) { lightIntensity = buffer; return; }
            
            if(name == std::string("lightRange")) { lightRange = buffer; return; }
            
            if(name == std::string("shadowVP")) { shadowVP = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ParticlePS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& ParticlePS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("particleSampler")) return particleSampler;
            
            if(name == std::string("triSampler")) return triSampler;
            
            if(name == std::string("shadowSampler")) return shadowSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& ParticlePS::bindlessTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("diffuse")) return diffuse;
            
            if(name == std::string("normal")) return normal;
            
            if(name == std::string("alpha")) return alpha;
            
            if(name == std::string("top")) return top;
            
            if(name == std::string("left")) return left;
            
            if(name == std::string("bottom")) return bottom;
            
            if(name == std::string("right")) return right;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& ParticlePS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& ParticlePS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& ParticlePS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t ParticlePS::descriptorCount() const
        {
            return 22;
        }
    }
}