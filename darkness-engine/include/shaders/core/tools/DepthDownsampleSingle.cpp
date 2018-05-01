#include "DepthDownsampleSingle.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool DepthDownsampleSingle::hasVertexShader() const
        {
            return false;
        }

        bool DepthDownsampleSingle::hasPixelShader() const
        {
            return false;
        }

        bool DepthDownsampleSingle::hasGeometryShader() const
        {
            return false;
        }

        bool DepthDownsampleSingle::hasHullShader() const
        {
            return false;
        }

        bool DepthDownsampleSingle::hasDomainShader() const
        {
            return false;
        }

        bool DepthDownsampleSingle::hasComputeShader() const
        {
            return true;
        }

        uint32_t DepthDownsampleSingle::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* DepthDownsampleSingle::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DepthDownsampleSingle::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DepthDownsampleSingle::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DepthDownsampleSingle::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DepthDownsampleSingle::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DepthDownsampleSingle::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}