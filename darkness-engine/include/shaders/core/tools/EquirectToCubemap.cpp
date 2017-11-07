#include "EquirectToCubemap.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool EquirectToCubemap::hasVertexShader() const
        {
            return true;
        }

        bool EquirectToCubemap::hasPixelShader() const
        {
            return true;
        }

        bool EquirectToCubemap::hasGeometryShader() const
        {
            return false;
        }

        bool EquirectToCubemap::hasHullShader() const
        {
            return false;
        }

        bool EquirectToCubemap::hasDomainShader() const
        {
            return false;
        }

        bool EquirectToCubemap::hasComputeShader() const
        {
            return false;
        }

        uint32_t EquirectToCubemap::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* EquirectToCubemap::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* EquirectToCubemap::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* EquirectToCubemap::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* EquirectToCubemap::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* EquirectToCubemap::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* EquirectToCubemap::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}