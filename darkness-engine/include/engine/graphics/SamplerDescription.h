#pragma once

#include "Common.h"
#include <cfloat>

namespace engine
{
    enum class Filter
    {
        Point,
        Bilinear,
        Trilinear,
        Anisotropic,
        Comparison
    };

    enum class TextureAddressMode
    {
        Wrap,
        Mirror,
        Clamp,
        Border,
        MirrorOnce
    };

    struct SamplerDescription
    {
        struct description
        {
            Filter filter;
            TextureAddressMode addressU;
            TextureAddressMode addressV;
            TextureAddressMode addressW;
            float mipLODBias;
            unsigned int maxAnisotrophy;
            ComparisonFunction comparisonFunc;
            float borderColor[4];
            float minLOD;
            float maxLOD;
        } desc;

        SamplerDescription()
            : desc {
            Filter::Anisotropic,
            TextureAddressMode::Wrap,
            TextureAddressMode::Wrap,
            TextureAddressMode::Wrap,
            0.0f,
            16,
            ComparisonFunction::LessEqual,
            { 1.0f, 1.0f, 1.0f, 1.0f },
            0.0f,
            FLT_MAX
            }
        {}

        SamplerDescription& textureAddressMode(TextureAddressMode mode)
        {
            desc.addressU = mode;
            desc.addressV = mode;
            desc.addressW = mode;
            return *this;
        }

        SamplerDescription& filter(Filter samplerFilter)
        {
            desc.filter = samplerFilter;
            return *this;
        }

        SamplerDescription& addressU(TextureAddressMode mode)
        {
            desc.addressU = mode;
            return *this;
        }

        SamplerDescription& addressV(TextureAddressMode mode)
        {
            desc.addressV = mode;
            return *this;
        }

        SamplerDescription& addressW(TextureAddressMode mode)
        {
            desc.addressW = mode;
            return *this;
        }

        SamplerDescription& mipLODBias(float bias)
        {
            desc.mipLODBias = bias;
            return *this;
        }

        SamplerDescription& maxAnisotrophy(unsigned int anisotrophy)
        {
            desc.maxAnisotrophy = anisotrophy;
            return *this;
        }

        SamplerDescription& comparisonFunc(ComparisonFunction func)
        {
            desc.comparisonFunc = func;
            return *this;
        }

        SamplerDescription& minLOD(float lod)
        {
            desc.minLOD = lod;
            return *this;
        }

        SamplerDescription& maxLOD(float lod)
        {
            desc.maxLOD = lod;
            return *this;
        }

        SamplerDescription& borderColor(float r, float g, float b, float a)
        {
            desc.borderColor[0] = r;
            desc.borderColor[1] = g;
            desc.borderColor[2] = b;
            desc.borderColor[3] = a;
            return *this;
        }
    };

    enum class StaticBorderColor
    {
        TransparentBlack,
        OpaqueBlack,
        OpaqueWhite
    };

    struct StaticSamplerDescription
    {
        struct description
        {
            Filter filter;
            TextureAddressMode addressU;
            TextureAddressMode addressV;
            TextureAddressMode addressW;
            float mipLODBias;
            unsigned int maxAnisotrophy;
            ComparisonFunction comparisonFunc;
            StaticBorderColor borderColor;
            float minLOD;
            float maxLOD;
            unsigned int shaderRegister;
            unsigned int registerSpace;
            ShaderVisibility shaderVisibility;
        } desc;

        StaticSamplerDescription& filter(Filter samplerFilter)
        {
            desc.filter = samplerFilter;
            return *this;
        }

        StaticSamplerDescription& addressU(TextureAddressMode mode)
        {
            desc.addressU = mode;
            return *this;
        }

        StaticSamplerDescription& addressV(TextureAddressMode mode)
        {
            desc.addressV = mode;
            return *this;
        }

        StaticSamplerDescription& addressW(TextureAddressMode mode)
        {
            desc.addressW = mode;
            return *this;
        }

        StaticSamplerDescription& mipLODBias(float bias)
        {
            desc.mipLODBias = bias;
            return *this;
        }

        StaticSamplerDescription& maxAnisotrophy(unsigned int anisotrophy)
        {
            desc.maxAnisotrophy = anisotrophy;
            return *this;
        }

        StaticSamplerDescription& comparisonFunc(ComparisonFunction func)
        {
            desc.comparisonFunc = func;
            return *this;
        }

        StaticSamplerDescription& borderColor(StaticBorderColor color)
        {
            desc.borderColor = color;
            return *this;
        }

        StaticSamplerDescription& minLOD(float lod)
        {
            desc.minLOD = lod;
            return *this;
        }

        StaticSamplerDescription& maxLOD(float lod)
        {
            desc.maxLOD = lod;
            return *this;
        }

        StaticSamplerDescription& shaderRegister(unsigned int reg)
        {
            desc.shaderRegister = reg;
            return *this;
        }

        StaticSamplerDescription& registerSpace(unsigned int regSpace)
        {
            desc.registerSpace = regSpace;
            return *this;
        }

        StaticSamplerDescription& shaderVisibility(ShaderVisibility visibility)
        {
            desc.shaderVisibility = visibility;
            return *this;
        }
    };

}
