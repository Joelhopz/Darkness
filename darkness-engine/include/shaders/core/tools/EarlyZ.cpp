#include "EarlyZ.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool EarlyZ::hasVertexShader() const
        {
            return true;
        }

        bool EarlyZ::hasPixelShader() const
        {
            return true;
        }

        bool EarlyZ::hasGeometryShader() const
        {
            return false;
        }

        bool EarlyZ::hasHullShader() const
        {
            return false;
        }

        bool EarlyZ::hasDomainShader() const
        {
            return false;
        }

        bool EarlyZ::hasComputeShader() const
        {
            return false;
        }

        uint32_t EarlyZ::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* EarlyZ::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* EarlyZ::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* EarlyZ::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* EarlyZ::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* EarlyZ::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* EarlyZ::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}