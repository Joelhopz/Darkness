#pragma once

#include <string>
#include <vector>

namespace engine
{
    class Directory
    {
        std::string m_path;
    public:
        Directory(const std::string& path);
        bool exists() const;
        void create() const;
        void remove(bool removeFiles = false) const;
        std::vector<std::string> files() const;
    };
}
