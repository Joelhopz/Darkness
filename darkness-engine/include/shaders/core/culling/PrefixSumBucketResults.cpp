#include "PrefixSumBucketResults.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool PrefixSumBucketResults::hasVertexShader() const
        {
            return false;
        }

        bool PrefixSumBucketResults::hasPixelShader() const
        {
            return false;
        }

        bool PrefixSumBucketResults::hasGeometryShader() const
        {
            return false;
        }

        bool PrefixSumBucketResults::hasHullShader() const
        {
            return false;
        }

        bool PrefixSumBucketResults::hasDomainShader() const
        {
            return false;
        }

        bool PrefixSumBucketResults::hasComputeShader() const
        {
            return true;
        }

        uint32_t PrefixSumBucketResults::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* PrefixSumBucketResults::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefixSumBucketResults::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefixSumBucketResults::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefixSumBucketResults::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefixSumBucketResults::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefixSumBucketResults::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}