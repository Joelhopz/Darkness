#include "Tools.h"

bool fileExists(std::string filename)
{
    std::ifstream file(filename);
    return file.good();
}

void removeFile(std::string filename)
{
    remove(filename.data());
}
