#pragma once

#include "shaders/ShaderTypes.h"
#include <memory>
#include "DrawWithConstants.vs.h"
#include "DrawWithConstants.ps.h"





namespace engine
{
    namespace implementation
    {
        class PipelineImpl;
    }

    namespace shaders
    {
        class Shader;

        class DrawWithConstants : public PipelineConfiguration
        {
        public:
            DrawWithConstantsVS vs;
            DrawWithConstantsPS ps;
            
            
            
            

            uint32_t descriptorCount() const override;

        private:
            friend class PipelineImpl;
            const Shader* vertexShader() const override;
            const Shader* pixelShader() const override;
            const Shader* geometryShader() const override;
            const Shader* hullShader() const override;
            const Shader* domainShader() const override;
            const Shader* computeShader() const override;

            bool hasVertexShader() const override;
            bool hasPixelShader() const override;
            bool hasGeometryShader() const override;
            bool hasHullShader() const override;
            bool hasDomainShader() const override;
            bool hasComputeShader() const override;
        };
    }
}