#include "Pick.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool Pick::hasVertexShader() const
        {
            return false;
        }

        bool Pick::hasPixelShader() const
        {
            return false;
        }

        bool Pick::hasGeometryShader() const
        {
            return false;
        }

        bool Pick::hasHullShader() const
        {
            return false;
        }

        bool Pick::hasDomainShader() const
        {
            return false;
        }

        bool Pick::hasComputeShader() const
        {
            return true;
        }

        uint32_t Pick::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* Pick::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Pick::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Pick::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Pick::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Pick::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Pick::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}