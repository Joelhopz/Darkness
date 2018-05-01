#include "Lighting.ps.h"
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
        std::shared_ptr<const ShaderBinary> LightingPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            if((true)
            
            && (drawmode == Drawmode::Full)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps_000.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps.support", 1-1, {
                    
                    "ENUM_DRAWMODE_FULL"
                    
                    });
            }
            
            if((true)
            
            && (drawmode == Drawmode::DebugClusters)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps_001.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps.support", 2-1, {
                    
                    "ENUM_DRAWMODE_DEBUG_CLUSTERS"
                    
                    });
            }
            
            if((true)
            
            && (drawmode == Drawmode::MipAlbedo)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps_002.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps.support", 3-1, {
                    
                    "ENUM_DRAWMODE_MIP_ALBEDO"
                    
                    });
            }
            
            if((true)
            
            && (drawmode == Drawmode::MipRoughness)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps_003.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps.support", 4-1, {
                    
                    "ENUM_DRAWMODE_MIP_ROUGHNESS"
                    
                    });
            }
            
            if((true)
            
            && (drawmode == Drawmode::MipMetalness)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps_004.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps.support", 5-1, {
                    
                    "ENUM_DRAWMODE_MIP_METALNESS"
                    
                    });
            }
            
            if((true)
            
            && (drawmode == Drawmode::MipAo)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps_005.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps.support", 6-1, {
                    
                    "ENUM_DRAWMODE_MIP_AO"
                    
                    });
            }
            
            if((true)
            
            && (drawmode == Drawmode::Albedo)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps_006.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps.support", 7-1, {
                    
                    "ENUM_DRAWMODE_ALBEDO"
                    
                    });
            }
            
            if((true)
            
            && (drawmode == Drawmode::Roughness)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps_007.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps.support", 8-1, {
                    
                    "ENUM_DRAWMODE_ROUGHNESS"
                    
                    });
            }
            
            if((true)
            
            && (drawmode == Drawmode::Metalness)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps_008.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps.support", 9-1, {
                    
                    "ENUM_DRAWMODE_METALNESS"
                    
                    });
            }
            
            if((true)
            
            && (drawmode == Drawmode::Occlusion)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps_009.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps.support", 10-1, {
                    
                    "ENUM_DRAWMODE_OCCLUSION"
                    
                    });
            }
            
            if((true)
            
            && (drawmode == Drawmode::Uv)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps_010.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps.support", 11-1, {
                    
                    "ENUM_DRAWMODE_UV"
                    
                    });
            }
            
            if((true)
            
            && (drawmode == Drawmode::DebugNormal)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps_011.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lighting/Lighting.ps.support", 12-1, {
                    
                    "ENUM_DRAWMODE_DEBUG_NORMAL"
                    
                    });
            }
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        LightingPS::LightingPS()
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
            
            
            ShaderInputParameter{"uv", "TEXCOORD0", "float2"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        LightingPS::LightingPS(const LightingPS& cl)
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
            
            gbufferNormals = cl.gbufferNormals;
            
            gbufferUV = cl.gbufferUV;
            
            gbufferInstanceId = cl.gbufferInstanceId;
            
            shadowMap = cl.shadowMap;
            
            ssao = cl.ssao;
            
            depth = cl.depth;
            

            

            
            instanceMaterials = cl.instanceMaterials;
            
            shadowVP = cl.shadowVP;
            
            lightWorldPosition = cl.lightWorldPosition;
            
            lightDirection = cl.lightDirection;
            
            lightColor = cl.lightColor;
            
            lightParameters = cl.lightParameters;
            
            lightType = cl.lightType;
            
            lightIntensity = cl.lightIntensity;
            
            lightRange = cl.lightRange;
            

            

            
            materialTextures = cl.materialTextures;
            

            

            

            

            
            tex_sampler = cl.tex_sampler;
            
            tri_sampler = cl.tri_sampler;
            
            depth_sampler = cl.depth_sampler;
            
            point_sampler = cl.point_sampler;
            
            shadow_sampler = cl.shadow_sampler;
            

            
            drawmode = cl.drawmode;
            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        LightingPS::LightingPS(LightingPS&& cl)
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
            
            gbufferNormals = std::move(cl.gbufferNormals);
            
            gbufferUV = std::move(cl.gbufferUV);
            
            gbufferInstanceId = std::move(cl.gbufferInstanceId);
            
            shadowMap = std::move(cl.shadowMap);
            
            ssao = std::move(cl.ssao);
            
            depth = std::move(cl.depth);
            

            

            
            instanceMaterials = std::move(cl.instanceMaterials);
            
            shadowVP = std::move(cl.shadowVP);
            
            lightWorldPosition = std::move(cl.lightWorldPosition);
            
            lightDirection = std::move(cl.lightDirection);
            
            lightColor = std::move(cl.lightColor);
            
            lightParameters = std::move(cl.lightParameters);
            
            lightType = std::move(cl.lightType);
            
            lightIntensity = std::move(cl.lightIntensity);
            
            lightRange = std::move(cl.lightRange);
            

            

            
            materialTextures = std::move(cl.materialTextures);
            

            

            

            

            
            tex_sampler = std::move(cl.tex_sampler);
            
            tri_sampler = std::move(cl.tri_sampler);
            
            depth_sampler = std::move(cl.depth_sampler);
            
            point_sampler = std::move(cl.point_sampler);
            
            shadow_sampler = std::move(cl.shadow_sampler);
            

            
            drawmode = cl.drawmode;
            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        LightingPS& LightingPS::operator=(const LightingPS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            environmentIrradianceCubemap = cl.environmentIrradianceCubemap;
            
            environmentIrradiance = cl.environmentIrradiance;
            
            environmentSpecular = cl.environmentSpecular;
            
            environmentBrdfLut = cl.environmentBrdfLut;
            
            gbufferNormals = cl.gbufferNormals;
            
            gbufferUV = cl.gbufferUV;
            
            gbufferInstanceId = cl.gbufferInstanceId;
            
            shadowMap = cl.shadowMap;
            
            ssao = cl.ssao;
            
            depth = cl.depth;
            

            

            
            instanceMaterials = cl.instanceMaterials;
            
            shadowVP = cl.shadowVP;
            
            lightWorldPosition = cl.lightWorldPosition;
            
            lightDirection = cl.lightDirection;
            
            lightColor = cl.lightColor;
            
            lightParameters = cl.lightParameters;
            
            lightType = cl.lightType;
            
            lightIntensity = cl.lightIntensity;
            
            lightRange = cl.lightRange;
            

            

            
            materialTextures = cl.materialTextures;
            

            

            

            

            
            tex_sampler = cl.tex_sampler;
            
            tri_sampler = cl.tri_sampler;
            
            depth_sampler = cl.depth_sampler;
            
            point_sampler = cl.point_sampler;
            
            shadow_sampler = cl.shadow_sampler;
            

            
            drawmode = cl.drawmode;
            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        LightingPS& LightingPS::operator=(LightingPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            environmentIrradianceCubemap = std::move(cl.environmentIrradianceCubemap);
            
            environmentIrradiance = std::move(cl.environmentIrradiance);
            
            environmentSpecular = std::move(cl.environmentSpecular);
            
            environmentBrdfLut = std::move(cl.environmentBrdfLut);
            
            gbufferNormals = std::move(cl.gbufferNormals);
            
            gbufferUV = std::move(cl.gbufferUV);
            
            gbufferInstanceId = std::move(cl.gbufferInstanceId);
            
            shadowMap = std::move(cl.shadowMap);
            
            ssao = std::move(cl.ssao);
            
            depth = std::move(cl.depth);
            

            

            
            instanceMaterials = std::move(cl.instanceMaterials);
            
            shadowVP = std::move(cl.shadowVP);
            
            lightWorldPosition = std::move(cl.lightWorldPosition);
            
            lightDirection = std::move(cl.lightDirection);
            
            lightColor = std::move(cl.lightColor);
            
            lightParameters = std::move(cl.lightParameters);
            
            lightType = std::move(cl.lightType);
            
            lightIntensity = std::move(cl.lightIntensity);
            
            lightRange = std::move(cl.lightRange);
            

            

            
            materialTextures = std::move(cl.materialTextures);
            

            

            

            

            
            tex_sampler = std::move(cl.tex_sampler);
            
            tri_sampler = std::move(cl.tri_sampler);
            
            depth_sampler = std::move(cl.depth_sampler);
            
            point_sampler = std::move(cl.point_sampler);
            
            shadow_sampler = std::move(cl.shadow_sampler);
            

            
            drawmode = cl.drawmode;
            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> LightingPS::textureSrvNames() const
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
                
                
                "gbufferNormals"
                
                ,
                
                
                "gbufferUV"
                
                ,
                
                
                "gbufferInstanceId"
                
                ,
                
                
                "shadowMap"
                
                ,
                
                
                "ssao"
                
                ,
                
                
                "depth"
                
                
            };
        }

        std::vector<std::string> LightingPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> LightingPS::bufferSrvNames() const
        {
            return {
                
                "instanceMaterials"
                
                ,
                
                
                "shadowVP"
                
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
                
                
            };
        }

        std::vector<std::string> LightingPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> LightingPS::samplerNames() const
        {
            return {
                
                "tex_sampler"
                
                ,
                
                
                "tri_sampler"
                
                ,
                
                
                "depth_sampler"
                
                ,
                
                
                "point_sampler"
                
                ,
                
                
                "shadow_sampler"
                
                
            };
        }

        std::vector<std::string> LightingPS::srvNames() const
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
                
                
                "gbufferNormals"
                
                ,
                
                
                "gbufferUV"
                
                ,
                
                
                "gbufferInstanceId"
                
                ,
                
                
                "instanceMaterials"
                
                ,
                
                
                "shadowMap"
                
                ,
                
                
                "shadowVP"
                
                ,
                
                
                "ssao"
                
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
                
                
                "depth"
                
                ,
                
                
                "materialTextures"
                
                
            };
        }

        std::vector<std::string> LightingPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension LightingPS::textureDimension(const std::string& name) const
        {
            
            if("environmentIrradianceCubemap" == name) return engine::ResourceDimension::TextureCubemap;
            
            if("environmentIrradiance" == name) return engine::ResourceDimension::Texture2D;
            
            if("environmentSpecular" == name) return engine::ResourceDimension::TextureCubemap;
            
            if("environmentBrdfLut" == name) return engine::ResourceDimension::Texture2D;
            
            if("gbufferNormals" == name) return engine::ResourceDimension::Texture2D;
            
            if("gbufferUV" == name) return engine::ResourceDimension::Texture2D;
            
            if("gbufferInstanceId" == name) return engine::ResourceDimension::Texture2D;
            
            if("shadowMap" == name) return engine::ResourceDimension::Texture2DArray;
            
            if("ssao" == name) return engine::ResourceDimension::Texture2D;
            
            if("depth" == name) return engine::ResourceDimension::Texture2D;
            
            if("materialTextures" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> LightingPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(environmentIrradianceCubemap);
            
            result.emplace_back(environmentIrradiance);
            
            result.emplace_back(environmentSpecular);
            
            result.emplace_back(environmentBrdfLut);
            
            result.emplace_back(gbufferNormals);
            
            result.emplace_back(gbufferUV);
            
            result.emplace_back(gbufferInstanceId);
            
            result.emplace_back(shadowMap);
            
            result.emplace_back(ssao);
            
            result.emplace_back(depth);
            
            return result;
        }

        std::vector<TextureUAV> LightingPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> LightingPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(instanceMaterials);
            
            result.emplace_back(shadowVP);
            
            result.emplace_back(lightWorldPosition);
            
            result.emplace_back(lightDirection);
            
            result.emplace_back(lightColor);
            
            result.emplace_back(lightParameters);
            
            result.emplace_back(lightType);
            
            result.emplace_back(lightIntensity);
            
            result.emplace_back(lightRange);
            
            return result;
        }

        std::vector<BufferUAV> LightingPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> LightingPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            result.emplace_back(materialTextures);
            
            return result;
        }

        std::vector<TextureBindlessUAV> LightingPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> LightingPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> LightingPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& LightingPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> LightingPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(tex_sampler);
            
            result.emplace_back(tri_sampler);
            
            result.emplace_back(depth_sampler);
            
            result.emplace_back(point_sampler);
            
            result.emplace_back(shadow_sampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& LightingPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool LightingPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("environmentIrradianceCubemap")) return true;
            
            if(name == std::string("environmentIrradiance")) return true;
            
            if(name == std::string("environmentSpecular")) return true;
            
            if(name == std::string("environmentBrdfLut")) return true;
            
            if(name == std::string("gbufferNormals")) return true;
            
            if(name == std::string("gbufferUV")) return true;
            
            if(name == std::string("gbufferInstanceId")) return true;
            
            if(name == std::string("shadowMap")) return true;
            
            if(name == std::string("ssao")) return true;
            
            if(name == std::string("depth")) return true;
            
            
            return false;
        }

        bool LightingPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool LightingPS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("instanceMaterials")) return true;
            
            if (name == std::string("shadowVP")) return true;
            
            if (name == std::string("lightWorldPosition")) return true;
            
            if (name == std::string("lightDirection")) return true;
            
            if (name == std::string("lightColor")) return true;
            
            if (name == std::string("lightParameters")) return true;
            
            if (name == std::string("lightType")) return true;
            
            if (name == std::string("lightIntensity")) return true;
            
            if (name == std::string("lightRange")) return true;
            
            
            return false;
        }

        bool LightingPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool LightingPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            
            if (name == std::string("materialTextures")) return true;
            
            
            return false;
        }

        bool LightingPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool LightingPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool LightingPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& LightingPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("environmentIrradianceCubemap")) return environmentIrradianceCubemap;
            
            if(name == std::string("environmentIrradiance")) return environmentIrradiance;
            
            if(name == std::string("environmentSpecular")) return environmentSpecular;
            
            if(name == std::string("environmentBrdfLut")) return environmentBrdfLut;
            
            if(name == std::string("gbufferNormals")) return gbufferNormals;
            
            if(name == std::string("gbufferUV")) return gbufferUV;
            
            if(name == std::string("gbufferInstanceId")) return gbufferInstanceId;
            
            if(name == std::string("shadowMap")) return shadowMap;
            
            if(name == std::string("ssao")) return ssao;
            
            if(name == std::string("depth")) return depth;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& LightingPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& LightingPS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("instanceMaterials")) return instanceMaterials;
            
            if(name == std::string("shadowVP")) return shadowVP;
            
            if(name == std::string("lightWorldPosition")) return lightWorldPosition;
            
            if(name == std::string("lightDirection")) return lightDirection;
            
            if(name == std::string("lightColor")) return lightColor;
            
            if(name == std::string("lightParameters")) return lightParameters;
            
            if(name == std::string("lightType")) return lightType;
            
            if(name == std::string("lightIntensity")) return lightIntensity;
            
            if(name == std::string("lightRange")) return lightRange;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& LightingPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void LightingPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("environmentIrradianceCubemap")) { environmentIrradianceCubemap = texture; return; }
            
            if(name == std::string("environmentIrradiance")) { environmentIrradiance = texture; return; }
            
            if(name == std::string("environmentSpecular")) { environmentSpecular = texture; return; }
            
            if(name == std::string("environmentBrdfLut")) { environmentBrdfLut = texture; return; }
            
            if(name == std::string("gbufferNormals")) { gbufferNormals = texture; return; }
            
            if(name == std::string("gbufferUV")) { gbufferUV = texture; return; }
            
            if(name == std::string("gbufferInstanceId")) { gbufferInstanceId = texture; return; }
            
            if(name == std::string("shadowMap")) { shadowMap = texture; return; }
            
            if(name == std::string("ssao")) { ssao = texture; return; }
            
            if(name == std::string("depth")) { depth = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void LightingPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void LightingPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("instanceMaterials")) { instanceMaterials = buffer; return; }
            
            if(name == std::string("shadowVP")) { shadowVP = buffer; return; }
            
            if(name == std::string("lightWorldPosition")) { lightWorldPosition = buffer; return; }
            
            if(name == std::string("lightDirection")) { lightDirection = buffer; return; }
            
            if(name == std::string("lightColor")) { lightColor = buffer; return; }
            
            if(name == std::string("lightParameters")) { lightParameters = buffer; return; }
            
            if(name == std::string("lightType")) { lightType = buffer; return; }
            
            if(name == std::string("lightIntensity")) { lightIntensity = buffer; return; }
            
            if(name == std::string("lightRange")) { lightRange = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void LightingPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& LightingPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("tex_sampler")) return tex_sampler;
            
            if(name == std::string("tri_sampler")) return tri_sampler;
            
            if(name == std::string("depth_sampler")) return depth_sampler;
            
            if(name == std::string("point_sampler")) return point_sampler;
            
            if(name == std::string("shadow_sampler")) return shadow_sampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& LightingPS::bindlessTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("materialTextures")) return materialTextures;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& LightingPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& LightingPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& LightingPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t LightingPS::descriptorCount() const
        {
            return 21;
        }
    }
}