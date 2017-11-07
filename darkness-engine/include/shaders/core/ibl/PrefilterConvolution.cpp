#include "PrefilterConvolution.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool PrefilterConvolution::hasVertexShader() const
        {
            return true;
        }

        bool PrefilterConvolution::hasPixelShader() const
        {
            return true;
        }

        bool PrefilterConvolution::hasGeometryShader() const
        {
            return false;
        }

        bool PrefilterConvolution::hasHullShader() const
        {
            return false;
        }

        bool PrefilterConvolution::hasDomainShader() const
        {
            return false;
        }

        bool PrefilterConvolution::hasComputeShader() const
        {
            return false;
        }

        uint32_t PrefilterConvolution::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* PrefilterConvolution::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* PrefilterConvolution::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* PrefilterConvolution::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefilterConvolution::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefilterConvolution::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* PrefilterConvolution::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}