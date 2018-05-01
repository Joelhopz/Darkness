#include "Texture2DArrayVisualizer.vs.h"
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
        std::shared_ptr<const ShaderBinary> Texture2DArrayVisualizerVS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tests/Texture2DArrayVisualizer.vs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tests/Texture2DArrayVisualizer.vs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        Texture2DArrayVisualizerVS::Texture2DArrayVisualizerVS()
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
            
            ShaderInputParameter{"id", "SV_VertexID", "uint"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        Texture2DArrayVisualizerVS::Texture2DArrayVisualizerVS(const Texture2DArrayVisualizerVS& cl)
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

            

            

            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        Texture2DArrayVisualizerVS::Texture2DArrayVisualizerVS(Texture2DArrayVisualizerVS&& cl)
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

            

            

            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        Texture2DArrayVisualizerVS& Texture2DArrayVisualizerVS::operator=(const Texture2DArrayVisualizerVS& cl)
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
        Texture2DArrayVisualizerVS& Texture2DArrayVisualizerVS::operator=(Texture2DArrayVisualizerVS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> Texture2DArrayVisualizerVS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> Texture2DArrayVisualizerVS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> Texture2DArrayVisualizerVS::bufferSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> Texture2DArrayVisualizerVS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> Texture2DArrayVisualizerVS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> Texture2DArrayVisualizerVS::srvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> Texture2DArrayVisualizerVS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension Texture2DArrayVisualizerVS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> Texture2DArrayVisualizerVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> Texture2DArrayVisualizerVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> Texture2DArrayVisualizerVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            return result;
        }

        std::vector<BufferUAV> Texture2DArrayVisualizerVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> Texture2DArrayVisualizerVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> Texture2DArrayVisualizerVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> Texture2DArrayVisualizerVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> Texture2DArrayVisualizerVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& Texture2DArrayVisualizerVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> Texture2DArrayVisualizerVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& Texture2DArrayVisualizerVS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool Texture2DArrayVisualizerVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool Texture2DArrayVisualizerVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool Texture2DArrayVisualizerVS::hasBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool Texture2DArrayVisualizerVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool Texture2DArrayVisualizerVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool Texture2DArrayVisualizerVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool Texture2DArrayVisualizerVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool Texture2DArrayVisualizerVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& Texture2DArrayVisualizerVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& Texture2DArrayVisualizerVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& Texture2DArrayVisualizerVS::bufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& Texture2DArrayVisualizerVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void Texture2DArrayVisualizerVS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void Texture2DArrayVisualizerVS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void Texture2DArrayVisualizerVS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void Texture2DArrayVisualizerVS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& Texture2DArrayVisualizerVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& Texture2DArrayVisualizerVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& Texture2DArrayVisualizerVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& Texture2DArrayVisualizerVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& Texture2DArrayVisualizerVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t Texture2DArrayVisualizerVS::descriptorCount() const
        {
            return 1;
        }
    }
}