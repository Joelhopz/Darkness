#include "Outline.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool Outline::hasVertexShader() const
        {
            return true;
        }

        bool Outline::hasPixelShader() const
        {
            return true;
        }

        bool Outline::hasGeometryShader() const
        {
            return true;
        }

        bool Outline::hasHullShader() const
        {
            return false;
        }

        bool Outline::hasDomainShader() const
        {
            return false;
        }

        bool Outline::hasComputeShader() const
        {
            return false;
        }

        uint32_t Outline::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                gs.descriptorCount()+
                
                
                
                0;
        }

        const Shader* Outline::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Outline::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Outline::geometryShader() const
        {
            const Shader* shader = &gs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Outline::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Outline::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Outline::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}