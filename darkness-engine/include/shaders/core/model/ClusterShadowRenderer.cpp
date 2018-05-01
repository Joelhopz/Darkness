#include "ClusterShadowRenderer.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClusterShadowRenderer::hasVertexShader() const
        {
            return true;
        }

        bool ClusterShadowRenderer::hasPixelShader() const
        {
            return true;
        }

        bool ClusterShadowRenderer::hasGeometryShader() const
        {
            return false;
        }

        bool ClusterShadowRenderer::hasHullShader() const
        {
            return false;
        }

        bool ClusterShadowRenderer::hasDomainShader() const
        {
            return false;
        }

        bool ClusterShadowRenderer::hasComputeShader() const
        {
            return false;
        }

        uint32_t ClusterShadowRenderer::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* ClusterShadowRenderer::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* ClusterShadowRenderer::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* ClusterShadowRenderer::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClusterShadowRenderer::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClusterShadowRenderer::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClusterShadowRenderer::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}