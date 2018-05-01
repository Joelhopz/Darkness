#pragma once

#include <string>

namespace engine
{
    bool fileExists(const std::string& filename);
    bool fileCopy(const std::string& src, const std::string& dst);
    bool fileDelete(const std::string& file);
}
