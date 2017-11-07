#pragma once

#include "VulkanHeaders.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/RootParameter.h"
#include "engine/graphics/RootSignature.h"
#include <memory>
#include <vector>

namespace engine
{
    class Device;
    namespace implementation
    {
        class RootSignatureImpl
        {
        public:
            RootSignatureImpl(const Device& device, int rootParameterCount = 0, int staticSamplerCount = 0);
            
            void reset(int rootParameterCount, int staticSamplerCount);
            void initStaticSampler(int samplerNum, const SamplerDescription& description, ShaderVisibility visibility);
            void finalize(RootSignatureFlags flags = RootSignatureFlags::None);
            size_t rootParameterCount() const;
            RootParameter& operator[](int index);
            const RootParameter& operator[](int index) const;
            
            RootSignatureImpl(const RootSignatureImpl&) = delete;
            RootSignatureImpl(RootSignatureImpl&&) = delete;
            RootSignatureImpl& operator=(const RootSignatureImpl&) = delete;
            RootSignatureImpl& operator=(RootSignatureImpl&&) = delete;
            
            VkDescriptorSetLayout& layout();
            const VkDescriptorSetLayout& layout() const;
        private:
            const Device& m_device;
            
            std::vector<RootParameter> m_parameters;
            std::unique_ptr<StaticSamplerDescription[]> m_samplers;
            std::shared_ptr<VkDescriptorSetLayout> m_descriptorSetLayout;

            
            int m_parameterCount;
            int m_samplerCount;
            size_t m_numInitializedStaticSamplers;
            
            uint32_t m_descriptorTableBitMap;
            uint32_t m_descriptorTableSize[16];
            unsigned int m_maxDescriptorCacheHandleCount;
        };
    }
}

