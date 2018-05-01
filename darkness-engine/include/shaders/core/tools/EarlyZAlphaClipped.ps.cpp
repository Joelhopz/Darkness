#include "EarlyZAlphaClipped.ps.h"
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
        std::shared_ptr<const ShaderBinary> EarlyZAlphaClippedPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/EarlyZAlphaClipped.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/EarlyZAlphaClipped.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        EarlyZAlphaClippedPS::EarlyZAlphaClippedPS()
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
        EarlyZAlphaClippedPS::EarlyZAlphaClippedPS(const EarlyZAlphaClippedPS& cl)
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
        EarlyZAlphaClippedPS::EarlyZAlphaClippedPS(EarlyZAlphaClippedPS&& cl)
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
        EarlyZAlphaClippedPS& EarlyZAlphaClippedPS::operator=(const EarlyZAlphaClippedPS& cl)
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
        EarlyZAlphaClippedPS& EarlyZAlphaClippedPS::operator=(EarlyZAlphaClippedPS&& cl)
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

        std::vector<std::string> EarlyZAlphaClippedPS::textureSrvNames() const
        {
            return {
                
                "image"
                
                
            };
        }

        std::vector<std::string> EarlyZAlphaClippedPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> EarlyZAlphaClippedPS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> EarlyZAlphaClippedPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> EarlyZAlphaClippedPS::samplerNames() const
        {
            return {
                
                "imageSampler"
                
                
            };
        }

        std::vector<std::string> EarlyZAlphaClippedPS::srvNames() const
        {
            return {
                
                "image"
                
                
            };
        }

        std::vector<std::string> EarlyZAlphaClippedPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension EarlyZAlphaClippedPS::textureDimension(const std::string& name) const
        {
            
            if("image" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> EarlyZAlphaClippedPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(image);
            
            return result;
        }

        std::vector<TextureUAV> EarlyZAlphaClippedPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> EarlyZAlphaClippedPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> EarlyZAlphaClippedPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> EarlyZAlphaClippedPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> EarlyZAlphaClippedPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> EarlyZAlphaClippedPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> EarlyZAlphaClippedPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& EarlyZAlphaClippedPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> EarlyZAlphaClippedPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(imageSampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& EarlyZAlphaClippedPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool EarlyZAlphaClippedPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("image")) return true;
            
            
            return false;
        }

        bool EarlyZAlphaClippedPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZAlphaClippedPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& EarlyZAlphaClippedPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("image")) return image;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& EarlyZAlphaClippedPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& EarlyZAlphaClippedPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& EarlyZAlphaClippedPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void EarlyZAlphaClippedPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("image")) { image = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void EarlyZAlphaClippedPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void EarlyZAlphaClippedPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void EarlyZAlphaClippedPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& EarlyZAlphaClippedPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("imageSampler")) return imageSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& EarlyZAlphaClippedPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& EarlyZAlphaClippedPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& EarlyZAlphaClippedPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& EarlyZAlphaClippedPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t EarlyZAlphaClippedPS::descriptorCount() const
        {
            return 1;
        }
    }
}