#pragma once

#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/SwapChain.h"
#include <vector>

namespace engine
{
    class Device;
    class Queue;
    class Texture;
    class TextureRTV;
    class Semaphore;
    class Fence;
    class SwapChain;
    struct Size;

    namespace implementation
    {
        class BufferViewImpl;
        class SwapChainImpl
        {
        public:
            SwapChainImpl(
                const Device& device, 
                Queue& queue, 
                bool fullscreen = false, 
                bool vsync = true,
                SwapChain* oldSwapChain = nullptr);
            ~SwapChainImpl();

            SwapChainImpl(const SwapChainImpl&) = delete;
            SwapChainImpl(SwapChainImpl&&) = delete;
            SwapChainImpl& operator=(const SwapChainImpl&) = delete;
            SwapChainImpl& operator=(SwapChainImpl&&) = delete;

            TextureRTV& renderTarget(int index);
            size_t chainLength() const;
            unsigned int currentBackBufferIndex(const Semaphore& semaphore) const;
            unsigned int currentBackBufferIndex(const Fence& fence) const;
            void present();
            Size size() const;

            struct SwapChainDetails
            {
                VkSurfaceCapabilitiesKHR capabilities;
                std::vector<VkSurfaceFormatKHR> formats;
                std::vector<VkPresentModeKHR> presentModes;
            };
            static SwapChainDetails getDetails(
                const VkPhysicalDevice& device,
                const VkSurfaceKHR& surface);

            const VkSurfaceFormatKHR& surfaceFormat() const;
            const VkPresentModeKHR& presentMode() const;
            const VkExtent2D& extent() const;
            uint32_t bufferCount() const;

            VkSwapchainKHR& native();
            const VkSwapchainKHR& native() const;

            bool needRefresh() const;

            void resize(const Device& device, Size size);

        private:
            const Device& m_device;
            std::shared_ptr<VkSwapchainKHR> m_swapChain;
            int m_videoCardMemory;
            char m_videoCardDescription[128];
            bool m_vsync;
            mutable bool m_needRefresh;

            VkSurfaceFormatKHR m_surfaceFormat;
            VkPresentModeKHR m_presentMode;
            VkExtent2D m_extent;
            uint32_t m_bufferCount;
            std::vector<TextureRTV> m_views;

            VkSurfaceFormatKHR pickSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) const;
            VkPresentModeKHR pickPresentMode(const std::vector<VkPresentModeKHR> modes) const;
            VkExtent2D pickExtent(const Device& device, const VkSurfaceCapabilitiesKHR& capabilities) const;
            uint32_t pickBufferCount(const VkSurfaceCapabilitiesKHR& capabilities) const;
        };
    }
}
