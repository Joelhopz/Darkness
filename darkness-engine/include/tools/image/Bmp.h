#pragma once

#include <string>
#include <vector>
#include "tools/image/ImageIf.h"
#include "engine/graphics/Format.h"
#include "BmpBoilerplate.h"

namespace engine
{
    namespace image
    {
        class Bmp : public ImageIf
        {
        public:
            Bmp(const std::string& filename);
            Bmp(const std::string& filename,
                Format format,
                unsigned int width,
                unsigned int height,
                unsigned int slices,
                unsigned int mips);

            size_t width() const override;
            size_t height() const override;
            Format format() const override;
            size_t mipCount() const override;
            size_t arraySlices() const override;

            ImageSubresource map(
                uint32_t mipLevel,
                uint32_t arraySlice) const override;

            void reserve() override;
            void save(const char* data, size_t bytes) override;
            void save() override;
            const uint8_t* data() const override;
            size_t bytes() const override;

            void flipVertical() override;
            void convert() override;
        private:
            std::string mFilename;
            BitmapFileHeader mFileHeader;
            BimapV3Header mDibHeader;
            Format m_format;
            std::vector<uint8_t> m_data;

            void readHeader(std::fstream& file);
            void createBmpHeader(
                Format format,
                unsigned int width,
                unsigned int height);

            void readPixels(std::fstream& file);
            void writePixels(std::fstream& file);
            inline void convert(const uint8_t* src, uint8_t* dst, uint32_t bytes) const;
            inline void convertFrom(const uint8_t* src, uint8_t* dst, uint32_t bytes) const;
        };
    }
}
