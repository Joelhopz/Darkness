#include "SSAOBlur.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool SSAOBlur::hasVertexShader() const
        {
            return true;
        }

        bool SSAOBlur::hasPixelShader() const
        {
            return true;
        }

        bool SSAOBlur::hasGeometryShader() const
        {
            return false;
        }

        bool SSAOBlur::hasHullShader() const
        {
            return false;
        }

        bool SSAOBlur::hasDomainShader() const
        {
            return false;
        }

        bool SSAOBlur::hasComputeShader() const
        {
            return false;
        }

        uint32_t SSAOBlur::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* SSAOBlur::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* SSAOBlur::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* SSAOBlur::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* SSAOBlur::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* SSAOBlur::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* SSAOBlur::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}