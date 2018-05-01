#include "ClearTexture2du2.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClearTexture2du2::hasVertexShader() const
        {
            return false;
        }

        bool ClearTexture2du2::hasPixelShader() const
        {
            return false;
        }

        bool ClearTexture2du2::hasGeometryShader() const
        {
            return false;
        }

        bool ClearTexture2du2::hasHullShader() const
        {
            return false;
        }

        bool ClearTexture2du2::hasDomainShader() const
        {
            return false;
        }

        bool ClearTexture2du2::hasComputeShader() const
        {
            return true;
        }

        uint32_t ClearTexture2du2::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* ClearTexture2du2::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2du2::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2du2::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2du2::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2du2::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2du2::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}