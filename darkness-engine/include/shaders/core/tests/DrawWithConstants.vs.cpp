#include "DrawWithConstants.vs.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> DrawWithConstantsVS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tests/DrawWithConstants.vs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/tests/DrawWithConstants.vs.support");
        }

        DrawWithConstantsVS::DrawWithConstantsVS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const constants_struct*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const constants_struct*>(this)) + sizeof(constants_struct)),
                    nullptr,
                    "constants_struct"
                }
                
                ,
                
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const constants_struct2*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const constants_struct2*>(this)) + sizeof(constants_struct2)),
                    nullptr,
                    "constants_struct2"
                }
                
            
            
            }
        {}

        std::vector<TextureSRV> DrawWithConstantsVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> DrawWithConstantsVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> DrawWithConstantsVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> DrawWithConstantsVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> DrawWithConstantsVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> DrawWithConstantsVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> DrawWithConstantsVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> DrawWithConstantsVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& DrawWithConstantsVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> DrawWithConstantsVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool DrawWithConstantsVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithConstantsVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithConstantsVS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithConstantsVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithConstantsVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithConstantsVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithConstantsVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DrawWithConstantsVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& DrawWithConstantsVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& DrawWithConstantsVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& DrawWithConstantsVS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& DrawWithConstantsVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& DrawWithConstantsVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& DrawWithConstantsVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& DrawWithConstantsVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& DrawWithConstantsVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& DrawWithConstantsVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t DrawWithConstantsVS::descriptorCount() const
        {
            return 2;
        }
    }
}