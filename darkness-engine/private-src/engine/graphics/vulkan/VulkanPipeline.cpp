#include "engine/graphics/vulkan/VulkanPipeline.h"
#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/vulkan/VulkanShaderBinary.h"
#include "engine/graphics/vulkan/VulkanConversions.h"
#include "engine/graphics/vulkan/VulkanRootSignature.h"
#include "engine/graphics/vulkan/VulkanRootParameter.h"
#include "engine/graphics/vulkan/VulkanDevice.h"
#include "engine/graphics/vulkan/VulkanSwapChain.h"
#include "engine/graphics/vulkan/VulkanResources.h"
#include "engine/graphics/vulkan/VulkanCommandList.h"
#include "engine/graphics/vulkan/VulkanDescriptorHandle.h"
#include "engine/graphics/vulkan/VulkanDescriptorHeap.h"

#include "engine/graphics/ShaderBinary.h"
#include "engine/graphics/Pipeline.h"
#include "engine/graphics/RootSignature.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/SwapChain.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/Sampler.h"
#include "engine/graphics/CommandList.h"
#include "shaders/ShaderTypes.h"
#include "spirv_cross.hpp"
#include "tools/Debug.h"
#include "shaders/ShaderTypes.h"

#include <array>

namespace engine
{
    namespace implementation
    {
        PipelineImpl::PipelineImpl(
            Device& device,
            shaders::PipelineConfiguration* configuration,
            ShaderStorage& storage)
            : m_device{ device }
            , m_pipelineState{ nullptr }
            , m_pipelineStateDesc{ nullptr }
            , m_configuration{ configuration }

            , m_vertexShader{ configuration->hasVertexShader() ? configuration->vertexShader()->load(device, storage) : nullptr }
            , m_pixelShader{ configuration->hasPixelShader() ? configuration->pixelShader()->load(device, storage) : nullptr }
            , m_geometryShader{ configuration->hasGeometryShader() ? configuration->geometryShader()->load(device, storage) : nullptr }
            , m_hullShader{ configuration->hasHullShader() ? configuration->hullShader()->load(device, storage) : nullptr }
            , m_domainShader{ configuration->hasDomainShader() ? configuration->domainShader()->load(device, storage) : nullptr }
            , m_computeShader{ configuration->hasComputeShader() ? configuration->computeShader()->load(device, storage) : nullptr }

            , m_vertShaderStageInfo{ nullptr }
            , m_fragShaderStageInfo{ nullptr }
            , m_geometryShaderStageInfo{ nullptr }
            , m_computeShaderStageInfo{ nullptr }
            , m_domainShaderStageInfo{ nullptr }
            , m_hullShaderStageInfo{ nullptr }
            , m_inputAssembly{}
            , m_viewport{}
            , m_scissor{}
            , m_rasterizer{}
            , m_multisampling{}
            , m_colorBlendAttachement{}
            , m_colorBlending{}
            , m_pipelineLayoutInfo{}
            , m_colorAttachement{}
            , m_colorAttachmentRef{}
            , m_depthAttachment{}
            , m_depthAttachmentRef{}
            , m_subPass{}
            , m_dependency{}
            , m_renderPassInfo{}

