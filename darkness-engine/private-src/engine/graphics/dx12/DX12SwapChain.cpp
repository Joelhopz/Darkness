#include "engine/graphics/Queue.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/Barrier.h"

#include "engine/graphics/dx12/DX12Queue.h"
#include "engine/graphics/dx12/DX12Device.h"
#include "engine/graphics/dx12/DX12Resources.h"

#include "engine/graphics/dx12/DX12SwapChain.h"

#include "platform/window/Window.h"
#include "platform/window/WindowsWindow.h"

#include <stdlib.h>
#include "engine/graphics/dx12/DX12Headers.h"

#include "tools/Debug.h"
#include "tools/ComPtr.h"

using namespace platform;
using namespace tools;

namespace engine
{
    namespace implementation
    {
        SwapChainImpl::SwapChainImpl(
            const Device& device, 
            Queue& queue, 
            bool fullscreen, 
            bool vsync,
            SwapChain* /*oldSwapChain*/)
            : m_vsync{ vsync }
            , m_needRefresh{ false }
        {
            ComPtr<IDXGIFactory4> factory;
            ComPtr<IDXGIAdapter> adapter;
            ComPtr<IDXGIOutput> adapterOutput;
            DXGI_SWAP_CHAIN_DESC swapChainDesc;
            ComPtr<IDXGISwapChain> swapChain;

            // Create a DirectX graphics interface factory.
            auto res = CreateDXGIFactory1(DARKNESS_IID_PPV_ARGS(factory.GetAddressOf()));
            ASSERT(SUCCEEDED(res));

            // Use the factory to create an adapter for the primary graphics interface (video card).
            res = factory->EnumAdapters(0, adapter.GetAddressOf());
            ASSERT(SUCCEEDED(res));

            // Enumerate the primary adapter output (monitor).
            res = adapter->EnumOutputs(0, adapterOutput.GetAddressOf());
            ASSERT(SUCCEEDED(res));

            // Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
            unsigned int numModes{ 0 };
            res = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
            ASSERT(SUCCEEDED(res));

            // Create a list to hold all the possible display modes for this monitor/video card combination.
            std::unique_ptr<DXGI_MODE_DESC[]> displayModeList(new DXGI_MODE_DESC[numModes]);
            ASSERT(displayModeList);

            // Now fill the display mode list structures.
            res = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList.get());
            ASSERT(SUCCEEDED(res));

            // Now go through all the display modes and find the one that matches the screen height and width.
            // When a match is found store the numerator and denominator of the refresh rate for that monitor.
            unsigned int i, numerator, denominator;
            i = numerator = denominator = 0;
            for (i = 0; i<numModes; i++)
            {
                if (displayModeList[i].Height == (unsigned int)device.height())
                {
                    if (displayModeList[i].Width == (unsigned int)device.width())
                    {
                        numerator = displayModeList[i].RefreshRate.Numerator;
                        denominator = displayModeList[i].RefreshRate.Denominator;
                    }
                }
            }

            // Get the adapter (video card) description.
            DXGI_ADAPTER_DESC adapterDesc;
            res = adapter->GetDesc(&adapterDesc);
            ASSERT(SUCCEEDED(res));

            // Store the dedicated video card memory in megabytes.
            m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

            // Convert the name of the video card to a character array and store it.
            size_t stringLength;
            auto wcsres = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
            ASSERT(wcsres == 0);

            // Initialize the swap chain description.
            memset(&swapChainDesc, 0, sizeof(swapChainDesc));

            // Set the swap chain to use double buffering.
            swapChainDesc.BufferCount = 2;

            // Set the height and width of the back buffers in the swap chain.
            m_size.width = device.width();
            m_size.height = device.height();
            swapChainDesc.BufferDesc.Height = static_cast<UINT>(m_size.height);
            swapChainDesc.BufferDesc.Width = static_cast<UINT>(m_size.width);

            // Set a regular 32-bit surface for the back buffers.
            swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// DXGI_FORMAT_R10G10B10A2_UNORM;

            // Set the usage of the back buffers to be render target outputs.
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

            // Set the swap effect to discard the previous buffer contents after swapping.
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

            // Set the handle for the window to render to.
            swapChainDesc.OutputWindow = platform::implementation::WindowImplGet::impl(DeviceImplGet::impl(device).window()).handle();

