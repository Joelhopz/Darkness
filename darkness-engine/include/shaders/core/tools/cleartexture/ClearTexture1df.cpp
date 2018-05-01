#include "ClearTexture1df.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClearTexture1df::hasVertexShader() const
        {
            return false;
        }

        bool ClearTexture1df::hasPixelShader() const
        {
            return false;
        }

        bool ClearTexture1df::hasGeometryShader() const
        {
            return false;
        }

        bool ClearTexture1df::hasHullShader() const
        {
            return false;
        }

        bool ClearTexture1df::hasDomainShader() const
        {
            return false;
        }

        bool ClearTexture1df::hasComputeShader() const
        {
            return true;
        }

        uint32_t ClearTexture1df::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* ClearTexture1df::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1df::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1df::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1df::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1df::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture1df::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}