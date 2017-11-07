#pragma once

#include "engine/graphics/Device.h"
#include "engine/graphics/Fence.h"
#include "engine/graphics/Semaphore.h"
#include "engine/graphics/CommandList.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Queue.h"

#include <memory>
#include <vector>

namespace platform
{
    class Window;
}

namespace engine
{
    class SwapChain;
    class TextureRTV;

    class RenderSetup
    {
    public:
        RenderSetup(std::shared_ptr<platform::Window> window);
        RenderSetup();

        Device& device();
        ShaderStorage& shaderStorage();
        SwapChain& swapChain();
        platform::Window& window();
        void submit(CommandList&& commandList);
        void present();
        Semaphore& renderSemaphore();
        Semaphore& presentSemaphore();
        unsigned int currentBackBufferIndex();
        TextureRTV& currentRTV();
        TextureSRV& currentSRV();
        void shutdown();

        void createSwapChainSRVs();
        void releaseSwapChainSRVs();
    private:
        std::shared_ptr<platform::Window> m_window;
        Device m_device;
        std::shared_ptr<SwapChain> m_swapChain;
        ShaderStorage m_shaderStorage;
        Fence m_submitFence;
        Semaphore m_renderSemaphore;
        Semaphore m_presentSemaphore;
        uint64_t m_frameNumber;

        std::vector<TextureSRV> m_swapChainSRVs;

        struct CommandListExec
        {
            CommandList list;
            unsigned long long done;
            CommandListExec(CommandList&& list, unsigned long long done)
                : list{ std::move(list) }
                , done{ done }
            {}
            CommandListExec(CommandListExec&& cmd)
                : list{ std::move(cmd.list) }
                , done{ std::move(cmd.done) }
            {}
            CommandListExec& operator=(CommandListExec&& cmd)
            {
                list = std::move(cmd.list);
                done = std::move(cmd.done);
                return *this;
            }
            CommandListExec(const CommandListExec&) = delete;
            CommandListExec& operator=(const CommandListExec&) = delete;
        };
        std::vector<CommandListExec> m_lists;
    };

}