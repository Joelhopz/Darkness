#include "DebugBoundingSpheres.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool DebugBoundingSpheres::hasVertexShader() const
        {
            return true;
        }

        bool DebugBoundingSpheres::hasPixelShader() const
        {
            return true;
        }

        bool DebugBoundingSpheres::hasGeometryShader() const
        {
            return false;
        }

        bool DebugBoundingSpheres::hasHullShader() const
        {
            return false;
        }

        bool DebugBoundingSpheres::hasDomainShader() const
        {
            return false;
        }

        bool DebugBoundingSpheres::hasComputeShader() const
        {
            return false;
        }

        uint32_t DebugBoundingSpheres::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* DebugBoundingSpheres::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* DebugBoundingSpheres::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* DebugBoundingSpheres::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DebugBoundingSpheres::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DebugBoundingSpheres::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DebugBoundingSpheres::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}