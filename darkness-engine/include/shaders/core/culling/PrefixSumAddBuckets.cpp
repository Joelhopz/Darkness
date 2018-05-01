#include "PrefixSumAddBuckets.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool PrefixSumAddBuckets::hasVertexShader() const
        {
            return false;
        }

        bool PrefixSumAddBuckets::hasPixelShader() const
        {
            return false;
        }

        bool PrefixSumAddBuckets::hasGeometryShader() const
        {
            return false;
        }

        bool PrefixSumAddBuckets::hasHullShader() const
        {
            return false;
        }

        bool PrefixSumAddBuckets::hasDomainShader() const
        {
            return false;
        }

        bool PrefixSumAddBuckets::hasComputeShader() const
        {
            return true;
        }

        uint32_t PrefixSumAddBuckets::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* PrefixSumAddBuckets::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefixSumAddBuckets::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefixSumAddBuckets::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefixSumAddBuckets::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefixSumAddBuckets::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefixSumAddBuckets::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}