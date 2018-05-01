#include "ClearTexture1df2.cs.h"
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
        std::shared_ptr<const ShaderBinary> ClearTexture1df2CS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/cleartexture/ClearTexture1df2.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/cleartexture/ClearTexture1df2.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        ClearTexture1df2CS::ClearTexture1df2CS()
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
        ClearTexture1df2CS::ClearTexture1df2CS(const ClearTexture1df2CS& cl)
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
        ClearTexture1df2CS::ClearTexture1df2CS(ClearTexture1df2CS&& cl)
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
        ClearTexture1df2CS& ClearTexture1df2CS::operator=(const ClearTexture1df2CS& cl)
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
        ClearTexture1df2CS& ClearTexture1df2CS::operator=(ClearTexture1df2CS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            
            tex = std::move(cl.tex);
            

            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> ClearTexture1df2CS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClearTexture1df2CS::textureUavNames() const
        {
            return {
                
                "tex"
                
                
            };
        }

        std::vector<std::string> ClearTexture1df2CS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClearTexture1df2CS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClearTexture1df2CS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClearTexture1df2CS::srvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClearTexture1df2CS::uavNames() const
        {
            return {
                
                "tex"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension ClearTexture1df2CS::textureDimension(const std::string& name) const
        {
            
            if("tex" == name) return engine::ResourceDimension::Texture1D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> ClearTexture1df2CS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> ClearTexture1df2CS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            result.emplace_back(tex);
            
            return result;
        }

        std::vector<BufferSRV> ClearTexture1df2CS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> ClearTexture1df2CS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> ClearTexture1df2CS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> ClearTexture1df2CS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> ClearTexture1df2CS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> ClearTexture1df2CS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& ClearTexture1df2CS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> ClearTexture1df2CS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& ClearTexture1df2CS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool ClearTexture1df2CS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture1df2CS::hasTextureUav(const std::string& name) const
        {
            
            
            if (name == std::string("tex")) return true;
            
            
            return false;
        }

        bool ClearTexture1df2CS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture1df2CS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture1df2CS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture1df2CS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture1df2CS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture1df2CS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& ClearTexture1df2CS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& ClearTexture1df2CS::textureUav(const std::string& name) const
        {
            
            
            if(name == std::string("tex")) return tex;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& ClearTexture1df2CS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& ClearTexture1df2CS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void ClearTexture1df2CS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClearTexture1df2CS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            
            if(name == std::string("tex")) { tex = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClearTexture1df2CS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClearTexture1df2CS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& ClearTexture1df2CS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& ClearTexture1df2CS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& ClearTexture1df2CS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& ClearTexture1df2CS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& ClearTexture1df2CS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t ClearTexture1df2CS::descriptorCount() const
        {
            return 2;
        }
    }
}