            , m_pipelineLayout{ vulkanPtr<VkPipelineLayout>(DeviceImplGet::impl(device).device(), vkDestroyPipelineLayout) }
            , m_renderPass{ vulkanPtr<VkRenderPass>(DeviceImplGet::impl(device).device(), vkDestroyRenderPass) }
            , m_pipeline{ vulkanPtr<VkPipeline>(DeviceImplGet::impl(device).device(), vkDestroyPipeline) }
            , m_depthBufferView{}
        {
            /*auto swapChain = m_device.currentSwapChain().lock();
            ASSERT(swapChain);

            m_viewport = {
                0.0f, 0.0f,
                static_cast<float>(SwapChainImplGet::impl(*swapChain).extent().width),
                static_cast<float>(SwapChainImplGet::impl(*swapChain).extent().height),
                0.0f, 1.0f };

            m_scissor = { { 0, 0 }, SwapChainImplGet::impl(*swapChain).extent() };*/
            LOG("Check viewport and scissor settings");

            m_rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            m_rasterizer.depthClampEnable = VK_FALSE;
            m_rasterizer.rasterizerDiscardEnable = VK_FALSE;
            m_rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
            m_rasterizer.lineWidth = 1.0f;
            m_rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
            m_rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
            //m_rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
            m_rasterizer.depthBiasEnable = VK_FALSE;
            m_rasterizer.depthBiasConstantFactor = 0.0f;
            m_rasterizer.depthBiasClamp = 0.0f;
            m_rasterizer.depthBiasSlopeFactor = 0.0f;

            m_multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            m_multisampling.sampleShadingEnable = VK_FALSE;
            m_multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            m_multisampling.minSampleShading = 1.0f;
            m_multisampling.pSampleMask = nullptr;
            m_multisampling.alphaToCoverageEnable = VK_FALSE;
            m_multisampling.alphaToOneEnable = VK_FALSE;

            m_colorBlendAttachement.colorWriteMask =
                VK_COLOR_COMPONENT_R_BIT |
                VK_COLOR_COMPONENT_G_BIT |
                VK_COLOR_COMPONENT_B_BIT |
                VK_COLOR_COMPONENT_A_BIT;
            m_colorBlendAttachement.blendEnable = VK_FALSE;
            m_colorBlendAttachement.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
            m_colorBlendAttachement.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
            m_colorBlendAttachement.colorBlendOp = VK_BLEND_OP_ADD;
            m_colorBlendAttachement.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            m_colorBlendAttachement.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            m_colorBlendAttachement.alphaBlendOp = VK_BLEND_OP_ADD;

            m_colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            m_colorBlending.logicOpEnable = VK_FALSE;
            m_colorBlending.logicOp = VK_LOGIC_OP_COPY;
            m_colorBlending.attachmentCount = 1;
            m_colorBlending.pAttachments = &m_colorBlendAttachement;
            m_colorBlending.blendConstants[0] = 0.0f;
            m_colorBlending.blendConstants[1] = 0.0f;
            m_colorBlending.blendConstants[2] = 0.0f;
            m_colorBlending.blendConstants[3] = 0.0f;

            // dynamic stuff. that can change midflight with pipeline
            /*VkDynamicState dynamicStates[] = {
            VK_DYNAMIC_STATE_VIEWPORT
            };
            VkPipelineDynamicStateCreateInfo dynamicState = {};
            dynamicState.dynamicStateCount = 1;
            dynamicState.pDynamicStates = dynamicStates;*/

            m_pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            m_pipelineLayoutInfo.setLayoutCount = 0;
            m_pipelineLayoutInfo.pSetLayouts = nullptr;
            m_pipelineLayoutInfo.pushConstantRangeCount = 0;
            m_pipelineLayoutInfo.pPushConstantRanges = nullptr;

            //m_colorAttachement.format = SwapChainImplGet::impl(*swapChain).surfaceFormat().format;
            m_colorAttachement.format = VK_FORMAT_B8G8R8A8_UNORM;
            m_colorAttachement.samples = VK_SAMPLE_COUNT_1_BIT;
            m_colorAttachement.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            m_colorAttachement.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            m_colorAttachement.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            m_colorAttachement.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            m_colorAttachement.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            m_colorAttachement.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            m_colorAttachmentRef.attachment = 0;
            m_colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            m_depthAttachment.format = vulkanFormat(Format::D32_FLOAT);
            m_depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            m_depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            m_depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            m_depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            m_depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            m_depthAttachment.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            m_depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            m_depthAttachmentRef.attachment = 1;
            m_depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            m_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            m_dependency.dstSubpass = 0;
            m_dependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            m_dependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            m_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            m_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        }

