#include "Outline.ps.h"
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
        std::shared_ptr<const ShaderBinary> OutlinePS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/outline/Outline.ps.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/outline/Outline.ps.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        OutlinePS::OutlinePS()
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
            
            
            ShaderInputParameter{"colora", "COLOR0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"edgeFlag", "PSIZE", "float"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        OutlinePS::OutlinePS(const OutlinePS& cl)
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

            
            depth = cl.depth;
            

            

            

            

            

            

            

            

            
            depth_sampler = cl.depth_sampler;
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        OutlinePS::OutlinePS(OutlinePS&& cl)
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

            
            depth = std::move(cl.depth);
            

            

            

            

            

            

            

            

            
            depth_sampler = std::move(cl.depth_sampler);
            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        OutlinePS& OutlinePS::operator=(const OutlinePS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            
            depth = cl.depth;
            

            

            

            

            

            

            

            

            
            depth_sampler = cl.depth_sampler;
            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        OutlinePS& OutlinePS::operator=(OutlinePS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            
            depth = std::move(cl.depth);
            

            

            

            

            

            

            

            

            
            depth_sampler = std::move(cl.depth_sampler);
            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> OutlinePS::textureSrvNames() const
        {
            return {
                
                "depth"
                
                
            };
        }

        std::vector<std::string> OutlinePS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> OutlinePS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> OutlinePS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> OutlinePS::samplerNames() const
        {
            return {
                
                "depth_sampler"
                
                
            };
        }

        std::vector<std::string> OutlinePS::srvNames() const
        {
            return {
                
                "depth"
                
                
            };
        }

        std::vector<std::string> OutlinePS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension OutlinePS::textureDimension(const std::string& name) const
        {
            
            if("depth" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> OutlinePS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(depth);
            
            return result;
        }

        std::vector<TextureUAV> OutlinePS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> OutlinePS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> OutlinePS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> OutlinePS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> OutlinePS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> OutlinePS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> OutlinePS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& OutlinePS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> OutlinePS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(depth_sampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& OutlinePS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool OutlinePS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("depth")) return true;
            
            
            return false;
        }

        bool OutlinePS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool OutlinePS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool OutlinePS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool OutlinePS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool OutlinePS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool OutlinePS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool OutlinePS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& OutlinePS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("depth")) return depth;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& OutlinePS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& OutlinePS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& OutlinePS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void OutlinePS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            
            if(name == std::string("depth")) { depth = texture; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void OutlinePS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void OutlinePS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void OutlinePS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& OutlinePS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("depth_sampler")) return depth_sampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& OutlinePS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& OutlinePS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& OutlinePS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& OutlinePS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t OutlinePS::descriptorCount() const
        {
            return 2;
        }
    }
}