#include "engine/graphics/SwapChain.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/Semaphore.h"

#if defined(_WIN32) && !defined(VULKAN)
#include "engine/graphics/dx12/DX12SwapChain.h"
#endif

#if defined(VULKAN)
#include "engine/graphics/vulkan/VulkanSwapChain.h"
#endif

#ifdef __APPLE__
#include "engine/graphics/metal/MetalSwapChain.h"
#endif

namespace engine
{
    PIMPL_ACCESS_IMPLEMENTATION(SwapChain, SwapChainImpl)

    SwapChain::SwapChain(
        const Device& device, 
        Queue& queue, 
        bool fullscreen, 
        bool vsync,
        SwapChain* oldSwapChain)
        : m_impl{ tools::make_unique_impl<implementation::SwapChainImpl>(device, queue, fullscreen, vsync, oldSwapChain) }
    {
    }

    TextureRTV& SwapChain::renderTarget(int index)
    {
        return m_impl->renderTarget(index);
    }

    unsigned int SwapChain::currentBackBufferIndex(const Semaphore& semaphore) const
    {
        return m_impl->currentBackBufferIndex(semaphore);
    }

    unsigned int SwapChain::currentBackBufferIndex(const Fence& fence) const
    {
        return m_impl->currentBackBufferIndex(fence);
    }

    void SwapChain::present()
    {
        m_impl->present();
    }

    bool SwapChain::needRefresh() const
    {
        return m_impl->needRefresh();
    }

    Size SwapChain::size() const
    {
        return m_impl->size();
    }

    void SwapChain::resize(const Device& device, Size size)
    {
        m_impl->resize(device, size);
    }
}
