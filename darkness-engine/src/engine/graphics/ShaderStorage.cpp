#include "engine/graphics/ShaderStorage.h"
#include "tools/Debug.h"

namespace engine
{
    std::shared_ptr<const ShaderBinary> ShaderStorage::loadShader(
        const Device& device, 
        const std::string& path,
        const std::string& supportPath)
    {
        auto loaded = m_loaded.find(path);
        if (loaded != m_loaded.end())
            return m_loaded[path];

        m_loaded[path] = std::make_shared<const ShaderBinary>(device, path, supportPath, m_fileWatcher);

        return m_loaded[path];
    }

    void ShaderStorage::clear()
    {
        m_loaded.clear();
    }

    platform::FileWatcher& ShaderStorage::fileWatcher()
    {
        return m_fileWatcher;
    }
}
