#include "ClearTexture3df.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClearTexture3df::hasVertexShader() const
        {
            return false;
        }

        bool ClearTexture3df::hasPixelShader() const
        {
            return false;
        }

        bool ClearTexture3df::hasGeometryShader() const
        {
            return false;
        }

        bool ClearTexture3df::hasHullShader() const
        {
            return false;
        }

        bool ClearTexture3df::hasDomainShader() const
        {
            return false;
        }

        bool ClearTexture3df::hasComputeShader() const
        {
            return true;
        }

        uint32_t ClearTexture3df::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* ClearTexture3df::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3df::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3df::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3df::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3df::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClearTexture3df::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}