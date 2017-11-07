#include "engine/RenderSetup.h"
#include "platform/window/Window.h"
#include "engine/graphics/SwapChain.h"
#include "engine/graphics/Resources.h"

using namespace std;

namespace engine
{
    RenderSetup::RenderSetup(shared_ptr<platform::Window> window)
        : m_window{ window }
        , m_device{ Device(m_window) }
        , m_swapChain{ m_device.createSwapChain() }
        , m_submitFence{ m_device.createFence() }
        , m_renderSemaphore{ m_device.createSemaphore() }
        , m_presentSemaphore{ m_device.createSemaphore() }
        , m_frameNumber{ 0 }
    {
        m_device.frameNumber(m_frameNumber);
        createSwapChainSRVs();
    }

    void RenderSetup::createSwapChainSRVs()
    {
        for (int i = 0; i < BackBufferCount; ++i) { m_swapChainSRVs.emplace_back(m_device.createTextureSRV(m_swapChain->renderTarget(i).texture())); }
    }

    void RenderSetup::releaseSwapChainSRVs()
    {
        m_swapChainSRVs.clear();
    }

    RenderSetup::RenderSetup()
        : m_window{ make_shared<platform::Window>("Device test window", 1024, 768) }
        , m_device{ Device(m_window) }
        , m_swapChain{ m_device.createSwapChain() }
        , m_submitFence{ m_device.createFence() }
        , m_renderSemaphore{ m_device.createSemaphore() }
        , m_presentSemaphore{ m_device.createSemaphore() }
        , m_frameNumber{ 0 }
    { 
        m_device.frameNumber(m_frameNumber);
    }

    Device& RenderSetup::device() { return m_device; }
    ShaderStorage& RenderSetup::shaderStorage() { return m_shaderStorage; }
    SwapChain& RenderSetup::swapChain() { return *m_swapChain; }
    platform::Window& RenderSetup::window() { return *m_window; }
    void RenderSetup::submit(CommandList&& commandList)
    {
        m_lists.emplace_back(CommandListExec( std::move(commandList), m_submitFence.value()+1 ));
        m_device.queue().submit(m_lists.back().list, m_submitFence);
        
        if (m_lists.size() > BackBufferCount)
        {
            CommandListExec ce{ std::move(m_lists[0]) };
            m_submitFence.blockUntilSignaled(ce.done);
            m_lists.erase(m_lists.begin());
        }
    }
    void RenderSetup::present()
    {
        m_device.queue().present(m_renderSemaphore, *m_swapChain, m_swapChain->currentBackBufferIndex(m_submitFence));
        m_device.recycleUploadBuffers(static_cast<uint32_t>(m_frameNumber));
        ++m_frameNumber;
        m_device.frameNumber(m_frameNumber);
    }
    //Semaphore& RenderSetup::renderSemaphore() { return m_renderSemaphore; }
    //Semaphore& RenderSetup::presentSemaphore() { return m_presentSemaphore; }
    unsigned int RenderSetup::currentBackBufferIndex() { return m_swapChain->currentBackBufferIndex(m_submitFence); }
    TextureRTV& RenderSetup::currentRTV()
    {
        return m_swapChain->renderTarget(m_swapChain->currentBackBufferIndex(m_submitFence));
    }
    
    TextureSRV& RenderSetup::currentSRV()
    {
        return m_swapChainSRVs[m_swapChain->currentBackBufferIndex(m_submitFence)];
    }

    void RenderSetup::shutdown()
    {
        m_device.queue().waitForIdle();
        m_device.waitForIdle();

        // we need to clear shaderstorage as it's a static instance
        // and when the vulkan instance dies, it's going
        // to perform a debug layer check if all resources
        // have been released.
        m_shaderStorage.clear();
    }
}
