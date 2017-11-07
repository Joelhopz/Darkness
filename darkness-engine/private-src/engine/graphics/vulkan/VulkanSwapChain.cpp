#include "engine/graphics/Queue.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/Fence.h"
#include "engine/graphics/Semaphore.h"
#include "engine/graphics/SwapChain.h"

#include "engine/graphics/vulkan/VulkanQueue.h"
#include "engine/graphics/vulkan/VulkanDevice.h"
#include "engine/graphics/vulkan/VulkanResources.h"
#include "engine/graphics/vulkan/VulkanCommon.h"
#include "engine/graphics/vulkan/VulkanSemaphore.h"
#include "engine/graphics/vulkan/VulkanFence.h"

#include "engine/graphics/vulkan/VulkanSwapChain.h"

#include "platform/window/Window.h"
#ifdef _WIN32
#undef max
#include "platform/window/WindowsWindow.h"
#endif

#include <stdlib.h>
#include "engine/graphics/vulkan/VulkanHeaders.h"

#include "tools/Debug.h"

using namespace platform;

namespace engine
{
    namespace implementation
    {
        SwapChainImpl::SwapChainImpl(
            const Device& device, 
            Queue& /*queue*/, 
            bool /*fullscreen*/, 
            bool vsync,
            SwapChain* oldSwapChain)
            : m_device{ device }
            , m_vsync{ vsync }
            , m_swapChain{ vulkanPtr<VkSwapchainKHR>(DeviceImplGet::impl(device).device(), vkDestroySwapchainKHR) }
            , m_needRefresh{ false }
        {
            SwapChainDetails details = getDetails(
                DeviceImplGet::impl(device).physicalDevice(),
                DeviceImplGet::impl(device).surface());

            m_surfaceFormat = pickSurfaceFormat(details.formats);
            m_presentMode = pickPresentMode(details.presentModes);
            m_extent = pickExtent(device, details.capabilities);
            m_bufferCount = pickBufferCount(details.capabilities);

            VkSwapchainCreateInfoKHR createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = DeviceImplGet::impl(device).surface();
            createInfo.minImageCount = m_bufferCount;
            createInfo.imageFormat = m_surfaceFormat.format;
            createInfo.imageColorSpace = m_surfaceFormat.colorSpace;
            createInfo.imageExtent = m_extent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            std::vector<VulkanQueue> deviceQueues = DeviceImplGet::impl(device).deviceQueues();
            int graphicsFamily = -1;
            int presentFamily = -1;
            int queueIndex = 0;
            for (const auto& queue : deviceQueues)
            {
                if (queue.taken.size() > 0 && queue.flags & VK_QUEUE_GRAPHICS_BIT)
                    graphicsFamily = queueIndex;

                if (queue.taken.size() > 0 && queue.presentSupport)
                    presentFamily = queueIndex;

                if (graphicsFamily >= 0 && presentFamily >= 0)
                    break;

                ++queueIndex;
            }

            uint32_t queueFamilyIndices[] = { static_cast<uint32_t>(graphicsFamily), static_cast<uint32_t>(presentFamily) };
            if (graphicsFamily != presentFamily) {
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = queueFamilyIndices;
            }
            else {
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                createInfo.queueFamilyIndexCount = 0; // Optional
                createInfo.pQueueFamilyIndices = nullptr; // Optional
            }
            createInfo.preTransform = details.capabilities.currentTransform;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode = m_presentMode;
            createInfo.clipped = VK_TRUE;

            if (oldSwapChain != nullptr)
            {
                createInfo.oldSwapchain = *SwapChainImplGet::impl(*oldSwapChain).m_swapChain;
            }
            else
            {
                createInfo.oldSwapchain = VK_NULL_HANDLE;
            }
            

            ASSERT(vkCreateSwapchainKHR(
                DeviceImplGet::impl(device).device(),
                &createInfo,
                nullptr,
                m_swapChain.get()) == VK_SUCCESS);

            uint32_t imageCount;
            vkGetSwapchainImagesKHR(DeviceImplGet::impl(device).device(), *m_swapChain, &imageCount, nullptr);

            std::vector<VkImage> images(imageCount);
            vkGetSwapchainImagesKHR(DeviceImplGet::impl(device).device(), *m_swapChain, &imageCount, images.data());

            for (const auto& image : images)
            {
                m_views.emplace_back(
                    device.createTextureRTV(
                        Texture(std::make_shared<TextureImpl>(
                            std::make_shared<VkImage>(image),
                            TextureDescription()
                            .format(fromVulkanFormat(m_surfaceFormat.format))
                            .usage(ResourceUsage::GpuRenderTargetReadWrite)
                            .width(m_extent.width)
                            .height(m_extent.height)))
                    )
                );
            }

            /*for (const auto& image : images)
            {
                //m_buffers.emplace_back(Buffer{ tools::make_impl<BufferImpl>(DeviceImplGet::impl(device), image, false) });
                m_buffers.emplace_back(device.
            }

            for (const auto& buffer : m_buffers)
            {
                m_views.emplace_back(
                    BufferView {
                        tools::make_unique_impl<BufferViewImpl>(
                            device, 
                            buffer, 
                            fromVulkanFormat(m_surfaceFormat.format),
                            static_cast<ImageAspectFlags>(ImageAspectFlagBits::Color))
                    }
                );
            }*/
        }

