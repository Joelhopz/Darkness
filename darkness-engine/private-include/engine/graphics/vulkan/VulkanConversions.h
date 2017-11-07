#pragma once

#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/CommonNoDep.h"

namespace engine
{
    enum class Format;
    enum class CommandListType;
    enum class PrimitiveTopologyType;
    enum class InputClassification;
    enum class FillMode;
    enum class CullMode;
    enum class ImageTiling;
    enum class ResourceDimension;
    enum class ResourceState;
    struct SubResource;
    class Texture;

    namespace implementation
    {
        enum class DescriptorType;

        VkShaderStageFlags vulkanShaderVisibility(ShaderVisibility flags);
        VkQueueFlags vulkanCommandListType(CommandListType type);
        VkFormat vulkanFormat(Format format);
        VkPrimitiveTopology vulkanPrimitiveTopologyType(PrimitiveTopologyType type);
        VkVertexInputRate vulkanInputClassification(InputClassification cls);
        VkPolygonMode vulkanFillMode(FillMode mode);
        VkCullModeFlags vulkanCullMode(CullMode mode);
        VkBufferUsageFlags vulkanBufferUsageFlags(BufferUsageFlags flags);
        VkMemoryPropertyFlags vulkanMemoryFlags(BufferMemoryFlags flags);
        VkDescriptorType vulkanDescriptorType(DescriptorType type);
        VkImageTiling vulkanImageTiling(ImageTiling tiling);
        //VkImageLayout vulkanImageLayout(ImageLayout layout);
        VkImageAspectFlags vulkanImageAspectFlags(ImageAspectFlags flags);
        VkImageType vulkanDimension(ResourceDimension dimension);
        VkImageAspectFlags vulkanFormatAspects(Format format);
        VkImageSubresourceRange vulkanSubResource(const Texture& texture, SubResource subResources);
        VkImageViewType vulkanViewType(ResourceDimension dimension);
        VkImageLayout vulkanResourceStates(ResourceState state);

        ShaderVisibility fromVulkanShaderVisibility(VkShaderStageFlags flags);
        CommandListType fromVulkanCommandListType(VkQueueFlags type);
        Format fromVulkanFormat(VkFormat format);
        PrimitiveTopologyType fromVulkanPrimitiveTopologyType(VkPrimitiveTopology type);
        InputClassification fromVulkanInputClassification(VkVertexInputRate cls);
        FillMode fromVulkanFillMode(VkPolygonMode mode);
        CullMode fromVulkanCullMode(VkCullModeFlags mode);
        BufferUsageFlags fromVulkanBufferUsageFlags(VkBufferUsageFlags flags);
        BufferMemoryFlags fromVulkanMemoryFlags(VkMemoryPropertyFlags flags);
        DescriptorType fromVulkanDescriptorType(VkDescriptorType type);
        ImageTiling fromVulkanImageTiling(VkImageTiling tiling);
        //ImageLayout fromVulkanImageLayout(VkImageLayout layout);
        ImageAspectFlags fromVulkanImageAspectFlags(VkImageAspectFlags flags);
        ResourceDimension fromVulkanDimension(VkImageType dimension);
        SubResource fromVulkanSubResource(VkImageSubresourceRange subResources);
        ResourceDimension fromVulkanViewType(VkImageViewType dimension);
        ResourceState fromVulkanResourceStates(VkImageLayout state);
    }
}
