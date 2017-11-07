#include "ShadowRender.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ShadowRender::hasVertexShader() const
        {
            return true;
        }

        bool ShadowRender::hasPixelShader() const
        {
            return true;
        }

        bool ShadowRender::hasGeometryShader() const
        {
            return false;
        }

        bool ShadowRender::hasHullShader() const
        {
            return false;
        }

        bool ShadowRender::hasDomainShader() const
        {
            return false;
        }

        bool ShadowRender::hasComputeShader() const
        {
            return false;
        }

        uint32_t ShadowRender::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* ShadowRender::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* ShadowRender::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* ShadowRender::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ShadowRender::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ShadowRender::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ShadowRender::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}