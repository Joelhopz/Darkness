#include "engine/graphics/vulkan/VulkanSampler.h"
#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/vulkan/VulkanDevice.h"
#include "engine/graphics/vulkan/VulkanDescriptorHandle.h"
#include "engine/graphics/vulkan/VulkanConversions.h"

#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        SamplerImpl::SamplerImpl(
            const Device& device, 
            const DescriptorHandle& /*handle*/, 
            const SamplerDescription& /*desc*/)
            : m_device{ device }
            , m_sampler{ vulkanPtr<VkSampler>(DeviceImplGet::impl(device).device(), vkDestroySampler) }
        {
            VkSamplerCreateInfo samplerInfo = {};
            samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.magFilter = VK_FILTER_LINEAR;
            samplerInfo.minFilter = VK_FILTER_LINEAR;

            samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

            samplerInfo.anisotropyEnable = VK_TRUE;
            samplerInfo.maxAnisotropy = 16;
            samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
            samplerInfo.unnormalizedCoordinates = VK_FALSE;
            samplerInfo.compareEnable = VK_FALSE;
            samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
            samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            samplerInfo.mipLodBias = 0.0f;
            samplerInfo.minLod = 0.0f;
            samplerInfo.maxLod = 0.0f;

            auto result = vkCreateSampler(DeviceImplGet::impl(device).device(), &samplerInfo, nullptr, m_sampler.get());
            ASSERT(result == VK_SUCCESS);
        }

        SamplerImpl::SamplerImpl(
            const Device& device,
            const SamplerDescription& /*desc*/)
            : m_device{ device }
            , m_sampler{ vulkanPtr<VkSampler>(DeviceImplGet::impl(device).device(), vkDestroySampler) }
        {
            VkSamplerCreateInfo samplerInfo = {};
            samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.magFilter = VK_FILTER_LINEAR;
            samplerInfo.minFilter = VK_FILTER_LINEAR;

            samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

            samplerInfo.anisotropyEnable = VK_TRUE;
            samplerInfo.maxAnisotropy = 16;
            samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
            samplerInfo.unnormalizedCoordinates = VK_FALSE;
            samplerInfo.compareEnable = VK_FALSE;
            samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
            samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            samplerInfo.mipLodBias = 0.0f;
            samplerInfo.minLod = 0.0f;
            samplerInfo.maxLod = 0.0f;

            auto result = vkCreateSampler(DeviceImplGet::impl(device).device(), &samplerInfo, nullptr, m_sampler.get());
            ASSERT(result == VK_SUCCESS);
        }

        const VkSampler& SamplerImpl::native() const
        {
            return *m_sampler;
        }

        VkSampler& SamplerImpl::native()
        {
            return *m_sampler;
        }
    }
}
