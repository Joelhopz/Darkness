#include "ClusterRender.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ClusterRender::hasVertexShader() const
        {
            return true;
        }

        bool ClusterRender::hasPixelShader() const
        {
            return true;
        }

        bool ClusterRender::hasGeometryShader() const
        {
            return false;
        }

        bool ClusterRender::hasHullShader() const
        {
            return false;
        }

        bool ClusterRender::hasDomainShader() const
        {
            return false;
        }

        bool ClusterRender::hasComputeShader() const
        {
            return false;
        }

        uint32_t ClusterRender::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* ClusterRender::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* ClusterRender::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* ClusterRender::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClusterRender::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClusterRender::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ClusterRender::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}