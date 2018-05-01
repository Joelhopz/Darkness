#include "CycleTransforms.cs.h"
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
        std::shared_ptr<const ShaderBinary> CycleTransformsCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/CycleTransforms.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/CycleTransforms.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        CycleTransformsCS::CycleTransformsCS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const CycleTransformsConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const CycleTransformsConstants*>(this)) + sizeof(CycleTransformsConstants)),
                    nullptr,
                    "CycleTransformsConstants"
                }
                
            
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"dispatchThreadID", "SV_DispatchThreadID", "uint3"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        CycleTransformsCS::CycleTransformsCS(const CycleTransformsCS& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const CycleTransformsConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const CycleTransformsConstants*>(this)) + sizeof(CycleTransformsConstants)),
                    nullptr,
                    "CycleTransformsConstants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            

            
            historyBuffer = cl.historyBuffer;
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CycleTransformsCS::CycleTransformsCS(CycleTransformsCS&& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const CycleTransformsConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const CycleTransformsConstants*>(this)) + sizeof(CycleTransformsConstants)),
                    nullptr,
                    "CycleTransformsConstants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            

            
            historyBuffer = std::move(cl.historyBuffer);
            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CycleTransformsCS& CycleTransformsCS::operator=(const CycleTransformsCS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            

            
            historyBuffer = cl.historyBuffer;
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CycleTransformsCS& CycleTransformsCS::operator=(CycleTransformsCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            

            
            historyBuffer = std::move(cl.historyBuffer);
            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> CycleTransformsCS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CycleTransformsCS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CycleTransformsCS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CycleTransformsCS::bufferUavNames() const
        {
            return {
                
                "historyBuffer"
                
                
            };
        }

        std::vector<std::string> CycleTransformsCS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CycleTransformsCS::srvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CycleTransformsCS::uavNames() const
        {
            return {
                
                "historyBuffer"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension CycleTransformsCS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> CycleTransformsCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> CycleTransformsCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> CycleTransformsCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> CycleTransformsCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            result.emplace_back(historyBuffer);
            
            return result;
        }

        std::vector<TextureBindlessSRV> CycleTransformsCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> CycleTransformsCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> CycleTransformsCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> CycleTransformsCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& CycleTransformsCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> CycleTransformsCS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& CycleTransformsCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool CycleTransformsCS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CycleTransformsCS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CycleTransformsCS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CycleTransformsCS::hasBufferUav(const std::string& name) const
        {
            
            
            if (name == std::string("historyBuffer")) return true;
            
            
            return false;
        }

        bool CycleTransformsCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CycleTransformsCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CycleTransformsCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CycleTransformsCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& CycleTransformsCS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& CycleTransformsCS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& CycleTransformsCS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& CycleTransformsCS::bufferUav(const std::string& name) const
        {
            
            
            if(name == std::string("historyBuffer")) return historyBuffer;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void CycleTransformsCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CycleTransformsCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CycleTransformsCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CycleTransformsCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            
            if(name == std::string("historyBuffer")) { historyBuffer = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& CycleTransformsCS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& CycleTransformsCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& CycleTransformsCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& CycleTransformsCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& CycleTransformsCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t CycleTransformsCS::descriptorCount() const
        {
            return 2;
        }
    }
}