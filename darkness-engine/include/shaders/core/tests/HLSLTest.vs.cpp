#include "HLSLTest.vs.h"
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
        std::shared_ptr<const ShaderBinary> HLSLTestVS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tests/HLSLTest.vs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tests/HLSLTest.vs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        HLSLTestVS::HLSLTestVS()
            : m_constantRange{
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"id", "SV_VertexID", "uint"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        HLSLTestVS::HLSLTestVS(const HLSLTestVS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            position = cl.position;
            
            color = cl.color;
            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        HLSLTestVS::HLSLTestVS(HLSLTestVS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            position = std::move(cl.position);
            
            color = std::move(cl.color);
            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        HLSLTestVS& HLSLTestVS::operator=(const HLSLTestVS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            position = cl.position;
            
            color = cl.color;
            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        HLSLTestVS& HLSLTestVS::operator=(HLSLTestVS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            position = std::move(cl.position);
            
            color = std::move(cl.color);
            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> HLSLTestVS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> HLSLTestVS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> HLSLTestVS::bufferSrvNames() const
        {
            return {
                
                "position"
                
                ,
                
                
                "color"
                
                
            };
        }

        std::vector<std::string> HLSLTestVS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> HLSLTestVS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> HLSLTestVS::srvNames() const
        {
            return {
                
                "position"
                
                ,
                
                
                "color"
                
                
            };
        }

        std::vector<std::string> HLSLTestVS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension HLSLTestVS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> HLSLTestVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> HLSLTestVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> HLSLTestVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(position);
            
            result.emplace_back(color);
            
            return result;
        }

        std::vector<BufferUAV> HLSLTestVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> HLSLTestVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> HLSLTestVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> HLSLTestVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> HLSLTestVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& HLSLTestVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> HLSLTestVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& HLSLTestVS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool HLSLTestVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool HLSLTestVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool HLSLTestVS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("position")) return true;
            
            if (name == std::string("color")) return true;
            
            
            return false;
        }

        bool HLSLTestVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool HLSLTestVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool HLSLTestVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool HLSLTestVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool HLSLTestVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& HLSLTestVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& HLSLTestVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& HLSLTestVS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("position")) return position;
            
            if(name == std::string("color")) return color;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& HLSLTestVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void HLSLTestVS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void HLSLTestVS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void HLSLTestVS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("position")) { position = buffer; return; }
            
            if(name == std::string("color")) { color = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void HLSLTestVS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& HLSLTestVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& HLSLTestVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& HLSLTestVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& HLSLTestVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& HLSLTestVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t HLSLTestVS::descriptorCount() const
        {
            return 2;
        }
    }
}