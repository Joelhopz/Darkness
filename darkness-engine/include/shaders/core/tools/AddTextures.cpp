#include "AddTextures.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool AddTextures::hasVertexShader() const
        {
            return true;
        }

        bool AddTextures::hasPixelShader() const
        {
            return true;
        }

        bool AddTextures::hasGeometryShader() const
        {
            return false;
        }

        bool AddTextures::hasHullShader() const
        {
            return false;
        }

        bool AddTextures::hasDomainShader() const
        {
            return false;
        }

        bool AddTextures::hasComputeShader() const
        {
            return false;
        }

        uint32_t AddTextures::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* AddTextures::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* AddTextures::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* AddTextures::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* AddTextures::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* AddTextures::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* AddTextures::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}