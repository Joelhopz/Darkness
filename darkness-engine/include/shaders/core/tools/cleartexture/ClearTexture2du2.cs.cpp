#include "ClearTexture2du2.cs.h"
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
        std::shared_ptr<const ShaderBinary> ClearTexture2du2CS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/cleartexture/ClearTexture2du2.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/cleartexture/ClearTexture2du2.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        ClearTexture2du2CS::ClearTexture2du2CS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const ClearConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const ClearConstants*>(this)) + sizeof(ClearConstants)),
                    nullptr,
                    "ClearConstants"
                }
                
            
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"dispatchThreadID", "SV_DispatchThreadID", "uint3"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClearTexture2du2CS::ClearTexture2du2CS(const ClearTexture2du2CS& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const ClearConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const ClearConstants*>(this)) + sizeof(ClearConstants)),
                    nullptr,
                    "ClearConstants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            
            tex = cl.tex;
            

            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClearTexture2du2CS::ClearTexture2du2CS(ClearTexture2du2CS&& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const ClearConstants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const ClearConstants*>(this)) + sizeof(ClearConstants)),
                    nullptr,
                    "ClearConstants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            
            tex = std::move(cl.tex);
            

            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClearTexture2du2CS& ClearTexture2du2CS::operator=(const ClearTexture2du2CS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            
            tex = cl.tex;
            

            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClearTexture2du2CS& ClearTexture2du2CS::operator=(ClearTexture2du2CS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            
            tex = std::move(cl.tex);
            

            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> ClearTexture2du2CS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClearTexture2du2CS::textureUavNames() const
        {
            return {
                
                "tex"
                
                
            };
        }

        std::vector<std::string> ClearTexture2du2CS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClearTexture2du2CS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClearTexture2du2CS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClearTexture2du2CS::srvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClearTexture2du2CS::uavNames() const
        {
            return {
                
                "tex"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension ClearTexture2du2CS::textureDimension(const std::string& name) const
        {
            
            if("tex" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> ClearTexture2du2CS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> ClearTexture2du2CS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            result.emplace_back(tex);
            
            return result;
        }

        std::vector<BufferSRV> ClearTexture2du2CS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> ClearTexture2du2CS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> ClearTexture2du2CS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> ClearTexture2du2CS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> ClearTexture2du2CS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> ClearTexture2du2CS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& ClearTexture2du2CS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> ClearTexture2du2CS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& ClearTexture2du2CS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool ClearTexture2du2CS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture2du2CS::hasTextureUav(const std::string& name) const
        {
            
            
            if (name == std::string("tex")) return true;
            
            
            return false;
        }

        bool ClearTexture2du2CS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture2du2CS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture2du2CS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture2du2CS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture2du2CS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture2du2CS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& ClearTexture2du2CS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& ClearTexture2du2CS::textureUav(const std::string& name) const
        {
            
            
            if(name == std::string("tex")) return tex;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& ClearTexture2du2CS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& ClearTexture2du2CS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void ClearTexture2du2CS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClearTexture2du2CS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            
            if(name == std::string("tex")) { tex = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClearTexture2du2CS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClearTexture2du2CS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& ClearTexture2du2CS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& ClearTexture2du2CS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& ClearTexture2du2CS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& ClearTexture2du2CS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& ClearTexture2du2CS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t ClearTexture2du2CS::descriptorCount() const
        {
            return 2;
        }
    }
}