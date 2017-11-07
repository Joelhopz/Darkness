#pragma once

#include "VulkanHeaders.h"

namespace engine
{
    struct SamplerDescription;
    class Device;
    class DescriptorHandle;
    
    namespace implementation
    {
        class SamplerImpl
        {
        public:
            SamplerImpl(
                const Device& device, 
                const DescriptorHandle& heap, 
                const SamplerDescription& desc);

            SamplerImpl(
                const Device& device, 
                const SamplerDescription& desc);
            
            const VkSampler& native() const;
            VkSampler& native();
        private:
            const Device& m_device;
            std::shared_ptr<VkSampler> m_sampler{ nullptr };
        };
    }
}

