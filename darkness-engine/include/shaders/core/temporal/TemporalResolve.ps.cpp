#include "TemporalResolve.ps.h"
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
        std::shared_ptr<const ShaderBinary> TemporalResolvePS::load(const Device& device, ShaderStorage& storage) const
        {
            
            if((true)
            
            && (visualizeMotion == false)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/temporal/TemporalResolve.ps_000.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/temporal/TemporalResolve.ps.support", 1-1, {
                    
                    });
            }
            
            if((true)
            
            && (visualizeMotion == true)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/temporal/TemporalResolve.ps_001.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/temporal/TemporalResolve.ps.support", 2-1, {
                    
                    "OPTION_VISUALIZE_MOTION"
                    
                    });
            }
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        TemporalResolvePS::TemporalResolvePS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)) + sizeof(Constants)),
                    nullptr,
                    "Constants"
                }
                
            
            
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
        TemporalResolvePS::TemporalResolvePS(const TemporalResolvePS& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)) + sizeof(Constants)),
                    nullptr,
                    "Constants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            currentFrame = cl.currentFrame;
            
            history = cl.history;
            
            depth = cl.depth;
            
            gbufferMotion = cl.gbufferMotion;
            

            

            

            

            

            

            

            

            
            pointSampler = cl.pointSampler;
            
            bilinearSampler = cl.bilinearSampler;
            

            
            visualizeMotion = cl.visualizeMotion;
            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        TemporalResolvePS::TemporalResolvePS(TemporalResolvePS&& cl)
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const Constants*>(this)) + sizeof(Constants)),
                    nullptr,
                    "Constants"
                }
                
            
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            currentFrame = std::move(cl.currentFrame);
            
            history = std::move(cl.history);
            
            depth = std::move(cl.depth);
            
            gbufferMotion = std::move(cl.gbufferMotion);
            

            

            

            

            

            

            

            

            
            pointSampler = std::move(cl.pointSampler);
            
            bilinearSampler = std::move(cl.bilinearSampler);
            

            
            visualizeMotion = std::move(cl.visualizeMotion);
            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        TemporalResolvePS& TemporalResolvePS::operator=(const TemporalResolvePS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            currentFrame = cl.currentFrame;
            
            history = cl.history;
            
            depth = cl.depth;
            
            gbufferMotion = cl.gbufferMotion;
            

            

            

            

            

            

            

            

            
            pointSampler = cl.pointSampler;
            
            bilinearSampler = cl.bilinearSampler;
            

            
            visualizeMotion = cl.visualizeMotion;
            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        TemporalResolvePS& TemporalResolvePS::operator=(TemporalResolvePS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            currentFrame = std::move(cl.currentFrame);
            
            history = std::move(cl.history);
            
            depth = std::move(cl.depth);
            
            gbufferMotion = std::move(cl.gbufferMotion);
            

            

            

            

            

            

            

            

            
            pointSampler = std::move(cl.pointSampler);
            
            bilinearSampler = std::move(cl.bilinearSampler);
            

            
            visualizeMotion = std::move(cl.visualizeMotion);
            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> TemporalResolvePS::textureSrvNames() const
        {
            return {
                
                "currentFrame"
                
                ,
                
                
                "history"
                
                ,
                
                
                "depth"
                
                ,
                
                
                "gbufferMotion"
                
                
            };
        }

        std::vector<std::string> TemporalResolvePS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> TemporalResolvePS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> TemporalResolvePS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> TemporalResolvePS::samplerNames() const
        {
            return {
                
                "pointSampler"
                
                ,
                
                
                "bilinearSampler"
                
                
            };
        }

        std::vector<std::string> TemporalResolvePS::srvNames() const
        {
            return {
                
                "currentFrame"
                
                ,
                
                
                "history"
                
                ,
                
                
                "depth"
                
                ,
                
                
                "gbufferMotion"
                
                
            };
        }

        std::vector<std::string> TemporalResolvePS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension TemporalResolvePS::textureDimension(const std::string& name) const
        {
            
            if("currentFrame" == name) return engine::ResourceDimension::Texture2D;
            
            if("history" == name) return engine::ResourceDimension::Texture2D;
            
            if("depth" == name) return engine::ResourceDimension::Texture2D;
            
            if("gbufferMotion" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> TemporalResolvePS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(currentFrame);
            
            result.emplace_back(history);
            
            result.emplace_back(depth);
            
            result.emplace_back(gbufferMotion);
            
            return result;
        }

        std::vector<TextureUAV> TemporalResolvePS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> TemporalResolvePS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> TemporalResolvePS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> TemporalResolvePS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> TemporalResolvePS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> TemporalResolvePS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> TemporalResolvePS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& TemporalResolvePS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> TemporalResolvePS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(pointSampler);
            
            result.emplace_back(bilinearSampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& TemporalResolvePS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool TemporalResolvePS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("currentFrame")) return true;
            
            if(name == std::string("history")) return true;
            
            if(name == std::string("depth")) return true;
            
            if(name == std::string("gbufferMotion")) return true;
            
            
            return false;
        }

        bool TemporalResolvePS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolvePS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolvePS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolvePS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolvePS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolvePS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool TemporalResolvePS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& TemporalResolvePS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("currentFrame")) return currentFrame;
            
            if(name == std::string("history")) return history;
            
            if(name == std::string("depth")) return depth;
            
            if(name == std::string("gbufferMotion")) return gbufferMotion;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& TemporalResolvePS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& TemporalResolvePS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& TemporalResolvePS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void TemporalResolvePS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("currentFrame")) { currentFrame = texture; return; }
            
            if(name == std::string("history")) { history = texture; return; }
            
            if(name == std::string("depth")) { depth = texture; return; }
            
            if(name == std::string("gbufferMotion")) { gbufferMotion = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void TemporalResolvePS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void TemporalResolvePS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void TemporalResolvePS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& TemporalResolvePS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("pointSampler")) return pointSampler;
            
            if(name == std::string("bilinearSampler")) return bilinearSampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& TemporalResolvePS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& TemporalResolvePS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& TemporalResolvePS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& TemporalResolvePS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t TemporalResolvePS::descriptorCount() const
        {
            return 5;
        }
    }
}