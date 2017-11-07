#pragma once

#include "tools/SmartPimpl.h"

namespace engine
{
    PIMPL_FWD(Barrier, BarrierImpl)

    class CommandList;
    class Buffer;
    class TextureRTV;
    class Semaphore;

    enum class ResourceBarrierFlags
    {
        None,
        BeginOnly,
        EndOnly
    };

    enum class ResourceState
    {
        Common,
        VertexAndConstantBuffer,
        IndexBuffer,
        RenderTarget,
        UnorderedAccess,
        DepthWrite,
        DepthRead,
        NonPixelShaderResource,
        PixelShaderResource,
        StreamOut,
        IndirectArgument,
        CopyDest,
        CopySource,
        ResolveDest,
        ResolveSource,
        GenericRead,
        Present,
        Predication
    };

    const char* resourceStateToString(ResourceState state);

    static const unsigned int BarrierAllSubresources = 0xffffffff;

    class Barrier
    {
    public:
        void update(
            ResourceState before,
            ResourceState after);

    private:
        friend class CommandList;
        Barrier(
            const CommandList& commandList,
            ResourceBarrierFlags flags,
            const TextureRTV& resource,
            ResourceState before,
            ResourceState after,
            unsigned int subResource,
            const Semaphore& waitSemaphore,
            const Semaphore& signalSemaphore
        );

        PIMPL_FRIEND_ACCESS(BarrierImpl)
    };
}
