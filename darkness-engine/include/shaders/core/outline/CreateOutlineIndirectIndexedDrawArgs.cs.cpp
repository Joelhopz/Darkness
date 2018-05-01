#include "CreateOutlineIndirectIndexedDrawArgs.cs.h"
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
        std::shared_ptr<const ShaderBinary> CreateOutlineIndirectIndexedDrawArgsCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/outline/CreateOutlineIndirectIndexedDrawArgs.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/outline/CreateOutlineIndirectIndexedDrawArgs.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        CreateOutlineIndirectIndexedDrawArgsCS::CreateOutlineIndirectIndexedDrawArgsCS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const OutlineConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const OutlineConstants*>(this)) + sizeof(OutlineConstants)),
                    nullptr,
                    "OutlineConstants"
                }
                
            
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"dispatchThreadID", "SV_DispatchThreadID", "uint3"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        CreateOutlineIndirectIndexedDrawArgsCS::CreateOutlineIndirectIndexedDrawArgsCS(const CreateOutlineIndirectIndexedDrawArgsCS& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const OutlineConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const OutlineConstants*>(this)) + sizeof(OutlineConstants)),
                    nullptr,
                    "OutlineConstants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            subMeshBinding = cl.subMeshBinding;
            
            subMeshAdjacency = cl.subMeshAdjacency;
            

            
            indexedDrawArguments = cl.indexedDrawArguments;
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CreateOutlineIndirectIndexedDrawArgsCS::CreateOutlineIndirectIndexedDrawArgsCS(CreateOutlineIndirectIndexedDrawArgsCS&& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const OutlineConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const OutlineConstants*>(this)) + sizeof(OutlineConstants)),
                    nullptr,
                    "OutlineConstants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            subMeshBinding = std::move(cl.subMeshBinding);
            
            subMeshAdjacency = std::move(cl.subMeshAdjacency);
            

            
            indexedDrawArguments = std::move(cl.indexedDrawArguments);
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CreateOutlineIndirectIndexedDrawArgsCS& CreateOutlineIndirectIndexedDrawArgsCS::operator=(const CreateOutlineIndirectIndexedDrawArgsCS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            subMeshBinding = cl.subMeshBinding;
            
            subMeshAdjacency = cl.subMeshAdjacency;
            

            
            indexedDrawArguments = cl.indexedDrawArguments;
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CreateOutlineIndirectIndexedDrawArgsCS& CreateOutlineIndirectIndexedDrawArgsCS::operator=(CreateOutlineIndirectIndexedDrawArgsCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            subMeshBinding = std::move(cl.subMeshBinding);
            
            subMeshAdjacency = std::move(cl.subMeshAdjacency);
            

            
            indexedDrawArguments = std::move(cl.indexedDrawArguments);
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> CreateOutlineIndirectIndexedDrawArgsCS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CreateOutlineIndirectIndexedDrawArgsCS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CreateOutlineIndirectIndexedDrawArgsCS::bufferSrvNames() const
        {
            return {
                
                "subMeshBinding"
                
                ,
                
                
                "subMeshAdjacency"
                
                
            };
        }

        std::vector<std::string> CreateOutlineIndirectIndexedDrawArgsCS::bufferUavNames() const
        {
            return {
                
                "indexedDrawArguments"
                
                
            };
        }

        std::vector<std::string> CreateOutlineIndirectIndexedDrawArgsCS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CreateOutlineIndirectIndexedDrawArgsCS::srvNames() const
        {
            return {
                
                "subMeshBinding"
                
                ,
                
                
                "subMeshAdjacency"
                
                
            };
        }

        std::vector<std::string> CreateOutlineIndirectIndexedDrawArgsCS::uavNames() const
        {
            return {
                
                "indexedDrawArguments"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension CreateOutlineIndirectIndexedDrawArgsCS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> CreateOutlineIndirectIndexedDrawArgsCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> CreateOutlineIndirectIndexedDrawArgsCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> CreateOutlineIndirectIndexedDrawArgsCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(subMeshBinding);
            
            result.emplace_back(subMeshAdjacency);
            
            return result;
        }

        std::vector<BufferUAV> CreateOutlineIndirectIndexedDrawArgsCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            result.emplace_back(indexedDrawArguments);
            
            return result;
        }

        std::vector<TextureBindlessSRV> CreateOutlineIndirectIndexedDrawArgsCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> CreateOutlineIndirectIndexedDrawArgsCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> CreateOutlineIndirectIndexedDrawArgsCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> CreateOutlineIndirectIndexedDrawArgsCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& CreateOutlineIndirectIndexedDrawArgsCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> CreateOutlineIndirectIndexedDrawArgsCS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& CreateOutlineIndirectIndexedDrawArgsCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool CreateOutlineIndirectIndexedDrawArgsCS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CreateOutlineIndirectIndexedDrawArgsCS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CreateOutlineIndirectIndexedDrawArgsCS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("subMeshBinding")) return true;
            
            if (name == std::string("subMeshAdjacency")) return true;
            
            
            return false;
        }

        bool CreateOutlineIndirectIndexedDrawArgsCS::hasBufferUav(const std::string& name) const
        {
            
            
            if (name == std::string("indexedDrawArguments")) return true;
            
            
            return false;
        }

        bool CreateOutlineIndirectIndexedDrawArgsCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CreateOutlineIndirectIndexedDrawArgsCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CreateOutlineIndirectIndexedDrawArgsCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CreateOutlineIndirectIndexedDrawArgsCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& CreateOutlineIndirectIndexedDrawArgsCS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& CreateOutlineIndirectIndexedDrawArgsCS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& CreateOutlineIndirectIndexedDrawArgsCS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("subMeshBinding")) return subMeshBinding;
            
            if(name == std::string("subMeshAdjacency")) return subMeshAdjacency;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& CreateOutlineIndirectIndexedDrawArgsCS::bufferUav(const std::string& name) const
        {
            
            
            if(name == std::string("indexedDrawArguments")) return indexedDrawArguments;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void CreateOutlineIndirectIndexedDrawArgsCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CreateOutlineIndirectIndexedDrawArgsCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CreateOutlineIndirectIndexedDrawArgsCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("subMeshBinding")) { subMeshBinding = buffer; return; }
            
            if(name == std::string("subMeshAdjacency")) { subMeshAdjacency = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CreateOutlineIndirectIndexedDrawArgsCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            
            if(name == std::string("indexedDrawArguments")) { indexedDrawArguments = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& CreateOutlineIndirectIndexedDrawArgsCS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& CreateOutlineIndirectIndexedDrawArgsCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& CreateOutlineIndirectIndexedDrawArgsCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& CreateOutlineIndirectIndexedDrawArgsCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& CreateOutlineIndirectIndexedDrawArgsCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t CreateOutlineIndirectIndexedDrawArgsCS::descriptorCount() const
        {
            return 4;
        }
    }
}