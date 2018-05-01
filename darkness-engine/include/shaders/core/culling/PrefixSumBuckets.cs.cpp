#include "PrefixSumBuckets.cs.h"
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
        std::shared_ptr<const ShaderBinary> PrefixSumBucketsCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/PrefixSumBuckets.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/PrefixSumBuckets.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        PrefixSumBucketsCS::PrefixSumBucketsCS()
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
        PrefixSumBucketsCS::PrefixSumBucketsCS(const PrefixSumBucketsCS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            clusterCountBuffer = cl.clusterCountBuffer;
            
            frustumCullingOutput = cl.frustumCullingOutput;
            

            
            output = cl.output;
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        PrefixSumBucketsCS::PrefixSumBucketsCS(PrefixSumBucketsCS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            clusterCountBuffer = std::move(cl.clusterCountBuffer);
            
            frustumCullingOutput = std::move(cl.frustumCullingOutput);
            

            
            output = std::move(cl.output);
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        PrefixSumBucketsCS& PrefixSumBucketsCS::operator=(const PrefixSumBucketsCS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            clusterCountBuffer = cl.clusterCountBuffer;
            
            frustumCullingOutput = cl.frustumCullingOutput;
            

            
            output = cl.output;
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        PrefixSumBucketsCS& PrefixSumBucketsCS::operator=(PrefixSumBucketsCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            clusterCountBuffer = std::move(cl.clusterCountBuffer);
            
            frustumCullingOutput = std::move(cl.frustumCullingOutput);
            

            
            output = std::move(cl.output);
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> PrefixSumBucketsCS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> PrefixSumBucketsCS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> PrefixSumBucketsCS::bufferSrvNames() const
        {
            return {
                
                "clusterCountBuffer"
                
                ,
                
                
                "frustumCullingOutput"
                
                
            };
        }

        std::vector<std::string> PrefixSumBucketsCS::bufferUavNames() const
        {
            return {
                
                "output"
                
                
            };
        }

        std::vector<std::string> PrefixSumBucketsCS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> PrefixSumBucketsCS::srvNames() const
        {
            return {
                
                "clusterCountBuffer"
                
                ,
                
                
                "frustumCullingOutput"
                
                
            };
        }

        std::vector<std::string> PrefixSumBucketsCS::uavNames() const
        {
            return {
                
                "output"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension PrefixSumBucketsCS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> PrefixSumBucketsCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> PrefixSumBucketsCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> PrefixSumBucketsCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(clusterCountBuffer);
            
            result.emplace_back(frustumCullingOutput);
            
            return result;
        }

        std::vector<BufferUAV> PrefixSumBucketsCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            result.emplace_back(output);
            
            return result;
        }

        std::vector<TextureBindlessSRV> PrefixSumBucketsCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> PrefixSumBucketsCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> PrefixSumBucketsCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> PrefixSumBucketsCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& PrefixSumBucketsCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> PrefixSumBucketsCS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& PrefixSumBucketsCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool PrefixSumBucketsCS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PrefixSumBucketsCS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool PrefixSumBucketsCS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("clusterCountBuffer")) return true;
            
            if (name == std::string("frustumCullingOutput")) return true;
            
            
            return false;
        }

        bool PrefixSumBucketsCS::hasBufferUav(const std::string& name) const
        {
            
            
            if (name == std::string("output")) return true;
            
            
            return false;
        }

        bool PrefixSumBucketsCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PrefixSumBucketsCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool PrefixSumBucketsCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PrefixSumBucketsCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& PrefixSumBucketsCS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& PrefixSumBucketsCS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& PrefixSumBucketsCS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("clusterCountBuffer")) return clusterCountBuffer;
            
            if(name == std::string("frustumCullingOutput")) return frustumCullingOutput;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& PrefixSumBucketsCS::bufferUav(const std::string& name) const
        {
            
            
            if(name == std::string("output")) return output;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void PrefixSumBucketsCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void PrefixSumBucketsCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void PrefixSumBucketsCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("clusterCountBuffer")) { clusterCountBuffer = buffer; return; }
            
            if(name == std::string("frustumCullingOutput")) { frustumCullingOutput = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void PrefixSumBucketsCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            
            if(name == std::string("output")) { output = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& PrefixSumBucketsCS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& PrefixSumBucketsCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& PrefixSumBucketsCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& PrefixSumBucketsCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& PrefixSumBucketsCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t PrefixSumBucketsCS::descriptorCount() const
        {
            return 3;
        }
    }
}