#include "PrefixSumAddBuckets.cs.h"
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
        std::shared_ptr<const ShaderBinary> PrefixSumAddBucketsCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/PrefixSumAddBuckets.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/PrefixSumAddBuckets.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        PrefixSumAddBucketsCS::PrefixSumAddBucketsCS()
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
        PrefixSumAddBucketsCS::PrefixSumAddBucketsCS(const PrefixSumAddBucketsCS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            clusterCountBuffer = cl.clusterCountBuffer;
            
            input1 = cl.input1;
            
            input2 = cl.input2;
            

            
            output = cl.output;
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        PrefixSumAddBucketsCS::PrefixSumAddBucketsCS(PrefixSumAddBucketsCS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            clusterCountBuffer = std::move(cl.clusterCountBuffer);
            
            input1 = std::move(cl.input1);
            
            input2 = std::move(cl.input2);
            

            
            output = std::move(cl.output);
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        PrefixSumAddBucketsCS& PrefixSumAddBucketsCS::operator=(const PrefixSumAddBucketsCS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            clusterCountBuffer = cl.clusterCountBuffer;
            
            input1 = cl.input1;
            
            input2 = cl.input2;
            

            
            output = cl.output;
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        PrefixSumAddBucketsCS& PrefixSumAddBucketsCS::operator=(PrefixSumAddBucketsCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            clusterCountBuffer = std::move(cl.clusterCountBuffer);
            
            input1 = std::move(cl.input1);
            
            input2 = std::move(cl.input2);
            

            
            output = std::move(cl.output);
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> PrefixSumAddBucketsCS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> PrefixSumAddBucketsCS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> PrefixSumAddBucketsCS::bufferSrvNames() const
        {
            return {
                
                "clusterCountBuffer"
                
                ,
                
                
                "input1"
                
                ,
                
                
                "input2"
                
                
            };
        }

        std::vector<std::string> PrefixSumAddBucketsCS::bufferUavNames() const
        {
            return {
                
                "output"
                
                
            };
        }

        std::vector<std::string> PrefixSumAddBucketsCS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> PrefixSumAddBucketsCS::srvNames() const
        {
            return {
                
                "clusterCountBuffer"
                
                ,
                
                
                "input1"
                
                ,
                
                
                "input2"
                
                
            };
        }

        std::vector<std::string> PrefixSumAddBucketsCS::uavNames() const
        {
            return {
                
                "output"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension PrefixSumAddBucketsCS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> PrefixSumAddBucketsCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> PrefixSumAddBucketsCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> PrefixSumAddBucketsCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(clusterCountBuffer);
            
            result.emplace_back(input1);
            
            result.emplace_back(input2);
            
            return result;
        }

        std::vector<BufferUAV> PrefixSumAddBucketsCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            result.emplace_back(output);
            
            return result;
        }

        std::vector<TextureBindlessSRV> PrefixSumAddBucketsCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> PrefixSumAddBucketsCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> PrefixSumAddBucketsCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> PrefixSumAddBucketsCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& PrefixSumAddBucketsCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> PrefixSumAddBucketsCS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& PrefixSumAddBucketsCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool PrefixSumAddBucketsCS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PrefixSumAddBucketsCS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool PrefixSumAddBucketsCS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("clusterCountBuffer")) return true;
            
            if (name == std::string("input1")) return true;
            
            if (name == std::string("input2")) return true;
            
            
            return false;
        }

        bool PrefixSumAddBucketsCS::hasBufferUav(const std::string& name) const
        {
            
            
            if (name == std::string("output")) return true;
            
            
            return false;
        }

        bool PrefixSumAddBucketsCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PrefixSumAddBucketsCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool PrefixSumAddBucketsCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PrefixSumAddBucketsCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& PrefixSumAddBucketsCS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& PrefixSumAddBucketsCS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& PrefixSumAddBucketsCS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("clusterCountBuffer")) return clusterCountBuffer;
            
            if(name == std::string("input1")) return input1;
            
            if(name == std::string("input2")) return input2;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& PrefixSumAddBucketsCS::bufferUav(const std::string& name) const
        {
            
            
            if(name == std::string("output")) return output;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void PrefixSumAddBucketsCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void PrefixSumAddBucketsCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void PrefixSumAddBucketsCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("clusterCountBuffer")) { clusterCountBuffer = buffer; return; }
            
            if(name == std::string("input1")) { input1 = buffer; return; }
            
            if(name == std::string("input2")) { input2 = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void PrefixSumAddBucketsCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            
            if(name == std::string("output")) { output = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& PrefixSumAddBucketsCS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& PrefixSumAddBucketsCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& PrefixSumAddBucketsCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& PrefixSumAddBucketsCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& PrefixSumAddBucketsCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t PrefixSumAddBucketsCS::descriptorCount() const
        {
            return 4;
        }
    }
}