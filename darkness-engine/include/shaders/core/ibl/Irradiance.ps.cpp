#include "Irradiance.ps.h"
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
        std::shared_ptr<const ShaderBinary> IrradiancePS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/ibl/Irradiance.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/ibl/Irradiance.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        IrradiancePS::IrradiancePS()
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
        IrradiancePS::IrradiancePS(const IrradiancePS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            environmentMap = cl.environmentMap;
            

            

            

            

            

            

            

            

            
            environmentSampler = cl.environmentSampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        IrradiancePS::IrradiancePS(IrradiancePS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            environmentMap = std::move(cl.environmentMap);
            

            

            

            

            

            

            

            

            
            environmentSampler = std::move(cl.environmentSampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        IrradiancePS& IrradiancePS::operator=(const IrradiancePS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            environmentMap = cl.environmentMap;
            

            

            

            

            

            

            

            

            
            environmentSampler = cl.environmentSampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        IrradiancePS& IrradiancePS::operator=(IrradiancePS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            environmentMap = std::move(cl.environmentMap);
            

            

            

            

            

            

            

            

            
            environmentSampler = std::move(cl.environmentSampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> IrradiancePS::textureSrvNames() const
        {
            return {
                
                "environmentMap"
                
                
            };
        }

        std::vector<std::string> IrradiancePS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> IrradiancePS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> IrradiancePS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> IrradiancePS::samplerNames() const
        {
            return {
                
                "environmentSampler"
                
                
            };
        }

        std::vector<std::string> IrradiancePS::srvNames() const
        {
            return {
                
                "environmentMap"
                
                
            };
        }

        std::vector<std::string> IrradiancePS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension IrradiancePS::textureDimension(const std::string& name) const
        {
            
            if("environmentMap" == name) return engine::ResourceDimension::TextureCubemap;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> IrradiancePS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(environmentMap);
            
            return result;
        }

        std::vector<TextureUAV> IrradiancePS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> IrradiancePS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> IrradiancePS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> IrradiancePS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> IrradiancePS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> IrradiancePS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> IrradiancePS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& IrradiancePS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> IrradiancePS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(environmentSampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& IrradiancePS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool IrradiancePS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("environmentMap")) return true;
            
            
            return false;
        }

        bool IrradiancePS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool IrradiancePS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool IrradiancePS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool IrradiancePS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool IrradiancePS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool IrradiancePS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool IrradiancePS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& IrradiancePS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("environmentMap")) return environmentMap;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& IrradiancePS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& IrradiancePS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& IrradiancePS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void IrradiancePS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("environmentMap")) { environmentMap = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void IrradiancePS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void IrradiancePS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void IrradiancePS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& IrradiancePS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("environmentSampler")) return environmentSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& IrradiancePS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& IrradiancePS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& IrradiancePS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& IrradiancePS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t IrradiancePS::descriptorCount() const
        {
            return 1;
        }
    }
}