#include "ClearTexture1du3.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClearTexture1du3::hasVertexShader() const
        {
            return false;
        }

        bool ClearTexture1du3::hasPixelShader() const
        {
            return false;
        }

        bool ClearTexture1du3::hasGeometryShader() const
        {
            return false;
        }

        bool ClearTexture1du3::hasHullShader() const
        {
            return false;
        }

        bool ClearTexture1du3::hasDomainShader() const
        {
            return false;
        }

        bool ClearTexture1du3::hasComputeShader() const
        {
            return true;
        }

        uint32_t ClearTexture1du3::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* ClearTexture1du3::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1du3::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1du3::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1du3::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1du3::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1du3::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}