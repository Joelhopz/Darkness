#include "CopyTexture3df.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool CopyTexture3df::hasVertexShader() const
        {
            return false;
        }

        bool CopyTexture3df::hasPixelShader() const
        {
            return false;
        }

        bool CopyTexture3df::hasGeometryShader() const
        {
            return false;
        }

        bool CopyTexture3df::hasHullShader() const
        {
            return false;
        }

        bool CopyTexture3df::hasDomainShader() const
        {
            return false;
        }

        bool CopyTexture3df::hasComputeShader() const
        {
            return true;
        }

        uint32_t CopyTexture3df::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* CopyTexture3df::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture3df::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture3df::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture3df::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture3df::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CopyTexture3df::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}