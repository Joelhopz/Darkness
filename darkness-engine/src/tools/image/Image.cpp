#include "tools/image/Image.h"
#include "tools/image/Dds.h"
#include "tools/image/Bmp.h"
#include "tools/PathTools.h"
#include "platform/File.h"
#include <algorithm>

using namespace std;

namespace engine
{
    namespace image
    {
        shared_ptr<ImageIf> Image::createImage(
            const std::string& filename,
            ImageType imageType,
            Format format,
            int width,
            int height,
            int slices,
            int mips)
        {
            if (width != -1 && height != -1)
            {
                if (imageType == ImageType::DDS)
                {
                    return make_shared<Dds>(
                        filename,
                        format,
                        static_cast<unsigned int>(width), static_cast<unsigned int>(height),
                        slices != -1 ? slices : 1,
                        mips != -1 ? mips : 1
                        );
                }
                else if (imageType == ImageType::BMP)
                {
                    return make_shared<Bmp>(
                        filename,
                        format,
                        static_cast<unsigned int>(width), static_cast<unsigned int>(height),
                        slices != -1 ? slices : 1,
                        mips != -1 ? mips : 1
                        );
                }
            }
            else
            {
                if (!fileExists(filename))
                    return nullptr;

                if (imageType == ImageType::DDS)
                {
                    return make_shared<Dds>(filename);
                }
                else if (imageType == ImageType::BMP)
                {
                    return make_shared<Bmp>(filename);
                }
            }
            return nullptr;
        }

    }
}
