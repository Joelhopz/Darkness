#pragma once

#include "shaders/ShaderTypes.h"
#include <memory>





#include "CopyTexture1df.cs.h"

namespace engine
{
    namespace implementation
    {
        class PipelineImpl;
    }

    namespace shaders
    {
        class Shader;

        class CopyTexture1df : public PipelineConfiguration
        {
        public:
            
            
            
            
            
            CopyTexture1dfCS cs;

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