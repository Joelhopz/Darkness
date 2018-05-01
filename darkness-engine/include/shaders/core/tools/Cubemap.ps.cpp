#include "Cubemap.ps.h"
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
        std::shared_ptr<const ShaderBinary> CubemapPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/Cubemap.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/Cubemap.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        CubemapPS::CubemapPS()
            : m_constantRange{
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"position", "SV_Position0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"pos", "NORMAL", "float4"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        CubemapPS::CubemapPS(const CubemapPS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            cubemap = cl.cubemap;
            
            irradiance = cl.irradiance;
            
            convolution = cl.convolution;
            

            

            

            

            

            

            

            

            
            cubemapSampler = cl.cubemapSampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CubemapPS::CubemapPS(CubemapPS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            cubemap = std::move(cl.cubemap);
            
            irradiance = std::move(cl.irradiance);
            
            convolution = std::move(cl.convolution);
            

            

            

            

            

            

            

            

            
            cubemapSampler = std::move(cl.cubemapSampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CubemapPS& CubemapPS::operator=(const CubemapPS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            cubemap = cl.cubemap;
            
            irradiance = cl.irradiance;
            
            convolution = cl.convolution;
            

            

            

            

            

            

            

            

            
            cubemapSampler = cl.cubemapSampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        CubemapPS& CubemapPS::operator=(CubemapPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            cubemap = std::move(cl.cubemap);
            
            irradiance = std::move(cl.irradiance);
            
            convolution = std::move(cl.convolution);
            

            

            

            

            

            

            

            

            
            cubemapSampler = std::move(cl.cubemapSampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> CubemapPS::textureSrvNames() const
        {
            return {
                
                "cubemap"
                
                ,
                
                
                "irradiance"
                
                ,
                
                
                "convolution"
                
                
            };
        }

        std::vector<std::string> CubemapPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CubemapPS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CubemapPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> CubemapPS::samplerNames() const
        {
            return {
                
                "cubemapSampler"
                
                
            };
        }

        std::vector<std::string> CubemapPS::srvNames() const
        {
            return {
                
                "cubemap"
                
                ,
                
                
                "irradiance"
                
                ,
                
                
                "convolution"
                
                
            };
        }

        std::vector<std::string> CubemapPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension CubemapPS::textureDimension(const std::string& name) const
        {
            
            if("cubemap" == name) return engine::ResourceDimension::TextureCubemap;
            
            if("irradiance" == name) return engine::ResourceDimension::TextureCubemap;
            
            if("convolution" == name) return engine::ResourceDimension::TextureCubemap;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> CubemapPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(cubemap);
            
            result.emplace_back(irradiance);
            
            result.emplace_back(convolution);
            
            return result;
        }

        std::vector<TextureUAV> CubemapPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> CubemapPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> CubemapPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> CubemapPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> CubemapPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> CubemapPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> CubemapPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& CubemapPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> CubemapPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(cubemapSampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& CubemapPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool CubemapPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("cubemap")) return true;
            
            if(name == std::string("irradiance")) return true;
            
            if(name == std::string("convolution")) return true;
            
            
            return false;
        }

        bool CubemapPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CubemapPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CubemapPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool CubemapPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CubemapPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool CubemapPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool CubemapPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& CubemapPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("cubemap")) return cubemap;
            
            if(name == std::string("irradiance")) return irradiance;
            
            if(name == std::string("convolution")) return convolution;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& CubemapPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& CubemapPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& CubemapPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void CubemapPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("cubemap")) { cubemap = texture; return; }
            
            if(name == std::string("irradiance")) { irradiance = texture; return; }
            
            if(name == std::string("convolution")) { convolution = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CubemapPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CubemapPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void CubemapPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& CubemapPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("cubemapSampler")) return cubemapSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& CubemapPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& CubemapPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& CubemapPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& CubemapPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t CubemapPS::descriptorCount() const
        {
            return 3;
        }
    }
}