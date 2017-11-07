#include "engine/graphics/vulkan/VulkanRootSignature.h"
#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/vulkan/VulkanRootParameter.h"
#include "engine/graphics/vulkan/VulkanSampler.h"
#include "engine/graphics/vulkan/VulkanDevice.h"
#include "engine/graphics/vulkan/VulkanConversions.h"

#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/RootParameter.h"
#include "engine/graphics/RootSignature.h"
#include "engine/graphics/Device.h"

#include "tools/Debug.h"
#include "tools/ComPtr.h"

namespace engine
{
    namespace implementation
    {
        RootSignatureImpl::RootSignatureImpl(const Device& device, int rootParameterCount, int staticSamplerCount)
            : m_device{ device }
            , m_numInitializedStaticSamplers{ 0 }
            , m_samplers{ nullptr }
            , m_parameterCount{ 0 }
            , m_samplerCount{ 0 }
            , m_descriptorSetLayout{ vulkanPtr<VkDescriptorSetLayout>(DeviceImplGet::impl(m_device).device(), vkDestroyDescriptorSetLayout) }
        {
            reset(rootParameterCount, staticSamplerCount);
        }

        void RootSignatureImpl::reset(int rootParameterCount, int staticSamplerCount)
        {
            m_parameterCount = rootParameterCount;
            m_samplerCount = staticSamplerCount;

            if (rootParameterCount > 0)
                m_parameters.resize(static_cast<size_t>(rootParameterCount));

            if (staticSamplerCount > 0)
                m_samplers.reset(new StaticSamplerDescription[static_cast<size_t>(staticSamplerCount)]);
            else
                m_samplers = nullptr;
        }

        VkDescriptorSetLayout& RootSignatureImpl::layout()
        {
            return *m_descriptorSetLayout;
        }

        const VkDescriptorSetLayout& RootSignatureImpl::layout() const
        {
            return *m_descriptorSetLayout;
        }


        void RootSignatureImpl::initStaticSampler(int samplerNum, const SamplerDescription& description, ShaderVisibility visibility)
        {
            StaticSamplerDescription& staticSamplerDesc = m_samplers[m_numInitializedStaticSamplers++];
            staticSamplerDesc.desc.filter = description.desc.filter;
            staticSamplerDesc.desc.addressU = description.desc.addressU;
            staticSamplerDesc.desc.addressV = description.desc.addressV;
            staticSamplerDesc.desc.addressW = description.desc.addressW;
            staticSamplerDesc.desc.mipLODBias = description.desc.mipLODBias;
            staticSamplerDesc.desc.maxAnisotrophy = description.desc.maxAnisotrophy;
            staticSamplerDesc.desc.comparisonFunc = description.desc.comparisonFunc;
            staticSamplerDesc.desc.borderColor = StaticBorderColor::OpaqueWhite;
            staticSamplerDesc.desc.minLOD = description.desc.minLOD;
            staticSamplerDesc.desc.maxLOD = description.desc.maxLOD;
            staticSamplerDesc.desc.shaderRegister = static_cast<uint32_t>(samplerNum);
            staticSamplerDesc.desc.registerSpace = 0;
            staticSamplerDesc.desc.shaderVisibility = visibility;

            if (staticSamplerDesc.desc.addressU == TextureAddressMode::Border ||
                staticSamplerDesc.desc.addressV == TextureAddressMode::Border ||
                staticSamplerDesc.desc.addressW == TextureAddressMode::Border)
            {
                if (description.desc.borderColor[3] == 1.0f)
                {
                    if (description.desc.borderColor[0] == 1.0f)
                        staticSamplerDesc.desc.borderColor = StaticBorderColor::OpaqueWhite;
                    else
                        staticSamplerDesc.desc.borderColor = StaticBorderColor::OpaqueBlack;
                }
                else
                    staticSamplerDesc.desc.borderColor = StaticBorderColor::TransparentBlack;
            }
        }

        void RootSignatureImpl::finalize(RootSignatureFlags /*flags*/)
        {
            ASSERT(static_cast<int>(m_numInitializedStaticSamplers) == m_samplerCount);

            std::vector<VkDescriptorSetLayoutBinding> bindings;
            for (auto& parameter : m_parameters)
            {
                bindings.emplace_back(RootParameterImplGet::impl(parameter).layoutBinding());
            }
            
            VkDescriptorSetLayoutCreateInfo layoutInfo = {};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();

            auto result = vkCreateDescriptorSetLayout(DeviceImplGet::impl(m_device).device(), &layoutInfo, nullptr, m_descriptorSetLayout.get());
            ASSERT(result == VK_SUCCESS);

        }

        size_t RootSignatureImpl::rootParameterCount() const
        {
            return static_cast<size_t>(m_parameterCount);
        }

        RootParameter& RootSignatureImpl::operator[](int index)
        {
            ASSERT(index >= 0 && index < m_parameterCount);
            return m_parameters[static_cast<size_t>(index)];
        }

        const RootParameter& RootSignatureImpl::operator[](int index) const
        {
            ASSERT(index >= 0 && index < m_parameterCount);
            return m_parameters[static_cast<size_t>(index)];
        }

    }
}
