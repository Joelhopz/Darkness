#include "engine/graphics/Fence.h"
#include "engine/graphics/Device.h"

#if defined(_WIN32) && !defined(VULKAN)
#include "engine/graphics/dx12/DX12Fence.h"
#endif

#if defined(VULKAN)
#include "engine/graphics/vulkan/VulkanFence.h"
#endif

#ifdef __APPLE__
#include "engine/graphics/metal/MetalFence.h"
#endif


using namespace tools;
using namespace engine::implementation;

namespace engine
{
    PIMPL_ACCESS_IMPLEMENTATION(Fence, FenceImpl)

    Fence::Fence(const Device& device)
        : m_impl{ make_unique_impl<FenceImpl>(DeviceImplGet::impl(device)) }
    {
    }

    void Fence::reset()
    {
        m_impl->reset();
    }

    bool Fence::signaled() const
    {
        return m_impl->signaled();
    }

    void Fence::blockUntilSignaled()
    {
        m_impl->blockUntilSignaled();
    }

    void Fence::blockUntilSignaled(unsigned long long value)
    {
        m_impl->blockUntilSignaled(value);
    }

    unsigned long long Fence::value() const
    {
        return m_impl->value();
    }
}
