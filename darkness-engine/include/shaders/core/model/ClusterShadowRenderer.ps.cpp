#include "ClusterShadowRenderer.ps.h"
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
        std::shared_ptr<const ShaderBinary> ClusterShadowRendererPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/model/ClusterShadowRenderer.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/model/ClusterShadowRenderer.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        ClusterShadowRendererPS::ClusterShadowRendererPS()
            : m_constantRange{
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"position", "SV_Position", "float4"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterShadowRendererPS::ClusterShadowRendererPS(const ClusterShadowRendererPS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterShadowRendererPS::ClusterShadowRendererPS(ClusterShadowRendererPS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterShadowRendererPS& ClusterShadowRendererPS::operator=(const ClusterShadowRendererPS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterShadowRendererPS& ClusterShadowRendererPS::operator=(ClusterShadowRendererPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> ClusterShadowRendererPS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterShadowRendererPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterShadowRendererPS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterShadowRendererPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterShadowRendererPS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterShadowRendererPS::srvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterShadowRendererPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension ClusterShadowRendererPS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> ClusterShadowRendererPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> ClusterShadowRendererPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> ClusterShadowRendererPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> ClusterShadowRendererPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> ClusterShadowRendererPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> ClusterShadowRendererPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> ClusterShadowRendererPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> ClusterShadowRendererPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& ClusterShadowRendererPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> ClusterShadowRendererPS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& ClusterShadowRendererPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool ClusterShadowRendererPS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterShadowRendererPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterShadowRendererPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterShadowRendererPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterShadowRendererPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterShadowRendererPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterShadowRendererPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterShadowRendererPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& ClusterShadowRendererPS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& ClusterShadowRendererPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& ClusterShadowRendererPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& ClusterShadowRendererPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void ClusterShadowRendererPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClusterShadowRendererPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClusterShadowRendererPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClusterShadowRendererPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& ClusterShadowRendererPS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& ClusterShadowRendererPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& ClusterShadowRendererPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& ClusterShadowRendererPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& ClusterShadowRendererPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t ClusterShadowRendererPS::descriptorCount() const
        {
            return 0;
        }
    }
}