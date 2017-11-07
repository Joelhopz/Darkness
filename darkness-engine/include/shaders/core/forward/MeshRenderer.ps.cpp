#include "MeshRenderer.ps.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
        std::shared_ptr<const ShaderBinary> MeshRendererPS::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "C:/work/darkness/darkness-engine/data/shaders/dx12/core/forward/MeshRenderer.ps.cso", "C:/work/darkness/darkness-engine/data/shaders/dx12/core/forward/MeshRenderer.ps.support");
        }

        MeshRendererPS::MeshRendererPS()
            : m_constantRange{
            
            
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const ConstData*>(this)) + sizeof(ConstData)),
                    nullptr,
                    "ConstData"
                }
                
            
            
            }
        {}

        std::vector<TextureSRV> MeshRendererPS::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            
            result.emplace_back(environmentIrradiance);
            
            result.emplace_back(environmentSpecular);
            
            result.emplace_back(albedo);
            
            result.emplace_back(normal);
            
            result.emplace_back(roughness);
            
            result.emplace_back(metalness);
            
            result.emplace_back(occlusion);
            
            result.emplace_back(shadowMap);
            
            return result;
        }

        std::vector<TextureUAV> MeshRendererPS::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            
            return result;
        }

        std::vector<BufferSRV> MeshRendererPS::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            
            result.emplace_back(lightWorldPosition);
            
            result.emplace_back(lightDirection);
            
            result.emplace_back(lightColor);
            
            result.emplace_back(lightParameters);
            
            result.emplace_back(lightType);
            
            result.emplace_back(lightIntensity);
            
            result.emplace_back(lightRange);
            
            return result;
        }

        std::vector<BufferUAV> MeshRendererPS::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            
            return result;
        }

        std::vector<TextureBindlessSRV> MeshRendererPS::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            
            return result;
        }

        std::vector<TextureBindlessUAV> MeshRendererPS::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            
            return result;
        }

        std::vector<BufferBindlessSRV> MeshRendererPS::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            
            return result;
        }

        std::vector<BufferBindlessUAV> MeshRendererPS::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            
            return result;
        }

        std::vector<Shader::ConstantRange>& MeshRendererPS::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> MeshRendererPS::samplers() const
        {
            std::vector<Sampler> result;
            
            result.emplace_back(tex_sampler);
            
            result.emplace_back(shadow_sampler);
            
            return result;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

        bool MeshRendererPS::hasTextureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("environmentIrradiance")) return true;
            
            if(name == std::string("environmentSpecular")) return true;
            
            if(name == std::string("albedo")) return true;
            
            if(name == std::string("normal")) return true;
            
            if(name == std::string("roughness")) return true;
            
            if(name == std::string("metalness")) return true;
            
            if(name == std::string("occlusion")) return true;
            
            if(name == std::string("shadowMap")) return true;
            
            
            return false;
        }

        bool MeshRendererPS::hasTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererPS::hasBufferSrv(const std::string& name) const
        {
            
            
            if (name == std::string("lightWorldPosition")) return true;
            
            if (name == std::string("lightDirection")) return true;
            
            if (name == std::string("lightColor")) return true;
            
            if (name == std::string("lightParameters")) return true;
            
            if (name == std::string("lightType")) return true;
            
            if (name == std::string("lightIntensity")) return true;
            
            if (name == std::string("lightRange")) return true;
            
            
            return false;
        }

        bool MeshRendererPS::hasBufferUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererPS::hasBindlessTextureSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererPS::hasBindlessTextureUav(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererPS::hasBindlessBufferSrv(const std::string& name) const
        {
            
            return false;
        }

        bool MeshRendererPS::hasBindlessBufferUav(const std::string& name) const
        {
            
            return false;
        }

        const TextureSRV& MeshRendererPS::textureSrv(const std::string& name) const
        {
            
            
            if(name == std::string("environmentIrradiance")) return environmentIrradiance;
            
            if(name == std::string("environmentSpecular")) return environmentSpecular;
            
            if(name == std::string("albedo")) return albedo;
            
            if(name == std::string("normal")) return normal;
            
            if(name == std::string("roughness")) return roughness;
            
            if(name == std::string("metalness")) return metalness;
            
            if(name == std::string("occlusion")) return occlusion;
            
            if(name == std::string("shadowMap")) return shadowMap;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& MeshRendererPS::textureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& MeshRendererPS::bufferSrv(const std::string& name) const
        {
            
            
            if(name == std::string("lightWorldPosition")) return lightWorldPosition;
            
            if(name == std::string("lightDirection")) return lightDirection;
            
            if(name == std::string("lightColor")) return lightColor;
            
            if(name == std::string("lightParameters")) return lightParameters;
            
            if(name == std::string("lightType")) return lightType;
            
            if(name == std::string("lightIntensity")) return lightIntensity;
            
            if(name == std::string("lightRange")) return lightRange;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& MeshRendererPS::bufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        const Sampler& MeshRendererPS::sampler(const std::string& name) const
        {
            
            
            if(name == std::string("tex_sampler")) return tex_sampler;
            
            if(name == std::string("shadow_sampler")) return shadow_sampler;
            
            
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& MeshRendererPS::bindlessTextureSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& MeshRendererPS::bindlessTextureUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& MeshRendererPS::bindlessBufferSrv(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& MeshRendererPS::bindlessBufferUav(const std::string& name) const
        {
            
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t MeshRendererPS::descriptorCount() const
        {
            return 16;
        }
    }
}