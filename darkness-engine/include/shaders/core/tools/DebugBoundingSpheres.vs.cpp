#include "DebugBoundingSpheres.vs.h"
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
        std::shared_ptr<const ShaderBinary> DebugBoundingSpheresVS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/DebugBoundingSpheres.vs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/DebugBoundingSpheres.vs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        DebugBoundingSpheresVS::DebugBoundingSpheresVS()
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
        DebugBoundingSpheresVS::DebugBoundingSpheresVS(const DebugBoundingSpheresVS& cl)
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

            

            

            
            clusters = cl.clusters;
            
            clusterData = cl.clusterData;
            
            transformHistory = cl.transformHistory;
            
            boundingBoxes = cl.boundingBoxes;
            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DebugBoundingSpheresVS::DebugBoundingSpheresVS(DebugBoundingSpheresVS&& cl)
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

            

            

            
            clusters = std::move(cl.clusters);
            
            clusterData = std::move(cl.clusterData);
            
            transformHistory = std::move(cl.transformHistory);
            
            boundingBoxes = std::move(cl.boundingBoxes);
            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DebugBoundingSpheresVS& DebugBoundingSpheresVS::operator=(const DebugBoundingSpheresVS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            clusters = cl.clusters;
            
            clusterData = cl.clusterData;
            
            transformHistory = cl.transformHistory;
            
            boundingBoxes = cl.boundingBoxes;
            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        DebugBoundingSpheresVS& DebugBoundingSpheresVS::operator=(DebugBoundingSpheresVS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            clusters = std::move(cl.clusters);
            
            clusterData = std::move(cl.clusterData);
            
            transformHistory = std::move(cl.transformHistory);
            
            boundingBoxes = std::move(cl.boundingBoxes);
            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> DebugBoundingSpheresVS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DebugBoundingSpheresVS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DebugBoundingSpheresVS::bufferSrvNames() const
        {
            return {
                
                "clusters"
                
                ,
                
                
                "clusterData"
                
                ,
                
                
                "transformHistory"
                
                ,
                
                
                "boundingBoxes"
                
                
            };
        }

        std::vector<std::string> DebugBoundingSpheresVS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DebugBoundingSpheresVS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> DebugBoundingSpheresVS::srvNames() const
        {
            return {
                
                "clusters"
                
                ,
                
                
                "clusterData"
                
                ,
                
                
                "transformHistory"
                
                ,
                
                
                "boundingBoxes"
                
                
            };
        }

        std::vector<std::string> DebugBoundingSpheresVS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension DebugBoundingSpheresVS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> DebugBoundingSpheresVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> DebugBoundingSpheresVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> DebugBoundingSpheresVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(clusters);
            
            result.emplace_back(clusterData);
            
            result.emplace_back(transformHistory);
            
            result.emplace_back(boundingBoxes);
            
            return result;
        }

        std::vector<BufferUAV> DebugBoundingSpheresVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> DebugBoundingSpheresVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> DebugBoundingSpheresVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> DebugBoundingSpheresVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> DebugBoundingSpheresVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& DebugBoundingSpheresVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> DebugBoundingSpheresVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& DebugBoundingSpheresVS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool DebugBoundingSpheresVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DebugBoundingSpheresVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DebugBoundingSpheresVS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("clusters")) return true;
            
            if (name == std::string("clusterData")) return true;
            
            if (name == std::string("transformHistory")) return true;
            
            if (name == std::string("boundingBoxes")) return true;
            
            
            return false;
        }

        bool DebugBoundingSpheresVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool DebugBoundingSpheresVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DebugBoundingSpheresVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool DebugBoundingSpheresVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool DebugBoundingSpheresVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& DebugBoundingSpheresVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& DebugBoundingSpheresVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& DebugBoundingSpheresVS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("clusters")) return clusters;
            
            if(name == std::string("clusterData")) return clusterData;
            
            if(name == std::string("transformHistory")) return transformHistory;
            
            if(name == std::string("boundingBoxes")) return boundingBoxes;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& DebugBoundingSpheresVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void DebugBoundingSpheresVS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DebugBoundingSpheresVS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DebugBoundingSpheresVS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("clusters")) { clusters = buffer; return; }
            
            if(name == std::string("clusterData")) { clusterData = buffer; return; }
            
            if(name == std::string("transformHistory")) { transformHistory = buffer; return; }
            
            if(name == std::string("boundingBoxes")) { boundingBoxes = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void DebugBoundingSpheresVS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& DebugBoundingSpheresVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& DebugBoundingSpheresVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& DebugBoundingSpheresVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& DebugBoundingSpheresVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& DebugBoundingSpheresVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t DebugBoundingSpheresVS::descriptorCount() const
        {
            return 5;
        }
    }
}