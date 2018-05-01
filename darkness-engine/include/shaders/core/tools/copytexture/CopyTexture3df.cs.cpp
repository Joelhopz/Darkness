#include "CopyTexture3df.cs.h"
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
        std::shared_ptr<const ShaderBinary> CopyTexture3dfCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/copytexture/CopyTexture3df.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/copytexture/CopyTexture3df.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        CopyTexture3dfCS::CopyTexture3dfCS()
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
        CopyTexture3dfCS::CopyTexture3dfCS(const CopyTexture3dfCS& cl)
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
        CopyTexture3dfCS::CopyTexture3dfCS(CopyTexture3dfCS&& cl)
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
        CopyTexture3dfCS& CopyTexture3dfCS::operator=(const CopyTexture3dfCS& cl)
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
        CopyTexture3dfCS& CopyTexture3dfCS::operator=(CopyTexture3dfCS&& cl)
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

        std::vector<std::string> CopyTexture3dfCS::textureSrvNames() const
        {
            return {
                
                "src"
                
                
            };
        }

        std::vector<std::string> CopyTexture3dfCS::textureUavNames() const
        {
            return {
                
                "dst"
                
                
            };
        }

        std::vector<std::string> CopyTexture3dfCS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CopyTexture3dfCS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CopyTexture3dfCS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CopyTexture3dfCS::srvNames() const
        {
            return {
                
                "src"
                
                
            };
        }

        std::vector<std::string> CopyTexture3dfCS::uavNames() const
        {
            return {
                
                "dst"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension CopyTexture3dfCS::textureDimension(const std::string& name) const
        {
            
            if("src" == name) return engine::ResourceDimension::Texture3D;
            
            if("dst" == name) return engine::ResourceDimension::Texture3D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> CopyTexture3dfCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(src);
            
            return result;
        }

        std::vector<TextureUAV> CopyTexture3dfCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            result.emplace_back(dst);
            
            return result;
        }

        std::vector<BufferSRV> CopyTexture3dfCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> CopyTexture3dfCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> CopyTexture3dfCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> CopyTexture3dfCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> CopyTexture3dfCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> CopyTexture3dfCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& CopyTexture3dfCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> CopyTexture3dfCS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& CopyTexture3dfCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool CopyTexture3dfCS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("src")) return true;
            
            
            return false;
        }

        bool CopyTexture3dfCS::hasTextureUav(const std::string& name) const
        {
            
            
            if (name == std::string("dst")) return true;
            
            
            return false;
        }

        bool CopyTexture3dfCS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CopyTexture3dfCS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool CopyTexture3dfCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CopyTexture3dfCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CopyTexture3dfCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CopyTexture3dfCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& CopyTexture3dfCS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("src")) return src;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& CopyTexture3dfCS::textureUav(const std::string& name) const
        {
            
            
            if(name == std::string("dst")) return dst;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& CopyTexture3dfCS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& CopyTexture3dfCS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void CopyTexture3dfCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("src")) { src = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CopyTexture3dfCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            
            if(name == std::string("dst")) { dst = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CopyTexture3dfCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CopyTexture3dfCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& CopyTexture3dfCS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& CopyTexture3dfCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& CopyTexture3dfCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& CopyTexture3dfCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& CopyTexture3dfCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t CopyTexture3dfCS::descriptorCount() const
        {
            return 3;
        }
    }
}