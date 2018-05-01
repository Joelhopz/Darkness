#include "SSAOBlur.ps.h"
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
        std::shared_ptr<const ShaderBinary> SSAOBlurPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/ssao/SSAOBlur.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/ssao/SSAOBlur.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        SSAOBlurPS::SSAOBlurPS()
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
            
            ShaderInputParameter{"position", "SV_Position0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"uv", "TEXCOORD0", "float2"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        SSAOBlurPS::SSAOBlurPS(const SSAOBlurPS& cl)
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

            
            image = cl.image;
            

            

            
            blurKernel = cl.blurKernel;
            

            

            

            

            

            

            
            imageSampler = cl.imageSampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        SSAOBlurPS::SSAOBlurPS(SSAOBlurPS&& cl)
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

            
            image = std::move(cl.image);
            

            

            
            blurKernel = std::move(cl.blurKernel);
            

            

            

            

            

            

            
            imageSampler = std::move(cl.imageSampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        SSAOBlurPS& SSAOBlurPS::operator=(const SSAOBlurPS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            image = cl.image;
            

            

            
            blurKernel = cl.blurKernel;
            

            

            

            

            

            

            
            imageSampler = cl.imageSampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        SSAOBlurPS& SSAOBlurPS::operator=(SSAOBlurPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            image = std::move(cl.image);
            

            

            
            blurKernel = std::move(cl.blurKernel);
            

            

            

            

            

            

            
            imageSampler = std::move(cl.imageSampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> SSAOBlurPS::textureSrvNames() const
        {
            return {
                
                "image"
                
                
            };
        }

        std::vector<std::string> SSAOBlurPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> SSAOBlurPS::bufferSrvNames() const
        {
            return {
                
                "blurKernel"
                
                
            };
        }

        std::vector<std::string> SSAOBlurPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> SSAOBlurPS::samplerNames() const
        {
            return {
                
                "imageSampler"
                
                
            };
        }

        std::vector<std::string> SSAOBlurPS::srvNames() const
        {
            return {
                
                "image"
                
                ,
                
                
                "blurKernel"
                
                
            };
        }

        std::vector<std::string> SSAOBlurPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension SSAOBlurPS::textureDimension(const std::string& name) const
        {
            
            if("image" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> SSAOBlurPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(image);
            
            return result;
        }

        std::vector<TextureUAV> SSAOBlurPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> SSAOBlurPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(blurKernel);
            
            return result;
        }

        std::vector<BufferUAV> SSAOBlurPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> SSAOBlurPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> SSAOBlurPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> SSAOBlurPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> SSAOBlurPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& SSAOBlurPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> SSAOBlurPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(imageSampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& SSAOBlurPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool SSAOBlurPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("image")) return true;
            
            
            return false;
        }

        bool SSAOBlurPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurPS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("blurKernel")) return true;
            
            
            return false;
        }

        bool SSAOBlurPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool SSAOBlurPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& SSAOBlurPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("image")) return image;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& SSAOBlurPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& SSAOBlurPS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("blurKernel")) return blurKernel;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& SSAOBlurPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void SSAOBlurPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("image")) { image = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void SSAOBlurPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void SSAOBlurPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("blurKernel")) { blurKernel = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void SSAOBlurPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& SSAOBlurPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("imageSampler")) return imageSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& SSAOBlurPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& SSAOBlurPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& SSAOBlurPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& SSAOBlurPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t SSAOBlurPS::descriptorCount() const
        {
            return 3;
        }
    }
}