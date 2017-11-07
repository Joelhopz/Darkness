#include "engine/graphics/vulkan/VulkanDescriptorHandle.h"
#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/vulkan/VulkanDevice.h"

#include "engine/graphics/Device.h"
#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        DescriptorHandleImpl::DescriptorHandleImpl(VkDescriptorSet descriptor)
            : m_descriptor{ descriptor }
        {
        }
        
        DescriptorHandleType DescriptorHandleImpl::type() const
        {
            return m_type;
        }

        DescriptorHandleImpl& DescriptorHandleImpl::operator++()
        {
            return *this;
        }

        DescriptorHandleImpl& DescriptorHandleImpl::operator++(int)
        {
            return *this;
        }

        DescriptorHandleImpl& DescriptorHandleImpl::operator+=(int /*count*/)
        {
            return *this;
        }

        VkDescriptorSet& DescriptorHandleImpl::native()
        {
            return m_descriptor;
        }

        const VkDescriptorSet& DescriptorHandleImpl::native() const
        {
            return m_descriptor;
        }
    }
}
