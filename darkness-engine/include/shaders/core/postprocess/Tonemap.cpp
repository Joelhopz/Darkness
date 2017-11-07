#include "Tonemap.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool Tonemap::hasVertexShader() const
        {
            return true;
        }

        bool Tonemap::hasPixelShader() const
        {
            return true;
        }

        bool Tonemap::hasGeometryShader() const
        {
            return false;
        }

        bool Tonemap::hasHullShader() const
        {
            return false;
        }

        bool Tonemap::hasDomainShader() const
        {
            return false;
        }

        bool Tonemap::hasComputeShader() const
        {
            return false;
        }

        uint32_t Tonemap::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* Tonemap::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Tonemap::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Tonemap::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Tonemap::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Tonemap::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Tonemap::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}