        void PipelineImpl::createRootSignature()
        {
#if 0
            InputElementDescription vertDesc;
            vertDesc.alignedByteOffset(sizeof(Vertex))
                .format(Format::R32G32B32_FLOAT)
                .inputSlot(0)
                .inputSlotClass(InputClassification::PerVertexData)
                .offset(offsetof(Vertex, pos));

            InputElementDescription colorDesc;
            colorDesc.alignedByteOffset(sizeof(Vertex))
                .format(Format::R32G32B32_FLOAT)
                .inputSlot(0)
                .inputSlotClass(InputClassification::PerVertexData)
                .offset(offsetof(Vertex, color));

            InputElementDescription uvDesc;
            uvDesc.alignedByteOffset(sizeof(Vertex))
                .format(Format::R32G32_FLOAT)
                .inputSlot(0)
                .inputSlotClass(InputClassification::PerVertexData)
                .offset(offsetof(Vertex, texCoord));

            m_pipelineDescriptions.emplace_back(vertDesc);
            m_pipelineDescriptions.emplace_back(colorDesc);
            m_pipelineDescriptions.emplace_back(uvDesc);

            setInputLayout(static_cast<unsigned int>(
                m_pipelineDescriptions.size()),
                m_pipelineDescriptions.data());

            /*if (m_vertexShader) setVertexShader(*m_vertexShader);
            if (m_pixelShader) setPixelShader(*m_pixelShader);
            if (m_geometryShader) setGeometryShader(*m_geometryShader);
            if (m_hullShader) setHullShader(*m_hullShader);
            if (m_domainShader) setDomainShader(*m_domainShader);
            if (m_computeShader) setComputeShader(*m_computeShader);*/


            m_rootSignature = std::make_shared<RootSignature>(m_device.createRootSignature());
            RootSignature& rootSignature = *m_rootSignature;
            rootSignature.reset(2, 0);
            rootSignature[0].binding(0);
            rootSignature[0].descriptorType(DescriptorType::UniformBuffer);
            rootSignature[0].visibility(static_cast<ShaderVisibility>(ShaderVisibilityBits::Vertex));
            rootSignature[1].binding(1);
            rootSignature[1].descriptorType(DescriptorType::CombinedImageSampler);
            rootSignature[1].visibility(static_cast<ShaderVisibility>(ShaderVisibilityBits::Pixel));
            rootSignature.finalize();
            setRootSignature(rootSignature);
#endif
        }

        const VkDescriptorSet& PipelineImpl::descriptorSet() const
        {
            return m_uniform->native(); // m_descriptorSet
        }
        
        PipelineImpl::~PipelineImpl()
        {
            for (auto&& item : m_framebuffers)
            {
                vkDestroyFramebuffer(DeviceImplGet::impl(m_device).device(), item, nullptr);
            }

            if (m_pipelineStateDesc)
            {
                //delete m_pipelineStateDesc;
                m_pipelineStateDesc = nullptr;
            }
            if (m_pipelineState)
            {
                //m_pipelineState->Release();
                m_pipelineState = nullptr;
            }
        }

        void PipelineImpl::setDepthBufferView(std::shared_ptr<TextureDSV> view)
        {
            m_depthBufferView = view;
        }
        
        void* PipelineImpl::native() const
        {
            return m_pipelineState;
        }
        
        void PipelineImpl::setRootSignature(const RootSignature& signature)
        {
            m_currentLayouts.clear();
            m_currentLayouts.emplace_back(RootSignatureImplGet::impl(signature).layout());
            m_pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            m_pipelineLayoutInfo.setLayoutCount = 1;
            m_pipelineLayoutInfo.pSetLayouts = m_currentLayouts.data();
        }
        
        void PipelineImpl::setBlendState(const BlendDescription& /*desc*/)
        {
            // TODO
        }
        
        void PipelineImpl::setRasterizerState(const RasterizerDescription& desc)
        {
            m_rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            m_rasterizer.depthClampEnable = desc.desc.depthClipEnable ? static_cast<uint32_t>(VK_TRUE) : static_cast<uint32_t>(VK_FALSE);
            m_rasterizer.rasterizerDiscardEnable = VK_FALSE;
            m_rasterizer.polygonMode = vulkanFillMode(desc.desc.fillMode);
            m_rasterizer.lineWidth = 1.0f;
            m_rasterizer.cullMode = vulkanCullMode(desc.desc.cullMode);
            m_rasterizer.frontFace = desc.desc.frontCounterClockwise ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
            m_rasterizer.depthBiasEnable = desc.desc.depthBias > 0 ? static_cast<uint32_t>(VK_TRUE) : static_cast<uint32_t>(VK_FALSE);
            m_rasterizer.depthBiasConstantFactor = 0.0f;
            m_rasterizer.depthBiasClamp = desc.desc.depthBiasClamp;
            m_rasterizer.depthBiasSlopeFactor = desc.desc.slopeScaledDepthBias;

            m_multisampling.sampleShadingEnable = desc.desc.multisampleEnable ? static_cast<uint32_t>(VK_TRUE) : static_cast<uint32_t>(VK_FALSE);
        }
        
