#include "engine/graphics/Format.h"
#include <algorithm>

using namespace std;

namespace engine
{
    size_t formatBytes(Format format, unsigned int width, unsigned int height)
    {
        auto elementBytes = formatBytes(format);
        if (!isBlockCompressedFormat(format))
            return elementBytes * width * height;

        switch (format)
        {
            case Format::Format_BC1_TYPELESS    : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC1_UNORM        : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC1_UNORM_SRGB    : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC2_TYPELESS    : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC2_UNORM        : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC2_UNORM_SRGB    : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC3_UNORM        : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC3_UNORM_SRGB    : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC3_TYPELESS    : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC4_TYPELESS    : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC4_UNORM        : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC4_SNORM        : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC5_TYPELESS    : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC5_UNORM        : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC5_SNORM        : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC6H_TYPELESS    : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC6H_UF16        : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC6H_SF16        : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC7_TYPELESS    : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC7_UNORM        : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
            case Format::Format_BC7_UNORM_SRGB    : return max<unsigned int>(1u, ((width + 3u) / 4u)) * max<unsigned int>(1u, ((height + 3u) / 4u)) * elementBytes;
        }
        return elementBytes * width * height;
    }

    size_t formatBytes(Format format)
    {
        return std::max<size_t>(formatBits(format) / 8, 1);
    }

