#include "platform/File.h"
#include <fstream>

namespace engine
{
    bool fileExists(const std::string& filename)
    {
        std::ifstream file(filename);
        return file.good();
    }
}