        void PipelineImpl::setDepthStencilState(const DepthStencilDescription& /*desc*/)
        {
            // TODO
        }
        
        void PipelineImpl::setSampleMask(unsigned int /*mask*/)
        {
            // TODO
        }

        void PipelineImpl::setPrimitiveTopologyType(PrimitiveTopologyType type, bool adjacency)
        {
            m_inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            m_inputAssembly.topology = vulkanPrimitiveTopologyType(type);
            m_inputAssembly.primitiveRestartEnable = VK_FALSE;
        }
        
        void PipelineImpl::setRenderTargetFormat(Format RTVFormat, Format DSVFormat, unsigned int msaaCount, unsigned int msaaQuality)
        {
            setRenderTargetFormats({ RTVFormat }, DSVFormat, msaaCount, msaaQuality);
        }
        
        void PipelineImpl::setRenderTargetFormats(std::vector<Format> RTVFormats, Format /*DSVFormat*/, unsigned int /*msaaCount*/, unsigned int /*msaaQuality*/)
        {
            

            // Null format array conflicts with non-zero length
            
            /*for (UINT i = 0; i < numRTVs; ++i)
                m_pipelineStateDesc->RTVFormats[i] = dxFormat(RTVFormats[i]);
            for (UINT i = numRTVs; i < m_pipelineStateDesc->NumRenderTargets; ++i)
                m_pipelineStateDesc->RTVFormats[i] = DXGI_FORMAT_UNKNOWN;
            m_pipelineStateDesc->NumRenderTargets = numRTVs;
            m_pipelineStateDesc->DSVFormat = dxFormat(DSVFormat);
            m_pipelineStateDesc->SampleDesc.Count = msaaCount;
            m_pipelineStateDesc->SampleDesc.Quality = msaaQuality;*/
        }
        
        void PipelineImpl::setInputLayout(unsigned int numElements, const InputElementDescription* inputElementDescs)
        {
            m_bindings.resize(1);
            m_bindingAttributes.resize(numElements);

            for (uint32_t i = 0; i < numElements; ++i)
            {
                if (i == 0)
                {
                    m_bindings[i].binding = i;
                    m_bindings[i].stride = inputElementDescs[i].desc.alignedByteOffset;
                    m_bindings[i].inputRate = vulkanInputClassification(inputElementDescs[i].desc.inputSlotClass); // VK_VERTEX_INPUT_RATE_INSTANCE
                }
                m_bindingAttributes[i].binding = inputElementDescs[i].desc.inputSlot;
                m_bindingAttributes[i].format = vulkanFormat(inputElementDescs[i].desc.format);
                m_bindingAttributes[i].location = i;
                m_bindingAttributes[i].offset = inputElementDescs[i].desc.offset;
            }
        }
        
        void PipelineImpl::setPrimitiveRestart(IndexBufferStripCutValue /*value*/)
        {
            // TODO
        }
        
        void PipelineImpl::setVertexShader(const ShaderBinary& shaderBinary)
        {
            /*auto trColor = TriangleColor();
            auto inst = trColor->construct();
            auto res = inst->resources;


            spirv_cross::Compiler compiler(ShaderBinaryImplGet::impl(shaderBinary).data());
            spirv_cross::ShaderResources resources = compiler.get_shader_resources();

            for (auto &resource : resources.sampled_images)
            {
                unsigned set = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
                unsigned binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
                //add_sampled_image_to_layout(set, binding);
            }*/

            m_vertShaderStageInfo = std::make_unique<VkPipelineShaderStageCreateInfo>();
            m_vertShaderStageInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            m_vertShaderStageInfo->stage = VK_SHADER_STAGE_VERTEX_BIT;
            m_vertShaderStageInfo->module = ShaderBinaryImplGet::impl(shaderBinary).native();
            m_vertShaderStageInfo->pName = "main";
            m_vertShaderStageInfo->pSpecializationInfo = nullptr; // this can be used to add defines and such
        }
        
