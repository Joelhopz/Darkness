#pragma once

#include "engine/graphics/Resources.h"
#include "engine/graphics/Sampler.h"
#include "shaders/ShaderTypes.h"
#include <vector>
#include <memory>
#include <string>

namespace engine
{
    class ShaderBinary;
    class ShaderStorage;
    class Device;
    class Sampler;

    namespace implementation
    {
        class PipelineImpl;
        class CommandListImpl;
    }

    namespace shaders
    {
        class MeshRendererPbr;

        namespace meshrendererpbrps_namespace
        {
        struct ConstData
            {
                Float roughnessStrength;
                Float metalnessStrength;
                Float occlusionStrength;
                Uint hasAlbedo;
                Uint hasNormal;
                Uint hasRoughness;
                Uint hasMetalness;
                Uint hasOcclusion;
                Float4 color;
                Float2 materialScale;
                Uint objectId;
                Uint mouseX;
                Uint mouseY;
                Float2 jitter;
                Uint albedoId;
                Uint normalId;
                Uint roughnessId;
                Uint metalnessId;
                Uint occlusionId;
                };
        }
        class MeshRendererPbrPS : public PixelShader
            , public meshrendererpbrps_namespace::ConstData
            {
        public:

            MeshRendererPbrPS();

            MeshRendererPbrPS(const MeshRendererPbrPS&);
            MeshRendererPbrPS(MeshRendererPbrPS&&);
            MeshRendererPbrPS& operator=(const MeshRendererPbrPS&);
            MeshRendererPbrPS& operator=(MeshRendererPbrPS&&);

            TextureBindlessSRV materialTextures;
            BufferUAV pick;
            

            

            Sampler tex_sampler;
            

        protected:
            friend class implementation::CommandListImpl;
            const TextureSRV& textureSrv(const std::string& name) const override;
            const TextureUAV& textureUav(const std::string& name) const override;
            const BufferSRV& bufferSrv(const std::string& name) const override;
            const BufferUAV& bufferUav(const std::string& name) const override;
            const Sampler& sampler(const std::string& name) const override;
            
            std::vector<std::string> textureSrvNames() const override;
            std::vector<std::string> textureUavNames() const override;
            std::vector<std::string> bufferSrvNames() const override;
            std::vector<std::string> bufferUavNames() const override;
            std::vector<std::string> samplerNames() const override;

            std::vector<std::string> srvNames() const override;
            std::vector<std::string> uavNames() const override;

            engine::ResourceDimension textureDimension(const std::string& name) const;

            const TextureBindlessSRV& bindlessTextureSrv(const std::string& name) const override;
            const TextureBindlessUAV& bindlessTextureUav(const std::string& name) const override;
            const BufferBindlessSRV& bindlessBufferSrv(const std::string& name) const override;
            const BufferBindlessUAV& bindlessBufferUav(const std::string& name) const override;

            void textureSrv(const std::string& name, TextureSRV& texture) override;
            void textureUav(const std::string& name, TextureUAV& texture) override;
            void bufferSrv(const std::string& name, BufferSRV& buffer) override;
            void bufferUav(const std::string& name, BufferUAV& buffer) override;

            bool hasTextureSrv(const std::string& name) const override;
            bool hasTextureUav(const std::string& name) const override;
            bool hasBufferSrv(const std::string& name) const override;
            bool hasBufferUav(const std::string& name) const override;

            bool hasBindlessTextureSrv(const std::string& name) const override;
            bool hasBindlessTextureUav(const std::string& name) const override;
            bool hasBindlessBufferSrv(const std::string& name) const override;
            bool hasBindlessBufferUav(const std::string& name) const override;

        private:
            friend class implementation::PipelineImpl;
            std::shared_ptr<const ShaderBinary> load(const Device& device, ShaderStorage& storage) const override;
            std::vector<TextureSRV> texture_srvs() const override;
            std::vector<TextureUAV> texture_uavs() const override;
            std::vector<BufferSRV> buffer_srvs() const override;
            std::vector<BufferUAV> buffer_uavs() const override;
            
            std::vector<TextureBindlessSRV> bindless_texture_srvs() const override;
            std::vector<TextureBindlessUAV> bindless_texture_uavs() const override;
            std::vector<BufferBindlessSRV> bindless_buffer_srvs() const override;
            std::vector<BufferBindlessUAV> bindless_buffer_uavs() const override;
            
            std::vector<Shader::ConstantRange>& constants() override;
            std::vector<Sampler> samplers() const override;

            const std::vector<ShaderInputParameter>& inputParameters() const override;
        private:
            friend class MeshRendererPbr;
            uint32_t descriptorCount() const;
            std::vector<Shader::ConstantRange> m_constantRange;
            std::vector<ShaderInputParameter> m_inputParameters;
        };
    }
}