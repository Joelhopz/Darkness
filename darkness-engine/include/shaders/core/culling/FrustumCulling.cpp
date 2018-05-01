#include "FrustumCulling.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool FrustumCulling::hasVertexShader() const
        {
            return false;
        }

        bool FrustumCulling::hasPixelShader() const
        {
            return false;
        }

        bool FrustumCulling::hasGeometryShader() const
        {
            return false;
        }

        bool FrustumCulling::hasHullShader() const
        {
            return false;
        }

        bool FrustumCulling::hasDomainShader() const
        {
            return false;
        }

        bool FrustumCulling::hasComputeShader() const
        {
            return true;
        }

        uint32_t FrustumCulling::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* FrustumCulling::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* FrustumCulling::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* FrustumCulling::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* FrustumCulling::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* FrustumCulling::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* FrustumCulling::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}