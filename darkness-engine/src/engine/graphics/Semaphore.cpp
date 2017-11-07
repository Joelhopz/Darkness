#include "engine/graphics/Semaphore.h"

#if defined(_WIN32) && !defined(VULKAN)
#include "engine/graphics/dx12/DX12Semaphore.h"
#endif

#if defined(VULKAN)
#include "engine/graphics/vulkan/VulkanSemaphore.h"
#endif

#ifdef __APPLE__
#include "engine/graphics/metal/MetalSemaphore.h"
#endif


using namespace tools;
using namespace engine::implementation;

namespace engine
{
    PIMPL_ACCESS_IMPLEMENTATION(Semaphore, SemaphoreImpl)

    Semaphore::Semaphore(const Device& device)
        : m_impl{ make_unique_impl<SemaphoreImpl>(device) }
    {
    }
}
