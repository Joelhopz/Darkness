#pragma once

#include "engine/graphics/dx12/DX12Headers.h"
#include "engine/graphics/SwapChain.h"
#include "tools/ComPtr.h"

struct IDXGISwapChain3;

namespace engine
{
    class Device;
    class Queue;
    class Texture;
    class TextureRTV;
    class Semaphore;
    class Fence;
    class SwapChain;

    namespace implementation
    {
        class BufferViewImpl;
        class SwapChainImpl
        {
        public:
            SwapChainImpl(
                const Device& device, 
                Queue& commandQueue, 
                bool fullscreen = false, 
                bool vsync = true,
                SwapChain* oldSwapChain = nullptr);

            TextureRTV& renderTarget(int index);
            size_t chainLength() const;
            unsigned int currentBackBufferIndex(const Semaphore& semaphore) const;
            unsigned int currentBackBufferIndex(const Fence& fence) const;
            void present();
            Size size() const;

            IDXGISwapChain3* native();
            const IDXGISwapChain3* native() const;

            bool needRefresh();

            void resize(const Device& device, Size size);
        private:
            tools::ComPtr<IDXGISwapChain3> m_swapChain;
            Size m_size;
            void createSwapChainTextures(const Device& device);
            std::vector<std::shared_ptr<Texture>> m_swapChainTextures;
            std::vector<TextureRTV> m_swapChainTextureRTVs;
            int m_videoCardMemory;
            char m_videoCardDescription[128];
            bool m_vsync;
			bool m_needRefresh;
        };
    }
}
