#include "Cubemap.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool Cubemap::hasVertexShader() const
        {
            return true;
        }

        bool Cubemap::hasPixelShader() const
        {
            return true;
        }

        bool Cubemap::hasGeometryShader() const
        {
            return false;
        }

        bool Cubemap::hasHullShader() const
        {
            return false;
        }

        bool Cubemap::hasDomainShader() const
        {
            return false;
        }

        bool Cubemap::hasComputeShader() const
        {
            return false;
        }

        uint32_t Cubemap::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* Cubemap::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Cubemap::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Cubemap::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Cubemap::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Cubemap::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Cubemap::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}