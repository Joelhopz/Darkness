#include "Texture2DArrayVisualizer.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool Texture2DArrayVisualizer::hasVertexShader() const
        {
            return true;
        }

        bool Texture2DArrayVisualizer::hasPixelShader() const
        {
            return true;
        }

        bool Texture2DArrayVisualizer::hasGeometryShader() const
        {
            return false;
        }

        bool Texture2DArrayVisualizer::hasHullShader() const
        {
            return false;
        }

        bool Texture2DArrayVisualizer::hasDomainShader() const
        {
            return false;
        }

        bool Texture2DArrayVisualizer::hasComputeShader() const
        {
            return false;
        }

        uint32_t Texture2DArrayVisualizer::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* Texture2DArrayVisualizer::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Texture2DArrayVisualizer::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* Texture2DArrayVisualizer::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Texture2DArrayVisualizer::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Texture2DArrayVisualizer::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* Texture2DArrayVisualizer::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}