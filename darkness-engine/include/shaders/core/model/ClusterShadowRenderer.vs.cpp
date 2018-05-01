#include "ClusterShadowRenderer.vs.h"
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
        std::shared_ptr<const ShaderBinary> ClusterShadowRendererVS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/model/ClusterShadowRenderer.vs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/model/ClusterShadowRenderer.vs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        ClusterShadowRendererVS::ClusterShadowRendererVS()
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
        ClusterShadowRendererVS::ClusterShadowRendererVS(const ClusterShadowRendererVS& cl)
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
            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterShadowRendererVS::ClusterShadowRendererVS(ClusterShadowRendererVS&& cl)
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
            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterShadowRendererVS& ClusterShadowRendererVS::operator=(const ClusterShadowRendererVS& cl)
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
            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterShadowRendererVS& ClusterShadowRendererVS::operator=(ClusterShadowRendererVS&& cl)
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
            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> ClusterShadowRendererVS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterShadowRendererVS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterShadowRendererVS::bufferSrvNames() const
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
                
                
            };
        }

        std::vector<std::string> ClusterShadowRendererVS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterShadowRendererVS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterShadowRendererVS::srvNames() const
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
                
                
            };
        }

        std::vector<std::string> ClusterShadowRendererVS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension ClusterShadowRendererVS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> ClusterShadowRendererVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> ClusterShadowRendererVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> ClusterShadowRendererVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(vertices);
            
            result.emplace_back(indexes);
            
            result.emplace_back(clusters);
            
            result.emplace_back(clusterData);
            
            result.emplace_back(transformHistory);
            
            return result;
        }

        std::vector<BufferUAV> ClusterShadowRendererVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> ClusterShadowRendererVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> ClusterShadowRendererVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> ClusterShadowRendererVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> ClusterShadowRendererVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& ClusterShadowRendererVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> ClusterShadowRendererVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& ClusterShadowRendererVS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool ClusterShadowRendererVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterShadowRendererVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterShadowRendererVS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("vertices")) return true;
            
            if (name == std::string("indexes")) return true;
            
            if (name == std::string("clusters")) return true;
            
            if (name == std::string("clusterData")) return true;
            
            if (name == std::string("transformHistory")) return true;
            
            
            return false;
        }

        bool ClusterShadowRendererVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterShadowRendererVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterShadowRendererVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterShadowRendererVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterShadowRendererVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& ClusterShadowRendererVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& ClusterShadowRendererVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& ClusterShadowRendererVS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("vertices")) return vertices;
            
            if(name == std::string("indexes")) return indexes;
            
            if(name == std::string("clusters")) return clusters;
            
            if(name == std::string("clusterData")) return clusterData;
            
            if(name == std::string("transformHistory")) return transformHistory;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& ClusterShadowRendererVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void ClusterShadowRendererVS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClusterShadowRendererVS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClusterShadowRendererVS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("vertices")) { vertices = buffer; return; }
            
            if(name == std::string("indexes")) { indexes = buffer; return; }
            
            if(name == std::string("clusters")) { clusters = buffer; return; }
            
            if(name == std::string("clusterData")) { clusterData = buffer; return; }
            
            if(name == std::string("transformHistory")) { transformHistory = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClusterShadowRendererVS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& ClusterShadowRendererVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& ClusterShadowRendererVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& ClusterShadowRendererVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& ClusterShadowRendererVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& ClusterShadowRendererVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t ClusterShadowRendererVS::descriptorCount() const
        {
            return 6;
        }
    }
}