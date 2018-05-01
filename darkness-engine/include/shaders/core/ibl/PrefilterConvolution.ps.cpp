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
#pragma warning( push )
#pragma warning( disable : 4702 )
        std::shared_ptr<const ShaderBinary> PrefilterConvolutionPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/ibl/PrefilterConvolution.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/ibl/PrefilterConvolution.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

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
            , m_inputParameters
            {
            
            ShaderInputParameter{"position", "SV_Position0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"pos", "NORMAL", "float4"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        PrefilterConvolutionPS::PrefilterConvolutionPS(const PrefilterConvolutionPS& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)) + sizeof(ConstData)),
                    nullptr,
                    "ConstData"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            environmentMap = cl.environmentMap;
            

            

            

            

            

            

            

            

            
            environmentSampler = cl.environmentSampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        PrefilterConvolutionPS::PrefilterConvolutionPS(PrefilterConvolutionPS&& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)) + sizeof(ConstData)),
                    nullptr,
                    "ConstData"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            environmentMap = std::move(cl.environmentMap);
            

            

            

            

            

            

            

            

            
            environmentSampler = std::move(cl.environmentSampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        PrefilterConvolutionPS& PrefilterConvolutionPS::operator=(const PrefilterConvolutionPS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            environmentMap = cl.environmentMap;
            

            

            

            

            

            

            

            

            
            environmentSampler = cl.environmentSampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        PrefilterConvolutionPS& PrefilterConvolutionPS::operator=(PrefilterConvolutionPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            environmentMap = std::move(cl.environmentMap);
            

            

            

            

            

            

            

            

            
            environmentSampler = std::move(cl.environmentSampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> PrefilterConvolutionPS::textureSrvNames() const
        {
            return {
                
                "environmentMap"
                
                
            };
        }

        std::vector<std::string> PrefilterConvolutionPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> PrefilterConvolutionPS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> PrefilterConvolutionPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> PrefilterConvolutionPS::samplerNames() const
        {
            return {
                
                "environmentSampler"
                
                
            };
        }

        std::vector<std::string> PrefilterConvolutionPS::srvNames() const
        {
            return {
                
                "environmentMap"
                
                
            };
        }

        std::vector<std::string> PrefilterConvolutionPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension PrefilterConvolutionPS::textureDimension(const std::string& name) const
        {
            
            if("environmentMap" == name) return engine::ResourceDimension::TextureCubemap;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

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

        const std::vector<ShaderInputParameter>& PrefilterConvolutionPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

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

        void PrefilterConvolutionPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("environmentMap")) { environmentMap = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void PrefilterConvolutionPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void PrefilterConvolutionPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void PrefilterConvolutionPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
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