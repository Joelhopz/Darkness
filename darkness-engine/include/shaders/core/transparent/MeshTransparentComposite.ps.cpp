#include "MeshTransparentComposite.ps.h"
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
        std::shared_ptr<const ShaderBinary> MeshTransparentCompositePS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/transparent/MeshTransparentComposite.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/transparent/MeshTransparentComposite.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        MeshTransparentCompositePS::MeshTransparentCompositePS()
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
            
            ShaderInputParameter{"position", "SV_Position", "float4"}
            
            ,
            
            
            ShaderInputParameter{"uv", "TEXCOORD0", "float2"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        MeshTransparentCompositePS::MeshTransparentCompositePS(const MeshTransparentCompositePS& cl)
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

            
            color = cl.color;
            
            alpha = cl.alpha;
            

            

            

            

            

            

            

            

            
            tex_sampler = cl.tex_sampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        MeshTransparentCompositePS::MeshTransparentCompositePS(MeshTransparentCompositePS&& cl)
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

            
            color = std::move(cl.color);
            
            alpha = std::move(cl.alpha);
            

            

            

            

            

            

            

            

            
            tex_sampler = std::move(cl.tex_sampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        MeshTransparentCompositePS& MeshTransparentCompositePS::operator=(const MeshTransparentCompositePS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            color = cl.color;
            
            alpha = cl.alpha;
            

            

            

            

            

            

            

            

            
            tex_sampler = cl.tex_sampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        MeshTransparentCompositePS& MeshTransparentCompositePS::operator=(MeshTransparentCompositePS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            color = std::move(cl.color);
            
            alpha = std::move(cl.alpha);
            

            

            

            

            

            

            

            

            
            tex_sampler = std::move(cl.tex_sampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> MeshTransparentCompositePS::textureSrvNames() const
        {
            return {
                
                "color"
                
                ,
                
                
                "alpha"
                
                
            };
        }

        std::vector<std::string> MeshTransparentCompositePS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MeshTransparentCompositePS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MeshTransparentCompositePS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MeshTransparentCompositePS::samplerNames() const
        {
            return {
                
                "tex_sampler"
                
                
            };
        }

        std::vector<std::string> MeshTransparentCompositePS::srvNames() const
        {
            return {
                
                "color"
                
                ,
                
                
                "alpha"
                
                
            };
        }

        std::vector<std::string> MeshTransparentCompositePS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension MeshTransparentCompositePS::textureDimension(const std::string& name) const
        {
            
            if("color" == name) return engine::ResourceDimension::Texture2D;
            
            if("alpha" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> MeshTransparentCompositePS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(color);
            
            result.emplace_back(alpha);
            
            return result;
        }

        std::vector<TextureUAV> MeshTransparentCompositePS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> MeshTransparentCompositePS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> MeshTransparentCompositePS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> MeshTransparentCompositePS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> MeshTransparentCompositePS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> MeshTransparentCompositePS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> MeshTransparentCompositePS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& MeshTransparentCompositePS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> MeshTransparentCompositePS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(tex_sampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& MeshTransparentCompositePS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool MeshTransparentCompositePS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("color")) return true;
            
            if(name == std::string("alpha")) return true;
            
            
            return false;
        }

        bool MeshTransparentCompositePS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshTransparentCompositePS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshTransparentCompositePS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshTransparentCompositePS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshTransparentCompositePS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshTransparentCompositePS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshTransparentCompositePS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& MeshTransparentCompositePS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("color")) return color;
            
            if(name == std::string("alpha")) return alpha;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& MeshTransparentCompositePS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& MeshTransparentCompositePS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& MeshTransparentCompositePS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void MeshTransparentCompositePS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("color")) { color = texture; return; }
            
            if(name == std::string("alpha")) { alpha = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void MeshTransparentCompositePS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void MeshTransparentCompositePS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void MeshTransparentCompositePS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& MeshTransparentCompositePS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("tex_sampler")) return tex_sampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& MeshTransparentCompositePS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& MeshTransparentCompositePS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& MeshTransparentCompositePS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& MeshTransparentCompositePS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t MeshTransparentCompositePS::descriptorCount() const
        {
            return 3;
        }
    }
}