#pragma once

#include "engine/graphics/Device.h"
#include "engine/graphics/Fence.h"
#include "engine/graphics/Semaphore.h"
#include "engine/graphics/CommandList.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Queue.h"

#include <memory>
#include <vector>
#include <mutex>

namespace platform
{
    class Window;
}

namespace engine
{
    class SwapChain;
    class TextureRTV;

    using MessageCallback = std::function<void(const std::vector<std::string>&)>;

    class RenderSetup
    {
    public:
        RenderSetup(std::shared_ptr<platform::Window> window, MessageCallback messageCallback = [](const std::vector<std::string>& messages) 
        {
            if (messages.size() > 0)
            {
                for (auto&& msg : messages)
                {
                    LOG("%s", msg.c_str());
                }
            }
        });
        RenderSetup();

        Device& device();
        ShaderStorage& shaderStorage();
        SwapChain& swapChain();
        platform::Window& window();
        void submit(CommandList&& commandList);
        void present();
        unsigned int currentBackBufferIndex();
        TextureRTV& currentRTV();
        TextureSRV& currentSRV();

        TextureRTV& previousRTV();

        void shutdown();

        void createSwapChainSRVs();
        void releaseSwapChainSRVs();
    private:
        std::shared_ptr<platform::Window> m_window;
        MessageCallback m_messageCallback;
        ShaderStorage m_shaderStorage;
        Device m_device;
        std::shared_ptr<SwapChain> m_swapChain;
        Fence m_submitFence;
        Semaphore m_renderSemaphore;
        Semaphore m_presentSemaphore;
        uint64_t m_frameNumber;
        std::mutex m_mutex;

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

        void processShaderHotreload();
    };

}