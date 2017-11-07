#pragma once

#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "tools/SmartPimpl.h"
#include "tools/Codegen.h"

namespace engine
{
    enum class DescriptorHandleType;
    class Device;

    namespace implementation
    {
        class DescriptorHandleImpl
        {
        public:
            DEFAULT_CONSTRUCTORS(DescriptorHandleImpl)
            DescriptorHandleImpl(VkDescriptorSet descriptor);
            
            DescriptorHandleType type() const;
            VkDescriptorSet& native();
            const VkDescriptorSet& native() const;

            DescriptorHandleImpl& operator++();
            DescriptorHandleImpl& operator++(int);
            DescriptorHandleImpl& operator+=(int count);
        private:
            VkDescriptorSet m_descriptor;
            DescriptorHandleType m_type;
        };
    }
}
