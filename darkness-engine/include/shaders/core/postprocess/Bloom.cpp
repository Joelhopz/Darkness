#include "Bloom.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool Bloom::hasVertexShader() const
        {
            return true;
        }

        bool Bloom::hasPixelShader() const
        {
            return true;
        }

        bool Bloom::hasGeometryShader() const
        {
            return false;
        }

        bool Bloom::hasHullShader() const
        {
            return false;
        }

        bool Bloom::hasDomainShader() const
        {
            return false;
        }

        bool Bloom::hasComputeShader() const
        {
            return false;
        }

        uint32_t Bloom::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* Bloom::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Bloom::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Bloom::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Bloom::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Bloom::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Bloom::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}