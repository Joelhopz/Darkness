#include "DispatchBasic.cs.h"
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
        std::shared_ptr<const ShaderBinary> DispatchBasicCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tests/DispatchBasic.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tests/DispatchBasic.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        DispatchBasicCS::DispatchBasicCS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const DispatchConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const DispatchConstants*>(this)) + sizeof(DispatchConstants)),
                    nullptr,
                    "DispatchConstants"
                }
                
            
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"dispatchThreadID", "SV_DispatchThreadID", "uint3"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        DispatchBasicCS::DispatchBasicCS(const DispatchBasicCS& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const DispatchConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const DispatchConstants*>(this)) + sizeof(DispatchConstants)),
                    nullptr,
                    "DispatchConstants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            inputs = cl.inputs;
            

            
            outputs = cl.outputs;
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DispatchBasicCS::DispatchBasicCS(DispatchBasicCS&& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const DispatchConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const DispatchConstants*>(this)) + sizeof(DispatchConstants)),
                    nullptr,
                    "DispatchConstants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            inputs = std::move(cl.inputs);
            

            
            outputs = std::move(cl.outputs);
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DispatchBasicCS& DispatchBasicCS::operator=(const DispatchBasicCS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            inputs = cl.inputs;
            

            
            outputs = cl.outputs;
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DispatchBasicCS& DispatchBasicCS::operator=(DispatchBasicCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            inputs = std::move(cl.inputs);
            

            
            outputs = std::move(cl.outputs);
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> DispatchBasicCS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DispatchBasicCS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DispatchBasicCS::bufferSrvNames() const
        {
            return {
                
                "inputs"
                
                
            };
        }

        std::vector<std::string> DispatchBasicCS::bufferUavNames() const
        {
            return {
                
                "outputs"
                
                
            };
        }

        std::vector<std::string> DispatchBasicCS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DispatchBasicCS::srvNames() const
        {
            return {
                
                "inputs"
                
                
            };
        }

        std::vector<std::string> DispatchBasicCS::uavNames() const
        {
            return {
                
                "outputs"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension DispatchBasicCS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> DispatchBasicCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> DispatchBasicCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> DispatchBasicCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(inputs);
            
            return result;
        }

        std::vector<BufferUAV> DispatchBasicCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            result.emplace_back(outputs);
            
            return result;
        }

        std::vector<TextureBindlessSRV> DispatchBasicCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> DispatchBasicCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> DispatchBasicCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> DispatchBasicCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& DispatchBasicCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> DispatchBasicCS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& DispatchBasicCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool DispatchBasicCS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DispatchBasicCS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DispatchBasicCS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("inputs")) return true;
            
            
            return false;
        }

        bool DispatchBasicCS::hasBufferUav(const std::string& name) const
        {
            
            
            if (name == std::string("outputs")) return true;
            
            
            return false;
        }

        bool DispatchBasicCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DispatchBasicCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DispatchBasicCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DispatchBasicCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& DispatchBasicCS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& DispatchBasicCS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& DispatchBasicCS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("inputs")) return inputs;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& DispatchBasicCS::bufferUav(const std::string& name) const
        {
            
            
            if(name == std::string("outputs")) return outputs;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void DispatchBasicCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DispatchBasicCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DispatchBasicCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("inputs")) { inputs = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DispatchBasicCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            
            if(name == std::string("outputs")) { outputs = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& DispatchBasicCS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& DispatchBasicCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& DispatchBasicCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& DispatchBasicCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& DispatchBasicCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t DispatchBasicCS::descriptorCount() const
        {
            return 3;
        }
    }
}