        void PipelineImpl::setPixelShader(const ShaderBinary& shaderBinary)
        {
            /*spirv_cross::Compiler compiler(ShaderBinaryImplGet::impl(shaderBinary).data());
            spirv_cross::ShaderResources resources = compiler.get_shader_resources();

            for (auto &resource : resources.sampled_images)
            {
                unsigned set = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
                unsigned binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
                //add_sampled_image_to_layout(set, binding);
            }*/

            m_fragShaderStageInfo = std::make_unique<VkPipelineShaderStageCreateInfo>();
            m_fragShaderStageInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            m_fragShaderStageInfo->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            m_fragShaderStageInfo->module = ShaderBinaryImplGet::impl(shaderBinary).native();
            m_fragShaderStageInfo->pName = "main";
            m_fragShaderStageInfo->pSpecializationInfo = nullptr; // this can be used to add defines and such
        }
        
        void PipelineImpl::setGeometryShader(const ShaderBinary& shaderBinary)
        {
            m_geometryShaderStageInfo = std::make_unique<VkPipelineShaderStageCreateInfo>();
            m_geometryShaderStageInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            m_geometryShaderStageInfo->stage = VK_SHADER_STAGE_GEOMETRY_BIT;
            m_geometryShaderStageInfo->module = ShaderBinaryImplGet::impl(shaderBinary).native();
            m_geometryShaderStageInfo->pName = "main";
            m_geometryShaderStageInfo->pSpecializationInfo = nullptr;
        }
        
        void PipelineImpl::setHullShader(const ShaderBinary& shaderBinary)
        {
            m_hullShaderStageInfo = std::make_unique<VkPipelineShaderStageCreateInfo>();
            m_hullShaderStageInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            m_hullShaderStageInfo->stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
            m_hullShaderStageInfo->module = ShaderBinaryImplGet::impl(shaderBinary).native();
            m_hullShaderStageInfo->pName = "main";
            m_hullShaderStageInfo->pSpecializationInfo = nullptr;
        }
        
        void PipelineImpl::setDomainShader(const ShaderBinary& shaderBinary)
        {
            m_domainShaderStageInfo = std::make_unique<VkPipelineShaderStageCreateInfo>();
            m_domainShaderStageInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            m_domainShaderStageInfo->stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
            m_domainShaderStageInfo->module = ShaderBinaryImplGet::impl(shaderBinary).native();
            m_domainShaderStageInfo->pName = "main";
            m_domainShaderStageInfo->pSpecializationInfo = nullptr;
        }

        void PipelineImpl::setComputeShader(const ShaderBinary& shaderBinary)
        {
            m_computeShaderStageInfo = std::make_unique<VkPipelineShaderStageCreateInfo>();
            m_computeShaderStageInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            m_computeShaderStageInfo->stage = VK_SHADER_STAGE_COMPUTE_BIT;
            m_computeShaderStageInfo->module = ShaderBinaryImplGet::impl(shaderBinary).native();
            m_computeShaderStageInfo->pName = "main";
            m_computeShaderStageInfo->pSpecializationInfo = nullptr;
        }

        void PipelineImpl::configure(CommandListImpl& commandList, shaders::PipelineConfiguration* configuration)
        {
            //m_colorAttachement.format = SwapChainImplGet::impl(*swapChain).surfaceFormat().format;
        }
        
