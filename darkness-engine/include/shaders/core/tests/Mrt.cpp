#include "Mrt.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool Mrt::hasVertexShader() const
        {
            return true;
        }

        bool Mrt::hasPixelShader() const
        {
            return true;
        }

        bool Mrt::hasGeometryShader() const
        {
            return false;
        }

        bool Mrt::hasHullShader() const
        {
            return false;
        }

        bool Mrt::hasDomainShader() const
        {
            return false;
        }

        bool Mrt::hasComputeShader() const
        {
            return false;
        }

        uint32_t Mrt::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* Mrt::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Mrt::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Mrt::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Mrt::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Mrt::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Mrt::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}