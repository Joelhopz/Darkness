#include "ClearTexture2du.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClearTexture2du::hasVertexShader() const
        {
            return false;
        }

        bool ClearTexture2du::hasPixelShader() const
        {
            return false;
        }

        bool ClearTexture2du::hasGeometryShader() const
        {
            return false;
        }

        bool ClearTexture2du::hasHullShader() const
        {
            return false;
        }

        bool ClearTexture2du::hasDomainShader() const
        {
            return false;
        }

        bool ClearTexture2du::hasComputeShader() const
        {
            return true;
        }

        uint32_t ClearTexture2du::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* ClearTexture2du::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2du::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2du::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2du::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2du::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2du::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}