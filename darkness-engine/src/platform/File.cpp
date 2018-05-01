#include "platform/File.h"
#include <fstream>
#include <experimental/filesystem>

using namespace std::experimental;

namespace engine
{
    bool fileExists(const std::string& filename)
    {
        std::ifstream file(filename);
        return file.good();
    }

    bool fileCopy(const std::string& src, const std::string& dst)
    {
        std::experimental::filesystem::copy(src, dst);
        return true;
    }

    bool fileDelete(const std::string& file)
    {
        return std::experimental::filesystem::remove(file);
    }
}
