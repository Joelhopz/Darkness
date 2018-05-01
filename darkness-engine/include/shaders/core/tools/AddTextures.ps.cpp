#include "AddTextures.ps.h"
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
        std::shared_ptr<const ShaderBinary> AddTexturesPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/AddTextures.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/AddTextures.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        AddTexturesPS::AddTexturesPS()
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
        AddTexturesPS::AddTexturesPS(const AddTexturesPS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            texturea = cl.texturea;
            
            textureb = cl.textureb;
            

            

            

            

            

            

            

            

            
            samp = cl.samp;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        AddTexturesPS::AddTexturesPS(AddTexturesPS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            texturea = std::move(cl.texturea);
            
            textureb = std::move(cl.textureb);
            

            

            

            

            

            

            

            

            
            samp = std::move(cl.samp);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        AddTexturesPS& AddTexturesPS::operator=(const AddTexturesPS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            texturea = cl.texturea;
            
            textureb = cl.textureb;
            

            

            

            

            

            

            

            

            
            samp = cl.samp;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        AddTexturesPS& AddTexturesPS::operator=(AddTexturesPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            texturea = std::move(cl.texturea);
            
            textureb = std::move(cl.textureb);
            

            

            

            

            

            

            

            

            
            samp = std::move(cl.samp);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> AddTexturesPS::textureSrvNames() const
        {
            return {
                
                "texturea"
                
                ,
                
                
                "textureb"
                
                
            };
        }

        std::vector<std::string> AddTexturesPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> AddTexturesPS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> AddTexturesPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> AddTexturesPS::samplerNames() const
        {
            return {
                
                "samp"
                
                
            };
        }

        std::vector<std::string> AddTexturesPS::srvNames() const
        {
            return {
                
                "texturea"
                
                ,
                
                
                "textureb"
                
                
            };
        }

        std::vector<std::string> AddTexturesPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension AddTexturesPS::textureDimension(const std::string& name) const
        {
            
            if("texturea" == name) return engine::ResourceDimension::Texture2D;
            
            if("textureb" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> AddTexturesPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(texturea);
            
            result.emplace_back(textureb);
            
            return result;
        }

        std::vector<TextureUAV> AddTexturesPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> AddTexturesPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> AddTexturesPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> AddTexturesPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> AddTexturesPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> AddTexturesPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> AddTexturesPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& AddTexturesPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> AddTexturesPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(samp);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& AddTexturesPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool AddTexturesPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("texturea")) return true;
            
            if(name == std::string("textureb")) return true;
            
            
            return false;
        }

        bool AddTexturesPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool AddTexturesPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool AddTexturesPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool AddTexturesPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool AddTexturesPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool AddTexturesPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool AddTexturesPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& AddTexturesPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("texturea")) return texturea;
            
            if(name == std::string("textureb")) return textureb;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& AddTexturesPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& AddTexturesPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& AddTexturesPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void AddTexturesPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("texturea")) { texturea = texture; return; }
            
            if(name == std::string("textureb")) { textureb = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void AddTexturesPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void AddTexturesPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void AddTexturesPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& AddTexturesPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("samp")) return samp;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& AddTexturesPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& AddTexturesPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& AddTexturesPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& AddTexturesPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t AddTexturesPS::descriptorCount() const
        {
            return 2;
        }
    }
}