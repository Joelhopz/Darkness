#include "tools/CompressedFile.h"
#include "tools/CompressedFileBzip2.h"
#include "tools/CompressedFileZstd.h"
#include <vector>
#include <fstream>

using namespace std;

static const unsigned char zstdSignature[] = { 0x27, 0xb5, 0x2f, 0xfd };
static const unsigned char bzip2Signature[] = { 0x42, 0x5a, 0x68 };

bool fileIs_zstd(vector<unsigned char>& magicBuffer)
{
    if (magicBuffer.size() < 4)
        return false;
    return 
        magicBuffer[0] == zstdSignature[0] &&
        magicBuffer[1] == zstdSignature[1] &&
        magicBuffer[2] == zstdSignature[2] && 
        magicBuffer[3] == zstdSignature[3];
}

size_t getMagicDataMaxSize()
{
    return 4;
}

vector<unsigned char> getMagicData(const string& filename)
{
    vector<unsigned char> data;

    ifstream file;
    file.open(filename, ios::binary);
    if (file.is_open())
    {
        data.resize(getMagicDataMaxSize());

        file.seekg(0, ios::end);
        size_t size = static_cast<size_t>(file.tellg());
        if (size < getMagicDataMaxSize())
        {
            file.close();
            return data;
        }
        file.seekg(0, ios::beg);
        file.read(reinterpret_cast<char*>(data.data()), static_cast<streamsize>(getMagicDataMaxSize()));
        file.close();
    }
    return data;
}

CompressedFileIf* createDefault(CompressionTypes defaultType)
{
    switch (defaultType)
    {
        case CompressionTypes::Zstd: return new CompressedFileZstd();
        case CompressionTypes::Bzip2: return new CompressedFileBzip2();
    }
    return new CompressedFileZstd();
}

CompressedFileIf* fileInstance(const string& filename, CompressionTypes defaultType)
{
    vector<unsigned char> magic = getMagicData(filename);
    if (magic.size() == 0)
        return createDefault(defaultType);


    if (magic.size() >= 4 &&
        magic[0] == zstdSignature[0] &&
        magic[1] == zstdSignature[1] &&
        magic[2] == zstdSignature[2] &&
        magic[3] == zstdSignature[3])
    {
        return new CompressedFileZstd();
    }
    else if (
        magic.size() >= 3 &&
        magic[0] == bzip2Signature[0] &&
        magic[1] == bzip2Signature[1] &&
        magic[2] == bzip2Signature[2])
    {
        return new CompressedFileBzip2();
    }

    return createDefault(defaultType);
}

CompressedFileIf* fileInstance(std::vector<char>& memory, CompressionTypes defaultType)
{
    if (memory.size() < 4)
        return createDefault(defaultType);

    if (memory.size() >= 4 &&
        static_cast<unsigned char>(memory[0]) == zstdSignature[0] &&
        static_cast<unsigned char>(memory[1]) == zstdSignature[1] &&
        static_cast<unsigned char>(memory[2]) == zstdSignature[2] &&
        static_cast<unsigned char>(memory[3]) == zstdSignature[3])
    {
        return new CompressedFileZstd();
    }
    else if (
        memory.size() >= 3 &&
        static_cast<unsigned char>(memory[0]) == bzip2Signature[0] &&
        static_cast<unsigned char>(memory[1]) == bzip2Signature[1] &&
        static_cast<unsigned char>(memory[2]) == bzip2Signature[2])
    {
        return new CompressedFileBzip2();
    }

    return createDefault(defaultType);
}

CompressedFile::CompressedFile()
    : m_impl{ nullptr }
{}

void CompressedFile::open(const string& filename, int mode)
{
    open(filename, mode, CompressionTypes::Zstd);
}

void CompressedFile::open(std::vector<char>& memory, int mode)
{
    open(memory, mode, CompressionTypes::Zstd);
}

void CompressedFile::open(const string& filename, int mode, CompressionTypes type)
{
    if (!m_impl)
        m_impl.reset(fileInstance(filename, type));
    m_impl->open(filename, mode);
}

void CompressedFile::open(std::vector<char>& memory, int mode, CompressionTypes type)
{
    if (!m_impl)
        m_impl.reset(fileInstance(memory, type));
    m_impl->open(memory, mode);
}

bool CompressedFile::is_open() const
{
    return m_impl->is_open();
}

void CompressedFile::read(char* buffer, streamsize count)
{
    m_impl->read(buffer, count);
}

void CompressedFile::write(const char* buffer, streamsize count)
{
    m_impl->write(buffer, count);
}

void CompressedFile::close()
{
    m_impl->close();
}

void CompressedFile::clear()
{
    m_impl->clear();
}

void CompressedFile::seekg(streampos off, ios_base::seekdir way)
{
    m_impl->seekg(off, way);
}

size_t CompressedFile::tellg()
{
    return m_impl->tellg();
}

bool CompressedFile::eof() const
{
    return m_impl->eof();
}
