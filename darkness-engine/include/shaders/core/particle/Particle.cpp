#include "Particle.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool Particle::hasVertexShader() const
        {
            return true;
        }

        bool Particle::hasPixelShader() const
        {
            return true;
        }

        bool Particle::hasGeometryShader() const
        {
            return false;
        }

        bool Particle::hasHullShader() const
        {
            return false;
        }

        bool Particle::hasDomainShader() const
        {
            return false;
        }

        bool Particle::hasComputeShader() const
        {
            return false;
        }

        uint32_t Particle::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* Particle::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Particle::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Particle::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Particle::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Particle::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Particle::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}