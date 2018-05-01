#include "DispatchBasic.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool DispatchBasic::hasVertexShader() const
        {
            return false;
        }

        bool DispatchBasic::hasPixelShader() const
        {
            return false;
        }

        bool DispatchBasic::hasGeometryShader() const
        {
            return false;
        }

        bool DispatchBasic::hasHullShader() const
        {
            return false;
        }

        bool DispatchBasic::hasDomainShader() const
        {
            return false;
        }

        bool DispatchBasic::hasComputeShader() const
        {
            return true;
        }

        uint32_t DispatchBasic::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* DispatchBasic::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DispatchBasic::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DispatchBasic::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DispatchBasic::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DispatchBasic::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* DispatchBasic::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}