#include "engine/graphics/vulkan/VulkanConversions.h"
#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/vulkan/VulkanPipeline.h"
#include "engine/graphics/Common.h"
#include "engine/graphics/Format.h"
#include "engine/graphics/Barrier.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/Pipeline.h"
#include "engine/graphics/SamplerDescription.h"

#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        VkQueueFlags vulkanCommandListType(CommandListType type)
        {
            switch (type)
            {
                case CommandListType::Graphics: return VK_QUEUE_GRAPHICS_BIT;
                case CommandListType::Compute: return VK_QUEUE_COMPUTE_BIT;
                case CommandListType::Transfer: return VK_QUEUE_TRANSFER_BIT;
            }
            return VK_QUEUE_GRAPHICS_BIT;
        }

        VkFormat vulkanFormat(Format format)
        {
            switch (format)
            {
                case Format::Format_UNKNOWN: return VK_FORMAT_UNDEFINED;
                case Format::Format_R32G32B32A32_TYPELESS: return VK_FORMAT_R32G32B32A32_UINT;
                case Format::Format_R32G32B32A32_FLOAT: return VK_FORMAT_R32G32B32A32_SFLOAT;
                case Format::Format_R32G32B32A32_UINT: return VK_FORMAT_R32G32B32A32_UINT;
                case Format::Format_R32G32B32A32_SINT: return VK_FORMAT_R32G32B32A32_SINT;
                case Format::Format_R32G32B32_TYPELESS: return VK_FORMAT_R32G32B32_UINT;
                case Format::Format_R32G32B32_FLOAT: return VK_FORMAT_R32G32B32_SFLOAT;
                case Format::Format_R32G32B32_UINT: return VK_FORMAT_R32G32B32_UINT;
                case Format::Format_R32G32B32_SINT: return VK_FORMAT_R32G32B32_SINT;
                case Format::Format_R16G16B16A16_TYPELESS: return VK_FORMAT_R16G16B16A16_UNORM;
                case Format::Format_R16G16B16A16_FLOAT: return VK_FORMAT_R16G16B16A16_SFLOAT;
                case Format::Format_R16G16B16A16_UNORM: return VK_FORMAT_R16G16B16A16_UNORM;
                case Format::Format_R16G16B16A16_UINT: return VK_FORMAT_R16G16B16A16_UINT;
                case Format::Format_R16G16B16A16_SNORM: return VK_FORMAT_R16G16B16A16_SNORM;
                case Format::Format_R16G16B16A16_SINT: return VK_FORMAT_R16G16B16A16_SINT;
                case Format::Format_R32G32_TYPELESS: return VK_FORMAT_R32G32_UINT;
                case Format::Format_R32G32_FLOAT: return VK_FORMAT_R32G32_SFLOAT;
                case Format::Format_R32G32_UINT: return VK_FORMAT_R32G32_UINT;
                case Format::Format_R32G32_SINT: return VK_FORMAT_R32G32_SINT;
                case Format::Format_R32G8X24_TYPELESS: return VK_FORMAT_D32_SFLOAT_S8_UINT;
                case Format::Format_D32_FLOAT_S8X24_UINT: return VK_FORMAT_D32_SFLOAT_S8_UINT;
                case Format::Format_R32_FLOAT_X8X24_TYPELESS: return VK_FORMAT_D32_SFLOAT_S8_UINT;
                case Format::Format_X32_TYPELESS_G8X24_UINT: return VK_FORMAT_D32_SFLOAT_S8_UINT;
                case Format::Format_R10G10B10A2_TYPELESS: return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
                case Format::Format_R10G10B10A2_UNORM: return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
                case Format::Format_R10G10B10A2_UINT: return VK_FORMAT_A2R10G10B10_UINT_PACK32;
                case Format::Format_R11G11B10_FLOAT: return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
                case Format::Format_R8G8B8A8_TYPELESS: return VK_FORMAT_R8G8B8A8_UNORM;
                case Format::Format_R8G8B8A8_UNORM: return VK_FORMAT_R8G8B8A8_UNORM;
                case Format::Format_R8G8B8A8_UNORM_SRGB: return VK_FORMAT_R8G8B8A8_SRGB;
                case Format::Format_R8G8B8A8_UINT: return VK_FORMAT_R8G8B8A8_UINT;
                case Format::Format_R8G8B8A8_SNORM: return VK_FORMAT_R8G8B8A8_SNORM;
                case Format::Format_R8G8B8A8_SINT: return VK_FORMAT_R8G8B8A8_SINT;
                case Format::Format_R16G16_TYPELESS: return VK_FORMAT_R16G16_SFLOAT;
                case Format::Format_R16G16_FLOAT: return VK_FORMAT_R16G16_SFLOAT;
                case Format::Format_R16G16_UNORM: return VK_FORMAT_R16G16_UNORM;
                case Format::Format_R16G16_UINT: return VK_FORMAT_R16G16_UINT;
                case Format::Format_R16G16_SNORM: return VK_FORMAT_R16G16_SNORM;
                case Format::Format_R16G16_SINT: return VK_FORMAT_R16G16_SINT;
                case Format::Format_R32_TYPELESS: return VK_FORMAT_D32_SFLOAT;
                case Format::Format_D32_FLOAT: return VK_FORMAT_D32_SFLOAT;
                case Format::Format_R32_FLOAT: return VK_FORMAT_R32_SFLOAT;
                case Format::Format_R32_UINT: return VK_FORMAT_R32_UINT;
                case Format::Format_R32_SINT: return VK_FORMAT_R32_SINT;
                case Format::Format_R24G8_TYPELESS: return VK_FORMAT_D24_UNORM_S8_UINT;
                case Format::Format_D24_UNORM_S8_UINT: return VK_FORMAT_D24_UNORM_S8_UINT;
                case Format::Format_R24_UNORM_X8_TYPELESS: return VK_FORMAT_D24_UNORM_S8_UINT;
                case Format::Format_X24_TYPELESS_G8_UINT: return VK_FORMAT_D24_UNORM_S8_UINT;
                case Format::Format_R8G8_TYPELESS: return VK_FORMAT_R8G8_UNORM;
                case Format::Format_R8G8_UNORM: return VK_FORMAT_R8G8_UNORM;
                case Format::Format_R8G8_UINT: return VK_FORMAT_R8G8_UINT;
                case Format::Format_R8G8_SNORM: return VK_FORMAT_R8G8_SNORM;
                case Format::Format_R8G8_SINT: return VK_FORMAT_R8G8_SINT;
                case Format::Format_R16_TYPELESS: return VK_FORMAT_R16_UNORM;
                case Format::Format_R16_FLOAT: return VK_FORMAT_R16_SFLOAT;
                case Format::Format_D16_UNORM: return VK_FORMAT_R16_UNORM;
                case Format::Format_R16_UNORM: return VK_FORMAT_R16_UNORM;
                case Format::Format_R16_UINT: return VK_FORMAT_R16_UINT;
                case Format::Format_R16_SNORM: return VK_FORMAT_R16_SNORM;
                case Format::Format_R16_SINT: return VK_FORMAT_R16_SINT;
                case Format::Format_R8_TYPELESS: return VK_FORMAT_R8_UNORM;
                case Format::Format_R8_UNORM: return VK_FORMAT_R8_UNORM;
                case Format::Format_R8_UINT: return VK_FORMAT_R8_UINT;
                case Format::Format_R8_SNORM: return VK_FORMAT_R8_SNORM;
                case Format::Format_R8_SINT: return VK_FORMAT_R8_SINT;
                case Format::Format_A8_UNORM: return VK_FORMAT_R8_UNORM;
                case Format::Format_R1_UNORM: return VK_FORMAT_R8_UNORM;
                case Format::Format_R9G9B9E5_SHAREDEXP: return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
                case Format::Format_R8G8_B8G8_UNORM: return VK_FORMAT_B8G8R8A8_UNORM;
                case Format::Format_G8R8_G8B8_UNORM: return VK_FORMAT_B8G8R8A8_UNORM;
                case Format::Format_BC1_TYPELESS: return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
                case Format::Format_BC1_UNORM: return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
                case Format::Format_BC1_UNORM_SRGB: return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
                case Format::Format_BC2_TYPELESS: return VK_FORMAT_BC2_UNORM_BLOCK;
                case Format::Format_BC2_UNORM: return VK_FORMAT_BC2_UNORM_BLOCK;
                case Format::Format_BC2_UNORM_SRGB: return VK_FORMAT_BC2_SRGB_BLOCK;
                case Format::Format_BC3_TYPELESS: return VK_FORMAT_BC3_UNORM_BLOCK;
                case Format::Format_BC3_UNORM: return VK_FORMAT_BC3_UNORM_BLOCK;
                case Format::Format_BC3_UNORM_SRGB: return VK_FORMAT_BC3_SRGB_BLOCK;
                case Format::Format_BC4_TYPELESS: return VK_FORMAT_BC4_UNORM_BLOCK;
                case Format::Format_BC4_UNORM: return VK_FORMAT_BC4_UNORM_BLOCK;
                case Format::Format_BC4_SNORM: return VK_FORMAT_BC4_SNORM_BLOCK;
                case Format::Format_BC5_TYPELESS: return VK_FORMAT_BC5_UNORM_BLOCK;
                case Format::Format_BC5_UNORM: return VK_FORMAT_BC5_UNORM_BLOCK;
                case Format::Format_BC5_SNORM: return VK_FORMAT_BC5_SNORM_BLOCK;
                case Format::Format_B5G6R5_UNORM: return VK_FORMAT_R5G6B5_UNORM_PACK16;
                case Format::Format_B5G5R5A1_UNORM: return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
                case Format::Format_B8G8R8A8_UNORM: return VK_FORMAT_B8G8R8A8_UNORM;
                case Format::Format_B8G8R8X8_UNORM: return VK_FORMAT_B8G8R8A8_UNORM;
                case Format::Format_R10G10B10_XR_BIAS_A2_UNORM: return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
                case Format::Format_B8G8R8A8_TYPELESS: return VK_FORMAT_B8G8R8A8_SRGB;
                case Format::Format_B8G8R8A8_UNORM_SRGB: return VK_FORMAT_B8G8R8A8_SRGB;
                case Format::Format_B8G8R8X8_TYPELESS: return VK_FORMAT_B8G8R8A8_SRGB;
                case Format::Format_B8G8R8X8_UNORM_SRGB: return VK_FORMAT_B8G8R8A8_SRGB;
                case Format::Format_BC6H_TYPELESS: return VK_FORMAT_BC6H_UFLOAT_BLOCK;
                case Format::Format_BC6H_UF16: return VK_FORMAT_BC6H_UFLOAT_BLOCK;
                case Format::Format_BC6H_SF16: return VK_FORMAT_BC6H_SFLOAT_BLOCK;
                case Format::Format_BC7_TYPELESS: return VK_FORMAT_BC7_UNORM_BLOCK;
                case Format::Format_BC7_UNORM: return VK_FORMAT_BC7_UNORM_BLOCK;
                case Format::Format_BC7_UNORM_SRGB: return VK_FORMAT_BC7_SRGB_BLOCK;
                case Format::Format_AYUV: return VK_FORMAT_UNDEFINED;
                case Format::Format_Y410: return VK_FORMAT_UNDEFINED;
                case Format::Format_Y416: return VK_FORMAT_UNDEFINED;
                case Format::Format_NV12: return VK_FORMAT_UNDEFINED;
                case Format::Format_P010: return VK_FORMAT_UNDEFINED;
                case Format::Format_P016: return VK_FORMAT_UNDEFINED;
                case Format::Format_420_OPAQUE: return VK_FORMAT_UNDEFINED;
                case Format::Format_YUY2: return VK_FORMAT_UNDEFINED;
                case Format::Format_Y210: return VK_FORMAT_UNDEFINED;
                case Format::Format_Y216: return VK_FORMAT_UNDEFINED;
                case Format::Format_NV11: return VK_FORMAT_UNDEFINED;
                case Format::Format_AI44: return VK_FORMAT_UNDEFINED;
                case Format::Format_IA44: return VK_FORMAT_UNDEFINED;
                case Format::Format_P8: return VK_FORMAT_UNDEFINED;
                case Format::Format_A8P8: return VK_FORMAT_UNDEFINED;
                case Format::Format_B4G4R4A4_UNORM: return VK_FORMAT_B4G4R4A4_UNORM_PACK16;
                case Format::Format_P208: return VK_FORMAT_UNDEFINED;
                case Format::Format_V208: return VK_FORMAT_UNDEFINED;
                case Format::Format_V408: return VK_FORMAT_UNDEFINED;
                default: return VK_FORMAT_UNDEFINED;
            }
        }

        VkPrimitiveTopology vulkanPrimitiveTopologyType(PrimitiveTopologyType type)
        {
            switch (type)
            {
                case PrimitiveTopologyType::Undefined: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
                case PrimitiveTopologyType::PointList: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
                case PrimitiveTopologyType::LineList: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
                case PrimitiveTopologyType::TriangleList: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                case PrimitiveTopologyType::PatchList1: return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
            }
            return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        }

        VkVertexInputRate vulkanInputClassification(InputClassification cls)
        {
            switch (cls)
            {
                case InputClassification::PerVertexData: return VK_VERTEX_INPUT_RATE_VERTEX;
                case InputClassification::PerInstanceData: return VK_VERTEX_INPUT_RATE_INSTANCE;
            }
            return VK_VERTEX_INPUT_RATE_VERTEX;
        }

        VkPolygonMode vulkanFillMode(FillMode mode)
        {
            switch (mode)
            {
                case FillMode::Wireframe: return VK_POLYGON_MODE_LINE;
                case FillMode::Solid: return VK_POLYGON_MODE_FILL;
                case FillMode::Point: return VK_POLYGON_MODE_POINT;
            }
            return VK_POLYGON_MODE_LINE;
        }

        VkCullModeFlags vulkanCullMode(CullMode mode)
        {
            switch (mode)
            {
                case CullMode::None: return VK_CULL_MODE_NONE;
                case CullMode::Front: return VK_CULL_MODE_FRONT_BIT;
                case CullMode::Back: return VK_CULL_MODE_BACK_BIT;
            }
            return VK_CULL_MODE_NONE;
        }

        VkBufferUsageFlags vulkanBufferUsageFlags(BufferUsageFlags flags)
        {
            bool src = (flags & static_cast<uint32_t>(BufferUsageFlagBits::Src)) > 0;
            bool dst = (flags & static_cast<uint32_t>(BufferUsageFlagBits::Dst)) > 0;
            bool uniformTexel = (flags & static_cast<uint32_t>(BufferUsageFlagBits::UniformTexel)) > 0;
            bool storageTexel = (flags & static_cast<uint32_t>(BufferUsageFlagBits::StorageTexel)) > 0;
            bool uniform = (flags & static_cast<uint32_t>(BufferUsageFlagBits::Uniform)) > 0;
            bool storage = (flags & static_cast<uint32_t>(BufferUsageFlagBits::Storage)) > 0;
            bool index = (flags & static_cast<uint32_t>(BufferUsageFlagBits::Index)) > 0;
            bool vertex = (flags & static_cast<uint32_t>(BufferUsageFlagBits::Vertex)) > 0;
            bool indirect = (flags & static_cast<uint32_t>(BufferUsageFlagBits::Indirect)) > 0;
            VkBufferUsageFlags returnFlags{};
            returnFlags |= src ? VK_BUFFER_USAGE_TRANSFER_SRC_BIT : 0;
            returnFlags |= dst ? VK_BUFFER_USAGE_TRANSFER_DST_BIT : 0;
            returnFlags |= uniformTexel ? VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT : 0;
            returnFlags |= storageTexel ? VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT : 0;
            returnFlags |= uniform ? VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT : 0;
            returnFlags |= storage ? VK_BUFFER_USAGE_STORAGE_BUFFER_BIT : 0;
            returnFlags |= index ? VK_BUFFER_USAGE_INDEX_BUFFER_BIT : 0;
            returnFlags |= vertex ? VK_BUFFER_USAGE_VERTEX_BUFFER_BIT : 0;
            returnFlags |= indirect ? VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT : 0;
            return returnFlags;
        }

        VkMemoryPropertyFlags vulkanMemoryFlags(BufferMemoryFlags flags)
        {
            bool deviceLocal = (flags & static_cast<uint32_t>(BufferMemoryFlagBits::DeviceLocal)) > 0;
            bool hostVisible = (flags & static_cast<uint32_t>(BufferMemoryFlagBits::HostVisible)) > 0;
            bool hostCoherent = (flags & static_cast<uint32_t>(BufferMemoryFlagBits::HostCoherent)) > 0;
            bool hostCached = (flags & static_cast<uint32_t>(BufferMemoryFlagBits::HostCached)) > 0;
            bool lazy = (flags & static_cast<uint32_t>(BufferMemoryFlagBits::LazilyAllocated)) > 0;
            VkMemoryPropertyFlags returnFlags{};
            returnFlags |= deviceLocal ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT : 0;
            returnFlags |= hostVisible ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT : 0;
            returnFlags |= hostCoherent ? VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : 0;
            returnFlags |= hostCached ? VK_MEMORY_PROPERTY_HOST_CACHED_BIT : 0;
            returnFlags |= lazy ? VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT : 0;
            return returnFlags;
        }

        VkDescriptorType vulkanDescriptorType(DescriptorType type)
        {
            switch (type)
            {
                case DescriptorType::CBV_SRV_UAV: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                case DescriptorType::SAMPLER: return VK_DESCRIPTOR_TYPE_SAMPLER;
                case DescriptorType::RTV: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                case DescriptorType::DSV: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                case DescriptorType::CombinedImageSampler: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                case DescriptorType::SampledImage: return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                case DescriptorType::StorageImage: return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                case DescriptorType::UniformTexelBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
                case DescriptorType::StorageTexelBuffer: return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
                case DescriptorType::UniformBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                case DescriptorType::StorageBuffer: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                case DescriptorType::UniformBufferDynamic: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
                case DescriptorType::StorageBufferDynamic: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
                case DescriptorType::InputAttachment: return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
            }
            return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        }

        VkImageTiling vulkanImageTiling(ImageTiling tiling)
        {
            switch (tiling)
            {
                case ImageTiling::Optimal: return VK_IMAGE_TILING_OPTIMAL;
                case ImageTiling::Linear: return VK_IMAGE_TILING_LINEAR;
            }
            return VK_IMAGE_TILING_OPTIMAL;
        }

        /*VkImageLayout vulkanImageLayout(ImageLayout layout)
        {
            switch (layout)
            {
                case ImageLayout::Undefined: return VK_IMAGE_LAYOUT_UNDEFINED;
                case ImageLayout::General: return VK_IMAGE_LAYOUT_GENERAL;
                case ImageLayout::ColorAttachmentOptimal: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                case ImageLayout::DepthStencilAttachmentOptimal: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                case ImageLayout::DepthStencilReadOnlyOptimal: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
                case ImageLayout::ShaderReadOnlyOptimal: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                case ImageLayout::TransferSrcOptimal: return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                case ImageLayout::TransferDstOptimal: return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                case ImageLayout::Preinitialized: return VK_IMAGE_LAYOUT_PREINITIALIZED;
                case ImageLayout::PresentSrc: return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            }
            return VK_IMAGE_LAYOUT_UNDEFINED;
        }*/

        VkShaderStageFlags vulkanShaderVisibility(ShaderVisibility flags)
        {
            if (flags == 0x7FFFFFFF)
                return VK_SHADER_STAGE_ALL;
            if (flags == 0x0000001F)
                return VK_SHADER_STAGE_ALL_GRAPHICS;

            bool vertex = (flags & static_cast<uint32_t>(ShaderVisibilityBits::Vertex)) > 0;
            bool tesselationControl = (flags & static_cast<uint32_t>(ShaderVisibilityBits::Hull)) > 0;
            bool tesselationEval = (flags & static_cast<uint32_t>(ShaderVisibilityBits::Domain)) > 0;
            bool geometry = (flags & static_cast<uint32_t>(ShaderVisibilityBits::Geometry)) > 0;
            bool fragment = (flags & static_cast<uint32_t>(ShaderVisibilityBits::Pixel)) > 0;
            bool compute = (flags & static_cast<uint32_t>(ShaderVisibilityBits::Compute)) > 0;
            VkShaderStageFlags returnFlags{};
            returnFlags |= vertex ? VK_SHADER_STAGE_VERTEX_BIT : 0;
            returnFlags |= tesselationControl ? VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT : 0;
            returnFlags |= tesselationEval ? VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT : 0;
            returnFlags |= geometry ? VK_SHADER_STAGE_GEOMETRY_BIT : 0;
            returnFlags |= fragment ? VK_SHADER_STAGE_FRAGMENT_BIT : 0;
            returnFlags |= compute ? VK_SHADER_STAGE_COMPUTE_BIT : 0;
            return returnFlags;
        }

        VkImageAspectFlags vulkanImageAspectFlags(ImageAspectFlags flags)
        {
            bool color = (flags & static_cast<uint32_t>(ImageAspectFlagBits::Color)) > 0;
            bool depth = (flags & static_cast<uint32_t>(ImageAspectFlagBits::Depth)) > 0;
            bool stencil = (flags & static_cast<uint32_t>(ImageAspectFlagBits::Stencil)) > 0;
            bool metadata = (flags & static_cast<uint32_t>(ImageAspectFlagBits::Metadata)) > 0;
            VkImageAspectFlags returnFlags{};
            returnFlags |= color ? VK_IMAGE_ASPECT_COLOR_BIT : 0;
            returnFlags |= depth ? VK_IMAGE_ASPECT_DEPTH_BIT : 0;
            returnFlags |= stencil ? VK_IMAGE_ASPECT_STENCIL_BIT : 0;
            returnFlags |= metadata ? VK_IMAGE_ASPECT_METADATA_BIT : 0;
            return returnFlags;
        }

        VkImageType vulkanDimension(ResourceDimension dimension)
        {
            switch (dimension)
            {
                case ResourceDimension::Unknown: return VkImageType::VK_IMAGE_TYPE_1D;
                case ResourceDimension::Texture1D: return VkImageType::VK_IMAGE_TYPE_1D;
                case ResourceDimension::Texture2D: return VkImageType::VK_IMAGE_TYPE_2D;
                case ResourceDimension::Texture3D: return VkImageType::VK_IMAGE_TYPE_3D;
                case ResourceDimension::Texture1DArray: return VkImageType::VK_IMAGE_TYPE_1D;
                case ResourceDimension::Texture2DArray: return VkImageType::VK_IMAGE_TYPE_2D;
                case ResourceDimension::TextureCubemap: return VkImageType::VK_IMAGE_TYPE_3D;
            }
            return VkImageType::VK_IMAGE_TYPE_1D;
        }

        VkImageAspectFlags vulkanFormatAspects(Format format)
        {
            switch (vulkanFormat(format))
            {
            case VK_FORMAT_UNDEFINED: return 0;

            case VK_FORMAT_S8_UINT: return VK_IMAGE_ASPECT_STENCIL_BIT;

            case VK_FORMAT_D16_UNORM_S8_UINT:
            case VK_FORMAT_D24_UNORM_S8_UINT:
            case VK_FORMAT_D32_SFLOAT_S8_UINT: return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

            case VK_FORMAT_D16_UNORM:
            case VK_FORMAT_X8_D24_UNORM_PACK32:
            case VK_FORMAT_D32_SFLOAT: return VK_IMAGE_ASPECT_DEPTH_BIT;

            default: return VK_IMAGE_ASPECT_COLOR_BIT;
            }
            return 0;
        }

        VkImageSubresourceRange vulkanSubResource(const Texture& texture, SubResource subResources)
        {
            return{
                vulkanFormatAspects(texture.format()), // VK_IMAGE_ASPECT_METADATA_BIT
                static_cast<uint32_t>(subResources.firstMipLevel),
                static_cast<uint32_t>(subResources.mipCount == AllMipLevels ? texture.mipLevels() : subResources.mipCount),
                static_cast<uint32_t>(subResources.firstArraySlice),
                static_cast<uint32_t>(subResources.arraySliceCount == AllArraySlices ? texture.arraySlices() : subResources.arraySliceCount)
            };
        }

        VkImageViewType vulkanViewType(ResourceDimension dimension)
        {
            switch (dimension)
            {
                case ResourceDimension::Texture1D: return VK_IMAGE_VIEW_TYPE_1D;
                case ResourceDimension::Texture2D: return VK_IMAGE_VIEW_TYPE_2D;
                case ResourceDimension::Texture3D: return VK_IMAGE_VIEW_TYPE_3D;
                case ResourceDimension::Texture1DArray: return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
                case ResourceDimension::Texture2DArray: return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
                case ResourceDimension::TextureCubemap: return VK_IMAGE_VIEW_TYPE_CUBE;
                case ResourceDimension::TextureCubemapArray: return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
            }
            return VK_IMAGE_VIEW_TYPE_1D;
        }

        VkImageLayout vulkanResourceStates(ResourceState state)
        {
            switch (state)
            {
            case ResourceState::Common: return VK_IMAGE_LAYOUT_GENERAL;
            case ResourceState::VertexAndConstantBuffer: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            case ResourceState::IndexBuffer: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            case ResourceState::RenderTarget: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            case ResourceState::UnorderedAccess: return VK_IMAGE_LAYOUT_GENERAL;
            case ResourceState::DepthWrite: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            case ResourceState::DepthRead: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            case ResourceState::NonPixelShaderResource: return VK_IMAGE_LAYOUT_GENERAL;
            case ResourceState::PixelShaderResource: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            case ResourceState::StreamOut: return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            case ResourceState::IndirectArgument: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            case ResourceState::CopyDest: return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            case ResourceState::CopySource: return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            case ResourceState::ResolveDest: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            case ResourceState::ResolveSource: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            case ResourceState::GenericRead: return VK_IMAGE_LAYOUT_GENERAL;
            case ResourceState::Present: return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            case ResourceState::Predication: return VK_IMAGE_LAYOUT_GENERAL;
            default: return VK_IMAGE_LAYOUT_GENERAL;
            }
        }

        // ---------------------------------------------------------------

        ShaderVisibility fromVulkanShaderVisibility(VkShaderStageFlags flags)
        {
            if (flags == 0x7FFFFFFF)
                return static_cast<ShaderVisibility>(ShaderVisibilityBits::All);
            if (flags == 0x0000001F)
                return static_cast<ShaderVisibility>(ShaderVisibilityBits::AllGraphics);

            bool vertex = (flags & VK_SHADER_STAGE_VERTEX_BIT) > 0;
            bool tesselationControl = (flags & VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT) > 0;
            bool tesselationEval = (flags & VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT) > 0;
            bool geometry = (flags & VK_SHADER_STAGE_GEOMETRY_BIT) > 0;
            bool fragment = (flags & VK_SHADER_STAGE_FRAGMENT_BIT) > 0;
            bool compute = (flags & VK_SHADER_STAGE_COMPUTE_BIT) > 0;
            ShaderVisibility returnFlags{};
            returnFlags |= vertex ? static_cast<uint32_t>(ShaderVisibilityBits::Vertex) : 0;
            returnFlags |= tesselationControl ? static_cast<uint32_t>(ShaderVisibilityBits::Hull) : 0;
            returnFlags |= tesselationEval ? static_cast<uint32_t>(ShaderVisibilityBits::Domain) : 0;
            returnFlags |= geometry ? static_cast<uint32_t>(ShaderVisibilityBits::Geometry) : 0;
            returnFlags |= fragment ? static_cast<uint32_t>(ShaderVisibilityBits::Pixel) : 0;
            returnFlags |= compute ? static_cast<uint32_t>(ShaderVisibilityBits::Compute) : 0;
            return returnFlags;
        }

        CommandListType fromVulkanCommandListType(VkQueueFlags type)
        {
            switch (type)
            {
                case VK_QUEUE_GRAPHICS_BIT: return CommandListType::Graphics;
                case VK_QUEUE_COMPUTE_BIT: return CommandListType::Compute;
                case VK_QUEUE_TRANSFER_BIT: return CommandListType::Transfer;
            }
            return CommandListType::Graphics;
        }

        Format fromVulkanFormat(VkFormat format)
        {
            switch(format)
            {
                case VK_FORMAT_UNDEFINED: return Format::Format_UNKNOWN;
                case VK_FORMAT_R32G32B32A32_SFLOAT: return Format::Format_R32G32B32A32_FLOAT;
                case VK_FORMAT_R32G32B32A32_UINT: return Format::Format_R32G32B32A32_UINT;
                case VK_FORMAT_R32G32B32A32_SINT: return Format::Format_R32G32B32A32_SINT;
                case VK_FORMAT_R32G32B32_SFLOAT: return Format::Format_R32G32B32_FLOAT;
                case VK_FORMAT_R32G32B32_UINT: return Format::Format_R32G32B32_UINT;
                case VK_FORMAT_R32G32B32_SINT: return Format::Format_R32G32B32_SINT;
                case VK_FORMAT_R16G16B16A16_SFLOAT: return Format::Format_R16G16B16A16_FLOAT;
                case VK_FORMAT_R16G16B16A16_UNORM: return Format::Format_R16G16B16A16_UNORM;
                case VK_FORMAT_R16G16B16A16_UINT: return Format::Format_R16G16B16A16_UINT;
                case VK_FORMAT_R16G16B16A16_SNORM: return Format::Format_R16G16B16A16_SNORM;
                case VK_FORMAT_R16G16B16A16_SINT: return Format::Format_R16G16B16A16_SINT;
                case VK_FORMAT_R32G32_SFLOAT: return Format::Format_R32G32_FLOAT;
                case VK_FORMAT_R32G32_UINT: return Format::Format_R32G32_UINT;
                case VK_FORMAT_R32G32_SINT: return Format::Format_R32G32_SINT;
                case VK_FORMAT_D32_SFLOAT_S8_UINT: return Format::Format_D32_FLOAT_S8X24_UINT;
                case VK_FORMAT_A2R10G10B10_UNORM_PACK32: return Format::Format_R10G10B10A2_UNORM;
                case VK_FORMAT_A2R10G10B10_UINT_PACK32: return Format::Format_R10G10B10A2_UINT;
                case VK_FORMAT_B10G11R11_UFLOAT_PACK32: return Format::Format_R11G11B10_FLOAT;
                case VK_FORMAT_R8G8B8A8_UNORM: return Format::Format_R8G8B8A8_UNORM;
                case VK_FORMAT_R8G8B8A8_SRGB: return Format::Format_R8G8B8A8_UNORM_SRGB;
                case VK_FORMAT_R8G8B8A8_UINT: return Format::Format_R8G8B8A8_UINT;
                case VK_FORMAT_R8G8B8A8_SNORM: return Format::Format_R8G8B8A8_SNORM;
                case VK_FORMAT_R8G8B8A8_SINT: return Format::Format_R8G8B8A8_SINT;
                case VK_FORMAT_R16G16_SFLOAT: return Format::Format_R16G16_FLOAT;
                case VK_FORMAT_R16G16_UNORM: return Format::Format_R16G16_UNORM;
                case VK_FORMAT_R16G16_UINT: return Format::Format_R16G16_UINT;
                case VK_FORMAT_R16G16_SNORM: return Format::Format_R16G16_SNORM;
                case VK_FORMAT_R16G16_SINT: return Format::Format_R16G16_SINT;
                case VK_FORMAT_D32_SFLOAT: return Format::Format_D32_FLOAT;
                case VK_FORMAT_R32_SFLOAT: return Format::Format_R32_FLOAT;
                case VK_FORMAT_R32_UINT: return Format::Format_R32_UINT;
                case VK_FORMAT_R32_SINT: return Format::Format_R32_SINT;
                case VK_FORMAT_D24_UNORM_S8_UINT: return Format::Format_D24_UNORM_S8_UINT;
                case VK_FORMAT_R8G8_UNORM: return Format::Format_R8G8_UNORM;
                case VK_FORMAT_R8G8_UINT: return Format::Format_R8G8_UINT;
                case VK_FORMAT_R8G8_SNORM: return Format::Format_R8G8_SNORM;
                case VK_FORMAT_R8G8_SINT: return Format::Format_R8G8_SINT;
                case VK_FORMAT_R16_SFLOAT: return Format::Format_R16_FLOAT;
                case VK_FORMAT_R16_UNORM: return Format::Format_R16_UNORM;
                case VK_FORMAT_R16_UINT: return Format::Format_R16_UINT;
                case VK_FORMAT_R16_SNORM: return Format::Format_R16_SNORM;
                case VK_FORMAT_R16_SINT: return Format::Format_R16_SINT;
                case VK_FORMAT_R8_UNORM: return Format::Format_R8_UNORM;
                case VK_FORMAT_R8_UINT: return Format::Format_R8_UINT;
                case VK_FORMAT_R8_SNORM: return Format::Format_R8_SNORM;
                case VK_FORMAT_R8_SINT: return Format::Format_R8_SINT;
                case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: return Format::Format_R9G9B9E5_SHAREDEXP;
                case VK_FORMAT_BC1_RGBA_UNORM_BLOCK: return Format::Format_BC1_UNORM;
                case VK_FORMAT_BC1_RGBA_SRGB_BLOCK: return Format::Format_BC1_UNORM_SRGB;
                case VK_FORMAT_BC2_UNORM_BLOCK: return Format::Format_BC2_UNORM;
                case VK_FORMAT_BC2_SRGB_BLOCK: return Format::Format_BC2_UNORM_SRGB;
                case VK_FORMAT_BC3_UNORM_BLOCK: return Format::Format_BC3_UNORM;
                case VK_FORMAT_BC3_SRGB_BLOCK: return Format::Format_BC3_UNORM_SRGB;
                case VK_FORMAT_BC4_UNORM_BLOCK: return Format::Format_BC4_UNORM;
                case VK_FORMAT_BC4_SNORM_BLOCK: return Format::Format_BC4_SNORM;
                case VK_FORMAT_BC5_UNORM_BLOCK: return Format::Format_BC5_UNORM;
                case VK_FORMAT_BC5_SNORM_BLOCK: return Format::Format_BC5_SNORM;
                case VK_FORMAT_R5G6B5_UNORM_PACK16: return Format::Format_B5G6R5_UNORM;
                case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return Format::Format_B5G5R5A1_UNORM;
                case VK_FORMAT_B8G8R8A8_UNORM: return Format::Format_B8G8R8A8_UNORM;
                case VK_FORMAT_B8G8R8A8_SRGB: return Format::Format_B8G8R8A8_UNORM_SRGB;
                case VK_FORMAT_BC6H_UFLOAT_BLOCK: return Format::Format_BC6H_UF16;
                case VK_FORMAT_BC6H_SFLOAT_BLOCK: return Format::Format_BC6H_SF16;
                case VK_FORMAT_BC7_UNORM_BLOCK: return Format::Format_BC7_UNORM;
                case VK_FORMAT_BC7_SRGB_BLOCK: return Format::Format_BC7_UNORM_SRGB;
                case VK_FORMAT_B4G4R4A4_UNORM_PACK16: return Format::Format_B4G4R4A4_UNORM;

                // these are the formats that we do not currently handle
                case VK_FORMAT_R4G4_UNORM_PACK8: return Format::Format_UNKNOWN;
                case VK_FORMAT_R4G4B4A4_UNORM_PACK16: return Format::Format_UNKNOWN;
                case VK_FORMAT_B5G6R5_UNORM_PACK16: return Format::Format_UNKNOWN;
                case VK_FORMAT_B5G5R5A1_UNORM_PACK16: return Format::Format_UNKNOWN;
                case VK_FORMAT_A1R5G5B5_UNORM_PACK16: return Format::Format_UNKNOWN;
                case VK_FORMAT_R8_USCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_R8_SSCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_R8_SRGB: return Format::Format_UNKNOWN;
                case VK_FORMAT_R8G8_USCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_R8G8_SSCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_R8G8_SRGB: return Format::Format_UNKNOWN;
                case VK_FORMAT_R8G8B8_UNORM: return Format::Format_UNKNOWN;
                case VK_FORMAT_R8G8B8_SNORM: return Format::Format_UNKNOWN;
                case VK_FORMAT_R8G8B8_USCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_R8G8B8_SSCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_R8G8B8_UINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_R8G8B8_SINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_R8G8B8_SRGB: return Format::Format_UNKNOWN;
                case VK_FORMAT_B8G8R8_UNORM: return Format::Format_UNKNOWN;
                case VK_FORMAT_B8G8R8_SNORM: return Format::Format_UNKNOWN;
                case VK_FORMAT_B8G8R8_USCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_B8G8R8_SSCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_B8G8R8_UINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_B8G8R8_SINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_B8G8R8_SRGB: return Format::Format_UNKNOWN;
                case VK_FORMAT_R8G8B8A8_USCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_R8G8B8A8_SSCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_B8G8R8A8_SNORM: return Format::Format_UNKNOWN;
                case VK_FORMAT_B8G8R8A8_USCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_B8G8R8A8_SSCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_B8G8R8A8_UINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_B8G8R8A8_SINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_A8B8G8R8_UNORM_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_A8B8G8R8_SNORM_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_A8B8G8R8_USCALED_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_A8B8G8R8_SSCALED_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_A8B8G8R8_UINT_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_A8B8G8R8_SINT_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_A8B8G8R8_SRGB_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_A2R10G10B10_SNORM_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_A2R10G10B10_USCALED_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_A2R10G10B10_SINT_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_A2B10G10R10_UNORM_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_A2B10G10R10_SNORM_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_A2B10G10R10_USCALED_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_A2B10G10R10_UINT_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_A2B10G10R10_SINT_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_R16_USCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_R16_SSCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_R16G16_USCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_R16G16_SSCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_R16G16B16_UNORM: return Format::Format_UNKNOWN;
                case VK_FORMAT_R16G16B16_SNORM: return Format::Format_UNKNOWN;
                case VK_FORMAT_R16G16B16_USCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_R16G16B16_SSCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_R16G16B16_UINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_R16G16B16_SINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_R16G16B16_SFLOAT: return Format::Format_UNKNOWN;
                case VK_FORMAT_R16G16B16A16_USCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_R16G16B16A16_SSCALED: return Format::Format_UNKNOWN;
                case VK_FORMAT_R64_UINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_R64_SINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_R64_SFLOAT: return Format::Format_UNKNOWN;
                case VK_FORMAT_R64G64_UINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_R64G64_SINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_R64G64_SFLOAT: return Format::Format_UNKNOWN;
                case VK_FORMAT_R64G64B64_UINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_R64G64B64_SINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_R64G64B64_SFLOAT: return Format::Format_UNKNOWN;
                case VK_FORMAT_R64G64B64A64_UINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_R64G64B64A64_SINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_R64G64B64A64_SFLOAT: return Format::Format_UNKNOWN;
                case VK_FORMAT_D16_UNORM: return Format::Format_UNKNOWN;
                case VK_FORMAT_X8_D24_UNORM_PACK32: return Format::Format_UNKNOWN;
                case VK_FORMAT_S8_UINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_D16_UNORM_S8_UINT: return Format::Format_UNKNOWN;
                case VK_FORMAT_BC1_RGB_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_BC1_RGB_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_EAC_R11_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_EAC_R11_SNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_EAC_R11G11_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_EAC_R11G11_SNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_4x4_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_4x4_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_5x4_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_5x4_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_5x5_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_5x5_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_6x5_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_6x5_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_6x6_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_6x6_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_8x5_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_8x5_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_8x6_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_8x6_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_8x8_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_8x8_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_10x5_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_10x5_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_10x6_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_10x6_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_10x8_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_10x8_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_10x10_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_10x10_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_12x10_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_12x10_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_12x12_UNORM_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_ASTC_12x12_SRGB_BLOCK: return Format::Format_UNKNOWN;
                case VK_FORMAT_RANGE_SIZE: return Format::Format_UNKNOWN;
                case VK_FORMAT_MAX_ENUM: return Format::Format_UNKNOWN;
                default: return Format::Format_UNKNOWN;
            }
        }

        PrimitiveTopologyType fromVulkanPrimitiveTopologyType(VkPrimitiveTopology type)
        {
            switch (type)
            {
                case VK_PRIMITIVE_TOPOLOGY_POINT_LIST: return PrimitiveTopologyType::PointList;
                case VK_PRIMITIVE_TOPOLOGY_LINE_LIST: return PrimitiveTopologyType::LineList;
                case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST: return PrimitiveTopologyType::TriangleList;
                case VK_PRIMITIVE_TOPOLOGY_PATCH_LIST: return PrimitiveTopologyType::PatchList1;

                // we don't know what to do with these
                case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP: return PrimitiveTopologyType::LineStrip;
                case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP: return PrimitiveTopologyType::TriangleList;
                case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN: return PrimitiveTopologyType::TriangleList;
                case VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY: return PrimitiveTopologyType::LineList;
                case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY: return PrimitiveTopologyType::LineList;
                case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY: return PrimitiveTopologyType::TriangleList;
                case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY: return PrimitiveTopologyType::TriangleList;
                case VK_PRIMITIVE_TOPOLOGY_RANGE_SIZE: return PrimitiveTopologyType::PointList;
                case VK_PRIMITIVE_TOPOLOGY_MAX_ENUM: return PrimitiveTopologyType::PointList;
            }
            return PrimitiveTopologyType::PointList;
        }

        InputClassification fromVulkanInputClassification(VkVertexInputRate cls)
        {
            switch (cls)
            {
                case VK_VERTEX_INPUT_RATE_VERTEX: return InputClassification::PerVertexData;
                case VK_VERTEX_INPUT_RATE_INSTANCE: return InputClassification::PerInstanceData;
                case VK_VERTEX_INPUT_RATE_RANGE_SIZE: return InputClassification::PerVertexData;
                case VK_VERTEX_INPUT_RATE_MAX_ENUM: return InputClassification::PerVertexData;
            }
            return InputClassification::PerVertexData;
        }

        FillMode fromVulkanFillMode(VkPolygonMode mode)
        {
            switch (mode)
            {
                case VK_POLYGON_MODE_LINE: return FillMode::Wireframe;
                case VK_POLYGON_MODE_FILL: return FillMode::Solid;
                case VK_POLYGON_MODE_POINT: return FillMode::Point;

                // we don't know what to do with these
                case VK_POLYGON_MODE_RANGE_SIZE: return FillMode::Point;
                case VK_POLYGON_MODE_MAX_ENUM: return FillMode::Point;
            }
            return FillMode::Solid;
        }

        CullMode fromVulkanCullMode(VkCullModeFlags mode)
        {
            switch (mode)
            {
                case VK_CULL_MODE_NONE: return CullMode::None;
                case VK_CULL_MODE_FRONT_BIT: return CullMode::Front;
                case VK_CULL_MODE_BACK_BIT: return CullMode::Back;
            }
            return CullMode::None;
        }

        BufferUsageFlags fromVulkanBufferUsageFlags(VkBufferUsageFlags flags)
        {
            bool src = (flags & VK_BUFFER_USAGE_TRANSFER_SRC_BIT) > 0;
            bool dst = (flags & VK_BUFFER_USAGE_TRANSFER_DST_BIT) > 0;
            bool uniformTexel = (flags & VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT) > 0;
            bool storageTexel = (flags & VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT) > 0;
            bool uniform = (flags & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) > 0;
            bool storage = (flags & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT) > 0;
            bool index = (flags & VK_BUFFER_USAGE_INDEX_BUFFER_BIT) > 0;
            bool vertex = (flags & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) > 0;
            bool indirect = (flags & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT) > 0;
            BufferUsageFlags returnFlags{};
            returnFlags |= src ? static_cast<uint32_t>(BufferUsageFlagBits::Src) : 0;
            returnFlags |= dst ? static_cast<uint32_t>(BufferUsageFlagBits::Dst) : 0;
            returnFlags |= uniformTexel ? static_cast<uint32_t>(BufferUsageFlagBits::UniformTexel) : 0;
            returnFlags |= storageTexel ? static_cast<uint32_t>(BufferUsageFlagBits::StorageTexel) : 0;
            returnFlags |= uniform ? static_cast<uint32_t>(BufferUsageFlagBits::Uniform) : 0;
            returnFlags |= storage ? static_cast<uint32_t>(BufferUsageFlagBits::Storage) : 0;
            returnFlags |= index ? static_cast<uint32_t>(BufferUsageFlagBits::Index) : 0;
            returnFlags |= vertex ? static_cast<uint32_t>(BufferUsageFlagBits::Vertex) : 0;
            returnFlags |= indirect ? static_cast<uint32_t>(BufferUsageFlagBits::Indirect) : 0;
            return returnFlags;
        }

        BufferMemoryFlags fromVulkanMemoryFlags(VkMemoryPropertyFlags flags)
        {
            bool deviceLocal = (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) > 0;
            bool hostVisible = (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) > 0;
            bool hostCoherent = (flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) > 0;
            bool hostCached = (flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) > 0;
            bool lazy = (flags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) > 0;
            BufferMemoryFlags returnFlags{};
            returnFlags |= deviceLocal ? static_cast<uint32_t>(BufferMemoryFlagBits::DeviceLocal) : 0;
            returnFlags |= hostVisible ? static_cast<uint32_t>(BufferMemoryFlagBits::HostVisible) : 0;
            returnFlags |= hostCoherent ? static_cast<uint32_t>(BufferMemoryFlagBits::HostCoherent) : 0;
            returnFlags |= hostCached ? static_cast<uint32_t>(BufferMemoryFlagBits::HostCached) : 0;
            returnFlags |= lazy ? static_cast<uint32_t>(BufferMemoryFlagBits::LazilyAllocated) : 0;
            return returnFlags;
        }

        DescriptorType fromVulkanDescriptorType(VkDescriptorType type)
        {
            switch (type)
            {
                case VK_DESCRIPTOR_TYPE_SAMPLER: return DescriptorType::SAMPLER;
                case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: return DescriptorType::CombinedImageSampler;
                case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE: return DescriptorType::SampledImage;
                case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE: return DescriptorType::StorageImage;
                case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER: return DescriptorType::UniformTexelBuffer;
                case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: return DescriptorType::StorageTexelBuffer;
                case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER: return DescriptorType::UniformBuffer;
                case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER: return DescriptorType::StorageBuffer;
                case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC: return DescriptorType::UniformBufferDynamic;
                case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: return DescriptorType::StorageBufferDynamic;
                case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: return DescriptorType::InputAttachment;
                case VK_DESCRIPTOR_TYPE_RANGE_SIZE: return DescriptorType::SAMPLER;
                case VK_DESCRIPTOR_TYPE_MAX_ENUM: return DescriptorType::SAMPLER;
            }
            return DescriptorType::UniformBuffer;
        }

        ImageTiling fromVulkanImageTiling(VkImageTiling tiling)
        {
            switch (tiling)
            {
                case VK_IMAGE_TILING_OPTIMAL: return ImageTiling::Optimal;
                case VK_IMAGE_TILING_LINEAR: return ImageTiling::Linear;
                case VK_IMAGE_TILING_RANGE_SIZE: return ImageTiling::Optimal;
                case VK_IMAGE_TILING_MAX_ENUM: return ImageTiling::Optimal;
            }
            return ImageTiling::Optimal;
        }

        /*ImageLayout fromVulkanImageLayout(VkImageLayout layout)
        {
            switch (layout)
            {
                case VK_IMAGE_LAYOUT_UNDEFINED: return ImageLayout::Undefined;
                case VK_IMAGE_LAYOUT_GENERAL: return ImageLayout::General;
                case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: return ImageLayout::ColorAttachmentOptimal;
                case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: return ImageLayout::DepthStencilAttachmentOptimal;
                case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL: return ImageLayout::DepthStencilReadOnlyOptimal;
                case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: return ImageLayout::ShaderReadOnlyOptimal;
                case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL: return ImageLayout::TransferSrcOptimal;
                case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: return ImageLayout::TransferDstOptimal;
                case VK_IMAGE_LAYOUT_PREINITIALIZED: return ImageLayout::Preinitialized;
                case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: return ImageLayout::PresentSrc;
                case VK_IMAGE_LAYOUT_RANGE_SIZE: return ImageLayout::Undefined;
                case VK_IMAGE_LAYOUT_MAX_ENUM: return ImageLayout::Undefined;
            }
            return ImageLayout::Undefined;
        }*/

        ResourceState fromVulkanResourceStates(VkImageLayout state)
        {
            switch (state)
            {
            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: return ResourceState::VertexAndConstantBuffer;
            /*case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: return ResourceState::IndexBuffer;
            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: return ResourceState::PixelShaderResource;
            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: return ResourceState::IndirectArgument;*/

            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: return ResourceState::RenderTarget;
            /*case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: return ResourceState::ResolveDest;
            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: return ResourceState::ResolveSource;*/

            case VK_IMAGE_LAYOUT_GENERAL: return ResourceState::UnorderedAccess;
            /*case VK_IMAGE_LAYOUT_GENERAL: return ResourceState::Common;
            case VK_IMAGE_LAYOUT_GENERAL: return ResourceState::NonPixelShaderResource;
            case VK_IMAGE_LAYOUT_GENERAL: return ResourceState::GenericRead;
            case VK_IMAGE_LAYOUT_GENERAL: return ResourceState::Predication;*/

            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: return ResourceState::DepthWrite;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL: return ResourceState::DepthRead;

            case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL: return ResourceState::StreamOut;
            //case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL: return ResourceState::CopySource;

            case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: return ResourceState::CopyDest;
            case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: return ResourceState::Present;
            default: return ResourceState::Common;
            }
        }

        ImageAspectFlags fromVulkanImageAspectFlags(VkImageAspectFlags flags)
        {
            bool color = (flags & VK_IMAGE_ASPECT_COLOR_BIT) > 0;
            bool depth = (flags & VK_IMAGE_ASPECT_DEPTH_BIT) > 0;
            bool stencil = (flags & VK_IMAGE_ASPECT_STENCIL_BIT) > 0;
            bool metadata = (flags & VK_IMAGE_ASPECT_METADATA_BIT) > 0;
            ImageAspectFlags returnFlags{};
            returnFlags |= color ? static_cast<uint32_t>(ImageAspectFlagBits::Color) : 0;
            returnFlags |= depth ? static_cast<uint32_t>(ImageAspectFlagBits::Depth) : 0;
            returnFlags |= stencil ? static_cast<uint32_t>(ImageAspectFlagBits::Stencil) : 0;
            returnFlags |= metadata ? static_cast<uint32_t>(ImageAspectFlagBits::Metadata) : 0;
            return returnFlags;
        }

        ResourceDimension fromVulkanDimension(VkImageType dimension)
        {
            switch (dimension)
            {
                case VkImageType::VK_IMAGE_TYPE_1D: return ResourceDimension::Texture1D;
                case VkImageType::VK_IMAGE_TYPE_2D: return ResourceDimension::Texture2D;
                case VkImageType::VK_IMAGE_TYPE_3D: return ResourceDimension::Texture3D;
            }
            return ResourceDimension::Texture1D;
        }

        SubResource fromVulkanSubResource(VkImageSubresourceRange subResources)
        {
            SubResource res;
            res.firstMipLevel = subResources.baseMipLevel;
            res.mipCount = static_cast<int32_t>(subResources.levelCount);
            res.firstArraySlice = subResources.baseArrayLayer;
            res.arraySliceCount = static_cast<int32_t>(subResources.layerCount);
            return res;
        }

        ResourceDimension fromVulkanViewType(VkImageViewType dimension)
        {
            switch (dimension)
            {
                case VK_IMAGE_VIEW_TYPE_1D: return ResourceDimension::Texture1D;
                case VK_IMAGE_VIEW_TYPE_2D: return ResourceDimension::Texture2D;
                case VK_IMAGE_VIEW_TYPE_3D: return ResourceDimension::Texture3D;
                case VK_IMAGE_VIEW_TYPE_1D_ARRAY: return ResourceDimension::Texture1DArray;
                case VK_IMAGE_VIEW_TYPE_2D_ARRAY: return ResourceDimension::Texture2DArray;
                case VK_IMAGE_VIEW_TYPE_CUBE: return ResourceDimension::TextureCubemap;
                case VK_IMAGE_VIEW_TYPE_CUBE_ARRAY: return ResourceDimension::TextureCubemapArray;
            }
            return ResourceDimension::Texture1D;
        }
    }
}
