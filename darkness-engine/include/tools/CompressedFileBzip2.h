#pragma once

#include "CompressedFileIf.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class CompressedFileBzip2 : public CompressedFileIf
{
public:
    CompressedFileBzip2();
    void open(const std::string& filename, int mode) override;
    bool is_open() const override;

    void read(char* buffer, std::streamsize count) override;
    void write(const char* buffer, std::streamsize count) override;
    
    void close() override;
    void clear() override;

    void seekg(std::streampos off, std::ios_base::seekdir way) override;
    size_t tellg() override;

    bool eof() const override;
private:
    std::fstream m_file;
    std::string m_filename;
    int m_mode;

    std::vector<char> m_fileContents;
    bool m_eof;
    size_t m_filePosition;

    void encode();
    void decode();
};
