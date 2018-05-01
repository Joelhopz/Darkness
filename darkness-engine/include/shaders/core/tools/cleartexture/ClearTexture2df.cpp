#include "ClearTexture2df.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClearTexture2df::hasVertexShader() const
        {
            return false;
        }

        bool ClearTexture2df::hasPixelShader() const
        {
            return false;
        }

        bool ClearTexture2df::hasGeometryShader() const
        {
            return false;
        }

        bool ClearTexture2df::hasHullShader() const
        {
            return false;
        }

        bool ClearTexture2df::hasDomainShader() const
        {
            return false;
        }

        bool ClearTexture2df::hasComputeShader() const
        {
            return true;
        }

        uint32_t ClearTexture2df::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* ClearTexture2df::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2df::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2df::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2df::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2df::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture2df::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}