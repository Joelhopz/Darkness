#pragma once

#include <cstdint>

namespace engine
{
    static const int MaxNumberOfDescriptorsPerHeap = 256;

    enum class ShaderVisibilityBits
    {
        Vertex = 0x00000001,
        Hull = 0x00000002,
        Domain = 0x00000004,
        Geometry = 0x00000008,
        Pixel = 0x00000010,
        Compute = 0x00000020,
        AllGraphics = 0x0000001F,
        All = 0x7FFFFFFF
    };
    using ShaderVisibility = uint32_t;

    enum class ComparisonFunction
    {
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always
    };

    enum class DescriptorHeapFlags
    {
        None,
        ShaderVisible
    };

    enum class CommandListType
    {
        Graphics,
        Compute,
        Transfer
        // Bundle
        // Sparse binding
    };

    enum class BufferUsageFlagBits
    {
        Src = 0x00000001,
        Dst = 0x00000002,
        UniformTexel = 0x00000004,
        StorageTexel = 0x00000008,
        Uniform = 0x00000010,
        Storage = 0x00000020,
        Index = 0x00000040,
        Vertex = 0x00000080,
        Indirect = 0x00000100
    };
    using BufferUsageFlags = uint32_t;

    enum class BufferMemoryFlagBits
    {
        DeviceLocal = 0x00000001,
        HostVisible = 0x00000002,
        HostCoherent = 0x00000004,
        HostCached = 0x00000008,
        LazilyAllocated = 0x00000010
    };
    using BufferMemoryFlags = uint32_t;

    enum class ImageUsageFlagBits
    {
        Src = 0x00000001,
        Dst = 0x00000002,
        Sampled = 0x00000004,
        Storage = 0x00000008,
        ColorAttachment = 0x00000010,
        DepthStencilAttachment = 0x00000020,
        TransientAttachment = 0x00000040,
        InputAttachment = 0x00000080
    };
    using ImageUsageFlags = uint32_t;

    enum class ImageTiling
    {
        Optimal,
        Linear
    };

    enum class ImageAspectFlagBits
    {
        Color       = 0x00000001,
        Depth       = 0x00000002,
        Stencil     = 0x00000004,
        Metadata    = 0x00000008
    };
    using ImageAspectFlags = uint32_t;

    constexpr const int32_t AllMipLevels = -1;
    constexpr const int32_t AllArraySlices = -1;
    struct SubResource
    {
        uint32_t firstMipLevel = 0;
        int32_t mipCount = AllMipLevels;
        uint32_t firstArraySlice = 0;
        int32_t arraySliceCount = AllArraySlices;
    };
}
