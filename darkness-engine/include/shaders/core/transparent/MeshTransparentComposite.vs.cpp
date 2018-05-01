#include "MeshTransparentComposite.vs.h"
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
        std::shared_ptr<const ShaderBinary> MeshTransparentCompositeVS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/transparent/MeshTransparentComposite.vs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/transparent/MeshTransparentComposite.vs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        MeshTransparentCompositeVS::MeshTransparentCompositeVS()
            : m_constantRange{
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"id", "SV_VertexID", "uint"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        MeshTransparentCompositeVS::MeshTransparentCompositeVS(const MeshTransparentCompositeVS& cl)
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
        MeshTransparentCompositeVS::MeshTransparentCompositeVS(MeshTransparentCompositeVS&& cl)
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
        MeshTransparentCompositeVS& MeshTransparentCompositeVS::operator=(const MeshTransparentCompositeVS& cl)
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
        MeshTransparentCompositeVS& MeshTransparentCompositeVS::operator=(MeshTransparentCompositeVS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> MeshTransparentCompositeVS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MeshTransparentCompositeVS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MeshTransparentCompositeVS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MeshTransparentCompositeVS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MeshTransparentCompositeVS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MeshTransparentCompositeVS::srvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MeshTransparentCompositeVS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension MeshTransparentCompositeVS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> MeshTransparentCompositeVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> MeshTransparentCompositeVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> MeshTransparentCompositeVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> MeshTransparentCompositeVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> MeshTransparentCompositeVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> MeshTransparentCompositeVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> MeshTransparentCompositeVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> MeshTransparentCompositeVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& MeshTransparentCompositeVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> MeshTransparentCompositeVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& MeshTransparentCompositeVS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool MeshTransparentCompositeVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshTransparentCompositeVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshTransparentCompositeVS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshTransparentCompositeVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshTransparentCompositeVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshTransparentCompositeVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshTransparentCompositeVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshTransparentCompositeVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& MeshTransparentCompositeVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& MeshTransparentCompositeVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& MeshTransparentCompositeVS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& MeshTransparentCompositeVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void MeshTransparentCompositeVS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void MeshTransparentCompositeVS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void MeshTransparentCompositeVS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void MeshTransparentCompositeVS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& MeshTransparentCompositeVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& MeshTransparentCompositeVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& MeshTransparentCompositeVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& MeshTransparentCompositeVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& MeshTransparentCompositeVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t MeshTransparentCompositeVS::descriptorCount() const
        {
            return 0;
        }
    }
}