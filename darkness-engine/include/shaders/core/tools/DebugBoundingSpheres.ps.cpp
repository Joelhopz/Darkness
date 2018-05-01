#include "DebugBoundingSpheres.ps.h"
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
        std::shared_ptr<const ShaderBinary> DebugBoundingSpheresPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/DebugBoundingSpheres.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/DebugBoundingSpheres.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        DebugBoundingSpheresPS::DebugBoundingSpheresPS()
            : m_constantRange{
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"position", "SV_Position0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"mvPosCurrent", "POSITION0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"mvPosPrevious", "POSITION1", "float4"}
            
            ,
            
            
            ShaderInputParameter{"normal", "NORMAL0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"tangent", "TEXCOORD0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"uv", "TEXCOORD1", "float2"}
            
            ,
            
            
            ShaderInputParameter{"clusterId", "BLENDINDICES0", "uint"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        DebugBoundingSpheresPS::DebugBoundingSpheresPS(const DebugBoundingSpheresPS& cl)
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
        DebugBoundingSpheresPS::DebugBoundingSpheresPS(DebugBoundingSpheresPS&& cl)
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
        DebugBoundingSpheresPS& DebugBoundingSpheresPS::operator=(const DebugBoundingSpheresPS& cl)
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
        DebugBoundingSpheresPS& DebugBoundingSpheresPS::operator=(DebugBoundingSpheresPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> DebugBoundingSpheresPS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DebugBoundingSpheresPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DebugBoundingSpheresPS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DebugBoundingSpheresPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DebugBoundingSpheresPS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DebugBoundingSpheresPS::srvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DebugBoundingSpheresPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension DebugBoundingSpheresPS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> DebugBoundingSpheresPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> DebugBoundingSpheresPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> DebugBoundingSpheresPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> DebugBoundingSpheresPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> DebugBoundingSpheresPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> DebugBoundingSpheresPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> DebugBoundingSpheresPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> DebugBoundingSpheresPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& DebugBoundingSpheresPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> DebugBoundingSpheresPS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& DebugBoundingSpheresPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool DebugBoundingSpheresPS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DebugBoundingSpheresPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DebugBoundingSpheresPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DebugBoundingSpheresPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool DebugBoundingSpheresPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DebugBoundingSpheresPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DebugBoundingSpheresPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DebugBoundingSpheresPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& DebugBoundingSpheresPS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& DebugBoundingSpheresPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& DebugBoundingSpheresPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& DebugBoundingSpheresPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void DebugBoundingSpheresPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DebugBoundingSpheresPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DebugBoundingSpheresPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DebugBoundingSpheresPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& DebugBoundingSpheresPS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& DebugBoundingSpheresPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& DebugBoundingSpheresPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& DebugBoundingSpheresPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& DebugBoundingSpheresPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t DebugBoundingSpheresPS::descriptorCount() const
        {
            return 0;
        }
    }
}