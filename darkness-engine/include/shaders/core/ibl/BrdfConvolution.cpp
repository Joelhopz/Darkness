#include "BrdfConvolution.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool BrdfConvolution::hasVertexShader() const
        {
            return true;
        }

        bool BrdfConvolution::hasPixelShader() const
        {
            return true;
        }

        bool BrdfConvolution::hasGeometryShader() const
        {
            return false;
        }

        bool BrdfConvolution::hasHullShader() const
        {
            return false;
        }

        bool BrdfConvolution::hasDomainShader() const
        {
            return false;
        }

        bool BrdfConvolution::hasComputeShader() const
        {
            return false;
        }

        uint32_t BrdfConvolution::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* BrdfConvolution::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* BrdfConvolution::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* BrdfConvolution::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* BrdfConvolution::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* BrdfConvolution::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* BrdfConvolution::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}