    size_t formatBits(Format format)
    {
        switch (format)
        {
        case Format::Format_UNKNOWN:
            return 0;

        case Format::Format_R32G32B32A32_TYPELESS:
        case Format::Format_R32G32B32A32_FLOAT:
        case Format::Format_R32G32B32A32_UINT:
        case Format::Format_R32G32B32A32_SINT:
            return 128;
        
        case Format::Format_R32G32B32_TYPELESS:
        case Format::Format_R32G32B32_FLOAT:
        case Format::Format_R32G32B32_UINT:
        case Format::Format_R32G32B32_SINT:
            return 96;

        case Format::Format_R16G16B16A16_TYPELESS:
        case Format::Format_R16G16B16A16_FLOAT:
        case Format::Format_R16G16B16A16_UNORM:
        case Format::Format_R16G16B16A16_UINT:
        case Format::Format_R16G16B16A16_SNORM:
        case Format::Format_R16G16B16A16_SINT:
        case Format::Format_R32G32_TYPELESS:
        case Format::Format_R32G32_FLOAT:
        case Format::Format_R32G32_UINT:
        case Format::Format_R32G32_SINT:
        case Format::Format_R32G8X24_TYPELESS:
        case Format::Format_D32_FLOAT_S8X24_UINT:
        case Format::Format_R32_FLOAT_X8X24_TYPELESS:
        case Format::Format_X32_TYPELESS_G8X24_UINT:
            return 64;

        case Format::Format_R10G10B10A2_TYPELESS:
        case Format::Format_R10G10B10A2_UNORM:
        case Format::Format_R10G10B10A2_UINT:
        case Format::Format_R11G11B10_FLOAT:
        case Format::Format_R8G8B8A8_TYPELESS:
        case Format::Format_R8G8B8A8_UNORM:
        case Format::Format_R8G8B8A8_UNORM_SRGB:
        case Format::Format_R8G8B8A8_UINT:
        case Format::Format_R8G8B8A8_SNORM:
        case Format::Format_R8G8B8A8_SINT:
        case Format::Format_R16G16_TYPELESS:
        case Format::Format_R16G16_FLOAT:
        case Format::Format_R16G16_UNORM:
        case Format::Format_R16G16_UINT:
        case Format::Format_R16G16_SNORM:
        case Format::Format_R16G16_SINT:
        case Format::Format_R32_TYPELESS:
        case Format::Format_D32_FLOAT:
        case Format::Format_R32_FLOAT:
        case Format::Format_R32_UINT:
        case Format::Format_R32_SINT:
        case Format::Format_R24G8_TYPELESS:
        case Format::Format_D24_UNORM_S8_UINT:
        case Format::Format_R24_UNORM_X8_TYPELESS:
        case Format::Format_X24_TYPELESS_G8_UINT:
        case Format::Format_R9G9B9E5_SHAREDEXP:
        case Format::Format_R8G8_B8G8_UNORM:
        case Format::Format_G8R8_G8B8_UNORM:
        case Format::Format_B8G8R8A8_UNORM:
        case Format::Format_B8G8R8X8_UNORM:
        case Format::Format_R10G10B10_XR_BIAS_A2_UNORM:
        case Format::Format_B8G8R8A8_TYPELESS:
        case Format::Format_B8G8R8A8_UNORM_SRGB:
        case Format::Format_B8G8R8X8_TYPELESS:
        case Format::Format_B8G8R8X8_UNORM_SRGB:
            return 32;

        case Format::Format_R8G8_TYPELESS:
        case Format::Format_R8G8_UNORM:
        case Format::Format_R8G8_UINT:
        case Format::Format_R8G8_SNORM:
        case Format::Format_R8G8_SINT:
        case Format::Format_R16_TYPELESS:
        case Format::Format_R16_FLOAT:
        case Format::Format_D16_UNORM:
        case Format::Format_R16_UNORM:
        case Format::Format_R16_UINT:
        case Format::Format_R16_SNORM:
        case Format::Format_R16_SINT:
        case Format::Format_B5G6R5_UNORM:
        case Format::Format_B5G5R5A1_UNORM:
            return 16;

        case Format::Format_R8_TYPELESS:
        case Format::Format_R8_UNORM:
        case Format::Format_R8_UINT:
        case Format::Format_R8_SNORM:
        case Format::Format_R8_SINT:
        case Format::Format_A8_UNORM:
            return 8;

        case Format::Format_R1_UNORM:
            return 1;

        case Format::Format_BC1_TYPELESS:
        case Format::Format_BC1_UNORM:
        case Format::Format_BC1_UNORM_SRGB:
        case Format::Format_BC4_TYPELESS:
        case Format::Format_BC4_UNORM:
        case Format::Format_BC4_SNORM:
            return 64;

        case Format::Format_BC2_TYPELESS:
        case Format::Format_BC2_UNORM:
        case Format::Format_BC2_UNORM_SRGB:
        case Format::Format_BC3_TYPELESS:
        case Format::Format_BC3_UNORM:
        case Format::Format_BC3_UNORM_SRGB:
        case Format::Format_BC5_TYPELESS:
        case Format::Format_BC5_UNORM:
        case Format::Format_BC5_SNORM:
        case Format::Format_BC6H_TYPELESS:
        case Format::Format_BC6H_UF16:
        case Format::Format_BC6H_SF16:
        case Format::Format_BC7_TYPELESS:
        case Format::Format_BC7_UNORM:
        case Format::Format_BC7_UNORM_SRGB:
            return 128;

        case Format::Format_AYUV:
        case Format::Format_Y410:                        return 32;

        // these formats can't really be expressed correctly here
        case Format::Format_Y416:                        return 64;
        case Format::Format_NV12:                        return 16;
        case Format::Format_P010:                        return 32;
        case Format::Format_P016:                        return 32;
        case Format::Format_420_OPAQUE:                    return 8;
        case Format::Format_YUY2:                        return 32;
        case Format::Format_Y210:                        return 64;
        case Format::Format_Y216:                        return 32;

        case Format::Format_NV11:                        return 8;
        case Format::Format_AI44:                        return 4;
        case Format::Format_IA44:                        return 4;
        case Format::Format_P8:                            return 8;
        case Format::Format_A8P8:                        return 8;
        case Format::Format_B4G4R4A4_UNORM:                return 16;

        case Format::Format_P208:                        return 8;
        case Format::Format_V208:                        return 8;
        case Format::Format_V408:                        return 8;
        }
        return 8;
    };

    Format srgbFormat(Format format)
    {
        if ((format == Format::Format_R8G8B8A8_TYPELESS) ||
            (format == Format::Format_R8G8B8A8_UNORM) ||
            (format == Format::Format_R8G8B8A8_UNORM_SRGB) ||
            (format == Format::Format_R8G8B8A8_UINT) ||
            (format == Format::Format_R8G8B8A8_SNORM) ||
            (format == Format::Format_R8G8B8A8_SINT))
            return Format::Format_R8G8B8A8_UNORM_SRGB;

        else if (
            (format == Format::Format_BC1_TYPELESS) ||
            (format == Format::Format_BC1_UNORM) ||
            (format == Format::Format_BC1_UNORM_SRGB))
            return Format::Format_BC1_UNORM_SRGB;

        else if (
            (format == Format::Format_BC2_TYPELESS) ||
            (format == Format::Format_BC2_UNORM) ||
            (format == Format::Format_BC2_UNORM_SRGB))
            return Format::Format_BC2_UNORM_SRGB;

        else if (
            (format == Format::Format_BC3_TYPELESS) ||
            (format == Format::Format_BC3_UNORM) ||
            (format == Format::Format_BC3_UNORM_SRGB))
            return Format::Format_BC3_UNORM_SRGB;

        else if (
            (format == Format::Format_B8G8R8A8_TYPELESS) ||
            (format == Format::Format_B8G8R8A8_UNORM_SRGB) ||
            (format == Format::Format_B8G8R8X8_TYPELESS) ||
            (format == Format::Format_B8G8R8X8_UNORM_SRGB))
            return Format::Format_B8G8R8X8_UNORM_SRGB;

        else if (
            (format == Format::Format_BC7_TYPELESS) ||
            (format == Format::Format_BC7_UNORM) ||
            (format == Format::Format_BC7_UNORM_SRGB))
            return Format::Format_BC7_UNORM_SRGB;

        return format;
    }

