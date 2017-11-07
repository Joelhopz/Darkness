#include "PrefilterConvolution.vs.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> PrefilterConvolutionVS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/ibl/PrefilterConvolution.vs.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/ibl/PrefilterConvolution.vs.support");
        }

        PrefilterConvolutionVS::PrefilterConvolutionVS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)) + sizeof(ConstData)),
                    nullptr,
                    "ConstData"
                }
                
            
            
            }
        {}

        std::vector<TextureSRV> PrefilterConvolutionVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> PrefilterConvolutionVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> PrefilterConvolutionVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> PrefilterConvolutionVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> PrefilterConvolutionVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> PrefilterConvolutionVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> PrefilterConvolutionVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> PrefilterConvolutionVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& PrefilterConvolutionVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> PrefilterConvolutionVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool PrefilterConvolutionVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PrefilterConvolutionVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool PrefilterConvolutionVS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PrefilterConvolutionVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool PrefilterConvolutionVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PrefilterConvolutionVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool PrefilterConvolutionVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PrefilterConvolutionVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& PrefilterConvolutionVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& PrefilterConvolutionVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& PrefilterConvolutionVS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& PrefilterConvolutionVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& PrefilterConvolutionVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& PrefilterConvolutionVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& PrefilterConvolutionVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& PrefilterConvolutionVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& PrefilterConvolutionVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t PrefilterConvolutionVS::descriptorCount() const
        {
            return 1;
        }
    }
}