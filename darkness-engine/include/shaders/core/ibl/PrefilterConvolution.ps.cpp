#include "PrefilterConvolution.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> PrefilterConvolutionPS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/ibl/PrefilterConvolution.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/ibl/PrefilterConvolution.ps.support");
        }

        PrefilterConvolutionPS::PrefilterConvolutionPS()
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

        std::vector<TextureSRV> PrefilterConvolutionPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(environmentMap);
            
            return result;
        }

        std::vector<TextureUAV> PrefilterConvolutionPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> PrefilterConvolutionPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> PrefilterConvolutionPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> PrefilterConvolutionPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> PrefilterConvolutionPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> PrefilterConvolutionPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> PrefilterConvolutionPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& PrefilterConvolutionPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> PrefilterConvolutionPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(environmentSampler);
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool PrefilterConvolutionPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("environmentMap")) return true;
            
            
            return false;
        }

        bool PrefilterConvolutionPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool PrefilterConvolutionPS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PrefilterConvolutionPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool PrefilterConvolutionPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PrefilterConvolutionPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool PrefilterConvolutionPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool PrefilterConvolutionPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& PrefilterConvolutionPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("environmentMap")) return environmentMap;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& PrefilterConvolutionPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& PrefilterConvolutionPS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& PrefilterConvolutionPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& PrefilterConvolutionPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("environmentSampler")) return environmentSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& PrefilterConvolutionPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& PrefilterConvolutionPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& PrefilterConvolutionPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& PrefilterConvolutionPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t PrefilterConvolutionPS::descriptorCount() const
        {
            return 2;
        }
    }
}