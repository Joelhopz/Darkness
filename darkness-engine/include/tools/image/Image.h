#pragma once

#include <string>
#include <vector>
#include <memory>
#include "engine/primitives/Color.h"
#include "ImageIf.h"

// Factory class to create any supported image

namespace engine
{
    namespace image
    {
        enum class ImageType
        {
            DDS,
            BMP
        };

        class Image
        {
        public:
            static std::shared_ptr<ImageIf> createImage(
                const std::string& filename,
                ImageType imageType,
                Format type = Format::BC7_UNORM,

                int width = -1,
                int height = -1,
                int slices = -1,
                int mips = -1);
        };

    }
}
