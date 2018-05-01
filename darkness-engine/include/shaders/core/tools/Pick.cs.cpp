#include "Pick.cs.h"
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
        std::shared_ptr<const ShaderBinary> PickCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/Pick.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/Pick.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        PickCS::PickCS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const PickConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const PickConstants*>(this)) + sizeof(PickConstants)),
                    nullptr,
                    "PickConstants"
                }
                
            
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"dispatchThreadID", "SV_DispatchThreadID", "uint3"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        PickCS::PickCS(const PickCS& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const PickConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const PickConstants*>(this)) + sizeof(PickConstants)),
                    nullptr,
                    "PickConstants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            instanceId = cl.instanceId;
            

            

            

            
            output = cl.output;
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        PickCS::PickCS(PickCS&& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const PickConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const PickConstants*>(this)) + sizeof(PickConstants)),
                    nullptr,
                    "PickConstants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            instanceId = std::move(cl.instanceId);
            

            

            

            
            output = std::move(cl.output);
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        PickCS& PickCS::operator=(const PickCS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            instanceId = cl.instanceId;
            

            

            

            
            output = cl.output;
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        PickCS& PickCS::operator=(PickCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            instanceId = std::move(cl.instanceId);
            

            

            

            
            output = std::move(cl.output);
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> PickCS::textureSrvNames() const
        {
            return {
                
                "instanceId"
                
                
            };
        }

        std::vector<std::string> PickCS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> PickCS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> PickCS::bufferUavNames() const
        {
            return {
                
                "output"
                
                
            };
        }

        std::vector<std::string> PickCS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> PickCS::srvNames() const
        {
            return {
                
                "instanceId"
                
                
            };
        }

        std::vector<std::string> PickCS::uavNames() const
        {
            return {
                
                "output"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension PickCS::textureDimension(const std::string& name) const
        {
            
            if("instanceId" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> PickCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(instanceId);
            
            return result;
        }

        std::vector<TextureUAV> PickCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> PickCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> PickCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            result.emplace_back(output);
            
            return result;
        }

        std::vector<TextureBindlessSRV> PickCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> PickCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> PickCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> PickCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& PickCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> PickCS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& PickCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool PickCS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("instanceId")) return true;
            
            
            return false;
        }

        bool PickCS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool PickCS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PickCS::hasBufferUav(const std::string& name) const
        {
            
            
            if (name == std::string("output")) return true;
            
            
            return false;
        }

        bool PickCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PickCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool PickCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PickCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& PickCS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("instanceId")) return instanceId;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& PickCS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& PickCS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& PickCS::bufferUav(const std::string& name) const
        {
            
            
            if(name == std::string("output")) return output;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void PickCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("instanceId")) { instanceId = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void PickCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void PickCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void PickCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            
            if(name == std::string("output")) { output = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& PickCS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& PickCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& PickCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& PickCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& PickCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t PickCS::descriptorCount() const
        {
            return 3;
        }
    }
}