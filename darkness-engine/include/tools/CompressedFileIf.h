#pragma once

#include <string>
#include <fstream>

static const int CompressionBufferSize = 16384;

class CompressedFileIf
{
public:
    virtual ~CompressedFileIf() {};

    virtual void open(const std::string& filename, int mode) = 0;
    virtual bool is_open() const = 0;

    virtual void read(char* buffer, std::streamsize count) = 0;
    virtual void write(const char* buffer, std::streamsize count) = 0;

    virtual void close() = 0;
    virtual void clear() = 0;

    virtual void seekg(std::streampos off, std::ios_base::seekdir way) = 0;
    virtual size_t tellg() = 0;

    virtual bool eof() const = 0;
};
