#include "FrustumCulling.cs.h"
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
        std::shared_ptr<const ShaderBinary> FrustumCullingCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/FrustumCulling.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/FrustumCulling.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        FrustumCullingCS::FrustumCullingCS()
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
        FrustumCullingCS::FrustumCullingCS(const FrustumCullingCS& cl)
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

            

            

            
            subMeshBoundingBoxes = cl.subMeshBoundingBoxes;
            
            instanceSubMeshBinding = cl.instanceSubMeshBinding;
            
            subMeshData = cl.subMeshData;
            
            transformHistory = cl.transformHistory;
            

            
            cullingOutput = cl.cullingOutput;
            
            clusterCountBuffer = cl.clusterCountBuffer;
            
            outputAllocationShared = cl.outputAllocationShared;
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        FrustumCullingCS::FrustumCullingCS(FrustumCullingCS&& cl)
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

            

            

            
            subMeshBoundingBoxes = std::move(cl.subMeshBoundingBoxes);
            
            instanceSubMeshBinding = std::move(cl.instanceSubMeshBinding);
            
            subMeshData = std::move(cl.subMeshData);
            
            transformHistory = std::move(cl.transformHistory);
            

            
            cullingOutput = std::move(cl.cullingOutput);
            
            clusterCountBuffer = std::move(cl.clusterCountBuffer);
            
            outputAllocationShared = std::move(cl.outputAllocationShared);
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        FrustumCullingCS& FrustumCullingCS::operator=(const FrustumCullingCS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            subMeshBoundingBoxes = cl.subMeshBoundingBoxes;
            
            instanceSubMeshBinding = cl.instanceSubMeshBinding;
            
            subMeshData = cl.subMeshData;
            
            transformHistory = cl.transformHistory;
            

            
            cullingOutput = cl.cullingOutput;
            
            clusterCountBuffer = cl.clusterCountBuffer;
            
            outputAllocationShared = cl.outputAllocationShared;
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        FrustumCullingCS& FrustumCullingCS::operator=(FrustumCullingCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            subMeshBoundingBoxes = std::move(cl.subMeshBoundingBoxes);
            
            instanceSubMeshBinding = std::move(cl.instanceSubMeshBinding);
            
            subMeshData = std::move(cl.subMeshData);
            
            transformHistory = std::move(cl.transformHistory);
            

            
            cullingOutput = std::move(cl.cullingOutput);
            
            clusterCountBuffer = std::move(cl.clusterCountBuffer);
            
            outputAllocationShared = std::move(cl.outputAllocationShared);
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> FrustumCullingCS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> FrustumCullingCS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> FrustumCullingCS::bufferSrvNames() const
        {
            return {
                
                "subMeshBoundingBoxes"
                
                ,
                
                
                "instanceSubMeshBinding"
                
                ,
                
                
                "subMeshData"
                
                ,
                
                
                "transformHistory"
                
                
            };
        }

        std::vector<std::string> FrustumCullingCS::bufferUavNames() const
        {
            return {
                
                "cullingOutput"
                
                ,
                
                
                "clusterCountBuffer"
                
                ,
                
                
                "outputAllocationShared"
                
                
            };
        }

        std::vector<std::string> FrustumCullingCS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> FrustumCullingCS::srvNames() const
        {
            return {
                
                "subMeshBoundingBoxes"
                
                ,
                
                
                "instanceSubMeshBinding"
                
                ,
                
                
                "subMeshData"
                
                ,
                
                
                "transformHistory"
                
                
            };
        }

        std::vector<std::string> FrustumCullingCS::uavNames() const
        {
            return {
                
                "cullingOutput"
                
                ,
                
                
                "clusterCountBuffer"
                
                ,
                
                
                "outputAllocationShared"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension FrustumCullingCS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> FrustumCullingCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> FrustumCullingCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> FrustumCullingCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(subMeshBoundingBoxes);
            
            result.emplace_back(instanceSubMeshBinding);
            
            result.emplace_back(subMeshData);
            
            result.emplace_back(transformHistory);
            
            return result;
        }

        std::vector<BufferUAV> FrustumCullingCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            result.emplace_back(cullingOutput);
            
            result.emplace_back(clusterCountBuffer);
            
            result.emplace_back(outputAllocationShared);
            
            return result;
        }

        std::vector<TextureBindlessSRV> FrustumCullingCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> FrustumCullingCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> FrustumCullingCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> FrustumCullingCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& FrustumCullingCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> FrustumCullingCS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& FrustumCullingCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool FrustumCullingCS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool FrustumCullingCS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool FrustumCullingCS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("subMeshBoundingBoxes")) return true;
            
            if (name == std::string("instanceSubMeshBinding")) return true;
            
            if (name == std::string("subMeshData")) return true;
            
            if (name == std::string("transformHistory")) return true;
            
            
            return false;
        }

        bool FrustumCullingCS::hasBufferUav(const std::string& name) const
        {
            
            
            if (name == std::string("cullingOutput")) return true;
            
            if (name == std::string("clusterCountBuffer")) return true;
            
            if (name == std::string("outputAllocationShared")) return true;
            
            
            return false;
        }

        bool FrustumCullingCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool FrustumCullingCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool FrustumCullingCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool FrustumCullingCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& FrustumCullingCS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& FrustumCullingCS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& FrustumCullingCS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("subMeshBoundingBoxes")) return subMeshBoundingBoxes;
            
            if(name == std::string("instanceSubMeshBinding")) return instanceSubMeshBinding;
            
            if(name == std::string("subMeshData")) return subMeshData;
            
            if(name == std::string("transformHistory")) return transformHistory;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& FrustumCullingCS::bufferUav(const std::string& name) const
        {
            
            
            if(name == std::string("cullingOutput")) return cullingOutput;
            
            if(name == std::string("clusterCountBuffer")) return clusterCountBuffer;
            
            if(name == std::string("outputAllocationShared")) return outputAllocationShared;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void FrustumCullingCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void FrustumCullingCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void FrustumCullingCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("subMeshBoundingBoxes")) { subMeshBoundingBoxes = buffer; return; }
            
            if(name == std::string("instanceSubMeshBinding")) { instanceSubMeshBinding = buffer; return; }
            
            if(name == std::string("subMeshData")) { subMeshData = buffer; return; }
            
            if(name == std::string("transformHistory")) { transformHistory = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void FrustumCullingCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            
            if(name == std::string("cullingOutput")) { cullingOutput = buffer; return; }
            
            if(name == std::string("clusterCountBuffer")) { clusterCountBuffer = buffer; return; }
            
            if(name == std::string("outputAllocationShared")) { outputAllocationShared = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& FrustumCullingCS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& FrustumCullingCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& FrustumCullingCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& FrustumCullingCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& FrustumCullingCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t FrustumCullingCS::descriptorCount() const
        {
            return 8;
        }
    }
}