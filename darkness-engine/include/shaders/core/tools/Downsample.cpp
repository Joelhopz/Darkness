#include "Downsample.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool Downsample::hasVertexShader() const
        {
            return true;
        }

        bool Downsample::hasPixelShader() const
        {
            return true;
        }

        bool Downsample::hasGeometryShader() const
        {
            return false;
        }

        bool Downsample::hasHullShader() const
        {
            return false;
        }

        bool Downsample::hasDomainShader() const
        {
            return false;
        }

        bool Downsample::hasComputeShader() const
        {
            return false;
        }

        uint32_t Downsample::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* Downsample::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Downsample::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Downsample::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Downsample::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Downsample::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Downsample::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}