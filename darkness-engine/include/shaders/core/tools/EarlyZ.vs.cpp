#include "EarlyZ.vs.h"
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
        std::shared_ptr<const ShaderBinary> EarlyZVS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/EarlyZ.vs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/tools/EarlyZ.vs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        EarlyZVS::EarlyZVS()
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
        EarlyZVS::EarlyZVS(const EarlyZVS& cl)
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

            

            

            
            vertices = cl.vertices;
            
            indexes = cl.indexes;
            
            clusters = cl.clusters;
            
            clusterData = cl.clusterData;
            
            transformHistory = cl.transformHistory;
            
            clusterMap = cl.clusterMap;
            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        EarlyZVS::EarlyZVS(EarlyZVS&& cl)
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

            

            

            
            vertices = std::move(cl.vertices);
            
            indexes = std::move(cl.indexes);
            
            clusters = std::move(cl.clusters);
            
            clusterData = std::move(cl.clusterData);
            
            transformHistory = std::move(cl.transformHistory);
            
            clusterMap = std::move(cl.clusterMap);
            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        EarlyZVS& EarlyZVS::operator=(const EarlyZVS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            vertices = cl.vertices;
            
            indexes = cl.indexes;
            
            clusters = cl.clusters;
            
            clusterData = cl.clusterData;
            
            transformHistory = cl.transformHistory;
            
            clusterMap = cl.clusterMap;
            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        EarlyZVS& EarlyZVS::operator=(EarlyZVS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            vertices = std::move(cl.vertices);
            
            indexes = std::move(cl.indexes);
            
            clusters = std::move(cl.clusters);
            
            clusterData = std::move(cl.clusterData);
            
            transformHistory = std::move(cl.transformHistory);
            
            clusterMap = std::move(cl.clusterMap);
            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> EarlyZVS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> EarlyZVS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> EarlyZVS::bufferSrvNames() const
        {
            return {
                
                "vertices"
                
                ,
                
                
                "indexes"
                
                ,
                
                
                "clusters"
                
                ,
                
                
                "clusterData"
                
                ,
                
                
                "transformHistory"
                
                ,
                
                
                "clusterMap"
                
                
            };
        }

        std::vector<std::string> EarlyZVS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> EarlyZVS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> EarlyZVS::srvNames() const
        {
            return {
                
                "vertices"
                
                ,
                
                
                "indexes"
                
                ,
                
                
                "clusters"
                
                ,
                
                
                "clusterData"
                
                ,
                
                
                "transformHistory"
                
                ,
                
                
                "clusterMap"
                
                
            };
        }

        std::vector<std::string> EarlyZVS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension EarlyZVS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> EarlyZVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> EarlyZVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> EarlyZVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(vertices);
            
            result.emplace_back(indexes);
            
            result.emplace_back(clusters);
            
            result.emplace_back(clusterData);
            
            result.emplace_back(transformHistory);
            
            result.emplace_back(clusterMap);
            
            return result;
        }

        std::vector<BufferUAV> EarlyZVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> EarlyZVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> EarlyZVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> EarlyZVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> EarlyZVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& EarlyZVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> EarlyZVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& EarlyZVS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool EarlyZVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZVS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("vertices")) return true;
            
            if (name == std::string("indexes")) return true;
            
            if (name == std::string("clusters")) return true;
            
            if (name == std::string("clusterData")) return true;
            
            if (name == std::string("transformHistory")) return true;
            
            if (name == std::string("clusterMap")) return true;
            
            
            return false;
        }

        bool EarlyZVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool EarlyZVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& EarlyZVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& EarlyZVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& EarlyZVS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("vertices")) return vertices;
            
            if(name == std::string("indexes")) return indexes;
            
            if(name == std::string("clusters")) return clusters;
            
            if(name == std::string("clusterData")) return clusterData;
            
            if(name == std::string("transformHistory")) return transformHistory;
            
            if(name == std::string("clusterMap")) return clusterMap;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& EarlyZVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void EarlyZVS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void EarlyZVS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void EarlyZVS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("vertices")) { vertices = buffer; return; }
            
            if(name == std::string("indexes")) { indexes = buffer; return; }
            
            if(name == std::string("clusters")) { clusters = buffer; return; }
            
            if(name == std::string("clusterData")) { clusterData = buffer; return; }
            
            if(name == std::string("transformHistory")) { transformHistory = buffer; return; }
            
            if(name == std::string("clusterMap")) { clusterMap = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void EarlyZVS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& EarlyZVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& EarlyZVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& EarlyZVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& EarlyZVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& EarlyZVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t EarlyZVS::descriptorCount() const
        {
            return 7;
        }
    }
}