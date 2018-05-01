#include "CopyTexture.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool CopyTexture::hasVertexShader() const
        {
            return true;
        }

        bool CopyTexture::hasPixelShader() const
        {
            return true;
        }

        bool CopyTexture::hasGeometryShader() const
        {
            return false;
        }

        bool CopyTexture::hasHullShader() const
        {
            return false;
        }

        bool CopyTexture::hasDomainShader() const
        {
            return false;
        }

        bool CopyTexture::hasComputeShader() const
        {
            return false;
        }

        uint32_t CopyTexture::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* CopyTexture::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* CopyTexture::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* CopyTexture::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}