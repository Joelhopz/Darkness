#include "ClearTexture1df.cs.h"
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
        std::shared_ptr<const ShaderBinary> ClearTexture1dfCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/cleartexture/ClearTexture1df.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/cleartexture/ClearTexture1df.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        ClearTexture1dfCS::ClearTexture1dfCS()
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
        ClearTexture1dfCS::ClearTexture1dfCS(const ClearTexture1dfCS& cl)
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
        ClearTexture1dfCS::ClearTexture1dfCS(ClearTexture1dfCS&& cl)
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
        ClearTexture1dfCS& ClearTexture1dfCS::operator=(const ClearTexture1dfCS& cl)
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
        ClearTexture1dfCS& ClearTexture1dfCS::operator=(ClearTexture1dfCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            
            tex = std::move(cl.tex);
            

            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> ClearTexture1dfCS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClearTexture1dfCS::textureUavNames() const
        {
            return {
                
                "tex"
                
                
            };
        }

        std::vector<std::string> ClearTexture1dfCS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClearTexture1dfCS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClearTexture1dfCS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClearTexture1dfCS::srvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClearTexture1dfCS::uavNames() const
        {
            return {
                
                "tex"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension ClearTexture1dfCS::textureDimension(const std::string& name) const
        {
            
            if("tex" == name) return engine::ResourceDimension::Texture1D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> ClearTexture1dfCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> ClearTexture1dfCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            result.emplace_back(tex);
            
            return result;
        }

        std::vector<BufferSRV> ClearTexture1dfCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> ClearTexture1dfCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> ClearTexture1dfCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> ClearTexture1dfCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> ClearTexture1dfCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> ClearTexture1dfCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& ClearTexture1dfCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> ClearTexture1dfCS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& ClearTexture1dfCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool ClearTexture1dfCS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture1dfCS::hasTextureUav(const std::string& name) const
        {
            
            
            if (name == std::string("tex")) return true;
            
            
            return false;
        }

        bool ClearTexture1dfCS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture1dfCS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture1dfCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture1dfCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture1dfCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClearTexture1dfCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& ClearTexture1dfCS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& ClearTexture1dfCS::textureUav(const std::string& name) const
        {
            
            
            if(name == std::string("tex")) return tex;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& ClearTexture1dfCS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& ClearTexture1dfCS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void ClearTexture1dfCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClearTexture1dfCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            
            if(name == std::string("tex")) { tex = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClearTexture1dfCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClearTexture1dfCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& ClearTexture1dfCS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& ClearTexture1dfCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& ClearTexture1dfCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& ClearTexture1dfCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& ClearTexture1dfCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t ClearTexture1dfCS::descriptorCount() const
        {
            return 2;
        }
    }
}