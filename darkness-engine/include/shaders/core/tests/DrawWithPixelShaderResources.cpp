#include "DrawWithPixelShaderResources.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool DrawWithPixelShaderResources::hasVertexShader() const
        {
            return true;
        }

        bool DrawWithPixelShaderResources::hasPixelShader() const
        {
            return true;
        }

        bool DrawWithPixelShaderResources::hasGeometryShader() const
        {
            return false;
        }

        bool DrawWithPixelShaderResources::hasHullShader() const
        {
            return false;
        }

        bool DrawWithPixelShaderResources::hasDomainShader() const
        {
            return false;
        }

        bool DrawWithPixelShaderResources::hasComputeShader() const
        {
            return false;
        }

        uint32_t DrawWithPixelShaderResources::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* DrawWithPixelShaderResources::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* DrawWithPixelShaderResources::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* DrawWithPixelShaderResources::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DrawWithPixelShaderResources::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DrawWithPixelShaderResources::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DrawWithPixelShaderResources::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}