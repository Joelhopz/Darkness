#include "CreateDrawArguments.cs.h"
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
        std::shared_ptr<const ShaderBinary> CreateDrawArgumentsCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/CreateDrawArguments.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/culling/CreateDrawArguments.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        CreateDrawArgumentsCS::CreateDrawArgumentsCS()
            : m_constantRange{
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"dispatchThreadID", "SV_DispatchThreadID", "uint3"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        CreateDrawArgumentsCS::CreateDrawArgumentsCS(const CreateDrawArgumentsCS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            structureCounter = cl.structureCounter;
            

            
            occlusionDrawArgs = cl.occlusionDrawArgs;
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CreateDrawArgumentsCS::CreateDrawArgumentsCS(CreateDrawArgumentsCS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            structureCounter = std::move(cl.structureCounter);
            

            
            occlusionDrawArgs = std::move(cl.occlusionDrawArgs);
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CreateDrawArgumentsCS& CreateDrawArgumentsCS::operator=(const CreateDrawArgumentsCS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            structureCounter = cl.structureCounter;
            

            
            occlusionDrawArgs = cl.occlusionDrawArgs;
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CreateDrawArgumentsCS& CreateDrawArgumentsCS::operator=(CreateDrawArgumentsCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            structureCounter = std::move(cl.structureCounter);
            

            
            occlusionDrawArgs = std::move(cl.occlusionDrawArgs);
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> CreateDrawArgumentsCS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CreateDrawArgumentsCS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CreateDrawArgumentsCS::bufferSrvNames() const
        {
            return {
                
                "structureCounter"
                
                
            };
        }

        std::vector<std::string> CreateDrawArgumentsCS::bufferUavNames() const
        {
            return {
                
                "occlusionDrawArgs"
                
                
            };
        }

        std::vector<std::string> CreateDrawArgumentsCS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CreateDrawArgumentsCS::srvNames() const
        {
            return {
                
                "structureCounter"
                
                
            };
        }

        std::vector<std::string> CreateDrawArgumentsCS::uavNames() const
        {
            return {
                
                "occlusionDrawArgs"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension CreateDrawArgumentsCS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> CreateDrawArgumentsCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> CreateDrawArgumentsCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> CreateDrawArgumentsCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(structureCounter);
            
            return result;
        }

        std::vector<BufferUAV> CreateDrawArgumentsCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            result.emplace_back(occlusionDrawArgs);
            
            return result;
        }

        std::vector<TextureBindlessSRV> CreateDrawArgumentsCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> CreateDrawArgumentsCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> CreateDrawArgumentsCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> CreateDrawArgumentsCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& CreateDrawArgumentsCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> CreateDrawArgumentsCS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& CreateDrawArgumentsCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool CreateDrawArgumentsCS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CreateDrawArgumentsCS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CreateDrawArgumentsCS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("structureCounter")) return true;
            
            
            return false;
        }

        bool CreateDrawArgumentsCS::hasBufferUav(const std::string& name) const
        {
            
            
            if (name == std::string("occlusionDrawArgs")) return true;
            
            
            return false;
        }

        bool CreateDrawArgumentsCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CreateDrawArgumentsCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CreateDrawArgumentsCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CreateDrawArgumentsCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& CreateDrawArgumentsCS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& CreateDrawArgumentsCS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& CreateDrawArgumentsCS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("structureCounter")) return structureCounter;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& CreateDrawArgumentsCS::bufferUav(const std::string& name) const
        {
            
            
            if(name == std::string("occlusionDrawArgs")) return occlusionDrawArgs;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void CreateDrawArgumentsCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CreateDrawArgumentsCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CreateDrawArgumentsCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("structureCounter")) { structureCounter = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CreateDrawArgumentsCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            
            if(name == std::string("occlusionDrawArgs")) { occlusionDrawArgs = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& CreateDrawArgumentsCS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& CreateDrawArgumentsCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& CreateDrawArgumentsCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& CreateDrawArgumentsCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& CreateDrawArgumentsCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t CreateDrawArgumentsCS::descriptorCount() const
        {
            return 2;
        }
    }
}