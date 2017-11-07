#include "DrawWithTexture.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool DrawWithTexture::hasVertexShader() const
        {
            return true;
        }

        bool DrawWithTexture::hasPixelShader() const
        {
            return true;
        }

        bool DrawWithTexture::hasGeometryShader() const
        {
            return false;
        }

        bool DrawWithTexture::hasHullShader() const
        {
            return false;
        }

        bool DrawWithTexture::hasDomainShader() const
        {
            return false;
        }

        bool DrawWithTexture::hasComputeShader() const
        {
            return false;
        }

        uint32_t DrawWithTexture::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* DrawWithTexture::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* DrawWithTexture::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* DrawWithTexture::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DrawWithTexture::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DrawWithTexture::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DrawWithTexture::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}