        SwapChainImpl::SwapChainDetails SwapChainImpl::getDetails(
            const VkPhysicalDevice& device,
            const VkSurfaceKHR& surface)
        {
            SwapChainDetails details;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
            if (formatCount > 0)
            {
                details.formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
            }

            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
            if (presentModeCount > 0)
            {
                details.presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
            }

            return details;
        }

        VkSurfaceFormatKHR SwapChainImpl::pickSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) const
        {
            if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
                return{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

            for (const auto& format : formats)
            {
                if (format.format == VK_FORMAT_B8G8R8A8_UNORM &&
                    format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                    return format;
            }

            return formats[0];
        }

        VkPresentModeKHR SwapChainImpl::pickPresentMode(const std::vector<VkPresentModeKHR> modes) const
        {
            VkPresentModeKHR wantedModex = m_vsync ? VK_PRESENT_MODE_FIFO_KHR : VK_PRESENT_MODE_IMMEDIATE_KHR;
            for (const auto& mode : modes)
            {
                if (mode == wantedModex)
                {
                    return mode;
                }
            }
            return VK_PRESENT_MODE_FIFO_KHR;
        }

        VkExtent2D SwapChainImpl::pickExtent(const Device& device, const VkSurfaceCapabilitiesKHR& capabilities) const
        {
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
                return capabilities.currentExtent;
            else
            {
                VkExtent2D actualExtent{ 
                    static_cast<uint32_t>(device.width()),
                    static_cast<uint32_t>(device.height()) };
                actualExtent.width = capabilities.minImageExtent.width > actualExtent.width ? capabilities.minImageExtent.width : actualExtent.width;
                actualExtent.width = capabilities.maxImageExtent.width < actualExtent.width ? capabilities.maxImageExtent.width : actualExtent.width;
                actualExtent.height = capabilities.minImageExtent.height > actualExtent.height ? capabilities.minImageExtent.height : actualExtent.height;
                actualExtent.height = capabilities.maxImageExtent.height < actualExtent.height ? capabilities.maxImageExtent.height : actualExtent.height;
                return actualExtent;
            }
        }

        uint32_t SwapChainImpl::pickBufferCount(const VkSurfaceCapabilitiesKHR& capabilities) const
        {
            if (capabilities.maxImageCount == 0)
                return static_cast<uint32_t>(BackBufferCount);

            uint32_t imageCount{ BackBufferCount };
            imageCount = capabilities.minImageCount > imageCount ? capabilities.minImageCount : imageCount;
            imageCount = capabilities.maxImageCount < imageCount ? capabilities.maxImageCount : imageCount;
            return imageCount;
        }

        SwapChainImpl::~SwapChainImpl()
        {
            /*if (m_swapChain)
            {
                m_swapChain->Release();
                m_swapChain = NULL;
            }*/
        }

        const VkSurfaceFormatKHR& SwapChainImpl::surfaceFormat() const
        {
            return m_surfaceFormat;
        }

        const VkPresentModeKHR& SwapChainImpl::presentMode() const
        {
            return m_presentMode;
        }

        const VkExtent2D& SwapChainImpl::extent() const
        {
            return m_extent;
        }

        Size SwapChainImpl::size() const
        {
            Size size;
            size.width = m_extent.width;
            size.height = m_extent.height;
            return size;
        }

        uint32_t SwapChainImpl::bufferCount() const
        {
            return m_bufferCount;
        }

        TextureRTV& SwapChainImpl::renderTarget(int index)
        {
            ASSERT(static_cast<size_t>(index) < m_views.size());
            return m_views[static_cast<size_t>(index)];
        }

        size_t SwapChainImpl::chainLength() const
        {
            return m_views.size();
        }

        unsigned int SwapChainImpl::currentBackBufferIndex(const Semaphore& semaphore) const
        {
            uint32_t imageIndex;
            VkResult res = vkAcquireNextImageKHR(
                DeviceImplGet::impl(m_device).device(),
                *m_swapChain,
                std::numeric_limits<uint64_t>::max(),
                SemaphoreImplGet::impl(semaphore).native(),
                VK_NULL_HANDLE,
                &imageIndex);

            if (res == VK_ERROR_OUT_OF_DATE_KHR)
            {
                m_needRefresh = true;
            }
            else
            {
                ASSERT(res == VK_SUCCESS);
            }
            
            return static_cast<unsigned int>(imageIndex);
        }

        unsigned int SwapChainImpl::currentBackBufferIndex(const Fence& fence) const
        {
            uint32_t imageIndex;
            VkResult res = vkAcquireNextImageKHR(
                DeviceImplGet::impl(m_device).device(),
                *m_swapChain,
                std::numeric_limits<uint64_t>::max(),
                VK_NULL_HANDLE,
                FenceImplGet::impl(fence).native(),
                &imageIndex);

            if (res == VK_ERROR_OUT_OF_DATE_KHR)
            {
                m_needRefresh = true;
            }
            else
            {
                ASSERT(res == VK_SUCCESS);
            }

            return static_cast<unsigned int>(imageIndex);
        }

        void SwapChainImpl::resize(const Device& device, Size size)
        {}

        void SwapChainImpl::present()
        {
            /*if (m_vsync)
                ASSERT(SUCCEEDED(m_swapChain->Present(1, 0)));
            else
                ASSERT(SUCCEEDED(m_swapChain->Present(0, 0)));*/
        }

        VkSwapchainKHR& SwapChainImpl::native()
        {
            return *m_swapChain;
        }

        const VkSwapchainKHR& SwapChainImpl::native() const
        {
            return *m_swapChain;
        }

        bool SwapChainImpl::needRefresh() const
        {
            return m_needRefresh;
        }
    }
}