    bool isBlockCompressedFormat(Format format)
    {
        return ( // most common first
                (format == Format::Format_BC7_UNORM) ||
                (format == Format::Format_BC7_UNORM_SRGB) ||
                (format == Format::Format_BC3_UNORM) ||
                (format == Format::Format_BC3_UNORM_SRGB) ||

                (format == Format::Format_BC1_TYPELESS) ||
                (format == Format::Format_BC1_UNORM) ||
                (format == Format::Format_BC1_UNORM_SRGB) ||
                (format == Format::Format_BC2_TYPELESS) ||
                (format == Format::Format_BC2_UNORM) ||
                (format == Format::Format_BC2_UNORM_SRGB) ||
                (format == Format::Format_BC3_TYPELESS) ||
                (format == Format::Format_BC4_TYPELESS) ||
                (format == Format::Format_BC4_UNORM) ||
                (format == Format::Format_BC4_SNORM) ||
                (format == Format::Format_BC5_TYPELESS) ||
                (format == Format::Format_BC5_UNORM) ||
                (format == Format::Format_BC5_SNORM) ||
                (format == Format::Format_BC6H_TYPELESS) ||
                (format == Format::Format_BC6H_UF16) ||
                (format == Format::Format_BC6H_SF16) ||
                (format == Format::Format_BC7_TYPELESS)
                );
    }

