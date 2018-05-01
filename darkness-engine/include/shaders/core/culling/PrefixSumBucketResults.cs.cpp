#include "PrefixSumBucketResults.cs.h"
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
        std::shared_ptr<const ShaderBinary> PrefixSumBucketResultsCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/PrefixSumBucketResults.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/PrefixSumBucketResults.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        PrefixSumBucketResultsCS::PrefixSumBucketResultsCS()
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
        PrefixSumBucketResultsCS::PrefixSumBucketResultsCS(const PrefixSumBucketResultsCS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            clusterCountBuffer = cl.clusterCountBuffer;
            
            input = cl.input;
            

            
            output = cl.output;
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        PrefixSumBucketResultsCS::PrefixSumBucketResultsCS(PrefixSumBucketResultsCS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            clusterCountBuffer = std::move(cl.clusterCountBuffer);
            
            input = std::move(cl.input);
            

            
            output = std::move(cl.output);
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        PrefixSumBucketResultsCS& PrefixSumBucketResultsCS::operator=(const PrefixSumBucketResultsCS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            clusterCountBuffer = cl.clusterCountBuffer;
            
            input = cl.input;
            

            
            output = cl.output;
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        PrefixSumBucketResultsCS& PrefixSumBucketResultsCS::operator=(PrefixSumBucketResultsCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            clusterCountBuffer = std::move(cl.clusterCountBuffer);
            
            input = std::move(cl.input);
            

            
            output = std::move(cl.output);
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> PrefixSumBucketResultsCS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> PrefixSumBucketResultsCS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> PrefixSumBucketResultsCS::bufferSrvNames() const
        {
            return {
                
                "clusterCountBuffer"
                
                ,
                
                
                "input"
                
                
            };
        }

        std::vector<std::string> PrefixSumBucketResultsCS::bufferUavNames() const
        {
            return {
                
                "output"
                
                
            };
        }

        std::vector<std::string> PrefixSumBucketResultsCS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> PrefixSumBucketResultsCS::srvNames() const
        {
            return {
                
                "clusterCountBuffer"
                
                ,
                
                
                "input"
                
                
            };
        }

        std::vector<std::string> PrefixSumBucketResultsCS::uavNames() const
        {
            return {
                
                "output"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension PrefixSumBucketResultsCS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> PrefixSumBucketResultsCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> PrefixSumBucketResultsCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> PrefixSumBucketResultsCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(clusterCountBuffer);
            
            result.emplace_back(input);
            
            return result;
        }

        std::vector<BufferUAV> PrefixSumBucketResultsCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            result.emplace_back(output);
            
            return result;
        }

        std::vector<TextureBindlessSRV> PrefixSumBucketResultsCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> PrefixSumBucketResultsCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> PrefixSumBucketResultsCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> PrefixSumBucketResultsCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& PrefixSumBucketResultsCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> PrefixSumBucketResultsCS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& PrefixSumBucketResultsCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool PrefixSumBucketResultsCS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PrefixSumBucketResultsCS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool PrefixSumBucketResultsCS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("clusterCountBuffer")) return true;
            
            if (name == std::string("input")) return true;
            
            
            return false;
        }

        bool PrefixSumBucketResultsCS::hasBufferUav(const std::string& name) const
        {
            
            
            if (name == std::string("output")) return true;
            
            
            return false;
        }

        bool PrefixSumBucketResultsCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PrefixSumBucketResultsCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool PrefixSumBucketResultsCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PrefixSumBucketResultsCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& PrefixSumBucketResultsCS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& PrefixSumBucketResultsCS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& PrefixSumBucketResultsCS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("clusterCountBuffer")) return clusterCountBuffer;
            
            if(name == std::string("input")) return input;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& PrefixSumBucketResultsCS::bufferUav(const std::string& name) const
        {
            
            
            if(name == std::string("output")) return output;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void PrefixSumBucketResultsCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void PrefixSumBucketResultsCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void PrefixSumBucketResultsCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("clusterCountBuffer")) { clusterCountBuffer = buffer; return; }
            
            if(name == std::string("input")) { input = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void PrefixSumBucketResultsCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            
            if(name == std::string("output")) { output = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& PrefixSumBucketResultsCS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& PrefixSumBucketResultsCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& PrefixSumBucketResultsCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& PrefixSumBucketResultsCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& PrefixSumBucketResultsCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t PrefixSumBucketResultsCS::descriptorCount() const
        {
            return 3;
        }
    }
}