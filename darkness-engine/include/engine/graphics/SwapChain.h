#pragma once

#include "tools/SmartPimpl.h"
#include <utility>

namespace engine
{
    PIMPL_FWD(SwapChain, SwapChainImpl)

    class Queue;
    class TextureRTV;
    class Device;
    class Semaphore;
    class Fence;

    struct Size
    {
        uint32_t width;
        uint32_t height;
    };

    class SwapChain
    {
    public:
        TextureRTV& renderTarget(int index);
        unsigned int currentBackBufferIndex(const Semaphore& semaphore) const;
        unsigned int currentBackBufferIndex(const Fence& fence) const;
        void present();

        bool needRefresh() const;
        Size size() const;

        void resize(const Device& device, Size size);
    private:
        friend class Device;
        SwapChain(
            const Device& device, 
            Queue& commandQueue, 
            bool fullscreen = false, 
            bool vsync = true,
            SwapChain* oldSwapChain = nullptr);

        PIMPL_FRIEND_ACCESS(SwapChainImpl)
    };
}
