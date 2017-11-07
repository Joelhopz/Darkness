#include "MeshTransparentComposite.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool MeshTransparentComposite::hasVertexShader() const
        {
            return true;
        }

        bool MeshTransparentComposite::hasPixelShader() const
        {
            return true;
        }

        bool MeshTransparentComposite::hasGeometryShader() const
        {
            return false;
        }

        bool MeshTransparentComposite::hasHullShader() const
        {
            return false;
        }

        bool MeshTransparentComposite::hasDomainShader() const
        {
            return false;
        }

        bool MeshTransparentComposite::hasComputeShader() const
        {
            return false;
        }

        uint32_t MeshTransparentComposite::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* MeshTransparentComposite::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* MeshTransparentComposite::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* MeshTransparentComposite::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* MeshTransparentComposite::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* MeshTransparentComposite::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* MeshTransparentComposite::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}