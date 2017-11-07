#include "DrawWithoutBuffers.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool DrawWithoutBuffers::hasVertexShader() const
        {
            return true;
        }

        bool DrawWithoutBuffers::hasPixelShader() const
        {
            return true;
        }

        bool DrawWithoutBuffers::hasGeometryShader() const
        {
            return false;
        }

        bool DrawWithoutBuffers::hasHullShader() const
        {
            return false;
        }

        bool DrawWithoutBuffers::hasDomainShader() const
        {
            return false;
        }

        bool DrawWithoutBuffers::hasComputeShader() const
        {
            return false;
        }

        uint32_t DrawWithoutBuffers::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* DrawWithoutBuffers::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* DrawWithoutBuffers::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* DrawWithoutBuffers::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DrawWithoutBuffers::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DrawWithoutBuffers::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DrawWithoutBuffers::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}