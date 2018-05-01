#include "PrefixSumBuckets.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool PrefixSumBuckets::hasVertexShader() const
        {
            return false;
        }

        bool PrefixSumBuckets::hasPixelShader() const
        {
            return false;
        }

        bool PrefixSumBuckets::hasGeometryShader() const
        {
            return false;
        }

        bool PrefixSumBuckets::hasHullShader() const
        {
            return false;
        }

        bool PrefixSumBuckets::hasDomainShader() const
        {
            return false;
        }

        bool PrefixSumBuckets::hasComputeShader() const
        {
            return true;
        }

        uint32_t PrefixSumBuckets::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* PrefixSumBuckets::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefixSumBuckets::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefixSumBuckets::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefixSumBuckets::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefixSumBuckets::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefixSumBuckets::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}