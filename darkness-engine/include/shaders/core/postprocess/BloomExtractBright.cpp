#include "BloomExtractBright.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool BloomExtractBright::hasVertexShader() const
        {
            return true;
        }

        bool BloomExtractBright::hasPixelShader() const
        {
            return true;
        }

        bool BloomExtractBright::hasGeometryShader() const
        {
            return false;
        }

        bool BloomExtractBright::hasHullShader() const
        {
            return false;
        }

        bool BloomExtractBright::hasDomainShader() const
        {
            return false;
        }

        bool BloomExtractBright::hasComputeShader() const
        {
            return false;
        }

        uint32_t BloomExtractBright::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* BloomExtractBright::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* BloomExtractBright::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* BloomExtractBright::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* BloomExtractBright::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* BloomExtractBright::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* BloomExtractBright::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}