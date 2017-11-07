#include "platform/Directory.h"

using namespace std;

#ifndef __APPLE__
#include <experimental/filesystem>

using namespace std::experimental;

namespace engine
{
    Directory::Directory(const string& path)
        : m_path{ path }
    {
    }

    bool Directory::exists() const
    {
        return filesystem::exists(m_path);
    }

    void Directory::create() const
    {
        if (!exists())
            filesystem::create_directories(m_path);
    }

    void Directory::remove(bool removeFiles) const
    {
        if (!removeFiles)
            filesystem::remove(m_path);
        else
            filesystem::remove_all(m_path);
    }

    vector<string> Directory::files() const
    {
        vector<string> result;
        for (const auto& file : filesystem::directory_iterator{ m_path })
        {
            if (filesystem::is_regular_file(file.status()))
            {
                result.emplace_back(file.path().filename().u8string());
            }
        }
        return result;
    }
}
#else
namespace engine
{
    Directory::Directory(const string& path)
    : m_path{ path }
    {
    }
    
    bool Directory::exists() const
    {
        return false;
    }
    
    void Directory::create() const
    {
    }
    
    void Directory::remove(bool removeFiles) const
    {
    }
    
    vector<string> Directory::files() const
    {
        return {};
    }
}
#endif
