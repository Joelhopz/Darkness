#include "tools/image/Image.h"
#include "tools/image/Dds.h"
#include "platform/File.h"

using namespace std;

namespace engine
{
    namespace image
    {
        shared_ptr<ImageIf> Image::createImage(const std::string& filename,
            Format format,
            int width,
            int height,
            int slices,
            int mips)
        {
            if (width != -1 && height != -1)
            {
                return make_shared<Dds>(
                    filename, 
                    format, 
                    static_cast<unsigned int>(width), static_cast<unsigned int>(height),
                    slices != -1 ? slices : 1,
                    mips != -1 ? mips : 1
                    );
            }
            else
            {
                if (!fileExists(filename))
                    return nullptr;
                return make_shared<Dds>(filename);
            }
        }

    }
}
