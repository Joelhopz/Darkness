#include "CreateDrawArguments.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool CreateDrawArguments::hasVertexShader() const
        {
            return false;
        }

        bool CreateDrawArguments::hasPixelShader() const
        {
            return false;
        }

        bool CreateDrawArguments::hasGeometryShader() const
        {
            return false;
        }

        bool CreateDrawArguments::hasHullShader() const
        {
            return false;
        }

        bool CreateDrawArguments::hasDomainShader() const
        {
            return false;
        }

        bool CreateDrawArguments::hasComputeShader() const
        {
            return true;
        }

        uint32_t CreateDrawArguments::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* CreateDrawArguments::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CreateDrawArguments::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CreateDrawArguments::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CreateDrawArguments::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CreateDrawArguments::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CreateDrawArguments::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}