#include "DepthDownsampleSingle.cs.h"
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
        std::shared_ptr<const ShaderBinary> DepthDownsampleSingleCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/DepthDownsampleSingle.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/DepthDownsampleSingle.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        DepthDownsampleSingleCS::DepthDownsampleSingleCS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)) + sizeof(Constants)),
                    nullptr,
                    "Constants"
                }
                
            
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"groupIndex", "SV_GroupIndex", "uint"}
            
            ,
            
            
            ShaderInputParameter{"dispatchThreadID", "SV_DispatchThreadID", "uint3"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        DepthDownsampleSingleCS::DepthDownsampleSingleCS(const DepthDownsampleSingleCS& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)) + sizeof(Constants)),
                    nullptr,
                    "Constants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            src = cl.src;
            

            
            dst = cl.dst;
            

            

            

            

            

            

            

            
            srcSampler = cl.srcSampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DepthDownsampleSingleCS::DepthDownsampleSingleCS(DepthDownsampleSingleCS&& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)) + sizeof(Constants)),
                    nullptr,
                    "Constants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            src = std::move(cl.src);
            

            
            dst = std::move(cl.dst);
            

            

            

            

            

            

            

            
            srcSampler = std::move(cl.srcSampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DepthDownsampleSingleCS& DepthDownsampleSingleCS::operator=(const DepthDownsampleSingleCS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            src = cl.src;
            

            
            dst = cl.dst;
            

            

            

            

            

            

            

            
            srcSampler = cl.srcSampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DepthDownsampleSingleCS& DepthDownsampleSingleCS::operator=(DepthDownsampleSingleCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            src = std::move(cl.src);
            

            
            dst = std::move(cl.dst);
            

            

            

            

            

            

            

            
            srcSampler = std::move(cl.srcSampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> DepthDownsampleSingleCS::textureSrvNames() const
        {
            return {
                
                "src"
                
                
            };
        }

        std::vector<std::string> DepthDownsampleSingleCS::textureUavNames() const
        {
            return {
                
                "dst"
                
                
            };
        }

        std::vector<std::string> DepthDownsampleSingleCS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DepthDownsampleSingleCS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DepthDownsampleSingleCS::samplerNames() const
        {
            return {
                
                "srcSampler"
                
                
            };
        }

        std::vector<std::string> DepthDownsampleSingleCS::srvNames() const
        {
            return {
                
                "src"
                
                
            };
        }

        std::vector<std::string> DepthDownsampleSingleCS::uavNames() const
        {
            return {
                
                "dst"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension DepthDownsampleSingleCS::textureDimension(const std::string& name) const
        {
            
            if("src" == name) return engine::ResourceDimension::Texture2D;
            
            if("dst" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> DepthDownsampleSingleCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(src);
            
            return result;
        }

        std::vector<TextureUAV> DepthDownsampleSingleCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            result.emplace_back(dst);
            
            return result;
        }

        std::vector<BufferSRV> DepthDownsampleSingleCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> DepthDownsampleSingleCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> DepthDownsampleSingleCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> DepthDownsampleSingleCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> DepthDownsampleSingleCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> DepthDownsampleSingleCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& DepthDownsampleSingleCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> DepthDownsampleSingleCS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(srcSampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& DepthDownsampleSingleCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool DepthDownsampleSingleCS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("src")) return true;
            
            
            return false;
        }

        bool DepthDownsampleSingleCS::hasTextureUav(const std::string& name) const
        {
            
            
            if (name == std::string("dst")) return true;
            
            
            return false;
        }

        bool DepthDownsampleSingleCS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DepthDownsampleSingleCS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool DepthDownsampleSingleCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DepthDownsampleSingleCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DepthDownsampleSingleCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DepthDownsampleSingleCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& DepthDownsampleSingleCS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("src")) return src;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& DepthDownsampleSingleCS::textureUav(const std::string& name) const
        {
            
            
            if(name == std::string("dst")) return dst;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& DepthDownsampleSingleCS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& DepthDownsampleSingleCS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void DepthDownsampleSingleCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("src")) { src = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DepthDownsampleSingleCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            
            if(name == std::string("dst")) { dst = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DepthDownsampleSingleCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DepthDownsampleSingleCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& DepthDownsampleSingleCS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("srcSampler")) return srcSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& DepthDownsampleSingleCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& DepthDownsampleSingleCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& DepthDownsampleSingleCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& DepthDownsampleSingleCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t DepthDownsampleSingleCS::descriptorCount() const
        {
            return 3;
        }
    }
}