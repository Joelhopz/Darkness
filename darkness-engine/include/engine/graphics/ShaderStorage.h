#pragma once

#include "ShaderBinary.h"
#include "platform/FileWatcher.h"
#include <memory>
#include <map>
#include <string>

namespace engine
{
    class Device;
    class ShaderStorage
    {
    public:
        std::shared_ptr<const ShaderBinary> loadShader(
            const Device& device, 
            const std::string& path,
            const std::string& supportPath);
        void clear();

        platform::FileWatcher& fileWatcher();
    private:
        std::map<const std::string, std::shared_ptr<const ShaderBinary>> m_loaded;
        platform::FileWatcher m_fileWatcher;
    };
}
