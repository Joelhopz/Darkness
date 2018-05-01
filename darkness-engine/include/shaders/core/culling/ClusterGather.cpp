#include "ClusterGather.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClusterGather::hasVertexShader() const
        {
            return false;
        }

        bool ClusterGather::hasPixelShader() const
        {
            return false;
        }

        bool ClusterGather::hasGeometryShader() const
        {
            return false;
        }

        bool ClusterGather::hasHullShader() const
        {
            return false;
        }

        bool ClusterGather::hasDomainShader() const
        {
            return false;
        }

        bool ClusterGather::hasComputeShader() const
        {
            return true;
        }

        uint32_t ClusterGather::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* ClusterGather::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClusterGather::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClusterGather::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClusterGather::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClusterGather::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClusterGather::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}