#include "MeshRendererPbr.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool MeshRendererPbr::hasVertexShader() const
        {
            return true;
        }

        bool MeshRendererPbr::hasPixelShader() const
        {
            return true;
        }

        bool MeshRendererPbr::hasGeometryShader() const
        {
            return false;
        }

        bool MeshRendererPbr::hasHullShader() const
        {
            return false;
        }

        bool MeshRendererPbr::hasDomainShader() const
        {
            return false;
        }

        bool MeshRendererPbr::hasComputeShader() const
        {
            return false;
        }

        uint32_t MeshRendererPbr::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* MeshRendererPbr::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* MeshRendererPbr::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* MeshRendererPbr::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* MeshRendererPbr::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* MeshRendererPbr::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* MeshRendererPbr::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}