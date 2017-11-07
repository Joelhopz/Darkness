#include "TemporalResolve.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool TemporalResolve::hasVertexShader() const
        {
            return true;
        }

        bool TemporalResolve::hasPixelShader() const
        {
            return true;
        }

        bool TemporalResolve::hasGeometryShader() const
        {
            return false;
        }

        bool TemporalResolve::hasHullShader() const
        {
            return false;
        }

        bool TemporalResolve::hasDomainShader() const
        {
            return false;
        }

        bool TemporalResolve::hasComputeShader() const
        {
            return false;
        }

        uint32_t TemporalResolve::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* TemporalResolve::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* TemporalResolve::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* TemporalResolve::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* TemporalResolve::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* TemporalResolve::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* TemporalResolve::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}