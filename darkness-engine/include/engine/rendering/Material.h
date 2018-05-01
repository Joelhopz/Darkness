#pragma once

#include "engine/graphics/Format.h"
#include <vector>
#include <string>

namespace engine
{
    enum class TextureMapping
    {
        UV,
        Sphere,
        Cylinder,
        Box,
        Plane
    };

    enum class TextureMapMode
    {
        Wrap,
        Clamp,
        Decal,
        Mirror
    };

    enum class TextureType
    {
        Albedo,
        Roughness,
        Ambient,
        Emissive,
        Height,
        Normal,
        Shininess,
        Opacity,
        Displacement,
        Lightmap,
        Reflection,
        Metalness,
        Occlusion,
        Hdr,
        COUNT
    };

    std::string textureTypeToString(TextureType type);
    TextureType textureTypeFromString(const std::string& type);

    std::vector<engine::Format> possibleEncodingFormatRGB(TextureType type);
    std::vector<engine::Format> possibleEncodingFormatRGB(std::vector<TextureType> types);
    std::vector<engine::Format> possibleEncodingFormatSRGB(TextureType type);
    std::vector<engine::Format> possibleEncodingFormatSRGB(std::vector<TextureType> types);
    TextureType possibleTextureTypeFromFormat(engine::Format format);

    enum class TextureOp
    {
        Multiply,
        Add,
        Subtract,
        Divide,
        SmoothAdd,
        SignedAdd,

    };

    struct MaterialTexture
    {
        std::string filePath;
        TextureMapping mapping;
        TextureMapMode mode;
        TextureType type;
        TextureOp op;
        unsigned int uvIndex;

        size_t size() const;
        std::vector<uint8_t> data() const;
        size_t load(const uint8_t* data);
    };

    class Material
    {
    public:
        std::vector<MaterialTexture> textures;
        size_t size() const;
        std::vector<uint8_t> data() const;
        void load(std::vector<uint8_t> data);
    };
}
