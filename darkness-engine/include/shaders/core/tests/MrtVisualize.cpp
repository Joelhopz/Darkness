#include "MrtVisualize.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool MrtVisualize::hasVertexShader() const
        {
            return true;
        }

        bool MrtVisualize::hasPixelShader() const
        {
            return true;
        }

        bool MrtVisualize::hasGeometryShader() const
        {
            return false;
        }

        bool MrtVisualize::hasHullShader() const
        {
            return false;
        }

        bool MrtVisualize::hasDomainShader() const
        {
            return false;
        }

        bool MrtVisualize::hasComputeShader() const
        {
            return false;
        }

        uint32_t MrtVisualize::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* MrtVisualize::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* MrtVisualize::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* MrtVisualize::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* MrtVisualize::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* MrtVisualize::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* MrtVisualize::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}