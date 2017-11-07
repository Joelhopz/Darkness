#include "CompressionTest.h"
#include "tools/CompressedFile.h"
#include "tools/Debug.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

CompressionTest::CompressionTest(const std::string& testFile, CompressionTypes type)
{
    ifstream in;
    in.open(testFile, ios::binary);
    std::vector<char> inBuf;
    in.seekg(0, ios::end);
    size_t inSize = static_cast<size_t>(in.tellg());
    inBuf.resize(inSize);
    in.seekg(0, ios::beg);
    in.read(&inBuf[0], static_cast<streamsize>(inSize));
    in.close();

    CompressedFile outCompress;
    outCompress.open(testFile + ".compressed", ios::out | ios::binary, type);
    outCompress.write(&inBuf[0], static_cast<streamsize>(inSize));
    outCompress.close();

    CompressedFile inCompress;
    inCompress.open(testFile + ".compressed", ios::in | ios::binary);
    std::vector<char> resInBuf;
    inCompress.seekg(0, ios::end);
    size_t inCompressSize = inCompress.tellg();
    inCompress.seekg(0, ios::beg);
    resInBuf.resize(inCompressSize);
    inCompress.read(&resInBuf[0], static_cast<streamsize>(inCompressSize));
    inCompress.close();

    ofstream out;
    out.open(testFile + ".result", ios::binary);
    out.write(&resInBuf[0], static_cast<streamsize>(inCompressSize));
    out.close();

    // is the original file the same size as the resulting file
    ASSERT(inSize == inCompressSize);

    // is the content identical
    for (size_t i = 0; i < inSize; ++i)
    {
        ASSERT(inBuf[i] == resInBuf[i]);
    }
}
