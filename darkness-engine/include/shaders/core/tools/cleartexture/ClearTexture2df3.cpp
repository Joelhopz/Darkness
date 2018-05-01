#include "ClearTexture2df3.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClearTexture2df3::hasVertexShader() const
        {
            return false;
        }

        bool ClearTexture2df3::hasPixelShader() const
        {
            return false;
        }

        bool ClearTexture2df3::hasGeometryShader() const
        {
            return false;
        }

        bool ClearTexture2df3::hasHullShader() const
        {
            return false;
        }

        bool ClearTexture2df3::hasDomainShader() const
        {
            return false;
        }

        bool ClearTexture2df3::hasComputeShader() const
        {
            return true;
        }

        uint32_t ClearTexture2df3::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* ClearTexture2df3::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2df3::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2df3::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2df3::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2df3::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2df3::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}