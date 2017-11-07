#include "Wireframe.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool Wireframe::hasVertexShader() const
        {
            return true;
        }

        bool Wireframe::hasPixelShader() const
        {
            return true;
        }

        bool Wireframe::hasGeometryShader() const
        {
            return false;
        }

        bool Wireframe::hasHullShader() const
        {
            return false;
        }

        bool Wireframe::hasDomainShader() const
        {
            return false;
        }

        bool Wireframe::hasComputeShader() const
        {
            return false;
        }

        uint32_t Wireframe::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* Wireframe::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Wireframe::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Wireframe::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Wireframe::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Wireframe::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Wireframe::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}