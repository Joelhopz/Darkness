#include "engine/graphics/RootSignature.h"
#include "engine/graphics/Device.h"
#include "tools/Debug.h"

#if defined(_WIN32) && !defined(VULKAN)
#include "engine/graphics/dx12/DX12RootSignature.h"
#endif

#if defined(VULKAN)
#include "engine/graphics/vulkan/VulkanRootSignature.h"
#endif

#ifdef __APPLE__
#include "engine/graphics/metal/MetalRootSignature.h"
#endif

using namespace tools;
using namespace engine::implementation;

namespace engine
{
    PIMPL_ACCESS_IMPLEMENTATION(RootSignature, RootSignatureImpl)

    RootSignature::RootSignature(const Device& device, int rootParameterCount, int staticSamplerCount)
        : m_impl{ make_unique_impl<RootSignatureImpl>(device, rootParameterCount, staticSamplerCount) }
    {
    }

    void RootSignature::reset(int rootParameterCount, int staticSamplerCount)
    {
        m_impl->reset(rootParameterCount, staticSamplerCount);
    }

    void RootSignature::initStaticSampler(
        int samplerNum,
        const SamplerDescription& description,
        ShaderVisibility visibility)
    {
        m_impl->initStaticSampler(samplerNum, description, visibility);
    }

    void RootSignature::finalize(RootSignatureFlags flags)
    {
        m_impl->finalize(flags);
    }

    size_t RootSignature::rootParameterCount() const
    {
        return m_impl->rootParameterCount();
    }

    RootParameter& RootSignature::operator[](int index)
    {
        return (*m_impl)[index];
    }

    const RootParameter& RootSignature::operator[](int index) const
    {
        return (*m_impl)[index];
    }
}
