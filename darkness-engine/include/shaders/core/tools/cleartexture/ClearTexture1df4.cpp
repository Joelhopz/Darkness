#include "ClearTexture1df4.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClearTexture1df4::hasVertexShader() const
        {
            return false;
        }

        bool ClearTexture1df4::hasPixelShader() const
        {
            return false;
        }

        bool ClearTexture1df4::hasGeometryShader() const
        {
            return false;
        }

        bool ClearTexture1df4::hasHullShader() const
        {
            return false;
        }

        bool ClearTexture1df4::hasDomainShader() const
        {
            return false;
        }

        bool ClearTexture1df4::hasComputeShader() const
        {
            return true;
        }

        uint32_t ClearTexture1df4::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* ClearTexture1df4::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1df4::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1df4::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1df4::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1df4::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1df4::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}