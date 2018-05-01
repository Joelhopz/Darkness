#include "ClusterGather.cs.h"
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
        std::shared_ptr<const ShaderBinary> ClusterGatherCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/ClusterGather.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/ClusterGather.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        ClusterGatherCS::ClusterGatherCS()
            : m_constantRange{
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"dispatchThreadID", "SV_DispatchThreadID", "uint3"}
            
            ,
            
            
            ShaderInputParameter{"groupID", "SV_GroupID", "uint3"}
            
            ,
            
            
            ShaderInputParameter{"groupThreadID", "SV_GroupThreadID", "uint3"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterGatherCS::ClusterGatherCS(const ClusterGatherCS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            clusterCountBuffer = cl.clusterCountBuffer;
            
            frustumCullingOutput = cl.frustumCullingOutput;
            

            
            clustersForOcclusionCulling = cl.clustersForOcclusionCulling;
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterGatherCS::ClusterGatherCS(ClusterGatherCS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            clusterCountBuffer = std::move(cl.clusterCountBuffer);
            
            frustumCullingOutput = std::move(cl.frustumCullingOutput);
            

            
            clustersForOcclusionCulling = std::move(cl.clustersForOcclusionCulling);
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterGatherCS& ClusterGatherCS::operator=(const ClusterGatherCS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            clusterCountBuffer = cl.clusterCountBuffer;
            
            frustumCullingOutput = cl.frustumCullingOutput;
            

            
            clustersForOcclusionCulling = cl.clustersForOcclusionCulling;
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterGatherCS& ClusterGatherCS::operator=(ClusterGatherCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            clusterCountBuffer = std::move(cl.clusterCountBuffer);
            
            frustumCullingOutput = std::move(cl.frustumCullingOutput);
            

            
            clustersForOcclusionCulling = std::move(cl.clustersForOcclusionCulling);
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> ClusterGatherCS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterGatherCS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterGatherCS::bufferSrvNames() const
        {
            return {
                
                "clusterCountBuffer"
                
                ,
                
                
                "frustumCullingOutput"
                
                
            };
        }

        std::vector<std::string> ClusterGatherCS::bufferUavNames() const
        {
            return {
                
                "clustersForOcclusionCulling"
                
                
            };
        }

        std::vector<std::string> ClusterGatherCS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterGatherCS::srvNames() const
        {
            return {
                
                "clusterCountBuffer"
                
                ,
                
                
                "frustumCullingOutput"
                
                
            };
        }

        std::vector<std::string> ClusterGatherCS::uavNames() const
        {
            return {
                
                "clustersForOcclusionCulling"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension ClusterGatherCS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> ClusterGatherCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> ClusterGatherCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> ClusterGatherCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(clusterCountBuffer);
            
            result.emplace_back(frustumCullingOutput);
            
            return result;
        }

        std::vector<BufferUAV> ClusterGatherCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            result.emplace_back(clustersForOcclusionCulling);
            
            return result;
        }

        std::vector<TextureBindlessSRV> ClusterGatherCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> ClusterGatherCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> ClusterGatherCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> ClusterGatherCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& ClusterGatherCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> ClusterGatherCS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& ClusterGatherCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool ClusterGatherCS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterGatherCS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterGatherCS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("clusterCountBuffer")) return true;
            
            if (name == std::string("frustumCullingOutput")) return true;
            
            
            return false;
        }

        bool ClusterGatherCS::hasBufferUav(const std::string& name) const
        {
            
            
            if (name == std::string("clustersForOcclusionCulling")) return true;
            
            
            return false;
        }

        bool ClusterGatherCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterGatherCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterGatherCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterGatherCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& ClusterGatherCS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& ClusterGatherCS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& ClusterGatherCS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("clusterCountBuffer")) return clusterCountBuffer;
            
            if(name == std::string("frustumCullingOutput")) return frustumCullingOutput;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& ClusterGatherCS::bufferUav(const std::string& name) const
        {
            
            
            if(name == std::string("clustersForOcclusionCulling")) return clustersForOcclusionCulling;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void ClusterGatherCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClusterGatherCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClusterGatherCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("clusterCountBuffer")) { clusterCountBuffer = buffer; return; }
            
            if(name == std::string("frustumCullingOutput")) { frustumCullingOutput = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClusterGatherCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            
            if(name == std::string("clustersForOcclusionCulling")) { clustersForOcclusionCulling = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& ClusterGatherCS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& ClusterGatherCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& ClusterGatherCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& ClusterGatherCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& ClusterGatherCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t ClusterGatherCS::descriptorCount() const
        {
            return 3;
        }
    }
}