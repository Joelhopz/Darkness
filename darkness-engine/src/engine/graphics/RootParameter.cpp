#include "engine/graphics/RootParameter.h"
#include "engine/graphics/SamplerDescription.h"

#if defined(_WIN32) && !defined(VULKAN)
#include "engine/graphics/dx12/DX12RootParameter.h"
#endif

#if defined(VULKAN)
#include "engine/graphics/vulkan/VulkanRootParameter.h"
#endif

#ifdef __APPLE__
#include "engine/graphics/metal/MetalRootParameter.h"
#endif

using namespace tools;
using namespace engine::implementation;

namespace engine
{
    PIMPL_ACCESS_IMPLEMENTATION(RootParameter, RootParameterImpl)

    RootParameter::RootParameter()
        : m_impl{ std::make_shared<RootParameterImpl>() }
    {
    }

    void RootParameter::binding(unsigned int index)
    {
        m_impl->binding(index);
    }

    unsigned int RootParameter::binding() const
    {
        return m_impl->binding();
    }

    void RootParameter::visibility(ShaderVisibility visibility)
    {
        m_impl->visibility(visibility);
    }

    ShaderVisibility RootParameter::visibility() const
    {
        return m_impl->visibility();
    }

    void RootParameter::initAsConstants(unsigned int reg, unsigned int num32BitValues, ShaderVisibility visibility)
    {
        m_impl->initAsConstants(reg, num32BitValues, visibility);
    }

    void RootParameter::initAsCBV(unsigned int reg, ShaderVisibility visibility)
    {
        m_impl->initAsCBV(reg, visibility);
    }

    void RootParameter::initAsSRV(unsigned int reg, ShaderVisibility visibility)
    {
        m_impl->initAsSRV(reg, visibility);
    }

    void RootParameter::initAsUAV(unsigned int reg, ShaderVisibility visibility)
    {
        m_impl->initAsUAV(reg, visibility);
    }

    void RootParameter::initAsDescriptorRange(DescriptorRangeType type, unsigned int reg, unsigned int count, ShaderVisibility visibility)
    {
        m_impl->initAsDescriptorRange(type, reg, count, visibility);
    }

    void RootParameter::initAsDescriptorTable(unsigned int rangeCount, ShaderVisibility visibility)
    {
        m_impl->initAsDescriptorTable(rangeCount, visibility);
    }

    void RootParameter::setTableRange(unsigned int rangeIndex, DescriptorRangeType type, unsigned int reg, unsigned int count, unsigned int space)
    {
        m_impl->setTableRange(rangeIndex, type, reg, count, space);
    }

}
