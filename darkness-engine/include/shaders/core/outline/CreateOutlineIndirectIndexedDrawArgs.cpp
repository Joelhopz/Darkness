#include "CreateOutlineIndirectIndexedDrawArgs.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool CreateOutlineIndirectIndexedDrawArgs::hasVertexShader() const
        {
            return false;
        }

        bool CreateOutlineIndirectIndexedDrawArgs::hasPixelShader() const
        {
            return false;
        }

        bool CreateOutlineIndirectIndexedDrawArgs::hasGeometryShader() const
        {
            return false;
        }

        bool CreateOutlineIndirectIndexedDrawArgs::hasHullShader() const
        {
            return false;
        }

        bool CreateOutlineIndirectIndexedDrawArgs::hasDomainShader() const
        {
            return false;
        }

        bool CreateOutlineIndirectIndexedDrawArgs::hasComputeShader() const
        {
            return true;
        }

        uint32_t CreateOutlineIndirectIndexedDrawArgs::descriptorCount() const
        {
            return 
                
                
                
                
                
                cs.descriptorCount()+
                0;
        }

        const Shader* CreateOutlineIndirectIndexedDrawArgs::vertexShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CreateOutlineIndirectIndexedDrawArgs::pixelShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CreateOutlineIndirectIndexedDrawArgs::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CreateOutlineIndirectIndexedDrawArgs::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CreateOutlineIndirectIndexedDrawArgs::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* CreateOutlineIndirectIndexedDrawArgs::computeShader() const
        {
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            
        }
    }
}