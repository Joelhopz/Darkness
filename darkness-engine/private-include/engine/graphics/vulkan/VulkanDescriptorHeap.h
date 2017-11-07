#pragma once

#include <vector>
#include "tools/SmartPimpl.h"
#include "VulkanHeaders.h"

namespace engine
{
    class Device;
    class RootSignature;
    class Buffer;
    class Sampler;
    class BufferView;
    class TextureSRV;
    enum class DescriptorHeapFlags;
    namespace shaders
    {
        class PipelineConfiguration;
    }

    namespace implementation
    {
        class DescriptorHandleImpl;
        enum class DescriptorType;

        class DescriptorHeapImpl
        {
        public:
            DescriptorHeapImpl(
                const Device& device,
                const std::vector<DescriptorType>& type,
                DescriptorHeapFlags flags,
                int maxNumDescriptors = 1);

            DescriptorType type() const;
            tools::impl_ptr<DescriptorHandleImpl> allocate(
                const RootSignature& signature,
                const shaders::PipelineConfiguration& pipelineConfig
                /*, 
                const Buffer& buffer, 
                const TextureSRV& view,
                const Sampler& sampler,
                int count*/
            );

            VkDescriptorPool& native();
        private:
            const Device& m_device;
            const std::vector<DescriptorType>& m_type;
            int m_handlesAvailable;

            std::shared_ptr<VkDescriptorPool> m_heap;
        };
    }
}
