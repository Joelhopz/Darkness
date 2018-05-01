#include "OcclusionCulling.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool OcclusionCulling::hasVertexShader() const
        {
            return false;
        }

        bool OcclusionCulling::hasPixelShader() const
        {
            return false;
        }

        bool OcclusionCulling::hasGeometryShader() const
        {
            return false;
        }

        bool OcclusionCulling::hasHullShader() const
        {
            return false;
        }

        bool OcclusionCulling::hasDomainShader() const
        {
            return false;
        }

        bool OcclusionCulling::hasComputeShader() const
        {
            return true;
        }

        uint32_t OcclusionCulling::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* OcclusionCulling::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* OcclusionCulling::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* OcclusionCulling::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* OcclusionCulling::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* OcclusionCulling::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* OcclusionCulling::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}