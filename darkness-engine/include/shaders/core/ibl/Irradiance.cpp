#include "Irradiance.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool Irradiance::hasVertexShader() const
        {
            return true;
        }

        bool Irradiance::hasPixelShader() const
        {
            return true;
        }

        bool Irradiance::hasGeometryShader() const
        {
            return false;
        }

        bool Irradiance::hasHullShader() const
        {
            return false;
        }

        bool Irradiance::hasDomainShader() const
        {
            return false;
        }

        bool Irradiance::hasComputeShader() const
        {
            return false;
        }

        uint32_t Irradiance::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* Irradiance::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Irradiance::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Irradiance::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Irradiance::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Irradiance::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Irradiance::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}