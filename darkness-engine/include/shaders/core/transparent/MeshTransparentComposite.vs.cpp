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
        std::shared_ptr<const ShaderBinary> MeshTransparentCompositeVS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/transparent/MeshTransparentComposite.vs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/transparent/MeshTransparentComposite.vs.support");
        }

        MeshTransparentCompositeVS::MeshTransparentCompositeVS()
            : m_constantRange{
            
            }
        {}

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

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

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