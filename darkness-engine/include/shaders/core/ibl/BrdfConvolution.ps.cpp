#include "BrdfConvolution.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> BrdfConvolutionPS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/ibl/BrdfConvolution.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/ibl/BrdfConvolution.ps.support");
        }

        BrdfConvolutionPS::BrdfConvolutionPS()
            : m_constantRange{
            
            }
        {}

        std::vector<TextureSRV> BrdfConvolutionPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> BrdfConvolutionPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> BrdfConvolutionPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> BrdfConvolutionPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> BrdfConvolutionPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> BrdfConvolutionPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> BrdfConvolutionPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> BrdfConvolutionPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& BrdfConvolutionPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> BrdfConvolutionPS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool BrdfConvolutionPS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BrdfConvolutionPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool BrdfConvolutionPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BrdfConvolutionPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool BrdfConvolutionPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BrdfConvolutionPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool BrdfConvolutionPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool BrdfConvolutionPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& BrdfConvolutionPS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& BrdfConvolutionPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& BrdfConvolutionPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& BrdfConvolutionPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& BrdfConvolutionPS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& BrdfConvolutionPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& BrdfConvolutionPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& BrdfConvolutionPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& BrdfConvolutionPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t BrdfConvolutionPS::descriptorCount() const
        {
            return 0;
        }
    }
}