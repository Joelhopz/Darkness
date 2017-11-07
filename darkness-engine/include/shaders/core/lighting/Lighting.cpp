#include "Lighting.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool Lighting::hasVertexShader() const
        {
            return true;
        }

        bool Lighting::hasPixelShader() const
        {
            return true;
        }

        bool Lighting::hasGeometryShader() const
        {
            return false;
        }

        bool Lighting::hasHullShader() const
        {
            return false;
        }

        bool Lighting::hasDomainShader() const
        {
            return false;
        }

        bool Lighting::hasComputeShader() const
        {
            return false;
        }

        uint32_t Lighting::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* Lighting::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Lighting::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Lighting::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Lighting::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Lighting::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Lighting::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}