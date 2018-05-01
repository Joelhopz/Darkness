#include "engine/graphics/dx12/DX12Queue.h"
#include "engine/graphics/dx12/DX12Headers.h"
#include "engine/graphics/dx12/DX12CommandList.h"
#include "engine/graphics/dx12/DX12Fence.h"
#include "engine/graphics/dx12/DX12Semaphore.h"
#include "engine/graphics/dx12/DX12Conversions.h"
#include "engine/graphics/dx12/DX12Device.h"
#include "engine/graphics/dx12/DX12SwapChain.h"

#include "engine/graphics/Device.h"
#include "engine/graphics/Common.h"
#include "engine/graphics/CommandList.h"
#include "engine/graphics/Fence.h"
#include "engine/graphics/Semaphore.h"

#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        QueueImpl::QueueImpl(Device& device, CommandListType type)
            : m_waitForClearFence{ std::make_shared<Fence>(device.createFence()) }
        {
            //m_waitCmdList.

            D3D12_COMMAND_QUEUE_DESC commandQueueDesc;

            // Initialize the description of the command queue.
            ZeroMemory(&commandQueueDesc, sizeof(commandQueueDesc));

            // Set up the description of the command queue.
            commandQueueDesc.Type = dxCommandListType(type);
            commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
            commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
            commandQueueDesc.NodeMask = 0;

            // Create the command queue.
            auto res = DeviceImplGet::impl(device).device()->CreateCommandQueue(
                &commandQueueDesc, 
                DARKNESS_IID_PPV_ARGS(&m_queue));
            ASSERT(SUCCEEDED(res));
        }

        QueueImpl::~QueueImpl()
        {
            if (m_queue)
            {
                m_queue->Release();
                m_queue = NULL;
            }
        }

        ID3D12CommandQueue* QueueImpl::native()
        {
            return m_queue;
        }

        void QueueImpl::submit(std::vector<CommandList>& commandLists)
        {
            std::vector<ID3D12CommandList*> cmdLists;
            for (auto&& cmdList : commandLists)
            {
                if (CommandListImplGet::impl(cmdList).isOpen())
                    CommandListImplGet::impl(cmdList).end();
                cmdLists.emplace_back( CommandListImplGet::impl(cmdList).native() );
            }
            m_queue->ExecuteCommandLists(static_cast<UINT>(cmdLists.size()), cmdLists.data());
        }

        void QueueImpl::submit(CommandList& commandList)
        {
            if (CommandListImplGet::impl(commandList).isOpen())
                CommandListImplGet::impl(commandList).end();
            ID3D12CommandList* cmdLists[] = { CommandListImplGet::impl(commandList).native() };
            m_queue->ExecuteCommandLists(1, cmdLists);
        }

        void QueueImpl::submit(std::vector<CommandList>& commandLists, Fence& fence)
        {
            std::vector<ID3D12CommandList*> cmdLists;
            for (auto&& cmdList : commandLists)
            {
                if (CommandListImplGet::impl(cmdList).isOpen())
                    CommandListImplGet::impl(cmdList).end();
                cmdLists.emplace_back(CommandListImplGet::impl(cmdList).native());
            }
            m_queue->ExecuteCommandLists(static_cast<UINT>(cmdLists.size()), cmdLists.data());
            m_queue->Signal(FenceImplGet::impl(fence).native(), FenceImplGet::impl(fence).nextValue());
        }
        void QueueImpl::submit(CommandList& commandList, Fence& fence)
        {
            if (CommandListImplGet::impl(commandList).isOpen())
                CommandListImplGet::impl(commandList).end();
            ID3D12CommandList* cmdLists[] = { CommandListImplGet::impl(commandList).native() };
            m_queue->ExecuteCommandLists(1, cmdLists);
            m_queue->Signal(FenceImplGet::impl(fence).native(), FenceImplGet::impl(fence).nextValue());
        }

        void QueueImpl::waitForIdle()
        {
            m_queue->Signal(FenceImplGet::impl(*m_waitForClearFence).native(), FenceImplGet::impl(*m_waitForClearFence).nextValue());
            m_waitForClearFence->blockUntilSignaled();
        }

        void QueueImpl::signal(const Semaphore& semaphore)
        {
            SemaphoreImplGet::impl(semaphore).native()->Signal(1);
        }

        void QueueImpl::present(
            Semaphore& /*signalSemaphore*/,
            SwapChain& swapChain,
            unsigned int /*chainIndex*/)
        {
            SwapChainImplGet::impl(swapChain).present();
            //SemaphoreImplGet::impl(signalSemaphore).blockUntilSignaled();
            //SemaphoreImplGet::impl(signalSemaphore).reset();
        }

        bool QueueImpl::needRefresh() const
        {
            return false;
        }

        

        /*void QueueImpl::submit(const CommandList& commandList)
        {
            std::unique_ptr<ID3D12CommandList*[]> ppCommandLists(new ID3D12CommandList*[1]);
            ppCommandLists[0] = CommandListImplGet::impl(commandList).native();
            m_queue->submit(1u, ppCommandLists.get());
        }

        void QueueImpl::submit(std::vector<CommandList>& commandLists)
        {
            std::unique_ptr<ID3D12CommandList*[]> ppCommandLists(new ID3D12CommandList*[commandLists.size()]);
            for (size_t i = 0; i < commandLists.size(); ++i)
            {
                ppCommandLists[i] = CommandListImplGet::impl(commandLists[i]).native();
            }
            m_queue->submit(static_cast<UINT>(commandLists.size()), ppCommandLists.get());
        }

        void QueueImpl::signal(const Fence& fence)
        {
            ASSERT(SUCCEEDED(m_queue->Signal(
                FenceImplGet::impl(fence).native(),
                FenceImplGet::impl(fence).value())));
        }*/
    }
}
