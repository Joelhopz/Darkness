#pragma once

#include "tools/CompressedFile.h"
#include <string>

class CompressionTest
{
public:
    CompressionTest(const std::string& testFile, CompressionTypes type = CompressionTypes::Zstd);
};
