#include "DebugViewer.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool DebugViewer::hasVertexShader() const
        {
            return true;
        }

        bool DebugViewer::hasPixelShader() const
        {
            return true;
        }

        bool DebugViewer::hasGeometryShader() const
        {
            return false;
        }

        bool DebugViewer::hasHullShader() const
        {
            return false;
        }

        bool DebugViewer::hasDomainShader() const
        {
            return false;
        }

        bool DebugViewer::hasComputeShader() const
        {
            return false;
        }

        uint32_t DebugViewer::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* DebugViewer::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* DebugViewer::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* DebugViewer::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DebugViewer::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DebugViewer::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DebugViewer::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}