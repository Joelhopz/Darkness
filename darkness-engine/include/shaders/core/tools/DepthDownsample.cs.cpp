#include "DepthDownsample.cs.h"
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
        std::shared_ptr<const ShaderBinary> DepthDownsampleCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/DepthDownsample.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/DepthDownsample.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        DepthDownsampleCS::DepthDownsampleCS()
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
        DepthDownsampleCS::DepthDownsampleCS(const DepthDownsampleCS& cl)
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
            

            
            dst0 = cl.dst0;
            
            dst1 = cl.dst1;
            
            dst2 = cl.dst2;
            
            dst3 = cl.dst3;
            

            

            

            

            

            

            

            
            srcSampler = cl.srcSampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DepthDownsampleCS::DepthDownsampleCS(DepthDownsampleCS&& cl)
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
            

            
            dst0 = std::move(cl.dst0);
            
            dst1 = std::move(cl.dst1);
            
            dst2 = std::move(cl.dst2);
            
            dst3 = std::move(cl.dst3);
            

            

            

            

            

            

            

            
            srcSampler = std::move(cl.srcSampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DepthDownsampleCS& DepthDownsampleCS::operator=(const DepthDownsampleCS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            src = cl.src;
            

            
            dst0 = cl.dst0;
            
            dst1 = cl.dst1;
            
            dst2 = cl.dst2;
            
            dst3 = cl.dst3;
            

            

            

            

            

            

            

            
            srcSampler = cl.srcSampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DepthDownsampleCS& DepthDownsampleCS::operator=(DepthDownsampleCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            src = std::move(cl.src);
            

            
            dst0 = std::move(cl.dst0);
            
            dst1 = std::move(cl.dst1);
            
            dst2 = std::move(cl.dst2);
            
            dst3 = std::move(cl.dst3);
            

            

            

            

            

            

            

            
            srcSampler = std::move(cl.srcSampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> DepthDownsampleCS::textureSrvNames() const
        {
            return {
                
                "src"
                
                
            };
        }

        std::vector<std::string> DepthDownsampleCS::textureUavNames() const
        {
            return {
                
                "dst0"
                
                ,
                
                
                "dst1"
                
                ,
                
                
                "dst2"
                
                ,
                
                
                "dst3"
                
                
            };
        }

        std::vector<std::string> DepthDownsampleCS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DepthDownsampleCS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DepthDownsampleCS::samplerNames() const
        {
            return {
                
                "srcSampler"
                
                
            };
        }

        std::vector<std::string> DepthDownsampleCS::srvNames() const
        {
            return {
                
                "src"
                
                
            };
        }

        std::vector<std::string> DepthDownsampleCS::uavNames() const
        {
            return {
                
                "dst0"
                
                ,
                
                
                "dst1"
                
                ,
                
                
                "dst2"
                
                ,
                
                
                "dst3"
                
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension DepthDownsampleCS::textureDimension(const std::string& name) const
        {
            
            if("src" == name) return engine::ResourceDimension::Texture2D;
            
            if("dst0" == name) return engine::ResourceDimension::Texture2D;
            
            if("dst1" == name) return engine::ResourceDimension::Texture2D;
            
            if("dst2" == name) return engine::ResourceDimension::Texture2D;
            
            if("dst3" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> DepthDownsampleCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(src);
            
            return result;
        }

        std::vector<TextureUAV> DepthDownsampleCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            result.emplace_back(dst0);
            
            result.emplace_back(dst1);
            
            result.emplace_back(dst2);
            
            result.emplace_back(dst3);
            
            return result;
        }

        std::vector<BufferSRV> DepthDownsampleCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> DepthDownsampleCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> DepthDownsampleCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> DepthDownsampleCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> DepthDownsampleCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> DepthDownsampleCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& DepthDownsampleCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> DepthDownsampleCS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(srcSampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& DepthDownsampleCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool DepthDownsampleCS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("src")) return true;
            
            
            return false;
        }

        bool DepthDownsampleCS::hasTextureUav(const std::string& name) const
        {
            
            
            if (name == std::string("dst0")) return true;
            
            if (name == std::string("dst1")) return true;
            
            if (name == std::string("dst2")) return true;
            
            if (name == std::string("dst3")) return true;
            
            
            return false;
        }

        bool DepthDownsampleCS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DepthDownsampleCS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool DepthDownsampleCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DepthDownsampleCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DepthDownsampleCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DepthDownsampleCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& DepthDownsampleCS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("src")) return src;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& DepthDownsampleCS::textureUav(const std::string& name) const
        {
            
            
            if(name == std::string("dst0")) return dst0;
            
            if(name == std::string("dst1")) return dst1;
            
            if(name == std::string("dst2")) return dst2;
            
            if(name == std::string("dst3")) return dst3;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& DepthDownsampleCS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& DepthDownsampleCS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void DepthDownsampleCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("src")) { src = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DepthDownsampleCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            
            if(name == std::string("dst0")) { dst0 = texture; return; }
            
            if(name == std::string("dst1")) { dst1 = texture; return; }
            
            if(name == std::string("dst2")) { dst2 = texture; return; }
            
            if(name == std::string("dst3")) { dst3 = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DepthDownsampleCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DepthDownsampleCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& DepthDownsampleCS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("srcSampler")) return srcSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& DepthDownsampleCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& DepthDownsampleCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& DepthDownsampleCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& DepthDownsampleCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t DepthDownsampleCS::descriptorCount() const
        {
            return 6;
        }
    }
}