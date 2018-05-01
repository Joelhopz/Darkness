#pragma once

#include "CompressedFileIf.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

class CompressedFileZstd : public CompressedFileIf
{
public:
    CompressedFileZstd();

    void open(const std::string& filename, int mode) override;
    void open(std::vector<char>& memory, int mode) override;
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
    bool m_memoryFile;

    std::unique_ptr<std::vector<char>> m_fileContents;
    std::vector<char>* m_fileContentsActive;
    bool m_eof;
    size_t m_filePosition;

    void encode();
    void decode();
};
