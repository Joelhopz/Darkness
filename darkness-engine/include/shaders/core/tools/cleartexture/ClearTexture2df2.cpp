#include "ClearTexture2df2.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClearTexture2df2::hasVertexShader() const
        {
            return false;
        }

        bool ClearTexture2df2::hasPixelShader() const
        {
            return false;
        }

        bool ClearTexture2df2::hasGeometryShader() const
        {
            return false;
        }

        bool ClearTexture2df2::hasHullShader() const
        {
            return false;
        }

        bool ClearTexture2df2::hasDomainShader() const
        {
            return false;
        }

        bool ClearTexture2df2::hasComputeShader() const
        {
            return true;
        }

        uint32_t ClearTexture2df2::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* ClearTexture2df2::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2df2::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2df2::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2df2::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2df2::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2df2::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}