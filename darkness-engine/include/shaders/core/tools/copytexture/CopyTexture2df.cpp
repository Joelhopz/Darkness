#include "CopyTexture2df.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool CopyTexture2df::hasVertexShader() const
        {
            return false;
        }

        bool CopyTexture2df::hasPixelShader() const
        {
            return false;
        }

        bool CopyTexture2df::hasGeometryShader() const
        {
            return false;
        }

        bool CopyTexture2df::hasHullShader() const
        {
            return false;
        }

        bool CopyTexture2df::hasDomainShader() const
        {
            return false;
        }

        bool CopyTexture2df::hasComputeShader() const
        {
            return true;
        }

        uint32_t CopyTexture2df::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* CopyTexture2df::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture2df::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture2df::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture2df::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture2df::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture2df::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}