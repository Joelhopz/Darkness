#pragma once

#include "CompressedFileIf.h"
#include <memory>
#include <string>

enum class CompressionTypes
{
    Zstd,
    Bzip2
};

class CompressedFile : public CompressedFileIf
{
public:
    CompressedFile();
    CompressedFile(const CompressedFile&) = default;
    CompressedFile(CompressedFile&&) = default;
    CompressedFile& operator=(const CompressedFile&) = default;
    CompressedFile& operator=(CompressedFile&&) = default;
    virtual ~CompressedFile() {};

    void open(const std::string& filename, int mode) override;
    void open(std::vector<char>& memory, int mode) override;
    void open(const std::string& filename, int mode, CompressionTypes type);
    void open(std::vector<char>& memory, int mode, CompressionTypes type);
    bool is_open() const override;

    void read(char* buffer, std::streamsize count) override;
    void write(const char* buffer, std::streamsize count) override;

    void close() override;
    void clear() override;

    void seekg(std::streampos off, std::ios_base::seekdir way) override;
    size_t tellg() override;

    bool eof() const override;
private:
    std::shared_ptr<CompressedFileIf> m_impl;
};
