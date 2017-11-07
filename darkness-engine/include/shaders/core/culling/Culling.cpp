#include "Culling.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool Culling::hasVertexShader() const
        {
            return false;
        }

        bool Culling::hasPixelShader() const
        {
            return false;
        }

        bool Culling::hasGeometryShader() const
        {
            return false;
        }

        bool Culling::hasHullShader() const
        {
            return false;
        }

        bool Culling::hasDomainShader() const
        {
            return false;
        }

        bool Culling::hasComputeShader() const
        {
            return true;
        }

        uint32_t Culling::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* Culling::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Culling::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Culling::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Culling::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Culling::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Culling::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}