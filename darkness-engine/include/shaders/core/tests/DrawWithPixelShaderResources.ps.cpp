#include "DrawWithPixelShaderResources.ps.h"
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
        std::shared_ptr<const ShaderBinary> DrawWithPixelShaderResourcesPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tests/DrawWithPixelShaderResources.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tests/DrawWithPixelShaderResources.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        DrawWithPixelShaderResourcesPS::DrawWithPixelShaderResourcesPS()
            : m_constantRange{
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"position", "SV_Position", "float4"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        DrawWithPixelShaderResourcesPS::DrawWithPixelShaderResourcesPS(const DrawWithPixelShaderResourcesPS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            color = cl.color;
            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DrawWithPixelShaderResourcesPS::DrawWithPixelShaderResourcesPS(DrawWithPixelShaderResourcesPS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            color = std::move(cl.color);
            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DrawWithPixelShaderResourcesPS& DrawWithPixelShaderResourcesPS::operator=(const DrawWithPixelShaderResourcesPS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            color = cl.color;
            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DrawWithPixelShaderResourcesPS& DrawWithPixelShaderResourcesPS::operator=(DrawWithPixelShaderResourcesPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            color = std::move(cl.color);
            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> DrawWithPixelShaderResourcesPS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DrawWithPixelShaderResourcesPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DrawWithPixelShaderResourcesPS::bufferSrvNames() const
        {
            return {
                
                "color"
                
                
            };
        }

        std::vector<std::string> DrawWithPixelShaderResourcesPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DrawWithPixelShaderResourcesPS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DrawWithPixelShaderResourcesPS::srvNames() const
        {
            return {
                
                "color"
                
                
            };
        }

        std::vector<std::string> DrawWithPixelShaderResourcesPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension DrawWithPixelShaderResourcesPS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> DrawWithPixelShaderResourcesPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> DrawWithPixelShaderResourcesPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> DrawWithPixelShaderResourcesPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(color);
            
            return result;
        }

        std::vector<BufferUAV> DrawWithPixelShaderResourcesPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> DrawWithPixelShaderResourcesPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> DrawWithPixelShaderResourcesPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> DrawWithPixelShaderResourcesPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> DrawWithPixelShaderResourcesPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& DrawWithPixelShaderResourcesPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> DrawWithPixelShaderResourcesPS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& DrawWithPixelShaderResourcesPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool DrawWithPixelShaderResourcesPS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithPixelShaderResourcesPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithPixelShaderResourcesPS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("color")) return true;
            
            
            return false;
        }

        bool DrawWithPixelShaderResourcesPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithPixelShaderResourcesPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithPixelShaderResourcesPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithPixelShaderResourcesPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithPixelShaderResourcesPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& DrawWithPixelShaderResourcesPS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& DrawWithPixelShaderResourcesPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& DrawWithPixelShaderResourcesPS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("color")) return color;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& DrawWithPixelShaderResourcesPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void DrawWithPixelShaderResourcesPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DrawWithPixelShaderResourcesPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DrawWithPixelShaderResourcesPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("color")) { color = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DrawWithPixelShaderResourcesPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& DrawWithPixelShaderResourcesPS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& DrawWithPixelShaderResourcesPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& DrawWithPixelShaderResourcesPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& DrawWithPixelShaderResourcesPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& DrawWithPixelShaderResourcesPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t DrawWithPixelShaderResourcesPS::descriptorCount() const
        {
            return 1;
        }
    }
}