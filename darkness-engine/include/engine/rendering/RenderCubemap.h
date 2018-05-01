#pragma once

#include "engine/graphics/CommandList.h"
#include "components/Transform.h"
#include "components/MeshRendererComponent.h"
#include "components/MaterialComponent.h"
#include "components/LightComponent.h"
#include "components/Camera.h"
#include "shaders/core/tools/Cubemap.h"
#include "shaders/core/tools/EquirectToCubemap.h"
#include "shaders/core/ibl/Irradiance.h"
#include "shaders/core/ibl/PrefilterConvolution.h"
#include "shaders/core/ibl/BrdfConvolution.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/Scene.h"
#include "engine/rendering/LightData.h"
#include <memory>

namespace engine
{
    class RenderCubemap
    {
    public:
        RenderCubemap(Device& device, ShaderStorage& shaderStorage);

        void createCubemapFromEquirect(
            Device& device,
            TextureSRV& equirectEnvironmentMap
        );

        void createIrradianceCubemap(
            Device& device,
            TextureSRV& diffuseEnvironmentMap,
            CommandList& cmd
        );

        void prefilterConvolution(
            Device& device,
            TextureSRV& diffuseEnvironmentMap,
            CommandList& cmd
        );

        void brdfConvolution(CommandList& cmd);

        void render(
            TextureRTV& currentRenderTarget,
            Camera& camera,
            CommandList& cmd
        );

        void clearResources();

        TextureSRV& cubemap() { return m_cubemap; }
        void cubemap(TextureSRV& cubemap) { m_cubemap = cubemap; }
        TextureSRV& irradiance() { return m_irradiance; }
        TextureSRV& prefilteredEnvironmentMap() { return m_prefilteredEnvironmentMap; }
        TextureSRV& brdfConvolution() { return m_brdfConvolution; }

    private:
        Device& m_device;
        ShaderStorage& m_shaderStorage;
        engine::Pipeline<shaders::Cubemap> m_cubemapPipeline;
        engine::Pipeline<shaders::EquirectToCubemap> m_equirectToCubemapPipeline;
        engine::Pipeline<shaders::Irradiance> m_irradiancePipeline;
        engine::Pipeline<shaders::PrefilterConvolution> m_prefilterConvolutionPipeline;
        engine::Pipeline<shaders::BrdfConvolution> m_brdfConvolutionPipeline;
        engine::Camera m_cubemapCamera;

        TextureSRV m_cubemap;
        TextureSRV m_irradiance;
        TextureSRV m_prefilteredEnvironmentMap;

        TextureSRV m_brdfConvolution;
        TextureRTV m_brdfConvolutionRTV;

        uint32_t m_lastCubemapSize;
        std::vector<std::vector<TextureRTV>> m_cubemapRenderTargets;

        uint32_t m_lastIrradianceSize;
        std::vector<TextureRTV> m_irradianceRenderTargets;

        uint32_t m_lastPrefilterConvolutionSize;
        std::vector<std::vector<TextureRTV>> m_prefilterConvolutionTargets;
    };
}