    std::string formatToString(Format format)
    {
        switch (format)
        {
        case Format::Format_UNKNOWN: return "Format_UNKNOWN";
        case Format::Format_R32G32B32A32_TYPELESS:                return "Format_R32G32B32A32_TYPELESS";
        case Format::Format_R32G32B32A32_FLOAT:                    return "Format_R32G32B32A32_FLOAT";
        case Format::Format_R32G32B32A32_UINT:                    return "Format_R32G32B32A32_UINT";
        case Format::Format_R32G32B32A32_SINT:                    return "Format_R32G32B32A32_SINT";
        case Format::Format_R32G32B32_TYPELESS:                    return "Format_R32G32B32_TYPELESS";
        case Format::Format_R32G32B32_FLOAT:                    return "Format_R32G32B32_FLOAT";
        case Format::Format_R32G32B32_UINT:                        return "Format_R32G32B32_UINT";
        case Format::Format_R32G32B32_SINT:                        return "Format_R32G32B32_SINT";
        case Format::Format_R16G16B16A16_TYPELESS:                return "Format_R16G16B16A16_TYPELESS";
        case Format::Format_R16G16B16A16_FLOAT:                    return "Format_R16G16B16A16_FLOAT";
        case Format::Format_R16G16B16A16_UNORM:                    return "Format_R16G16B16A16_UNORM";
        case Format::Format_R16G16B16A16_UINT:                    return "Format_R16G16B16A16_UINT";
        case Format::Format_R16G16B16A16_SNORM:                    return "Format_R16G16B16A16_SNORM";
        case Format::Format_R16G16B16A16_SINT:                    return "Format_R16G16B16A16_SINT";
        case Format::Format_R32G32_TYPELESS:                    return "Format_R32G32_TYPELESS";
        case Format::Format_R32G32_FLOAT:                        return "Format_R32G32_FLOAT";
        case Format::Format_R32G32_UINT:                        return "Format_R32G32_UINT";
        case Format::Format_R32G32_SINT:                        return "Format_R32G32_SINT";
        case Format::Format_R32G8X24_TYPELESS:                    return "Format_R32G8X24_TYPELESS";
        case Format::Format_D32_FLOAT_S8X24_UINT:                return "Format_D32_FLOAT_S8X24_UINT";
        case Format::Format_R32_FLOAT_X8X24_TYPELESS:            return "Format_R32_FLOAT_X8X24_TYPELESS";
        case Format::Format_X32_TYPELESS_G8X24_UINT:            return "Format_X32_TYPELESS_G8X24_UINT";
        case Format::Format_R10G10B10A2_TYPELESS:                return "Format_R10G10B10A2_TYPELESS";
        case Format::Format_R10G10B10A2_UNORM:                    return "Format_R10G10B10A2_UNORM";
        case Format::Format_R10G10B10A2_UINT:                    return "Format_R10G10B10A2_UINT";
        case Format::Format_R11G11B10_FLOAT:                    return "Format_R11G11B10_FLOAT";
        case Format::Format_R8G8B8A8_TYPELESS:                    return "Format_R8G8B8A8_TYPELESS";
        case Format::Format_R8G8B8A8_UNORM:                        return "Format_R8G8B8A8_UNORM";
        case Format::Format_R8G8B8A8_UNORM_SRGB:                return "Format_R8G8B8A8_UNORM_SRGB";
        case Format::Format_R8G8B8A8_UINT:                        return "Format_R8G8B8A8_UINT";
        case Format::Format_R8G8B8A8_SNORM:                        return "Format_R8G8B8A8_SNORM";
        case Format::Format_R8G8B8A8_SINT:                        return "Format_R8G8B8A8_SINT";
        case Format::Format_R16G16_TYPELESS:                    return "Format_R16G16_TYPELESS";
        case Format::Format_R16G16_FLOAT:                        return "Format_R16G16_FLOAT";
        case Format::Format_R16G16_UNORM:                        return "Format_R16G16_UNORM";
        case Format::Format_R16G16_UINT:                        return "Format_R16G16_UINT";
        case Format::Format_R16G16_SNORM:                        return "Format_R16G16_SNORM";
        case Format::Format_R16G16_SINT:                        return "Format_R16G16_SINT";
        case Format::Format_R32_TYPELESS:                        return "Format_R32_TYPELESS";
        case Format::Format_D32_FLOAT:                            return "Format_D32_FLOAT";
        case Format::Format_R32_FLOAT:                            return "Format_R32_FLOAT";
        case Format::Format_R32_UINT:                            return "Format_R32_UINT";
        case Format::Format_R32_SINT:                            return "Format_R32_SINT";
        case Format::Format_R24G8_TYPELESS:                        return "Format_R24G8_TYPELESS";
        case Format::Format_D24_UNORM_S8_UINT:                    return "Format_D24_UNORM_S8_UINT";
        case Format::Format_R24_UNORM_X8_TYPELESS:                return "Format_R24_UNORM_X8_TYPELESS";
        case Format::Format_X24_TYPELESS_G8_UINT:                return "Format_X24_TYPELESS_G8_UINT";
        case Format::Format_R8G8_TYPELESS:                        return "Format_R8G8_TYPELESS";
        case Format::Format_R8G8_UNORM:                            return "Format_R8G8_UNORM";
        case Format::Format_R8G8_UINT:                            return "Format_R8G8_UINT";
        case Format::Format_R8G8_SNORM:                            return "Format_R8G8_SNORM";
        case Format::Format_R8G8_SINT:                            return "Format_R8G8_SINT";
        case Format::Format_R16_TYPELESS:                        return "Format_R16_TYPELESS";
        case Format::Format_R16_FLOAT:                            return "Format_R16_FLOAT";
        case Format::Format_D16_UNORM:                            return "Format_D16_UNORM";
        case Format::Format_R16_UNORM:                            return "Format_R16_UNORM";
        case Format::Format_R16_UINT:                            return "Format_R16_UINT";
        case Format::Format_R16_SNORM:                            return "Format_R16_SNORM";
        case Format::Format_R16_SINT:                            return "Format_R16_SINT";
        case Format::Format_R8_TYPELESS:                        return "Format_R8_TYPELESS";
        case Format::Format_R8_UNORM:                            return "Format_R8_UNORM";
        case Format::Format_R8_UINT:                            return "Format_R8_UINT";
        case Format::Format_R8_SNORM:                            return "Format_R8_SNORM";
        case Format::Format_R8_SINT:                            return "Format_R8_SINT";
        case Format::Format_A8_UNORM:                            return "Format_A8_UNORM";
        case Format::Format_R1_UNORM:                            return "Format_R1_UNORM";
        case Format::Format_R9G9B9E5_SHAREDEXP:                    return "Format_R9G9B9E5_SHAREDEXP";
        case Format::Format_R8G8_B8G8_UNORM:                    return "Format_R8G8_B8G8_UNORM";
        case Format::Format_G8R8_G8B8_UNORM:                    return "Format_G8R8_G8B8_UNORM";
        case Format::Format_BC1_TYPELESS:                        return "Format_BC1_TYPELESS";
        case Format::Format_BC1_UNORM:                            return "Format_BC1_UNORM";
        case Format::Format_BC1_UNORM_SRGB:                        return "Format_BC1_UNORM_SRGB";
        case Format::Format_BC2_TYPELESS:                        return "Format_BC2_TYPELESS";
        case Format::Format_BC2_UNORM:                            return "Format_BC2_UNORM";
        case Format::Format_BC2_UNORM_SRGB:                        return "Format_BC2_UNORM_SRGB";
        case Format::Format_BC3_TYPELESS:                        return "Format_BC3_TYPELESS";
        case Format::Format_BC3_UNORM:                            return "Format_BC3_UNORM";
        case Format::Format_BC3_UNORM_SRGB:                        return "Format_BC3_UNORM_SRGB";
        case Format::Format_BC4_TYPELESS:                        return "Format_BC4_TYPELESS";
        case Format::Format_BC4_UNORM:                            return "Format_BC4_UNORM";
        case Format::Format_BC4_SNORM:                            return "Format_BC4_SNORM";
        case Format::Format_BC5_TYPELESS:                        return "Format_BC5_TYPELESS";
        case Format::Format_BC5_UNORM:                            return "Format_BC5_UNORM";
        case Format::Format_BC5_SNORM:                            return "Format_BC5_SNORM";
        case Format::Format_B5G6R5_UNORM:                        return "Format_B5G6R5_UNORM";
        case Format::Format_B5G5R5A1_UNORM:                        return "Format_B5G5R5A1_UNORM";
        case Format::Format_B8G8R8A8_UNORM:                        return "Format_B8G8R8A8_UNORM";
        case Format::Format_B8G8R8X8_UNORM:                        return "Format_B8G8R8X8_UNORM";
        case Format::Format_R10G10B10_XR_BIAS_A2_UNORM:            return "Format_R10G10B10_XR_BIAS_A2_UNORM";
        case Format::Format_B8G8R8A8_TYPELESS:                    return "Format_B8G8R8A8_TYPELESS";
        case Format::Format_B8G8R8A8_UNORM_SRGB:                return "Format_B8G8R8A8_UNORM_SRGB";
        case Format::Format_B8G8R8X8_TYPELESS:                    return "Format_B8G8R8X8_TYPELESS";
        case Format::Format_B8G8R8X8_UNORM_SRGB:                return "Format_B8G8R8X8_UNORM_SRGB";
        case Format::Format_BC6H_TYPELESS:                        return "Format_BC6H_TYPELESS";
        case Format::Format_BC6H_UF16:                            return "Format_BC6H_UF16";
        case Format::Format_BC6H_SF16:                            return "Format_BC6H_SF16";
        case Format::Format_BC7_TYPELESS:                        return "Format_BC7_TYPELESS";
        case Format::Format_BC7_UNORM:                            return "Format_BC7_UNORM";
        case Format::Format_BC7_UNORM_SRGB:                        return "Format_BC7_UNORM_SRGB";
        case Format::Format_AYUV:                                return "Format_AYUV";
        case Format::Format_Y410:                                return "Format_Y410";
        case Format::Format_Y416:                                return "Format_Y416";
        case Format::Format_NV12:                                return "Format_NV12";
        case Format::Format_P010:                                return "Format_P010";
        case Format::Format_P016:                                return "Format_P016";
        case Format::Format_420_OPAQUE:                            return "Format_420_OPAQUE";
        case Format::Format_YUY2:                                return "Format_YUY2";
        case Format::Format_Y210:                                return "Format_Y210";
        case Format::Format_Y216:                                return "Format_Y216";
        case Format::Format_NV11:                                return "Format_NV11";
        case Format::Format_AI44:                                return "Format_AI44";
        case Format::Format_IA44:                                return "Format_IA44";
        case Format::Format_P8:                                    return "Format_P8";
        case Format::Format_A8P8:                                return "Format_A8P8";
        case Format::Format_B4G4R4A4_UNORM:                        return "Format_B4G4R4A4_UNORM";
        case Format::Format_P208:                                return "Format_P208";
        case Format::Format_V208:                                return "Format_V208";
        case Format::Format_V408:                                return "Format_V408";
        default: return "Format_BC1_UNORM";
        }
    }

