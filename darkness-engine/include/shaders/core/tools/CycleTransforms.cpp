#include "CycleTransforms.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool CycleTransforms::hasVertexShader() const
        {
            return false;
        }

        bool CycleTransforms::hasPixelShader() const
        {
            return false;
        }

        bool CycleTransforms::hasGeometryShader() const
        {
            return false;
        }

        bool CycleTransforms::hasHullShader() const
        {
            return false;
        }

        bool CycleTransforms::hasDomainShader() const
        {
            return false;
        }

        bool CycleTransforms::hasComputeShader() const
        {
            return true;
        }

        uint32_t CycleTransforms::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* CycleTransforms::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CycleTransforms::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CycleTransforms::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CycleTransforms::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CycleTransforms::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CycleTransforms::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}