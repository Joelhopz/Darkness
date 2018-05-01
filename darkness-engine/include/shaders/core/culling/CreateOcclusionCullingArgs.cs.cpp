#include "CreateOcclusionCullingArgs.cs.h"
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
        std::shared_ptr<const ShaderBinary> CreateOcclusionCullingArgsCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/CreateOcclusionCullingArgs.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/CreateOcclusionCullingArgs.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        CreateOcclusionCullingArgsCS::CreateOcclusionCullingArgsCS()
            : m_constantRange{
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"dispatchThreadID", "SV_DispatchThreadID", "uint3"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        CreateOcclusionCullingArgsCS::CreateOcclusionCullingArgsCS(const CreateOcclusionCullingArgsCS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            clusterCountBuffer = cl.clusterCountBuffer;
            

            
            gatherDispatchArgs = cl.gatherDispatchArgs;
            
            occlusionDispatchArgs = cl.occlusionDispatchArgs;
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CreateOcclusionCullingArgsCS::CreateOcclusionCullingArgsCS(CreateOcclusionCullingArgsCS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            clusterCountBuffer = std::move(cl.clusterCountBuffer);
            

            
            gatherDispatchArgs = std::move(cl.gatherDispatchArgs);
            
            occlusionDispatchArgs = std::move(cl.occlusionDispatchArgs);
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CreateOcclusionCullingArgsCS& CreateOcclusionCullingArgsCS::operator=(const CreateOcclusionCullingArgsCS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            clusterCountBuffer = cl.clusterCountBuffer;
            

            
            gatherDispatchArgs = cl.gatherDispatchArgs;
            
            occlusionDispatchArgs = cl.occlusionDispatchArgs;
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CreateOcclusionCullingArgsCS& CreateOcclusionCullingArgsCS::operator=(CreateOcclusionCullingArgsCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            clusterCountBuffer = std::move(cl.clusterCountBuffer);
            

            
            gatherDispatchArgs = std::move(cl.gatherDispatchArgs);
            
            occlusionDispatchArgs = std::move(cl.occlusionDispatchArgs);
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> CreateOcclusionCullingArgsCS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CreateOcclusionCullingArgsCS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CreateOcclusionCullingArgsCS::bufferSrvNames() const
        {
            return {
                
                "clusterCountBuffer"
                
                
            };
        }

        std::vector<std::string> CreateOcclusionCullingArgsCS::bufferUavNames() const
        {
            return {
                
                "gatherDispatchArgs"
                
                ,
                
                
                "occlusionDispatchArgs"
                
                
            };
        }

        std::vector<std::string> CreateOcclusionCullingArgsCS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CreateOcclusionCullingArgsCS::srvNames() const
        {
            return {
                
                "clusterCountBuffer"
                
                
            };
        }

        std::vector<std::string> CreateOcclusionCullingArgsCS::uavNames() const
        {
            return {
                
                "gatherDispatchArgs"
                
                ,
                
                
                "occlusionDispatchArgs"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension CreateOcclusionCullingArgsCS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> CreateOcclusionCullingArgsCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> CreateOcclusionCullingArgsCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> CreateOcclusionCullingArgsCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(clusterCountBuffer);
            
            return result;
        }

        std::vector<BufferUAV> CreateOcclusionCullingArgsCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            result.emplace_back(gatherDispatchArgs);
            
            result.emplace_back(occlusionDispatchArgs);
            
            return result;
        }

        std::vector<TextureBindlessSRV> CreateOcclusionCullingArgsCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> CreateOcclusionCullingArgsCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> CreateOcclusionCullingArgsCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> CreateOcclusionCullingArgsCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& CreateOcclusionCullingArgsCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> CreateOcclusionCullingArgsCS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& CreateOcclusionCullingArgsCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool CreateOcclusionCullingArgsCS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CreateOcclusionCullingArgsCS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CreateOcclusionCullingArgsCS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("clusterCountBuffer")) return true;
            
            
            return false;
        }

        bool CreateOcclusionCullingArgsCS::hasBufferUav(const std::string& name) const
        {
            
            
            if (name == std::string("gatherDispatchArgs")) return true;
            
            if (name == std::string("occlusionDispatchArgs")) return true;
            
            
            return false;
        }

        bool CreateOcclusionCullingArgsCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CreateOcclusionCullingArgsCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CreateOcclusionCullingArgsCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CreateOcclusionCullingArgsCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& CreateOcclusionCullingArgsCS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& CreateOcclusionCullingArgsCS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& CreateOcclusionCullingArgsCS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("clusterCountBuffer")) return clusterCountBuffer;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& CreateOcclusionCullingArgsCS::bufferUav(const std::string& name) const
        {
            
            
            if(name == std::string("gatherDispatchArgs")) return gatherDispatchArgs;
            
            if(name == std::string("occlusionDispatchArgs")) return occlusionDispatchArgs;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void CreateOcclusionCullingArgsCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CreateOcclusionCullingArgsCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CreateOcclusionCullingArgsCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("clusterCountBuffer")) { clusterCountBuffer = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CreateOcclusionCullingArgsCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            
            if(name == std::string("gatherDispatchArgs")) { gatherDispatchArgs = buffer; return; }
            
            if(name == std::string("occlusionDispatchArgs")) { occlusionDispatchArgs = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& CreateOcclusionCullingArgsCS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& CreateOcclusionCullingArgsCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& CreateOcclusionCullingArgsCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& CreateOcclusionCullingArgsCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& CreateOcclusionCullingArgsCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t CreateOcclusionCullingArgsCS::descriptorCount() const
        {
            return 3;
        }
    }
}