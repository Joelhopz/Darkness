#include "MeshRenderer.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool MeshRenderer::hasVertexShader() const
        {
            return true;
        }

        bool MeshRenderer::hasPixelShader() const
        {
            return true;
        }

        bool MeshRenderer::hasGeometryShader() const
        {
            return false;
        }

        bool MeshRenderer::hasHullShader() const
        {
            return false;
        }

        bool MeshRenderer::hasDomainShader() const
        {
            return false;
        }

        bool MeshRenderer::hasComputeShader() const
        {
            return false;
        }

        uint32_t MeshRenderer::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* MeshRenderer::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* MeshRenderer::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* MeshRenderer::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* MeshRenderer::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* MeshRenderer::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* MeshRenderer::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}