            // Set to full screen or windowed mode.
            if (fullscreen)
            {
                swapChainDesc.Windowed = false;
            }
            else
            {
                swapChainDesc.Windowed = true;
            }

            // Set the refresh rate of the back buffer.
            if (vsync)
            {
                swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
                swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
            }
            else
            {
                swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
                swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
            }

            // Turn multisampling off.
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;

            // Set the scan line ordering and scaling to unspecified.
            swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

            // Don't set the advanced flags.
            swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

            // Finally create the swap chain using the swap chain description.
            res = factory->CreateSwapChain(QueueImplGet::impl(queue).native(), &swapChainDesc, swapChain.GetAddressOf());
            ASSERT(SUCCEEDED(res));

            // Next upgrade the IDXGISwapChain to a IDXGISwapChain3 interface and store it in a private member variable named m_swapChain.
            // This will allow us to use the newer functionality such as getting the current back buffer index.
            res = swapChain->QueryInterface(DARKNESS_IID_PPV_ARGS(m_swapChain.GetAddressOf()));
            ASSERT(SUCCEEDED(res));

            createSwapChainTextures(device);
        }

        void SwapChainImpl::createSwapChainTextures(const Device& device)
        {
            TextureDescription chainImageDesc;
            chainImageDesc.descriptor.append = false;
            chainImageDesc.descriptor.arraySlices = 1;
            chainImageDesc.descriptor.depth = 1;
            chainImageDesc.descriptor.dimension = ResourceDimension::Texture2D;
            chainImageDesc.descriptor.format = fromDXFormat(DXGI_FORMAT_R8G8B8A8_UNORM);
            chainImageDesc.descriptor.height = m_size.height;
            chainImageDesc.descriptor.width = m_size.width;
            chainImageDesc.descriptor.mipLevels = 1;
            chainImageDesc.descriptor.name = "SwapChainImage";
            chainImageDesc.descriptor.samples = 1;
            chainImageDesc.descriptor.usage = ResourceUsage::GpuRenderTargetReadWrite;

            for (int i = 0; i < BackBufferCount; ++i)
            {
                ID3D12Resource* buffer;
                auto res = m_swapChain->GetBuffer(static_cast<UINT>(i), DARKNESS_IID_PPV_ARGS(&buffer));
                ASSERT(SUCCEEDED(res));

                m_swapChainTextures.emplace_back(
                    new Texture(
                        std::make_shared<TextureImpl>(
                            DeviceImplGet::impl(device),
                            chainImageDesc,
                            buffer,
                            ResourceState::Present)));
            }

            for (auto&& tex : m_swapChainTextures)
            {
                m_swapChainTextureRTVs.emplace_back(device.createTextureRTV(*tex));
            }
            
        }

        void SwapChainImpl::resize(const Device& device, Size size)
        {
            m_size = size;
            m_swapChainTextureRTVs.clear();
            m_swapChainTextures.clear();

            m_swapChain->ResizeBuffers(
                BackBufferCount,
                size.width,
                size.height,
                DXGI_FORMAT_R8G8B8A8_UNORM,
                DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

            createSwapChainTextures(device);
        }

        TextureRTV& SwapChainImpl::renderTarget(int index)
        {
            return m_swapChainTextureRTVs[index];
        }

        size_t SwapChainImpl::chainLength() const
        {
            return BackBufferCount;
        }

        unsigned int SwapChainImpl::currentBackBufferIndex(const Semaphore& /*semaphore*/) const
        {
            return m_swapChain->GetCurrentBackBufferIndex();
        }

        unsigned int SwapChainImpl::currentBackBufferIndex(const Fence& /*semaphore*/) const
        {
            return m_swapChain->GetCurrentBackBufferIndex();
        }

        Size SwapChainImpl::size() const
        {
            return m_size;
        }

        void SwapChainImpl::present()
        {
            if (m_vsync)
            {
                auto res = m_swapChain->Present(1, 0);
                if (!SUCCEEDED(res))
                {
                    m_needRefresh = true;
                }
            }
            else
            {
                auto res = m_swapChain->Present(0, 0);
                if (!SUCCEEDED(res))
                {
                    m_needRefresh = true;
                }
            }
        }

        bool SwapChainImpl::needRefresh()
        {
            auto res = m_needRefresh;
            m_needRefresh = false;
            return res;
        }
    }
}
