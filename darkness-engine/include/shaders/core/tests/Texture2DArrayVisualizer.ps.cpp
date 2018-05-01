#include "Texture2DArrayVisualizer.ps.h"
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
        std::shared_ptr<const ShaderBinary> Texture2DArrayVisualizerPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tests/Texture2DArrayVisualizer.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tests/Texture2DArrayVisualizer.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        Texture2DArrayVisualizerPS::Texture2DArrayVisualizerPS()
            : m_constantRange{
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"position", "SV_Position", "float4"}
            
            ,
            
            
            ShaderInputParameter{"uv", "TEXCOORD0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"mip", "PSIZE0", "float"}
            
            ,
            
            
            ShaderInputParameter{"slice", "BLENDINDICES0", "uint"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        Texture2DArrayVisualizerPS::Texture2DArrayVisualizerPS(const Texture2DArrayVisualizerPS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            textureArray = cl.textureArray;
            

            

            

            

            

            

            

            

            
            samp = cl.samp;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        Texture2DArrayVisualizerPS::Texture2DArrayVisualizerPS(Texture2DArrayVisualizerPS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            textureArray = std::move(cl.textureArray);
            

            

            

            

            

            

            

            

            
            samp = std::move(cl.samp);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        Texture2DArrayVisualizerPS& Texture2DArrayVisualizerPS::operator=(const Texture2DArrayVisualizerPS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            textureArray = cl.textureArray;
            

            

            

            

            

            

            

            

            
            samp = cl.samp;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        Texture2DArrayVisualizerPS& Texture2DArrayVisualizerPS::operator=(Texture2DArrayVisualizerPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            textureArray = std::move(cl.textureArray);
            

            

            

            

            

            

            

            

            
            samp = std::move(cl.samp);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> Texture2DArrayVisualizerPS::textureSrvNames() const
        {
            return {
                
                "textureArray"
                
                
            };
        }

        std::vector<std::string> Texture2DArrayVisualizerPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> Texture2DArrayVisualizerPS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> Texture2DArrayVisualizerPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> Texture2DArrayVisualizerPS::samplerNames() const
        {
            return {
                
                "samp"
                
                
            };
        }

        std::vector<std::string> Texture2DArrayVisualizerPS::srvNames() const
        {
            return {
                
                "textureArray"
                
                
            };
        }

        std::vector<std::string> Texture2DArrayVisualizerPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension Texture2DArrayVisualizerPS::textureDimension(const std::string& name) const
        {
            
            if("textureArray" == name) return engine::ResourceDimension::Texture2DArray;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> Texture2DArrayVisualizerPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(textureArray);
            
            return result;
        }

        std::vector<TextureUAV> Texture2DArrayVisualizerPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> Texture2DArrayVisualizerPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> Texture2DArrayVisualizerPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> Texture2DArrayVisualizerPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> Texture2DArrayVisualizerPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> Texture2DArrayVisualizerPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> Texture2DArrayVisualizerPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& Texture2DArrayVisualizerPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> Texture2DArrayVisualizerPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(samp);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& Texture2DArrayVisualizerPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool Texture2DArrayVisualizerPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("textureArray")) return true;
            
            
            return false;
        }

        bool Texture2DArrayVisualizerPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool Texture2DArrayVisualizerPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool Texture2DArrayVisualizerPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool Texture2DArrayVisualizerPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool Texture2DArrayVisualizerPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool Texture2DArrayVisualizerPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool Texture2DArrayVisualizerPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& Texture2DArrayVisualizerPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("textureArray")) return textureArray;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& Texture2DArrayVisualizerPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& Texture2DArrayVisualizerPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& Texture2DArrayVisualizerPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void Texture2DArrayVisualizerPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("textureArray")) { textureArray = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void Texture2DArrayVisualizerPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void Texture2DArrayVisualizerPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void Texture2DArrayVisualizerPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& Texture2DArrayVisualizerPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("samp")) return samp;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& Texture2DArrayVisualizerPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& Texture2DArrayVisualizerPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& Texture2DArrayVisualizerPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& Texture2DArrayVisualizerPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t Texture2DArrayVisualizerPS::descriptorCount() const
        {
            return 1;
        }
    }
}