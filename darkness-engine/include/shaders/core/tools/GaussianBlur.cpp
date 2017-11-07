#include "GaussianBlur.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool GaussianBlur::hasVertexShader() const
        {
            return true;
        }

        bool GaussianBlur::hasPixelShader() const
        {
            return true;
        }

        bool GaussianBlur::hasGeometryShader() const
        {
            return false;
        }

        bool GaussianBlur::hasHullShader() const
        {
            return false;
        }

        bool GaussianBlur::hasDomainShader() const
        {
            return false;
        }

        bool GaussianBlur::hasComputeShader() const
        {
            return false;
        }

        uint32_t GaussianBlur::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* GaussianBlur::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* GaussianBlur::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* GaussianBlur::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* GaussianBlur::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* GaussianBlur::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* GaussianBlur::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}