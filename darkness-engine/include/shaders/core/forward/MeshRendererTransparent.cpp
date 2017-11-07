#include "MeshRendererTransparent.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool MeshRendererTransparent::hasVertexShader() const
        {
            return true;
        }

        bool MeshRendererTransparent::hasPixelShader() const
        {
            return true;
        }

        bool MeshRendererTransparent::hasGeometryShader() const
        {
            return false;
        }

        bool MeshRendererTransparent::hasHullShader() const
        {
            return false;
        }

        bool MeshRendererTransparent::hasDomainShader() const
        {
            return false;
        }

        bool MeshRendererTransparent::hasComputeShader() const
        {
            return false;
        }

        uint32_t MeshRendererTransparent::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* MeshRendererTransparent::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* MeshRendererTransparent::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* MeshRendererTransparent::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* MeshRendererTransparent::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* MeshRendererTransparent::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* MeshRendererTransparent::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}