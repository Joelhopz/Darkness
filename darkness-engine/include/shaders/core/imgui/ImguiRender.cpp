#include "ImguiRender.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool ImguiRender::hasVertexShader() const
        {
            return true;
        }

        bool ImguiRender::hasPixelShader() const
        {
            return true;
        }

        bool ImguiRender::hasGeometryShader() const
        {
            return false;
        }

        bool ImguiRender::hasHullShader() const
        {
            return false;
        }

        bool ImguiRender::hasDomainShader() const
        {
            return false;
        }

        bool ImguiRender::hasComputeShader() const
        {
            return false;
        }

        uint32_t ImguiRender::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* ImguiRender::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* ImguiRender::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* ImguiRender::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ImguiRender::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ImguiRender::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* ImguiRender::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}