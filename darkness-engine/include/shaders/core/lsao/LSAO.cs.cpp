#include "LSAO.cs.h"
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
        std::shared_ptr<const ShaderBinary> LSAOCS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lsao/LSAO.cs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/lsao/LSAO.cs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        LSAOCS::LSAOCS()
            : m_constantRange{
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"dispatchThreadID", "SV_DispatchThreadID", "uint3"}
            
            ,
            
            
            ShaderInputParameter{"groupThreadID", "SV_GroupThreadID", "uint3"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        LSAOCS::LSAOCS(const LSAOCS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            depth = cl.depth;
            

            

            
            lineInfos = cl.lineInfos;
            

            

            

            

            

            

            
            depthSampler = cl.depthSampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        LSAOCS::LSAOCS(LSAOCS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            depth = std::move(cl.depth);
            

            

            
            lineInfos = std::move(cl.lineInfos);
            

            

            

            

            

            

            
            depthSampler = std::move(cl.depthSampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        LSAOCS& LSAOCS::operator=(const LSAOCS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            depth = cl.depth;
            

            

            
            lineInfos = cl.lineInfos;
            

            

            

            

            

            

            
            depthSampler = cl.depthSampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        LSAOCS& LSAOCS::operator=(LSAOCS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            depth = std::move(cl.depth);
            

            

            
            lineInfos = std::move(cl.lineInfos);
            

            

            

            

            

            

            
            depthSampler = std::move(cl.depthSampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> LSAOCS::textureSrvNames() const
        {
            return {
                
                "depth"
                
                
            };
        }

        std::vector<std::string> LSAOCS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> LSAOCS::bufferSrvNames() const
        {
            return {
                
                "lineInfos"
                
                
            };
        }

        std::vector<std::string> LSAOCS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> LSAOCS::samplerNames() const
        {
            return {
                
                "depthSampler"
                
                
            };
        }

        std::vector<std::string> LSAOCS::srvNames() const
        {
            return {
                
                "depth"
                
                ,
                
                
                "lineInfos"
                
                
            };
        }

        std::vector<std::string> LSAOCS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension LSAOCS::textureDimension(const std::string& name) const
        {
            
            if("depth" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> LSAOCS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(depth);
            
            return result;
        }

        std::vector<TextureUAV> LSAOCS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> LSAOCS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(lineInfos);
            
            return result;
        }

        std::vector<BufferUAV> LSAOCS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> LSAOCS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> LSAOCS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> LSAOCS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> LSAOCS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& LSAOCS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> LSAOCS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(depthSampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& LSAOCS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool LSAOCS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("depth")) return true;
            
            
            return false;
        }

        bool LSAOCS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool LSAOCS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("lineInfos")) return true;
            
            
            return false;
        }

        bool LSAOCS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool LSAOCS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool LSAOCS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool LSAOCS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool LSAOCS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& LSAOCS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("depth")) return depth;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& LSAOCS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& LSAOCS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("lineInfos")) return lineInfos;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& LSAOCS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void LSAOCS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("depth")) { depth = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void LSAOCS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void LSAOCS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("lineInfos")) { lineInfos = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void LSAOCS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& LSAOCS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("depthSampler")) return depthSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& LSAOCS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& LSAOCS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& LSAOCS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& LSAOCS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t LSAOCS::descriptorCount() const
        {
            return 2;
        }
    }
}