        void PipelineImpl::finalize()
        {
            /*InputElementDescription vertDesc;
            vertDesc.alignedByteOffset(sizeof(Vertex))
                .format(Format::R32G32B32_FLOAT)
                .inputSlot(0)
                .inputSlotClass(InputClassification::PerVertexData)
                .offset(offsetof(Vertex, pos));

            InputElementDescription colorDesc;
            colorDesc.alignedByteOffset(sizeof(Vertex))
                .format(Format::R32G32B32_FLOAT)
                .inputSlot(0)
                .inputSlotClass(InputClassification::PerVertexData)
                .offset(offsetof(Vertex, color));

            InputElementDescription uvDesc;
            uvDesc.alignedByteOffset(sizeof(Vertex))
                .format(Format::R32G32_FLOAT)
                .inputSlot(0)
                .inputSlotClass(InputClassification::PerVertexData)
                .offset(offsetof(Vertex, texCoord));

            m_pipelineDescriptions.emplace_back(vertDesc);
            m_pipelineDescriptions.emplace_back(colorDesc);
            m_pipelineDescriptions.emplace_back(uvDesc);

            setInputLayout(static_cast<unsigned int>(
                m_pipelineDescriptions.size()),
                m_pipelineDescriptions.data());*/

            size_t vsTexSrvCount = 0;
            size_t vsTexUavCount = 0;
            size_t vsBufSrvCount = 0;
            size_t vsBufUavCount = 0;
            size_t vsSamplerCount = 0;
            
            size_t psTexSrvCount = 0;
            size_t psTexUavCount = 0;
            size_t psBufSrvCount = 0;
            size_t psBufUavCount = 0;
            size_t psSamplerCount = 0;
            
            if (m_configuration->hasVertexShader())
            {
                setVertexShader(*m_vertexShader);
                vsTexSrvCount = m_configuration->vertexShader()->texture_srvs().size();
                vsTexUavCount = m_configuration->vertexShader()->texture_uavs().size();
                vsBufSrvCount = m_configuration->vertexShader()->buffer_srvs().size();
                vsBufUavCount = m_configuration->vertexShader()->buffer_uavs().size();
                vsSamplerCount = m_configuration->vertexShader()->samplers().size();
            }
            if (m_configuration->hasPixelShader())
            {
                setPixelShader(*m_pixelShader);
                psTexSrvCount = m_configuration->pixelShader()->texture_srvs().size();
                psTexUavCount = m_configuration->pixelShader()->texture_uavs().size();
                psBufSrvCount = m_configuration->pixelShader()->buffer_srvs().size();
                psBufUavCount = m_configuration->pixelShader()->buffer_uavs().size();
                psSamplerCount = m_configuration->vertexShader()->samplers().size();
            }
            if (m_configuration->hasGeometryShader())
                setGeometryShader(*m_geometryShader);
            if (m_configuration->hasHullShader())
                setHullShader(*m_hullShader);
            if (m_configuration->hasDomainShader())
                setDomainShader(*m_domainShader);
            if (m_configuration->hasComputeShader())
                setComputeShader(*m_computeShader);

            size_t resourceCount =
                vsTexSrvCount +
                vsTexUavCount +
                vsBufSrvCount +
                vsBufUavCount +
                vsSamplerCount +
                psTexSrvCount +
                psTexUavCount +
                psBufSrvCount +
                psBufUavCount +
                psSamplerCount;

            m_rootSignature = std::make_shared<RootSignature>(m_device.createRootSignature());
            RootSignature& rootSignature = *m_rootSignature;
            rootSignature.reset(static_cast<int>(resourceCount), 0);
            uint32_t currentIndex = 0;
            for (auto&& res : m_configuration->vertexShader()->texture_srvs())
            {
                rootSignature[currentIndex].binding(currentIndex);
                RootParameterImplGet::impl(rootSignature[currentIndex]).descriptorType(DescriptorType::StorageImage);
                rootSignature[currentIndex].visibility(static_cast<ShaderVisibility>(ShaderVisibilityBits::Vertex));
                ++currentIndex;
            }
            for (auto&& res : m_configuration->vertexShader()->texture_uavs())
            {
                rootSignature[currentIndex].binding(currentIndex);
                RootParameterImplGet::impl(rootSignature[currentIndex]).descriptorType(DescriptorType::StorageImage);
                rootSignature[currentIndex].visibility(static_cast<ShaderVisibility>(ShaderVisibilityBits::Vertex));
                ++currentIndex;
            }
            for (auto&& res : m_configuration->vertexShader()->buffer_srvs())
            {
                rootSignature[currentIndex].binding(currentIndex);
                RootParameterImplGet::impl(rootSignature[currentIndex]).descriptorType(DescriptorType::UniformTexelBuffer);
                rootSignature[currentIndex].visibility(static_cast<ShaderVisibility>(ShaderVisibilityBits::Vertex));
                ++currentIndex;
            }
            for (auto&& res : m_configuration->vertexShader()->buffer_uavs())
            {
                rootSignature[currentIndex].binding(currentIndex);
                RootParameterImplGet::impl(rootSignature[currentIndex]).descriptorType(DescriptorType::UniformTexelBuffer);
                rootSignature[currentIndex].visibility(static_cast<ShaderVisibility>(ShaderVisibilityBits::Vertex));
                ++currentIndex;
            }
            for (auto&& res : m_configuration->pixelShader()->texture_srvs())
            {
                rootSignature[currentIndex].binding(currentIndex);
                RootParameterImplGet::impl(rootSignature[currentIndex]).descriptorType(DescriptorType::StorageImage);
                rootSignature[currentIndex].visibility(static_cast<ShaderVisibility>(ShaderVisibilityBits::Pixel));
                ++currentIndex;
            }
            for (auto&& res : m_configuration->pixelShader()->texture_uavs())
            {
                rootSignature[currentIndex].binding(currentIndex);
                RootParameterImplGet::impl(rootSignature[currentIndex]).descriptorType(DescriptorType::StorageImage);
                rootSignature[currentIndex].visibility(static_cast<ShaderVisibility>(ShaderVisibilityBits::Pixel));
                ++currentIndex;
            }
            for (auto&& res : m_configuration->pixelShader()->buffer_srvs())
            {
                rootSignature[currentIndex].binding(currentIndex);
                RootParameterImplGet::impl(rootSignature[currentIndex]).descriptorType(DescriptorType::UniformTexelBuffer);
                rootSignature[currentIndex].visibility(static_cast<ShaderVisibility>(ShaderVisibilityBits::Pixel));
                ++currentIndex;
            }
            for (auto&& res : m_configuration->pixelShader()->buffer_uavs())
            {
                rootSignature[currentIndex].binding(currentIndex);
                RootParameterImplGet::impl(rootSignature[currentIndex]).descriptorType(DescriptorType::UniformTexelBuffer);
                rootSignature[currentIndex].visibility(static_cast<ShaderVisibility>(ShaderVisibilityBits::Pixel));
                ++currentIndex;
            }
            /*rootSignature[0].binding(0);
            rootSignature[0].descriptorType(DescriptorType::UniformBuffer);
            rootSignature[0].visibility(static_cast<ShaderVisibility>(ShaderVisibilityBits::Vertex));
            rootSignature[1].binding(1);
            rootSignature[1].descriptorType(DescriptorType::CombinedImageSampler);
            rootSignature[1].visibility(static_cast<ShaderVisibility>(ShaderVisibilityBits::Pixel));*/
            rootSignature.finalize();
            setRootSignature(rootSignature);


            m_subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            m_subPass.colorAttachmentCount = 1;
            m_subPass.pColorAttachments = &m_colorAttachmentRef;
            if (m_depthBufferView)
                m_subPass.pDepthStencilAttachment = &m_depthAttachmentRef;
            else
                m_subPass.pDepthStencilAttachment = nullptr;

            m_attachments.emplace_back(m_colorAttachement);
            if (m_depthBufferView)
                m_attachments.emplace_back(m_depthAttachment);

            m_renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            m_renderPassInfo.attachmentCount = static_cast<uint32_t>(m_attachments.size());
            m_renderPassInfo.pAttachments = m_attachments.data();
            m_renderPassInfo.subpassCount = 1;
            m_renderPassInfo.pSubpasses = &m_subPass;
            //m_renderPassInfo.dependencyCount = 1;
            //m_renderPassInfo.pDependencies = &m_dependency;

            //VkPipelineShaderStageCreateInfo shaderStages[] = { *m_vertShaderStageInfo.get(), *m_fragShaderStageInfo.get() };
            std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
            if (m_configuration->hasVertexShader()) shaderStages.emplace_back(*m_vertShaderStageInfo.get());
            if (m_configuration->hasPixelShader()) shaderStages.emplace_back(*m_fragShaderStageInfo.get());
            if (m_configuration->hasGeometryShader()) shaderStages.emplace_back(*m_geometryShaderStageInfo.get());
            if (m_configuration->hasComputeShader()) shaderStages.emplace_back(*m_computeShaderStageInfo.get());
            if (m_configuration->hasDomainShader()) shaderStages.emplace_back(*m_domainShaderStageInfo.get());
            if (m_configuration->hasHullShader()) shaderStages.emplace_back(*m_hullShaderStageInfo.get());

            VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(m_bindings.size());
            vertexInputInfo.pVertexBindingDescriptions = m_bindings.size() > 0 ? m_bindings.data() : nullptr;
            vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_bindingAttributes.size());
            vertexInputInfo.pVertexAttributeDescriptions = m_bindingAttributes.size() > 0 ? m_bindingAttributes.data() : nullptr;

