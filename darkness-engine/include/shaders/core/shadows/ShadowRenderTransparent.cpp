#include "ShadowRenderTransparent.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ShadowRenderTransparent::hasVertexShader() const
        {
            return true;
        }

        bool ShadowRenderTransparent::hasPixelShader() const
        {
            return true;
        }

        bool ShadowRenderTransparent::hasGeometryShader() const
        {
            return false;
        }

        bool ShadowRenderTransparent::hasHullShader() const
        {
            return false;
        }

        bool ShadowRenderTransparent::hasDomainShader() const
        {
            return false;
        }

        bool ShadowRenderTransparent::hasComputeShader() const
        {
            return false;
        }

        uint32_t ShadowRenderTransparent::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* ShadowRenderTransparent::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* ShadowRenderTransparent::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* ShadowRenderTransparent::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ShadowRenderTransparent::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ShadowRenderTransparent::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ShadowRenderTransparent::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}