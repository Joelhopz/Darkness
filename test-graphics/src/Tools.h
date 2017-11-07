#pragma once

#include <string>
#include <fstream>
#include <vector>

template<typename T>
std::vector<T> generateTestData(size_t size = 32768)
{
    std::vector<T> data;
    for (int i = 0; i < size; ++i)
    {
        data.emplace_back(i);
    }
    return data;
}

bool fileExists(std::string filename);
void removeFile(std::string filename);
