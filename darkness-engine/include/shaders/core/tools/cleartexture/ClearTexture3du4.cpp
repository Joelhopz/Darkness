#include "ClearTexture3du4.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClearTexture3du4::hasVertexShader() const
        {
            return false;
        }

        bool ClearTexture3du4::hasPixelShader() const
        {
            return false;
        }

        bool ClearTexture3du4::hasGeometryShader() const
        {
            return false;
        }

        bool ClearTexture3du4::hasHullShader() const
        {
            return false;
        }

        bool ClearTexture3du4::hasDomainShader() const
        {
            return false;
        }

        bool ClearTexture3du4::hasComputeShader() const
        {
            return true;
        }

        uint32_t ClearTexture3du4::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* ClearTexture3du4::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3du4::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3du4::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3du4::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3du4::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3du4::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}