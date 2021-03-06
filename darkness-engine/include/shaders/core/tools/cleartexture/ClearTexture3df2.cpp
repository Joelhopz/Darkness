#include "ClearTexture3df2.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClearTexture3df2::hasVertexShader() const
        {
            return false;
        }

        bool ClearTexture3df2::hasPixelShader() const
        {
            return false;
        }

        bool ClearTexture3df2::hasGeometryShader() const
        {
            return false;
        }

        bool ClearTexture3df2::hasHullShader() const
        {
            return false;
        }

        bool ClearTexture3df2::hasDomainShader() const
        {
            return false;
        }

        bool ClearTexture3df2::hasComputeShader() const
        {
            return true;
        }

        uint32_t ClearTexture3df2::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* ClearTexture3df2::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3df2::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3df2::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3df2::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3df2::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3df2::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}