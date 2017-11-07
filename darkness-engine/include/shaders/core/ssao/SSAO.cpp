#include "SSAO.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool SSAO::hasVertexShader() const
        {
            return true;
        }

        bool SSAO::hasPixelShader() const
        {
            return true;
        }

        bool SSAO::hasGeometryShader() const
        {
            return false;
        }

        bool SSAO::hasHullShader() const
        {
            return false;
        }

        bool SSAO::hasDomainShader() const
        {
            return false;
        }

        bool SSAO::hasComputeShader() const
        {
            return false;
        }

        uint32_t SSAO::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* SSAO::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* SSAO::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* SSAO::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* SSAO::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* SSAO::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* SSAO::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}