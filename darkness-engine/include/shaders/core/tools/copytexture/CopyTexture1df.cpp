#include "CopyTexture1df.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool CopyTexture1df::hasVertexShader() const
        {
            return false;
        }

        bool CopyTexture1df::hasPixelShader() const
        {
            return false;
        }

        bool CopyTexture1df::hasGeometryShader() const
        {
            return false;
        }

        bool CopyTexture1df::hasHullShader() const
        {
            return false;
        }

        bool CopyTexture1df::hasDomainShader() const
        {
            return false;
        }

        bool CopyTexture1df::hasComputeShader() const
        {
            return true;
        }

        uint32_t CopyTexture1df::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* CopyTexture1df::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture1df::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture1df::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture1df::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture1df::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture1df::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}