    Format formatFromString(const std::string& format)
    {
        if (format == "Format_UNKNOWN") return Format::Format_UNKNOWN;
        else if (format == "Format_R32G32B32A32_TYPELESS") return Format::Format_R32G32B32A32_TYPELESS;
        else if (format == "Format_R32G32B32A32_FLOAT") return Format::Format_R32G32B32A32_FLOAT;
        else if (format == "Format_R32G32B32A32_UINT") return Format::Format_R32G32B32A32_UINT;
        else if (format == "Format_R32G32B32A32_SINT") return Format::Format_R32G32B32A32_SINT;
        else if (format == "Format_R32G32B32_TYPELESS") return Format::Format_R32G32B32_TYPELESS;
        else if (format == "Format_R32G32B32_FLOAT") return Format::Format_R32G32B32_FLOAT;
        else if (format == "Format_R32G32B32_UINT") return Format::Format_R32G32B32_UINT;
        else if (format == "Format_R32G32B32_SINT") return Format::Format_R32G32B32_SINT;
        else if (format == "Format_R16G16B16A16_TYPELESS") return Format::Format_R16G16B16A16_TYPELESS;
        else if (format == "Format_R16G16B16A16_FLOAT") return Format::Format_R16G16B16A16_FLOAT;
        else if (format == "Format_R16G16B16A16_UNORM") return Format::Format_R16G16B16A16_UNORM;
        else if (format == "Format_R16G16B16A16_UINT") return Format::Format_R16G16B16A16_UINT;
        else if (format == "Format_R16G16B16A16_SNORM") return Format::Format_R16G16B16A16_SNORM;
        else if (format == "Format_R16G16B16A16_SINT") return Format::Format_R16G16B16A16_SINT;
        else if (format == "Format_R32G32_TYPELESS") return Format::Format_R32G32_TYPELESS;
        else if (format == "Format_R32G32_FLOAT") return Format::Format_R32G32_FLOAT;
        else if (format == "Format_R32G32_UINT") return Format::Format_R32G32_UINT;
        else if (format == "Format_R32G32_SINT") return Format::Format_R32G32_SINT;
        else if (format == "Format_R32G8X24_TYPELESS") return Format::Format_R32G8X24_TYPELESS;
        else if (format == "Format_D32_FLOAT_S8X24_UINT") return Format::Format_D32_FLOAT_S8X24_UINT;
        else if (format == "Format_R32_FLOAT_X8X24_TYPELESS") return Format::Format_R32_FLOAT_X8X24_TYPELESS;
        else if (format == "Format_X32_TYPELESS_G8X24_UINT") return Format::Format_X32_TYPELESS_G8X24_UINT;
        else if (format == "Format_R10G10B10A2_TYPELESS") return Format::Format_R10G10B10A2_TYPELESS;
        else if (format == "Format_R10G10B10A2_UNORM") return Format::Format_R10G10B10A2_UNORM;
        else if (format == "Format_R10G10B10A2_UINT") return Format::Format_R10G10B10A2_UINT;
        else if (format == "Format_R11G11B10_FLOAT") return Format::Format_R11G11B10_FLOAT;
        else if (format == "Format_R8G8B8A8_TYPELESS") return Format::Format_R8G8B8A8_TYPELESS;
        else if (format == "Format_R8G8B8A8_UNORM") return Format::Format_R8G8B8A8_UNORM;    
        else if (format == "Format_R8G8B8A8_UNORM_SRGB") return Format::Format_R8G8B8A8_UNORM_SRGB;
        else if (format == "Format_R8G8B8A8_UINT") return Format::Format_R8G8B8A8_UINT;    
        else if (format == "Format_R8G8B8A8_SNORM") return Format::Format_R8G8B8A8_SNORM;    
        else if (format == "Format_R8G8B8A8_SINT") return Format::Format_R8G8B8A8_SINT;    
        else if (format == "Format_R16G16_TYPELESS") return Format::Format_R16G16_TYPELESS;
        else if (format == "Format_R16G16_FLOAT") return Format::Format_R16G16_FLOAT;    
        else if (format == "Format_R16G16_UNORM") return Format::Format_R16G16_UNORM;    
        else if (format == "Format_R16G16_UINT") return Format::Format_R16G16_UINT;    
        else if (format == "Format_R16G16_SNORM") return Format::Format_R16G16_SNORM;    
        else if (format == "Format_R16G16_SINT") return Format::Format_R16G16_SINT;    
        else if (format == "Format_R32_TYPELESS") return Format::Format_R32_TYPELESS;    
        else if (format == "Format_D32_FLOAT") return Format::Format_D32_FLOAT;        
        else if (format == "Format_R32_FLOAT") return Format::Format_R32_FLOAT;        
        else if (format == "Format_R32_UINT") return Format::Format_R32_UINT;        
        else if (format == "Format_R32_SINT") return Format::Format_R32_SINT;        
        else if (format == "Format_R24G8_TYPELESS") return Format::Format_R24G8_TYPELESS;    
        else if (format == "Format_D24_UNORM_S8_UINT") return Format::Format_D24_UNORM_S8_UINT;
        else if (format == "Format_R24_UNORM_X8_TYPELESS") return Format::Format_R24_UNORM_X8_TYPELESS;
        else if (format == "Format_X24_TYPELESS_G8_UINT") return Format::Format_X24_TYPELESS_G8_UINT;
        else if (format == "Format_R8G8_TYPELESS") return Format::Format_R8G8_TYPELESS;    
        else if (format == "Format_R8G8_UNORM") return Format::Format_R8G8_UNORM;        
        else if (format == "Format_R8G8_UINT") return Format::Format_R8G8_UINT;        
        else if (format == "Format_R8G8_SNORM") return Format::Format_R8G8_SNORM;        
        else if (format == "Format_R8G8_SINT") return Format::Format_R8G8_SINT;        
        else if (format == "Format_R16_TYPELESS") return Format::Format_R16_TYPELESS;    
        else if (format == "Format_R16_FLOAT") return Format::Format_R16_FLOAT;        
        else if (format == "Format_D16_UNORM") return Format::Format_D16_UNORM;        
        else if (format == "Format_R16_UNORM") return Format::Format_R16_UNORM;        
        else if (format == "Format_R16_UINT") return Format::Format_R16_UINT;        
        else if (format == "Format_R16_SNORM") return Format::Format_R16_SNORM;        
        else if (format == "Format_R16_SINT") return Format::Format_R16_SINT;        
        else if (format == "Format_R8_TYPELESS") return Format::Format_R8_TYPELESS;    
        else if (format == "Format_R8_UNORM") return Format::Format_R8_UNORM;        
        else if (format == "Format_R8_UINT") return Format::Format_R8_UINT;        
        else if (format == "Format_R8_SNORM") return Format::Format_R8_SNORM;        
        else if (format == "Format_R8_SINT") return Format::Format_R8_SINT;        
        else if (format == "Format_A8_UNORM") return Format::Format_A8_UNORM;        
        else if (format == "Format_R1_UNORM") return Format::Format_R1_UNORM;        
        else if (format == "Format_R9G9B9E5_SHAREDEXP") return Format::Format_R9G9B9E5_SHAREDEXP;
        else if (format == "Format_R8G8_B8G8_UNORM") return Format::Format_R8G8_B8G8_UNORM;
        else if (format == "Format_G8R8_G8B8_UNORM") return Format::Format_G8R8_G8B8_UNORM;
        else if (format == "Format_BC1_TYPELESS") return Format::Format_BC1_TYPELESS;    
        else if (format == "Format_BC1_UNORM") return Format::Format_BC1_UNORM;        
        else if (format == "Format_BC1_UNORM_SRGB") return Format::Format_BC1_UNORM_SRGB;    
        else if (format == "Format_BC2_TYPELESS") return Format::Format_BC2_TYPELESS;    
        else if (format == "Format_BC2_UNORM") return Format::Format_BC2_UNORM;        
        else if (format == "Format_BC2_UNORM_SRGB") return Format::Format_BC2_UNORM_SRGB;    
        else if (format == "Format_BC3_TYPELESS") return Format::Format_BC3_TYPELESS;    
        else if (format == "Format_BC3_UNORM") return Format::Format_BC3_UNORM;        
        else if (format == "Format_BC3_UNORM_SRGB") return Format::Format_BC3_UNORM_SRGB;    
        else if (format == "Format_BC4_TYPELESS") return Format::Format_BC4_TYPELESS;    
        else if (format == "Format_BC4_UNORM") return Format::Format_BC4_UNORM;        
        else if (format == "Format_BC4_SNORM") return Format::Format_BC4_SNORM;        
        else if (format == "Format_BC5_TYPELESS") return Format::Format_BC5_TYPELESS;    
        else if (format == "Format_BC5_UNORM") return Format::Format_BC5_UNORM;        
        else if (format == "Format_BC5_SNORM") return Format::Format_BC5_SNORM;        
        else if (format == "Format_B5G6R5_UNORM") return Format::Format_B5G6R5_UNORM;    
        else if (format == "Format_B5G5R5A1_UNORM") return Format::Format_B5G5R5A1_UNORM;    
        else if (format == "Format_B8G8R8A8_UNORM") return Format::Format_B8G8R8A8_UNORM;    
        else if (format == "Format_B8G8R8X8_UNORM") return Format::Format_B8G8R8X8_UNORM;    
        else if (format == "Format_R10G10B10_XR_BIAS_A2_UNORM") return Format::Format_R10G10B10_XR_BIAS_A2_UNORM;
        else if (format == "Format_B8G8R8A8_TYPELESS") return Format::Format_B8G8R8A8_TYPELESS;
        else if (format == "Format_B8G8R8A8_UNORM_SRGB") return Format::Format_B8G8R8A8_UNORM_SRGB;
        else if (format == "Format_B8G8R8X8_TYPELESS") return Format::Format_B8G8R8X8_TYPELESS;
        else if (format == "Format_B8G8R8X8_UNORM_SRGB") return Format::Format_B8G8R8X8_UNORM_SRGB;
        else if (format == "Format_BC6H_TYPELESS") return Format::Format_BC6H_TYPELESS;    
        else if (format == "Format_BC6H_UF16") return Format::Format_BC6H_UF16;        
        else if (format == "Format_BC6H_SF16") return Format::Format_BC6H_SF16;        
        else if (format == "Format_BC7_TYPELESS") return Format::Format_BC7_TYPELESS;    
        else if (format == "Format_BC7_UNORM") return Format::Format_BC7_UNORM;        
        else if (format == "Format_BC7_UNORM_SRGB") return Format::Format_BC7_UNORM_SRGB;    
        else if (format == "Format_AYUV") return Format::Format_AYUV;            
        else if (format == "Format_Y410") return Format::Format_Y410;            
        else if (format == "Format_Y416") return Format::Format_Y416;            
        else if (format == "Format_NV12") return Format::Format_NV12;            
        else if (format == "Format_P010") return Format::Format_P010;            
        else if (format == "Format_P016") return Format::Format_P016;            
        else if (format == "Format_420_OPAQUE") return Format::Format_420_OPAQUE;        
        else if (format == "Format_YUY2") return Format::Format_YUY2;            
        else if (format == "Format_Y210") return Format::Format_Y210;            
        else if (format == "Format_Y216") return Format::Format_Y216;            
        else if (format == "Format_NV11") return Format::Format_NV11;            
        else if (format == "Format_AI44") return Format::Format_AI44;            
        else if (format == "Format_IA44") return Format::Format_IA44;            
        else if (format == "Format_P8") return Format::Format_P8;                
        else if (format == "Format_A8P8") return Format::Format_A8P8;            
        else if (format == "Format_B4G4R4A4_UNORM") return Format::Format_B4G4R4A4_UNORM;    
        else if (format == "Format_P208") return Format::Format_P208;            
        else if (format == "Format_V208") return Format::Format_V208;            
        else if (format == "Format_V408") return Format::Format_V408;            

        return Format::Format_BC1_UNORM;
    }

