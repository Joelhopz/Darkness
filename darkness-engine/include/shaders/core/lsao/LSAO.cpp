#include "LSAO.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool LSAO::hasVertexShader() const
        {
            return false;
        }

        bool LSAO::hasPixelShader() const
        {
            return false;
        }

        bool LSAO::hasGeometryShader() const
        {
            return false;
        }

        bool LSAO::hasHullShader() const
        {
            return false;
        }

        bool LSAO::hasDomainShader() const
        {
            return false;
        }

        bool LSAO::hasComputeShader() const
        {
            return true;
        }

        uint32_t LSAO::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* LSAO::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* LSAO::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* LSAO::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* LSAO::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* LSAO::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* LSAO::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}