            VkPipelineViewportStateCreateInfo viewportState = {};
            viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            viewportState.viewportCount = 1;
            viewportState.pViewports = &m_viewport;
            viewportState.scissorCount = 1;
            viewportState.pScissors = &m_scissor;

            auto result = vkCreatePipelineLayout(
                DeviceImplGet::impl(m_device).device(),
                &m_pipelineLayoutInfo,
                nullptr,
                m_pipelineLayout.get());
            ASSERT(result == VK_SUCCESS);

            result = vkCreateRenderPass(DeviceImplGet::impl(m_device).device(),
                &m_renderPassInfo, nullptr, m_renderPass.get());
            ASSERT(result == VK_SUCCESS);

            VkPipelineDepthStencilStateCreateInfo depthStencil = {};
            depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            depthStencil.depthTestEnable = VK_TRUE;
            depthStencil.depthWriteEnable = VK_TRUE;
            depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
            depthStencil.depthBoundsTestEnable = VK_FALSE;
            depthStencil.minDepthBounds = 0.0f; // Optional
            depthStencil.maxDepthBounds = 1.0f; // Optional
            depthStencil.stencilTestEnable = VK_FALSE;
            depthStencil.front = {}; // Optional
            depthStencil.back = {}; // Optional

            VkGraphicsPipelineCreateInfo pipelineInfo = {};
            pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
            pipelineInfo.pStages = shaderStages.data();
            pipelineInfo.pVertexInputState = &vertexInputInfo;
            pipelineInfo.pInputAssemblyState = &m_inputAssembly;
            pipelineInfo.pViewportState = &viewportState;
            pipelineInfo.pRasterizationState = &m_rasterizer;
            pipelineInfo.pMultisampleState = &m_multisampling;
            pipelineInfo.pDepthStencilState = &depthStencil;
            pipelineInfo.pColorBlendState = &m_colorBlending;
            pipelineInfo.pDynamicState = nullptr;
            pipelineInfo.layout = *m_pipelineLayout;
            pipelineInfo.renderPass = *m_renderPass;
            pipelineInfo.subpass = 0;
            pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
            pipelineInfo.basePipelineIndex = -1;

