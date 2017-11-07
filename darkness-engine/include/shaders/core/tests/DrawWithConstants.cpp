#include "DrawWithConstants.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool DrawWithConstants::hasVertexShader() const
        {
            return true;
        }

        bool DrawWithConstants::hasPixelShader() const
        {
            return true;
        }

        bool DrawWithConstants::hasGeometryShader() const
        {
            return false;
        }

        bool DrawWithConstants::hasHullShader() const
        {
            return false;
        }

        bool DrawWithConstants::hasDomainShader() const
        {
            return false;
        }

        bool DrawWithConstants::hasComputeShader() const
        {
            return false;
        }

        uint32_t DrawWithConstants::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* DrawWithConstants::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* DrawWithConstants::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* DrawWithConstants::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DrawWithConstants::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DrawWithConstants::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DrawWithConstants::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}