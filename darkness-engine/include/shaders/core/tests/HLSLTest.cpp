#include "HLSLTest.h"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool HLSLTest::hasVertexShader() const
        {
            return true;
        }

        bool HLSLTest::hasPixelShader() const
        {
            return true;
        }

        bool HLSLTest::hasGeometryShader() const
        {
            return false;
        }

        bool HLSLTest::hasHullShader() const
        {
            return false;
        }

        bool HLSLTest::hasDomainShader() const
        {
            return false;
        }

        bool HLSLTest::hasComputeShader() const
        {
            return false;
        }

        uint32_t HLSLTest::descriptorCount() const
        {
            return 
                vs.descriptorCount()+
                ps.descriptorCount()+
                
                
                
                
                0;
        }

        const Shader* HLSLTest::vertexShader() const
        {
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* HLSLTest::pixelShader() const
        {
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            
        }

        const Shader* HLSLTest::geometryShader() const
        {
            
            return nullptr;
            
        }

        const Shader* HLSLTest::hullShader() const
        {
            
            return nullptr;
            
        }

        const Shader* HLSLTest::domainShader() const
        {
            
            return nullptr;
            
        }

        const Shader* HLSLTest::computeShader() const
        {
            
            return nullptr;
            
        }
    }
}