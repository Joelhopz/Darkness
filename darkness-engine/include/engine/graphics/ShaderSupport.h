#pragma once

#include <string>

namespace engine
{
    namespace implementation
    {
        class ShaderSupport
        {
        public:
            ShaderSupport() = default;
            ShaderSupport(const std::string& supportFilePath);

            std::string binaryFile;
            std::string executable;
            std::string file;
            std::string graphicsApi;
            std::string shaderCompilerPath;
            std::string rootPath;
        };
    }
}
