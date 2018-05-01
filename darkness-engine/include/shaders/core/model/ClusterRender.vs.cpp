#include "ClusterRender.vs.h"
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
        std::shared_ptr<const ShaderBinary> ClusterRenderVS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/model/ClusterRender.vs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/model/ClusterRender.vs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        ClusterRenderVS::ClusterRenderVS()
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
        ClusterRenderVS::ClusterRenderVS(const ClusterRenderVS& cl)
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
            
            normals = cl.normals;
            
            tangents = cl.tangents;
            
            uv = cl.uv;
            
            indexes = cl.indexes;
            
            clusters = cl.clusters;
            
            clusterData = cl.clusterData;
            
            transformHistory = cl.transformHistory;
            
            instanceUV = cl.instanceUV;
            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterRenderVS::ClusterRenderVS(ClusterRenderVS&& cl)
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
            
            normals = std::move(cl.normals);
            
            tangents = std::move(cl.tangents);
            
            uv = std::move(cl.uv);
            
            indexes = std::move(cl.indexes);
            
            clusters = std::move(cl.clusters);
            
            clusterData = std::move(cl.clusterData);
            
            transformHistory = std::move(cl.transformHistory);
            
            instanceUV = std::move(cl.instanceUV);
            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterRenderVS& ClusterRenderVS::operator=(const ClusterRenderVS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            vertices = cl.vertices;
            
            normals = cl.normals;
            
            tangents = cl.tangents;
            
            uv = cl.uv;
            
            indexes = cl.indexes;
            
            clusters = cl.clusters;
            
            clusterData = cl.clusterData;
            
            transformHistory = cl.transformHistory;
            
            instanceUV = cl.instanceUV;
            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterRenderVS& ClusterRenderVS::operator=(ClusterRenderVS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            vertices = std::move(cl.vertices);
            
            normals = std::move(cl.normals);
            
            tangents = std::move(cl.tangents);
            
            uv = std::move(cl.uv);
            
            indexes = std::move(cl.indexes);
            
            clusters = std::move(cl.clusters);
            
            clusterData = std::move(cl.clusterData);
            
            transformHistory = std::move(cl.transformHistory);
            
            instanceUV = std::move(cl.instanceUV);
            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> ClusterRenderVS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterRenderVS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterRenderVS::bufferSrvNames() const
        {
            return {
                
                "vertices"
                
                ,
                
                
                "normals"
                
                ,
                
                
                "tangents"
                
                ,
                
                
                "uv"
                
                ,
                
                
                "indexes"
                
                ,
                
                
                "clusters"
                
                ,
                
                
                "clusterData"
                
                ,
                
                
                "transformHistory"
                
                ,
                
                
                "instanceUV"
                
                
            };
        }

        std::vector<std::string> ClusterRenderVS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterRenderVS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterRenderVS::srvNames() const
        {
            return {
                
                "vertices"
                
                ,
                
                
                "normals"
                
                ,
                
                
                "tangents"
                
                ,
                
                
                "uv"
                
                ,
                
                
                "indexes"
                
                ,
                
                
                "clusters"
                
                ,
                
                
                "clusterData"
                
                ,
                
                
                "transformHistory"
                
                ,
                
                
                "instanceUV"
                
                
            };
        }

        std::vector<std::string> ClusterRenderVS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension ClusterRenderVS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> ClusterRenderVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> ClusterRenderVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> ClusterRenderVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(vertices);
            
            result.emplace_back(normals);
            
            result.emplace_back(tangents);
            
            result.emplace_back(uv);
            
            result.emplace_back(indexes);
            
            result.emplace_back(clusters);
            
            result.emplace_back(clusterData);
            
            result.emplace_back(transformHistory);
            
            result.emplace_back(instanceUV);
            
            return result;
        }

        std::vector<BufferUAV> ClusterRenderVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> ClusterRenderVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> ClusterRenderVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> ClusterRenderVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> ClusterRenderVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& ClusterRenderVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> ClusterRenderVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& ClusterRenderVS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool ClusterRenderVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterRenderVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterRenderVS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("vertices")) return true;
            
            if (name == std::string("normals")) return true;
            
            if (name == std::string("tangents")) return true;
            
            if (name == std::string("uv")) return true;
            
            if (name == std::string("indexes")) return true;
            
            if (name == std::string("clusters")) return true;
            
            if (name == std::string("clusterData")) return true;
            
            if (name == std::string("transformHistory")) return true;
            
            if (name == std::string("instanceUV")) return true;
            
            
            return false;
        }

        bool ClusterRenderVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterRenderVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterRenderVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterRenderVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterRenderVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& ClusterRenderVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& ClusterRenderVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& ClusterRenderVS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("vertices")) return vertices;
            
            if(name == std::string("normals")) return normals;
            
            if(name == std::string("tangents")) return tangents;
            
            if(name == std::string("uv")) return uv;
            
            if(name == std::string("indexes")) return indexes;
            
            if(name == std::string("clusters")) return clusters;
            
            if(name == std::string("clusterData")) return clusterData;
            
            if(name == std::string("transformHistory")) return transformHistory;
            
            if(name == std::string("instanceUV")) return instanceUV;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& ClusterRenderVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void ClusterRenderVS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClusterRenderVS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClusterRenderVS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("vertices")) { vertices = buffer; return; }
            
            if(name == std::string("normals")) { normals = buffer; return; }
            
            if(name == std::string("tangents")) { tangents = buffer; return; }
            
            if(name == std::string("uv")) { uv = buffer; return; }
            
            if(name == std::string("indexes")) { indexes = buffer; return; }
            
            if(name == std::string("clusters")) { clusters = buffer; return; }
            
            if(name == std::string("clusterData")) { clusterData = buffer; return; }
            
            if(name == std::string("transformHistory")) { transformHistory = buffer; return; }
            
            if(name == std::string("instanceUV")) { instanceUV = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClusterRenderVS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& ClusterRenderVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& ClusterRenderVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& ClusterRenderVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& ClusterRenderVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& ClusterRenderVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t ClusterRenderVS::descriptorCount() const
        {
            return 10;
        }
    }
}