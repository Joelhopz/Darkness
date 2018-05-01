#include "OcclusionCulling.cs.h"
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
        std::shared_ptr<const ShaderBinary> OcclusionCullingCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            if((true)
            
            && (emitAll == false)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/OcclusionCulling.cs_000.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/OcclusionCulling.cs.support", 1-1, {
                    
                    });
            }
            
            if((true)
            
            && (emitAll == true)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/OcclusionCulling.cs_001.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/OcclusionCulling.cs.support", 2-1, {
                    
                    "OPTION_EMIT_ALL"
                    
                    });
            }
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        OcclusionCullingCS::OcclusionCullingCS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const CullingConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const CullingConstants*>(this)) + sizeof(CullingConstants)),
                    nullptr,
                    "CullingConstants"
                }
                
            
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"dispatchThreadID", "SV_DispatchThreadID", "uint3"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        OcclusionCullingCS::OcclusionCullingCS(const OcclusionCullingCS& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const CullingConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const CullingConstants*>(this)) + sizeof(CullingConstants)),
                    nullptr,
                    "CullingConstants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            depthPyramid = cl.depthPyramid;
            

            

            
            frustumCullingOutput = cl.frustumCullingOutput;
            
            boundingBoxes = cl.boundingBoxes;
            
            transformHistory = cl.transformHistory;
            
            clusterCount = cl.clusterCount;
            

            
            cullingOutput = cl.cullingOutput;
            
            occlusionDrawArgs = cl.occlusionDrawArgs;
            
            distributor = cl.distributor;
            

            

            

            

            

            
            depthSampler = cl.depthSampler;
            

            
            emitAll = cl.emitAll;
            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        OcclusionCullingCS::OcclusionCullingCS(OcclusionCullingCS&& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const CullingConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const CullingConstants*>(this)) + sizeof(CullingConstants)),
                    nullptr,
                    "CullingConstants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            depthPyramid = std::move(cl.depthPyramid);
            

            

            
            frustumCullingOutput = std::move(cl.frustumCullingOutput);
            
            boundingBoxes = std::move(cl.boundingBoxes);
            
            transformHistory = std::move(cl.transformHistory);
            
            clusterCount = std::move(cl.clusterCount);
            

            
            cullingOutput = std::move(cl.cullingOutput);
            
            occlusionDrawArgs = std::move(cl.occlusionDrawArgs);
            
            distributor = std::move(cl.distributor);
            

            

            

            

            

            
            depthSampler = std::move(cl.depthSampler);
            

            
            emitAll = std::move(cl.emitAll);
            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        OcclusionCullingCS& OcclusionCullingCS::operator=(const OcclusionCullingCS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            depthPyramid = cl.depthPyramid;
            

            

            
            frustumCullingOutput = cl.frustumCullingOutput;
            
            boundingBoxes = cl.boundingBoxes;
            
            transformHistory = cl.transformHistory;
            
            clusterCount = cl.clusterCount;
            

            
            cullingOutput = cl.cullingOutput;
            
            occlusionDrawArgs = cl.occlusionDrawArgs;
            
            distributor = cl.distributor;
            

            

            

            

            

            
            depthSampler = cl.depthSampler;
            

            
            emitAll = cl.emitAll;
            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        OcclusionCullingCS& OcclusionCullingCS::operator=(OcclusionCullingCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            depthPyramid = std::move(cl.depthPyramid);
            

            

            
            frustumCullingOutput = std::move(cl.frustumCullingOutput);
            
            boundingBoxes = std::move(cl.boundingBoxes);
            
            transformHistory = std::move(cl.transformHistory);
            
            clusterCount = std::move(cl.clusterCount);
            

            
            cullingOutput = std::move(cl.cullingOutput);
            
            occlusionDrawArgs = std::move(cl.occlusionDrawArgs);
            
            distributor = std::move(cl.distributor);
            

            

            

            

            

            
            depthSampler = std::move(cl.depthSampler);
            

            
            emitAll = std::move(cl.emitAll);
            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> OcclusionCullingCS::textureSrvNames() const
        {
            return {
                
                "depthPyramid"
                
                
            };
        }

        std::vector<std::string> OcclusionCullingCS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> OcclusionCullingCS::bufferSrvNames() const
        {
            return {
                
                "frustumCullingOutput"
                
                ,
                
                
                "boundingBoxes"
                
                ,
                
                
                "transformHistory"
                
                ,
                
                
                "clusterCount"
                
                
            };
        }

        std::vector<std::string> OcclusionCullingCS::bufferUavNames() const
        {
            return {
                
                "cullingOutput"
                
                ,
                
                
                "occlusionDrawArgs"
                
                ,
                
                
                "distributor"
                
                
            };
        }

        std::vector<std::string> OcclusionCullingCS::samplerNames() const
        {
            return {
                
                "depthSampler"
                
                
            };
        }

        std::vector<std::string> OcclusionCullingCS::srvNames() const
        {
            return {
                
                "frustumCullingOutput"
                
                ,
                
                
                "boundingBoxes"
                
                ,
                
                
                "transformHistory"
                
                ,
                
                
                "clusterCount"
                
                ,
                
                
                "depthPyramid"
                
                
            };
        }

        std::vector<std::string> OcclusionCullingCS::uavNames() const
        {
            return {
                
                "cullingOutput"
                
                ,
                
                
                "occlusionDrawArgs"
                
                ,
                
                
                "distributor"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension OcclusionCullingCS::textureDimension(const std::string& name) const
        {
            
            if("depthPyramid" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> OcclusionCullingCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(depthPyramid);
            
            return result;
        }

        std::vector<TextureUAV> OcclusionCullingCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> OcclusionCullingCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(frustumCullingOutput);
            
            result.emplace_back(boundingBoxes);
            
            result.emplace_back(transformHistory);
            
            result.emplace_back(clusterCount);
            
            return result;
        }

        std::vector<BufferUAV> OcclusionCullingCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            result.emplace_back(cullingOutput);
            
            result.emplace_back(occlusionDrawArgs);
            
            result.emplace_back(distributor);
            
            return result;
        }

        std::vector<TextureBindlessSRV> OcclusionCullingCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> OcclusionCullingCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> OcclusionCullingCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> OcclusionCullingCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& OcclusionCullingCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> OcclusionCullingCS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(depthSampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& OcclusionCullingCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool OcclusionCullingCS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("depthPyramid")) return true;
            
            
            return false;
        }

        bool OcclusionCullingCS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool OcclusionCullingCS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("frustumCullingOutput")) return true;
            
            if (name == std::string("boundingBoxes")) return true;
            
            if (name == std::string("transformHistory")) return true;
            
            if (name == std::string("clusterCount")) return true;
            
            
            return false;
        }

        bool OcclusionCullingCS::hasBufferUav(const std::string& name) const
        {
            
            
            if (name == std::string("cullingOutput")) return true;
            
            if (name == std::string("occlusionDrawArgs")) return true;
            
            if (name == std::string("distributor")) return true;
            
            
            return false;
        }

        bool OcclusionCullingCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool OcclusionCullingCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool OcclusionCullingCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool OcclusionCullingCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& OcclusionCullingCS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("depthPyramid")) return depthPyramid;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& OcclusionCullingCS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& OcclusionCullingCS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("frustumCullingOutput")) return frustumCullingOutput;
            
            if(name == std::string("boundingBoxes")) return boundingBoxes;
            
            if(name == std::string("transformHistory")) return transformHistory;
            
            if(name == std::string("clusterCount")) return clusterCount;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& OcclusionCullingCS::bufferUav(const std::string& name) const
        {
            
            
            if(name == std::string("cullingOutput")) return cullingOutput;
            
            if(name == std::string("occlusionDrawArgs")) return occlusionDrawArgs;
            
            if(name == std::string("distributor")) return distributor;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void OcclusionCullingCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("depthPyramid")) { depthPyramid = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void OcclusionCullingCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void OcclusionCullingCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("frustumCullingOutput")) { frustumCullingOutput = buffer; return; }
            
            if(name == std::string("boundingBoxes")) { boundingBoxes = buffer; return; }
            
            if(name == std::string("transformHistory")) { transformHistory = buffer; return; }
            
            if(name == std::string("clusterCount")) { clusterCount = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void OcclusionCullingCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            
            if(name == std::string("cullingOutput")) { cullingOutput = buffer; return; }
            
            if(name == std::string("occlusionDrawArgs")) { occlusionDrawArgs = buffer; return; }
            
            if(name == std::string("distributor")) { distributor = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& OcclusionCullingCS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("depthSampler")) return depthSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& OcclusionCullingCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& OcclusionCullingCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& OcclusionCullingCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& OcclusionCullingCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t OcclusionCullingCS::descriptorCount() const
        {
            return 9;
        }
    }
}