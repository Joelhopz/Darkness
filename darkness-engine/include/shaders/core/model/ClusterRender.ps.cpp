#include "ClusterRender.ps.h"
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
        std::shared_ptr<const ShaderBinary> ClusterRenderPS::load(const Device& device, ShaderStorage& storage) const
        {
            
            if((true)
            
            && (debugClusters == false)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/model/ClusterRender.ps_000.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/model/ClusterRender.ps.support", 1-1, {
                    
                    });
            }
            
            if((true)
            
            && (debugClusters == true)
            
            )
            {
                return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/model/ClusterRender.ps_001.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/model/ClusterRender.ps.support", 2-1, {
                    
                    "OPTION_DEBUG_CLUSTERS"
                    
                    });
            }
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        ClusterRenderPS::ClusterRenderPS()
            : m_constantRange{
            
            }
            , m_inputParameters
            {
            
            ShaderInputParameter{"position", "SV_Position0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"mvPosCurrent", "POSITION0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"mvPosPrevious", "POSITION1", "float4"}
            
            ,
            
            
            ShaderInputParameter{"normal", "NORMAL0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"tangent", "TEXCOORD0", "float4"}
            
            ,
            
            
            ShaderInputParameter{"instancePointer", "BLENDINDICES0", "uint"}
            
            ,
            
            
            ShaderInputParameter{"clusterPointer", "BLENDINDICES1", "uint"}
            
            
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterRenderPS::ClusterRenderPS(const ClusterRenderPS& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            instanceMaterials = cl.instanceMaterials;
            

            

            
            materialTextures = cl.materialTextures;
            

            

            

            

            
            tex_sampler = cl.tex_sampler;
            

            
            debugClusters = cl.debugClusters;
            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterRenderPS::ClusterRenderPS(ClusterRenderPS&& cl)
            : m_constantRange{
            
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            instanceMaterials = std::move(cl.instanceMaterials);
            

            

            
            materialTextures = std::move(cl.materialTextures);
            

            

            

            

            
            tex_sampler = std::move(cl.tex_sampler);
            

            
            debugClusters = std::move(cl.debugClusters);
            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterRenderPS& ClusterRenderPS::operator=(const ClusterRenderPS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            instanceMaterials = cl.instanceMaterials;
            

            

            
            materialTextures = cl.materialTextures;
            

            

            

            

            
            tex_sampler = cl.tex_sampler;
            

            
            debugClusters = cl.debugClusters;
            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        ClusterRenderPS& ClusterRenderPS::operator=(ClusterRenderPS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            instanceMaterials = std::move(cl.instanceMaterials);
            

            

            
            materialTextures = std::move(cl.materialTextures);
            

            

            

            

            
            tex_sampler = std::move(cl.tex_sampler);
            

            
            debugClusters = std::move(cl.debugClusters);
            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> ClusterRenderPS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterRenderPS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterRenderPS::bufferSrvNames() const
        {
            return {
                
                "instanceMaterials"
                
                
            };
        }

        std::vector<std::string> ClusterRenderPS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> ClusterRenderPS::samplerNames() const
        {
            return {
                
                "tex_sampler"
                
                
            };
        }

        std::vector<std::string> ClusterRenderPS::srvNames() const
        {
            return {
                
                "instanceMaterials"
                
                ,
                
                
                "materialTextures"
                
                
            };
        }

        std::vector<std::string> ClusterRenderPS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension ClusterRenderPS::textureDimension(const std::string& name) const
        {
            
            if("materialTextures" == name) return engine::ResourceDimension::Texture2D;
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> ClusterRenderPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> ClusterRenderPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> ClusterRenderPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(instanceMaterials);
            
            return result;
        }

        std::vector<BufferUAV> ClusterRenderPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> ClusterRenderPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            result.emplace_back(materialTextures);
            
            return result;
        }

        std::vector<TextureBindlessUAV> ClusterRenderPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> ClusterRenderPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> ClusterRenderPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& ClusterRenderPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> ClusterRenderPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(tex_sampler);
            
            return result;
        }

        const std::vector<ShaderInputParameter>& ClusterRenderPS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool ClusterRenderPS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterRenderPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterRenderPS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("instanceMaterials")) return true;
            
            
            return false;
        }

        bool ClusterRenderPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterRenderPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            
            if (name == std::string("materialTextures")) return true;
            
            
            return false;
        }

        bool ClusterRenderPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterRenderPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool ClusterRenderPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& ClusterRenderPS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& ClusterRenderPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& ClusterRenderPS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("instanceMaterials")) return instanceMaterials;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& ClusterRenderPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void ClusterRenderPS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClusterRenderPS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClusterRenderPS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("instanceMaterials")) { instanceMaterials = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void ClusterRenderPS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& ClusterRenderPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("tex_sampler")) return tex_sampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& ClusterRenderPS::bindlessTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("materialTextures")) return materialTextures;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& ClusterRenderPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& ClusterRenderPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& ClusterRenderPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t ClusterRenderPS::descriptorCount() const
        {
            return 2;
        }
    }
}