            result = vkCreateGraphicsPipelines(
                DeviceImplGet::impl(m_device).device(),
                VK_NULL_HANDLE, // VkPipelineCache object
                1,
                &pipelineInfo,
                nullptr,
                m_pipeline.get());
            ASSERT(result == VK_SUCCESS);


            auto swapChain = m_device.currentSwapChain().lock();
            ASSERT(swapChain);

            m_framebuffers.resize(
                SwapChainImplGet::impl(*swapChain).chainLength()

                // for some reason this causes framebuffer corruption.
                // so we're manually destroying the framebuffers in the destructor
                /*,vulkanPtr<VkFramebuffer>(DeviceImplGet::impl(m_device).device(), vkDestroyFramebuffer)*/
            );


            for (int i = 0; i < static_cast<int>(m_framebuffers.size()); ++i)
            {
                auto rtv = SwapChainImplGet::impl(*swapChain).renderTarget(i);
                
                std::vector<VkImageView> attachments;
                attachments.emplace_back(TextureRTVImplGet::impl(rtv)->native());
                if (m_depthBufferView)
                    attachments.emplace_back(TextureDSVImplGet::impl(*m_depthBufferView)->native());

                VkFramebufferCreateInfo framebufferInfo = {};
                framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                framebufferInfo.renderPass = *m_renderPass;
                framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
                framebufferInfo.pAttachments = attachments.data();
                framebufferInfo.width = SwapChainImplGet::impl(*swapChain).extent().width;
                framebufferInfo.height = SwapChainImplGet::impl(*swapChain).extent().height;
                framebufferInfo.layers = 1;

                auto result = vkCreateFramebuffer(DeviceImplGet::impl(m_device).device(), &framebufferInfo, nullptr, &m_framebuffers[static_cast<size_t>(i)]);
                ASSERT(result == VK_SUCCESS);
            }



            //m_descriptorHeap = m_device.createDescriptorHeap({ DescriptorType::UniformTexelBuffer });
            
            /*m_uniform = std::make_shared<DescriptorHandle>(m_descriptorHeap->allocate(
                *m_rootSignature,
                *m_configuration
                ));*/
        }

        void PipelineImpl::setUniformBuffer(const Buffer& buffer)
        {
            m_uniformBuffers.emplace_back(std::move(buffer));
        }

        void PipelineImpl::setTextureSRV(const TextureSRV& srv)
        {
            m_srvs.emplace_back(srv);
        }

        void PipelineImpl::setSampler(const Sampler& sampler)
        {
            m_samplers.emplace_back(sampler);
        }
    }
}
