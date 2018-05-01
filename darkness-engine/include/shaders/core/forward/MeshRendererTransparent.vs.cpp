#include "MeshRendererTransparent.vs.h"
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
        std::shared_ptr<const ShaderBinary> MeshRendererTransparentVS::load(const Device& device, ShaderStorage& storage) const
        {
            
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/forward/MeshRendererTransparent.vs.spv", "C:/work/darkness/darkness-engine/data/shaders/vulkan/core/forward/MeshRendererTransparent.vs.support", -1, {});
            
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        MeshRendererTransparentVS::MeshRendererTransparentVS()
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
        MeshRendererTransparentVS::MeshRendererTransparentVS(const MeshRendererTransparentVS& cl)
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
            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        MeshRendererTransparentVS::MeshRendererTransparentVS(MeshRendererTransparentVS&& cl)
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
            

            

            

            

            

            

            

            

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        MeshRendererTransparentVS& MeshRendererTransparentVS::operator=(const MeshRendererTransparentVS& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            

            

            
            vertices = cl.vertices;
            
            normals = cl.normals;
            
            tangents = cl.tangents;
            
            uv = cl.uv;
            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        MeshRendererTransparentVS& MeshRendererTransparentVS::operator=(MeshRendererTransparentVS&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            

            

            
            vertices = std::move(cl.vertices);
            
            normals = std::move(cl.normals);
            
            tangents = std::move(cl.tangents);
            
            uv = std::move(cl.uv);
            

            

            

            

            

            

            

            

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> MeshRendererTransparentVS::textureSrvNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MeshRendererTransparentVS::textureUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MeshRendererTransparentVS::bufferSrvNames() const
        {
            return {
                
                "vertices"
                
                ,
                
                
                "normals"
                
                ,
                
                
                "tangents"
                
                ,
                
                
                "uv"
                
                
            };
        }

        std::vector<std::string> MeshRendererTransparentVS::bufferUavNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MeshRendererTransparentVS::samplerNames() const
        {
            return {
                
            };
        }

        std::vector<std::string> MeshRendererTransparentVS::srvNames() const
        {
            return {
                
                "vertices"
                
                ,
                
                
                "normals"
                
                ,
                
                
                "tangents"
                
                ,
                
                
                "uv"
                
                
            };
        }

        std::vector<std::string> MeshRendererTransparentVS::uavNames() const
        {
            return {
                
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension MeshRendererTransparentVS::textureDimension(const std::string& name) const
        {
            
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> MeshRendererTransparentVS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            return result;
        }

        std::vector<TextureUAV> MeshRendererTransparentVS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> MeshRendererTransparentVS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(vertices);
            
            result.emplace_back(normals);
            
            result.emplace_back(tangents);
            
            result.emplace_back(uv);
            
            return result;
        }

        std::vector<BufferUAV> MeshRendererTransparentVS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> MeshRendererTransparentVS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> MeshRendererTransparentVS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> MeshRendererTransparentVS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> MeshRendererTransparentVS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& MeshRendererTransparentVS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> MeshRendererTransparentVS::samplers() const
        {
            std::vector<Sampler> result;
            
            return result;
        }

        const std::vector<ShaderInputParameter>& MeshRendererTransparentVS::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool MeshRendererTransparentVS::hasTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererTransparentVS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererTransparentVS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("vertices")) return true;
            
            if (name == std::string("normals")) return true;
            
            if (name == std::string("tangents")) return true;
            
            if (name == std::string("uv")) return true;
            
            
            return false;
        }

        bool MeshRendererTransparentVS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererTransparentVS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererTransparentVS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererTransparentVS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererTransparentVS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& MeshRendererTransparentVS::textureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& MeshRendererTransparentVS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& MeshRendererTransparentVS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("vertices")) return vertices;
            
            if(name == std::string("normals")) return normals;
            
            if(name == std::string("tangents")) return tangents;
            
            if(name == std::string("uv")) return uv;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& MeshRendererTransparentVS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void MeshRendererTransparentVS::textureSrv(const std::string& name, TextureSRV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void MeshRendererTransparentVS::textureUav(const std::string& name, TextureUAV& texture)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void MeshRendererTransparentVS::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            
            
            if(name == std::string("vertices")) { vertices = buffer; return; }
            
            if(name == std::string("normals")) { normals = buffer; return; }
            
            if(name == std::string("tangents")) { tangents = buffer; return; }
            
            if(name == std::string("uv")) { uv = buffer; return; }
            
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        void MeshRendererTransparentVS::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& MeshRendererTransparentVS::sampler(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& MeshRendererTransparentVS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& MeshRendererTransparentVS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& MeshRendererTransparentVS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& MeshRendererTransparentVS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t MeshRendererTransparentVS::descriptorCount() const
        {
            return 5;
        }
    }
}