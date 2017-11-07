#include "EarlyZAlphaClipped.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool EarlyZAlphaClipped::hasVertexShader() const
        {
            return true;
        }

        bool EarlyZAlphaClipped::hasPixelShader() const
        {
            return true;
        }

        bool EarlyZAlphaClipped::hasGeometryShader() const
        {
            return false;
        }

        bool EarlyZAlphaClipped::hasHullShader() const
        {
            return false;
        }

        bool EarlyZAlphaClipped::hasDomainShader() const
        {
            return false;
        }

        bool EarlyZAlphaClipped::hasComputeShader() const
        {
            return false;
        }

        uint32_t EarlyZAlphaClipped::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* EarlyZAlphaClipped::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* EarlyZAlphaClipped::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* EarlyZAlphaClipped::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* EarlyZAlphaClipped::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* EarlyZAlphaClipped::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* EarlyZAlphaClipped::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}