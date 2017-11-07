#pragma once

#include <string>
#include <vector>
#include "engine/graphics/Format.h"
#include "tools/image/ImageIf.h"
#include "engine/primitives/Color.h"
#include "DdsBoilerplate.h"

namespace engine
{
    namespace image
    {

        class Dds : public ImageIf
        {
        public:
            Dds(const std::string& filename);
            Dds(const std::string& filename,
                Format type,
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
            
        private:
            bool readHeader(std::fstream& file);
            void createHeader(unsigned int width, unsigned int height, Format format);
            void loadImage(std::fstream& file);
            size_t slicePitch() const;

            std::vector<uint8_t> m_data;
            std::string m_filename;
            DdsHeader m_header;
            DdsHeaderDXT10 m_extendedHeader;
            Format m_format;

            size_t m_slices;
            size_t m_mipmaps;
            size_t m_fileSize;
        };

    }
}
