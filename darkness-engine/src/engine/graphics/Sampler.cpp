#include "engine/graphics/Sampler.h"

#if defined(_WIN32) && !defined(VULKAN)
#include "engine/graphics/dx12/DX12Sampler.h"
#endif

#if defined(VULKAN)
#include "engine/graphics/vulkan/VulkanSampler.h"
#endif

#ifdef __APPLE__
#include "engine/graphics/metal/MetalSampler.h"
#endif

using namespace tools;
using namespace engine::implementation;

namespace engine
{
    PIMPL_ACCESS_IMPLEMENTATION_BUFFER(Sampler, SamplerImpl)

    Sampler::Sampler(const Device& device, const SamplerDescription& desc)
        : m_impl{ std::move(tools::make_impl<SamplerImpl>(device, desc)) }
    {
    }

    bool Sampler::valid() const { return m_impl.get(); };
}
