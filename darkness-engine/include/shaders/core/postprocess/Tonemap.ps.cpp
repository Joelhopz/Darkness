#include "Tonemap.ps.h"
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
        std::shared_ptr<const ShaderBinary> TonemapPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/postprocess/Tonemap.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/postprocess/Tonemap.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        TonemapPS::TonemapPS()
            : m_constantRange{
            
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
        TonemapPS::TonemapPS(const TonemapPS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            image = cl.image;
            

            

            

            

            

            

            

            

            
            imageSampler = cl.imageSampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        TonemapPS::TonemapPS(TonemapPS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            image = std::move(cl.image);
            

            

            

            

            

            

            

            

            
            imageSampler = std::move(cl.imageSampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        TonemapPS& TonemapPS::operator=(const TonemapPS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            image = cl.image;
            

            

            

            

            

            

            

            

            
            imageSampler = cl.imageSampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        TonemapPS& TonemapPS::operator=(TonemapPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            image = std::move(cl.image);
            

            

            

            

            

            

            

            

            
            imageSampler = std::move(cl.imageSampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> TonemapPS::textureSrvNames() const
        {
            return {
                
                "image"
                
                
            };
        }

        std::vector<std::string> TonemapPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> TonemapPS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> TonemapPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> TonemapPS::samplerNames() const
        {
            return {
                
                "imageSampler"
                
                
            };
        }

        std::vector<std::string> TonemapPS::srvNames() const
        {
            return {
                
                "image"
                
                
            };
        }

        std::vector<std::string> TonemapPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension TonemapPS::textureDimension(const std::string& name) const
        {
            
            if("image" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> TonemapPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(image);
            
            return result;
        }

        std::vector<TextureUAV> TonemapPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> TonemapPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> TonemapPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> TonemapPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> TonemapPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> TonemapPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> TonemapPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& TonemapPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> TonemapPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(imageSampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& TonemapPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool TonemapPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("image")) return true;
            
            
            return false;
        }

        bool TonemapPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool TonemapPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool TonemapPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool TonemapPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool TonemapPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool TonemapPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool TonemapPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& TonemapPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("image")) return image;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& TonemapPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& TonemapPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& TonemapPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void TonemapPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("image")) { image = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void TonemapPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void TonemapPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void TonemapPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& TonemapPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("imageSampler")) return imageSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& TonemapPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& TonemapPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& TonemapPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& TonemapPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t TonemapPS::descriptorCount() const
        {
            return 1;
        }
    }
}