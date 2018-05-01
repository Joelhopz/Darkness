#include "RtvClearTest.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool RtvClearTest::hasVertexShader() const
        {
            return true;
        }

        bool RtvClearTest::hasPixelShader() const
        {
            return true;
        }

        bool RtvClearTest::hasGeometryShader() const
        {
            return false;
        }

        bool RtvClearTest::hasHullShader() const
        {
            return false;
        }

        bool RtvClearTest::hasDomainShader() const
        {
            return false;
        }

        bool RtvClearTest::hasComputeShader() const
        {
            return false;
        }

        uint32_t RtvClearTest::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* RtvClearTest::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* RtvClearTest::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* RtvClearTest::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* RtvClearTest::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* RtvClearTest::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* RtvClearTest::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}