#include "EquirectToCubemap.ps.h"
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
        std::shared_ptr<const ShaderBinary> EquirectToCubemapPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/EquirectToCubemap.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/EquirectToCubemap.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        EquirectToCubemapPS::EquirectToCubemapPS()
            : m_constantRange{
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"position", "SV_Position0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"normal", "NORMAL", "float4"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        EquirectToCubemapPS::EquirectToCubemapPS(const EquirectToCubemapPS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            equirectangularMap = cl.equirectangularMap;
            

            

            

            

            

            

            

            

            
            equirectangularSampler = cl.equirectangularSampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        EquirectToCubemapPS::EquirectToCubemapPS(EquirectToCubemapPS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            equirectangularMap = std::move(cl.equirectangularMap);
            

            

            

            

            

            

            

            

            
            equirectangularSampler = std::move(cl.equirectangularSampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        EquirectToCubemapPS& EquirectToCubemapPS::operator=(const EquirectToCubemapPS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            equirectangularMap = cl.equirectangularMap;
            

            

            

            

            

            

            

            

            
            equirectangularSampler = cl.equirectangularSampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        EquirectToCubemapPS& EquirectToCubemapPS::operator=(EquirectToCubemapPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            equirectangularMap = std::move(cl.equirectangularMap);
            

            

            

            

            

            

            

            

            
            equirectangularSampler = std::move(cl.equirectangularSampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> EquirectToCubemapPS::textureSrvNames() const
        {
            return {
                
                "equirectangularMap"
                
                
            };
        }

        std::vector<std::string> EquirectToCubemapPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> EquirectToCubemapPS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> EquirectToCubemapPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> EquirectToCubemapPS::samplerNames() const
        {
            return {
                
                "equirectangularSampler"
                
                
            };
        }

        std::vector<std::string> EquirectToCubemapPS::srvNames() const
        {
            return {
                
                "equirectangularMap"
                
                
            };
        }

        std::vector<std::string> EquirectToCubemapPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension EquirectToCubemapPS::textureDimension(const std::string& name) const
        {
            
            if("equirectangularMap" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> EquirectToCubemapPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(equirectangularMap);
            
            return result;
        }

        std::vector<TextureUAV> EquirectToCubemapPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> EquirectToCubemapPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> EquirectToCubemapPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> EquirectToCubemapPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> EquirectToCubemapPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> EquirectToCubemapPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> EquirectToCubemapPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& EquirectToCubemapPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> EquirectToCubemapPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(equirectangularSampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& EquirectToCubemapPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool EquirectToCubemapPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("equirectangularMap")) return true;
            
            
            return false;
        }

        bool EquirectToCubemapPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EquirectToCubemapPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& EquirectToCubemapPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("equirectangularMap")) return equirectangularMap;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& EquirectToCubemapPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& EquirectToCubemapPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& EquirectToCubemapPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void EquirectToCubemapPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("equirectangularMap")) { equirectangularMap = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void EquirectToCubemapPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void EquirectToCubemapPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void EquirectToCubemapPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& EquirectToCubemapPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("equirectangularSampler")) return equirectangularSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& EquirectToCubemapPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& EquirectToCubemapPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& EquirectToCubemapPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& EquirectToCubemapPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t EquirectToCubemapPS::descriptorCount() const
        {
            return 1;
        }
    }
}