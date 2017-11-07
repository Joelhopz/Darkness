#pragma once

#include <memory>
#include <string>

struct METAL_SHADER_BYTECODE {};

namespace engine
{
    class Device;
    namespace implementation
    {
        class ShaderBinaryImpl
        {
        public:
            ShaderBinaryImpl(
                             const Device& device,
                             const std::string& binaryPath);
            
            METAL_SHADER_BYTECODE& native() const;
        private:
            std::unique_ptr<char> m_buffer;
            std::shared_ptr<METAL_SHADER_BYTECODE> m_shaderBinary;
            
            void readFile(const std::string& path);
            void clear();
        };
    }
}

