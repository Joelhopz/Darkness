#include "tools/CompressedFileBzip2.h"
#include "bzlib.h"
#include "tools/Debug.h"

using namespace std;

CompressedFileBzip2::CompressedFileBzip2()
    : m_filename{ "" }
    , m_mode{ 0 }
    , m_eof{ false }
    , m_filePosition{ 0 }
    , m_memoryFile{ false }
{}

void CompressedFileBzip2::open(const std::string& filename, int mode)
{
    m_fileContents = make_unique<std::vector<char>>();
    m_fileContentsActive = m_fileContents.get();
    m_filename = filename;
    m_mode = mode;
    m_file.open(filename, mode);
    if (((mode & ios::in) == ios::in) && m_file.is_open())
    {
        decode();
    }
}

void CompressedFileBzip2::open(std::vector<char>& memory, int mode)
{
    m_fileContentsActive = &memory;
    m_mode = mode;
    m_memoryFile = true;
    if (((mode & ios::in) == ios::in) && memory.size() > 0)
    {
        decode();
    }
}

bool CompressedFileBzip2::is_open() const
{
    if (m_memoryFile)
        return true;
    return m_file.is_open();
}

void CompressedFileBzip2::read(char* buffer, streamsize count)
{
    streamsize bytesToRead = count;
    if (count > static_cast<streamsize>(m_fileContentsActive->size() - m_filePosition))
    {
        m_eof = true;
        bytesToRead = static_cast<streamsize>(m_fileContentsActive->size() - m_filePosition);
    }
    if (bytesToRead > 0)
        memcpy(buffer, &(*m_fileContentsActive)[m_filePosition], static_cast<size_t>(bytesToRead));
    m_filePosition += static_cast<size_t>(bytesToRead);
}

void CompressedFileBzip2::write(const char* buffer, streamsize count)
{
    if (count == 0)
        return;

    if (m_fileContentsActive->size() < m_filePosition + count)
    {
        m_fileContentsActive->resize(m_filePosition + static_cast<size_t>(count));
    }
    memcpy(&(*m_fileContentsActive)[m_filePosition], buffer, static_cast<size_t>(count));
    m_filePosition += static_cast<size_t>(count);
}

void CompressedFileBzip2::seekg(std::streampos off, std::ios_base::seekdir way)
{
    if (way == ios::cur)
        m_filePosition += static_cast<size_t>(off);
    else if (way == ios::beg)
        m_filePosition = static_cast<size_t>(off);
    else if (way == ios::end)
    {
        if (static_cast<size_t>(off) < m_fileContentsActive->size())
            m_filePosition = m_fileContentsActive->size() - static_cast<size_t>(off);
        else
            m_filePosition = 0;
    }
}

size_t CompressedFileBzip2::tellg()
{
    return m_filePosition;
}

void CompressedFileBzip2::clear()
{
    m_eof = false;
}

void CompressedFileBzip2::close()
{
    if ((m_mode & ios::out) == ios::out)
    {
        encode();
    }
    if(!m_memoryFile)
        m_file.close();
}

bool CompressedFileBzip2::eof() const
{
    return m_eof;
}

void CompressedFileBzip2::decode()
{
    //m_fileContentsActive
    size_t fileSize = 0;
    if (!m_memoryFile)
    {
        m_file.seekg(0, ios::end);
        fileSize = static_cast<size_t>(m_file.tellg());
        m_file.seekg(0, ios::beg);
    }
    else
        fileSize = m_fileContentsActive->size();

    m_filePosition = 0;
    vector<char> decodedBuffer(CompressionBufferSize, 0);
    vector<char> encodedBuffer(CompressionBufferSize, 0);

    vector<char> result;

    bz_stream stream{ 0 };
    stream.bzalloc = nullptr;
    stream.bzfree = nullptr;
    stream.opaque = nullptr;
    int re = BZ2_bzDecompressInit(&stream, 0, 0);
    if (re)
    {
        re++;
    }
    stream.avail_in = 0;

    int ret = BZ_RUN_OK;
    do
    {
        const size_t BlockSize = encodedBuffer.size();
        size_t bytesRemaining = static_cast<size_t>(fileSize - m_filePosition);
        size_t bytesToRead = BlockSize > bytesRemaining ? bytesRemaining : BlockSize;
        if (bytesToRead > 0)
        {
            if (!m_memoryFile)
                m_file.read(&encodedBuffer[0], static_cast<streamsize>(bytesToRead));
            else
                memcpy(&encodedBuffer[0], &(*m_fileContentsActive)[m_filePosition], bytesToRead);
        }
        m_filePosition += bytesToRead;

        stream.avail_in = static_cast<unsigned int>(bytesToRead);
        stream.next_in = &encodedBuffer[0];

        do
        {
            stream.avail_out = static_cast<unsigned int>(decodedBuffer.size());
            stream.next_out = &decodedBuffer[0];

            ret = BZ2_bzDecompress(&stream);
            ASSERT(ret == BZ_OK || ret == BZ_STREAM_END);

            if (stream.avail_out < BlockSize)
            {
                unsigned int availableBytes = static_cast<unsigned int>(BlockSize) - stream.avail_out;
                size_t resSize = result.size();
                result.resize(resSize + availableBytes);
                memcpy(&result[resSize], &decodedBuffer[0], availableBytes);
            }
        } while (stream.avail_in > 0);

    } while (ret != BZ_STREAM_END);

    ret = BZ2_bzDecompressEnd(&stream);

    m_fileContentsActive->swap(result);
    m_filePosition = 0;
}

void CompressedFileBzip2::encode()
{
    m_filePosition = 0;
    vector<char> buffer(CompressionBufferSize, 0);

    bz_stream stream{ 0 };
    stream.bzalloc = nullptr;
    stream.bzfree = nullptr;
    stream.opaque = nullptr;
    BZ2_bzCompressInit(&stream, 9, 0, 0);
    stream.avail_in = 0;

    vector<char> tempStorage;

    int ret = BZ_RUN_OK;
    do
    {
        const size_t BlockSize = buffer.size();
        size_t bytesRemaining = static_cast<size_t>(m_fileContentsActive->size() - m_filePosition);
        size_t bytesToWrite = BlockSize > bytesRemaining ? bytesRemaining : BlockSize;

        stream.avail_in = static_cast<unsigned int>(bytesToWrite);
        stream.next_in = bytesToWrite > 0 ? &(*m_fileContentsActive)[m_filePosition] : nullptr;

        do
        {
            stream.avail_out = static_cast<unsigned int>(buffer.size());
            stream.next_out = &buffer[0];

            ret = BZ2_bzCompress(&stream, bytesRemaining > BlockSize ? BZ_RUN : BZ_FINISH);
            ASSERT(ret == BZ_RUN_OK || ret == BZ_STREAM_END || ret == BZ_FINISH_OK);

            if (stream.avail_out < BlockSize)
            {
                unsigned int availableBytes = static_cast<unsigned int>(BlockSize) - stream.avail_out;
                if(!m_memoryFile)
                    m_file.write(&buffer[0], availableBytes);
                else
                {
                    auto currentSize = tempStorage.size();
                    tempStorage.resize(tempStorage.size() + availableBytes);
                    memcpy(&tempStorage[currentSize], &buffer[0], availableBytes);
                }
            }
        } while (stream.avail_in > 0);
        ASSERT(stream.avail_in == 0);

        m_filePosition += bytesToWrite;
    } while (ret != BZ_STREAM_END);

    ret = BZ2_bzCompressEnd(&stream);

    m_fileContentsActive->swap(tempStorage);
}
