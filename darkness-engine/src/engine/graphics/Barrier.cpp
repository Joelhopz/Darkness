#include "engine/graphics/Barrier.h"

#include "engine/graphics/CommandList.h"

#if defined(_WIN32) && !defined(VULKAN)
#include "engine/graphics/dx12/DX12Barrier.h"
#endif

#if defined(VULKAN)
#include "engine/graphics/vulkan/VulkanBarrier.h"
#endif

#ifdef __APPLE__
#include "engine/graphics/metal/MetalBarrier.h"
#endif

using namespace tools;
using namespace engine::implementation;

namespace engine
{
    const char* resourceStateToString(ResourceState state)
    {
        switch (state)
        {
        case ResourceState::Common: return "Common";
            case ResourceState::VertexAndConstantBuffer: return "VertexAndConstantBuffer";
            case ResourceState::IndexBuffer: return "IndexBuffer";
            case ResourceState::RenderTarget: return "RenderTarget";
            case ResourceState::UnorderedAccess: return "UnorderedAccess";
            case ResourceState::DepthWrite: return "DepthWrite";
            case ResourceState::DepthRead: return "DepthRead";
            case ResourceState::NonPixelShaderResource: return "NonPixelShaderResource";
            case ResourceState::PixelShaderResource: return "PixelShaderResource";
            case ResourceState::StreamOut: return "StreamOut";
            case ResourceState::IndirectArgument: return "IndirectArgument";
            case ResourceState::CopyDest: return "CopyDest";
            case ResourceState::CopySource: return "CopySource";
            case ResourceState::ResolveDest: return "ResolveDest";
            case ResourceState::ResolveSource: return "ResolveSource";
            case ResourceState::GenericRead: return "GenericRead";
            case ResourceState::Present: return "Present";
            case ResourceState::Predication: return "Predication";
        }
        return "";
    }

    PIMPL_ACCESS_IMPLEMENTATION(Barrier, BarrierImpl)

    Barrier::Barrier(
        const CommandList& commandList,
        ResourceBarrierFlags flags,
        const TextureRTV& resource,
        ResourceState before,
        ResourceState after,
        unsigned int subResource,
        const Semaphore& waitSemaphore,
        const Semaphore& signalSemaphore
    )
        : m_impl{ make_unique_impl<BarrierImpl>(
            commandList, 
            flags, 
            resource, 
            before, 
            after, 
            subResource,
            waitSemaphore,
            signalSemaphore
            ) }
    {
    }

    void Barrier::update(
        ResourceState before,
        ResourceState after)
    {
        m_impl->update(before, after);
    }
}
