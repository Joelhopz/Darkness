#include "CopyTexture2df.cs.h"
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
        std::shared_ptr<const ShaderBinary> CopyTexture2dfCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/copytexture/CopyTexture2df.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/copytexture/CopyTexture2df.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        CopyTexture2dfCS::CopyTexture2dfCS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const CopyConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const CopyConstants*>(this)) + sizeof(CopyConstants)),
                    nullptr,
                    "CopyConstants"
                }
                
            
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"dispatchThreadID", "SV_DispatchThreadID", "uint3"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        CopyTexture2dfCS::CopyTexture2dfCS(const CopyTexture2dfCS& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const CopyConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const CopyConstants*>(this)) + sizeof(CopyConstants)),
                    nullptr,
                    "CopyConstants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            src = cl.src;
            

            
            dst = cl.dst;
            

            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CopyTexture2dfCS::CopyTexture2dfCS(CopyTexture2dfCS&& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const CopyConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const CopyConstants*>(this)) + sizeof(CopyConstants)),
                    nullptr,
                    "CopyConstants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            src = std::move(cl.src);
            

            
            dst = std::move(cl.dst);
            

            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CopyTexture2dfCS& CopyTexture2dfCS::operator=(const CopyTexture2dfCS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            src = cl.src;
            

            
            dst = cl.dst;
            

            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CopyTexture2dfCS& CopyTexture2dfCS::operator=(CopyTexture2dfCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            src = std::move(cl.src);
            

            
            dst = std::move(cl.dst);
            

            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> CopyTexture2dfCS::textureSrvNames() const
        {
            return {
                
                "src"
                
                
            };
        }

        std::vector<std::string> CopyTexture2dfCS::textureUavNames() const
        {
            return {
                
                "dst"
                
                
            };
        }

        std::vector<std::string> CopyTexture2dfCS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CopyTexture2dfCS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CopyTexture2dfCS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CopyTexture2dfCS::srvNames() const
        {
            return {
                
                "src"
                
                
            };
        }

        std::vector<std::string> CopyTexture2dfCS::uavNames() const
        {
            return {
                
                "dst"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension CopyTexture2dfCS::textureDimension(const std::string& name) const
        {
            
            if("src" == name) return engine::ResourceDimension::Texture2D;
            
            if("dst" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> CopyTexture2dfCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(src);
            
            return result;
        }

        std::vector<TextureUAV> CopyTexture2dfCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            result.emplace_back(dst);
            
            return result;
        }

        std::vector<BufferSRV> CopyTexture2dfCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> CopyTexture2dfCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> CopyTexture2dfCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> CopyTexture2dfCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> CopyTexture2dfCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> CopyTexture2dfCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& CopyTexture2dfCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> CopyTexture2dfCS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& CopyTexture2dfCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool CopyTexture2dfCS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("src")) return true;
            
            
            return false;
        }

        bool CopyTexture2dfCS::hasTextureUav(const std::string& name) const
        {
            
            
            if (name == std::string("dst")) return true;
            
            
            return false;
        }

        bool CopyTexture2dfCS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CopyTexture2dfCS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool CopyTexture2dfCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CopyTexture2dfCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CopyTexture2dfCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CopyTexture2dfCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& CopyTexture2dfCS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("src")) return src;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& CopyTexture2dfCS::textureUav(const std::string& name) const
        {
            
            
            if(name == std::string("dst")) return dst;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& CopyTexture2dfCS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& CopyTexture2dfCS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void CopyTexture2dfCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("src")) { src = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CopyTexture2dfCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            
            if(name == std::string("dst")) { dst = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CopyTexture2dfCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CopyTexture2dfCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& CopyTexture2dfCS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& CopyTexture2dfCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& CopyTexture2dfCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& CopyTexture2dfCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& CopyTexture2dfCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t CopyTexture2dfCS::descriptorCount() const
        {
            return 3;
        }
    }
}