    SurfaceInformation surfaceInformation(Format format, unsigned int width, unsigned int height)
    {
        SurfaceInformation info;
        if (isBlockCompressedFormat(format))
        {
            unsigned int blockWidth = width > 0 ? max(1u, ((width + 3u) / 4u)) : 0u;
            unsigned int blockHeight = height > 0 ? max(1u, ((height + 3u) / 4u)) : 0u;
            unsigned int bytesPerBlock = static_cast<unsigned int>(formatBytes(format));
            info.rowBytes = blockWidth * bytesPerBlock;
            info.numRows = blockHeight;
        }
        else if ((format == Format::Format_R8G8_B8G8_UNORM) ||
            (format == Format::Format_G8R8_G8B8_UNORM) ||
            (format == Format::Format_YUY2))
        {
            info.rowBytes = ((width + 1u) >> 1u) * 4u;
            info.numRows = height;
        }
        else
        {
            info.rowBytes = (width * static_cast<unsigned int>(formatBits(format)) + 7u) / 8u;
            info.numRows = height;
        }
        info.numBytes = info.rowBytes * info.numRows;
        return info;
    }

    size_t imageBytes(Format format, unsigned int width, unsigned int height, unsigned int slices, unsigned int mips)
    {
        size_t bytes = 0;
        for (unsigned int slice = 0; slice < slices; ++slice)
        {
            auto w = width;
            auto h = height;
            for (unsigned int mip = 0; mip < mips; ++mip)
            {
                auto info = surfaceInformation(format, w, h);
                bytes += info.numBytes;
                w /= 2;
                h /= 2;
            }
        }
        return bytes;
    }
}
