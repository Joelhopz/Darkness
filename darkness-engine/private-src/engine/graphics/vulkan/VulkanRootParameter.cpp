#include "engine/graphics/vulkan/VulkanRootParameter.h"
#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/vulkan/VulkanConversions.h"
#include "engine/graphics/vulkan/VulkanDevice.h"

#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/RootParameter.h"
#include "engine/graphics/Device.h"

#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        RootParameterImpl::RootParameterImpl()
            : m_layoutBinding{}
        {
            m_layoutBinding.binding = 0;
            m_layoutBinding.descriptorCount = 1;
            m_layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            m_layoutBinding.pImmutableSamplers = nullptr;
            m_layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        }

        void RootParameterImpl::binding(unsigned int index)
        {
            m_layoutBinding.binding = static_cast<uint32_t>(index);
        }

        unsigned int RootParameterImpl::binding() const
        {
            return static_cast<unsigned int>(m_layoutBinding.binding);
        }

        void RootParameterImpl::visibility(ShaderVisibility visibility)
        {
            m_layoutBinding.stageFlags = vulkanShaderVisibility(visibility);
        }

        const ShaderVisibility RootParameterImpl::visibility() const
        {
            return fromVulkanShaderVisibility(m_layoutBinding.stageFlags);
        }

        void RootParameterImpl::descriptorType(DescriptorType type)
        {
            m_layoutBinding.descriptorType = vulkanDescriptorType(type);
        }

        DescriptorType RootParameterImpl::descriptorType() const
        {
            return fromVulkanDescriptorType(m_layoutBinding.descriptorType);
        }

        const VkDescriptorSetLayoutBinding& RootParameterImpl::layoutBinding() const
        {
            return m_layoutBinding;
        }

        VkDescriptorSetLayoutBinding& RootParameterImpl::layoutBinding()
        {
            return m_layoutBinding;
        }

        void RootParameterImpl::initAsConstants(unsigned int /*reg*/, unsigned int /*num32BitValues*/, ShaderVisibility /*visibility*/)
        {
            // TODO
        }

        void RootParameterImpl::initAsCBV(unsigned int /*reg*/, ShaderVisibility /*visibility*/)
        {
            // TODO
        }

        void RootParameterImpl::initAsSRV(unsigned int /*reg*/, ShaderVisibility /*visibility*/)
        {
            // TODO
        }

        void RootParameterImpl::initAsUAV(unsigned int /*reg*/, ShaderVisibility /*visibility*/)
        {
            // TODO
        }

        void RootParameterImpl::initAsDescriptorRange(DescriptorRangeType /*type*/, unsigned int /*reg*/, unsigned int /*count*/, ShaderVisibility /*visibility*/)
        {
            // TODO
        }

        void RootParameterImpl::initAsDescriptorTable(unsigned int /*rangeCount*/, ShaderVisibility /*visibility*/)
        {
            // TODO
        }

        void RootParameterImpl::setTableRange(unsigned int /*rangeIndex*/, DescriptorRangeType /*type*/, unsigned int /*reg*/, unsigned int /*count*/, unsigned int /*space*/)
        {
            // TODO
        }
    }
}
