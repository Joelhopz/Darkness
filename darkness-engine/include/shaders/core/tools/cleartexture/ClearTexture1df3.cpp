#include "ClearTexture1df3.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClearTexture1df3::hasVertexShader() const
        {
            return false;
        }

        bool ClearTexture1df3::hasPixelShader() const
        {
            return false;
        }

        bool ClearTexture1df3::hasGeometryShader() const
        {
            return false;
        }

        bool ClearTexture1df3::hasHullShader() const
        {
            return false;
        }

        bool ClearTexture1df3::hasDomainShader() const
        {
            return false;
        }

        bool ClearTexture1df3::hasComputeShader() const
        {
            return true;
        }

        uint32_t ClearTexture1df3::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* ClearTexture1df3::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1df3::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1df3::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1df3::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1df3::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1df3::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}