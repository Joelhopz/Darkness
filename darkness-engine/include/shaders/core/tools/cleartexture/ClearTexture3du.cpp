#include "ClearTexture3du.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClearTexture3du::hasVertexShader() const
        {
            return false;
        }

        bool ClearTexture3du::hasPixelShader() const
        {
            return false;
        }

        bool ClearTexture3du::hasGeometryShader() const
        {
            return false;
        }

        bool ClearTexture3du::hasHullShader() const
        {
            return false;
        }

        bool ClearTexture3du::hasDomainShader() const
        {
            return false;
        }

        bool ClearTexture3du::hasComputeShader() const
        {
            return true;
        }

        uint32_t ClearTexture3du::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* ClearTexture3du::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3du::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3du::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3du::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3du::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3du::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}