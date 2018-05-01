#include "DepthDownsample.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool DepthDownsample::hasVertexShader() const
        {
            return false;
        }

        bool DepthDownsample::hasPixelShader() const
        {
            return false;
        }

        bool DepthDownsample::hasGeometryShader() const
        {
            return false;
        }

        bool DepthDownsample::hasHullShader() const
        {
            return false;
        }

        bool DepthDownsample::hasDomainShader() const
        {
            return false;
        }

        bool DepthDownsample::hasComputeShader() const
        {
            return true;
        }

        uint32_t DepthDownsample::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* DepthDownsample::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DepthDownsample::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DepthDownsample::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DepthDownsample::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DepthDownsample::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DepthDownsample::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}