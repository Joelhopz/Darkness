#include "CreateOcclusionCullingArgs.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool CreateOcclusionCullingArgs::hasVertexShader() const
        {
            return false;
        }

        bool CreateOcclusionCullingArgs::hasPixelShader() const
        {
            return false;
        }

        bool CreateOcclusionCullingArgs::hasGeometryShader() const
        {
            return false;
        }

        bool CreateOcclusionCullingArgs::hasHullShader() const
        {
            return false;
        }

        bool CreateOcclusionCullingArgs::hasDomainShader() const
        {
            return false;
        }

        bool CreateOcclusionCullingArgs::hasComputeShader() const
        {
            return true;
        }

        uint32_t CreateOcclusionCullingArgs::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* CreateOcclusionCullingArgs::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CreateOcclusionCullingArgs::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CreateOcclusionCullingArgs::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CreateOcclusionCullingArgs::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CreateOcclusionCullingArgs::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CreateOcclusionCullingArgs::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}