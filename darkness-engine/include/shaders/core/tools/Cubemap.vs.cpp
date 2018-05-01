#include "Cubemap.vs.h"
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
        std::shared_ptr<const ShaderBinary> CubemapVS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/Cubemap.vs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/Cubemap.vs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        CubemapVS::CubemapVS()
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
        CubemapVS::CubemapVS(const CubemapVS& cl)
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
        CubemapVS::CubemapVS(CubemapVS&& cl)
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
        CubemapVS& CubemapVS::operator=(const CubemapVS& cl)
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
        CubemapVS& CubemapVS::operator=(CubemapVS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> CubemapVS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CubemapVS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CubemapVS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CubemapVS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CubemapVS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CubemapVS::srvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CubemapVS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension CubemapVS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> CubemapVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> CubemapVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> CubemapVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> CubemapVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> CubemapVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> CubemapVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> CubemapVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> CubemapVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& CubemapVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> CubemapVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& CubemapVS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool CubemapVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CubemapVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CubemapVS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CubemapVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool CubemapVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CubemapVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CubemapVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CubemapVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& CubemapVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& CubemapVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& CubemapVS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& CubemapVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void CubemapVS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CubemapVS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CubemapVS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CubemapVS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& CubemapVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& CubemapVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& CubemapVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& CubemapVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& CubemapVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t CubemapVS::descriptorCount() const
        {
            return 1;
        }
    }
}