#include "MrtVisualize.vs.h"
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
        std::shared_ptr<const ShaderBinary> MrtVisualizeVS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tests/MrtVisualize.vs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tests/MrtVisualize.vs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        MrtVisualizeVS::MrtVisualizeVS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)) + sizeof(ConstData)),
                    nullptr,
                    "ConstData"
                }
                
            
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"id", "SV_VertexID", "uint"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        MrtVisualizeVS::MrtVisualizeVS(const MrtVisualizeVS& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)) + sizeof(ConstData)),
                    nullptr,
                    "ConstData"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        MrtVisualizeVS::MrtVisualizeVS(MrtVisualizeVS&& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)) + sizeof(ConstData)),
                    nullptr,
                    "ConstData"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        MrtVisualizeVS& MrtVisualizeVS::operator=(const MrtVisualizeVS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        MrtVisualizeVS& MrtVisualizeVS::operator=(MrtVisualizeVS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> MrtVisualizeVS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MrtVisualizeVS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MrtVisualizeVS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MrtVisualizeVS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MrtVisualizeVS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MrtVisualizeVS::srvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MrtVisualizeVS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension MrtVisualizeVS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> MrtVisualizeVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> MrtVisualizeVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> MrtVisualizeVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> MrtVisualizeVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> MrtVisualizeVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> MrtVisualizeVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> MrtVisualizeVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> MrtVisualizeVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& MrtVisualizeVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> MrtVisualizeVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& MrtVisualizeVS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool MrtVisualizeVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MrtVisualizeVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool MrtVisualizeVS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MrtVisualizeVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool MrtVisualizeVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MrtVisualizeVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool MrtVisualizeVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MrtVisualizeVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& MrtVisualizeVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& MrtVisualizeVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& MrtVisualizeVS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& MrtVisualizeVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void MrtVisualizeVS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void MrtVisualizeVS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void MrtVisualizeVS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void MrtVisualizeVS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& MrtVisualizeVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& MrtVisualizeVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& MrtVisualizeVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& MrtVisualizeVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& MrtVisualizeVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t MrtVisualizeVS::descriptorCount() const
        {
            return 1;
        }
    }
}