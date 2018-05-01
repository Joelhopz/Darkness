#pragma once

#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/CommonNoDep.h"
#include <memory>
#include <vector>

namespace engine
{
    namespace shaders
    {
        class PipelineConfiguration;
    }
    class Device;
    class TextureDSV;
    class TextureRTV;
    class BufferView;
    class Buffer;
    class RootSignature;
    struct BlendDescription;
    struct RasterizerDescription;
    struct DepthStencilDescription;
    enum class PrimitiveTopologyType;
    enum class Format;
    struct InputElementDescription;
    enum class IndexBufferStripCutValue;
    class ShaderBinary;
    class CommandList;
    class ShaderBinary;
    class Sampler;
    class TextureSRV;
    class ShaderStorage;

    namespace implementation
    {
        enum class DescriptorType
        {
            CBV_SRV_UAV,
            SAMPLER,
            RTV,
            DSV,
            CombinedImageSampler,
            SampledImage,
            StorageImage,
            UniformTexelBuffer,
            StorageTexelBuffer,
            UniformBuffer,
            StorageBuffer,
            UniformBufferDynamic,
            StorageBufferDynamic,
            InputAttachment
        };

        class CommandListImpl;
        class DescriptorHandleImpl;
        class PipelineImpl
        {
        public:
            PipelineImpl(
                Device& device,
                shaders::PipelineConfiguration* configuration,
                ShaderStorage& storage);
            ~PipelineImpl();
            
            PipelineImpl(const PipelineImpl&) = delete;
            PipelineImpl(PipelineImpl&&) = delete;
            PipelineImpl& operator=(const PipelineImpl&) = delete;
            PipelineImpl& operator=(PipelineImpl&&) = delete;
            
            void setBlendState(const BlendDescription& desc);
            void setRasterizerState(const RasterizerDescription& desc);
            void setDepthStencilState(const DepthStencilDescription& desc);
            void setSampleMask(unsigned int mask);
            void setPrimitiveTopologyType(PrimitiveTopologyType type, bool adjacency = false);
            void setRenderTargetFormat(Format RTVFormat, Format DSVFormat, unsigned int msaaCount = 1, unsigned int msaaQuality = 0);
            void setRenderTargetFormats(std::vector<Format> RTVFormats, Format DSVFormat, unsigned int msaaCount = 1, unsigned int msaaQuality = 0);
            void setInputLayout(unsigned int numElements, const InputElementDescription* inputElementDescs);
            void setPrimitiveRestart(IndexBufferStripCutValue value);
            void setDepthBufferView(std::shared_ptr<TextureDSV> view);

            void setUniformBuffer(const Buffer& buffer);
            void setTextureSRV(const TextureSRV& srv);
            void setSampler(const Sampler& sampler);

            void configure(CommandListImpl& commandList, shaders::PipelineConfiguration* configuration);

            void finalize();
            
            void* native() const;
            const VkDescriptorSet& descriptorSet() const;
        private:
            friend class CommandListImpl;

            const Device& m_device;
            void* m_pipelineState;
            void* m_pipelineStateDesc;
            shaders::PipelineConfiguration* m_configuration;

            std::shared_ptr<const ShaderBinary> m_vertexShader;
            std::shared_ptr<const ShaderBinary> m_pixelShader;
            std::shared_ptr<const ShaderBinary> m_geometryShader;
            std::shared_ptr<const ShaderBinary> m_hullShader;
            std::shared_ptr<const ShaderBinary> m_domainShader;
            std::shared_ptr<const ShaderBinary> m_computeShader;

            void setVertexShader(const ShaderBinary& shaderBinary);
            void setPixelShader(const ShaderBinary& shaderBinary);
            void setGeometryShader(const ShaderBinary& shaderBinary);
            void setHullShader(const ShaderBinary& shaderBinary);
            void setDomainShader(const ShaderBinary& shaderBinary);
            void setComputeShader(const ShaderBinary& shaderBinary);

            std::unique_ptr<VkPipelineShaderStageCreateInfo> m_vertShaderStageInfo;
            std::unique_ptr<VkPipelineShaderStageCreateInfo> m_fragShaderStageInfo;
            std::unique_ptr<VkPipelineShaderStageCreateInfo> m_geometryShaderStageInfo;
            std::unique_ptr<VkPipelineShaderStageCreateInfo> m_computeShaderStageInfo;
            std::unique_ptr<VkPipelineShaderStageCreateInfo> m_domainShaderStageInfo;
            std::unique_ptr<VkPipelineShaderStageCreateInfo> m_hullShaderStageInfo;

            void createRootSignature();
            void setRootSignature(const RootSignature& signature);

            VkPipelineInputAssemblyStateCreateInfo m_inputAssembly;
            VkViewport m_viewport;
            VkRect2D m_scissor;
            VkPipelineRasterizationStateCreateInfo m_rasterizer;
            VkPipelineMultisampleStateCreateInfo m_multisampling;
            VkPipelineColorBlendAttachmentState m_colorBlendAttachement;
            VkPipelineColorBlendStateCreateInfo m_colorBlending;

            VkPipelineLayoutCreateInfo m_pipelineLayoutInfo;
            VkAttachmentDescription m_colorAttachement;
            VkAttachmentReference m_colorAttachmentRef;
            VkAttachmentDescription m_depthAttachment;
            VkAttachmentReference m_depthAttachmentRef;
            VkSubpassDescription m_subPass;
            VkSubpassDependency m_dependency;
            VkRenderPassCreateInfo m_renderPassInfo;

            std::vector<VkVertexInputBindingDescription> m_bindings;
            std::vector<VkVertexInputAttributeDescription> m_bindingAttributes;
            std::vector<VkAttachmentDescription> m_attachments;

            std::shared_ptr<VkPipelineLayout> m_pipelineLayout;
            std::shared_ptr<VkRenderPass> m_renderPass;
            std::shared_ptr<VkPipeline> m_pipeline;

            std::shared_ptr<TextureDSV> m_depthBufferView;

            //std::vector<std::shared_ptr<VkFramebuffer>> m_framebuffers;
            std::vector<VkFramebuffer> m_framebuffers;
            std::vector<VkDescriptorSetLayout> m_currentLayouts;

            std::vector<InputElementDescription> m_pipelineDescriptions;
            struct PipelineInfo
            {
                std::string name;
                DescriptorType type;
                ShaderVisibility visibility;
            };
            std::vector<PipelineInfo> m_pipelineNames;
            VkDescriptorSet m_descriptorSet;

            //std::shared_ptr<DescriptorHeap> m_descriptorHeap;
            std::shared_ptr<DescriptorHandleImpl> m_uniform;

            std::vector<Buffer> m_uniformBuffers;
            std::vector<TextureSRV> m_srvs;
            std::vector<Sampler> m_samplers;
            std::shared_ptr<RootSignature> m_rootSignature;
        };
    }
}

