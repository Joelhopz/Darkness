#include "ClearTexture1du.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClearTexture1du::hasVertexShader() const
        {
            return false;
        }

        bool ClearTexture1du::hasPixelShader() const
        {
            return false;
        }

        bool ClearTexture1du::hasGeometryShader() const
        {
            return false;
        }

        bool ClearTexture1du::hasHullShader() const
        {
            return false;
        }

        bool ClearTexture1du::hasDomainShader() const
        {
            return false;
        }

        bool ClearTexture1du::hasComputeShader() const
        {
            return true;
        }

        uint32_t ClearTexture1du::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* ClearTexture1du::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1du::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1du::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1du::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1du::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1du::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}