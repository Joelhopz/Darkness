#include "engine/graphics/vulkan/VulkanCommandList.h"
#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/vulkan/VulkanSwapChain.h"
#include "engine/graphics/vulkan/VulkanDevice.h"
#include "engine/graphics/vulkan/VulkanCommandAllocator.h"
#include "engine/graphics/vulkan/VulkanPipeline.h"
#include "engine/graphics/vulkan/VulkanDescriptorHandle.h"
#include "engine/graphics/vulkan/VulkanResources.h"
#include "engine/graphics/vulkan/VulkanConversions.h"


#include "engine/graphics/Device.h"
#include "engine/graphics/Pipeline.h"
#include "engine/graphics/Barrier.h"
#include "engine/graphics/SwapChain.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/Viewport.h"

#include "engine/primitives/Color.h"
#include "engine/graphics/vulkan/VulkanQueue.h"
#include "engine/graphics/Queue.h"

#include "tools/Debug.h"
#include <array>

namespace engine
{
    namespace implementation
    {
        CommandListImpl::CommandListImpl(DeviceImpl* device)
            : m_device{ device }
            , m_clearColor{ 0.0f, 0.0f, 0.0f, 1.0f }
            , m_allocator{ device->createCommandAllocator() }
            , m_renderPassBeginInfo{}
        {
            m_renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

            VkCommandBufferAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = m_allocator->native();
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = 1;
            auto result = vkAllocateCommandBuffers(device->device(), &allocInfo, &m_commandList);
            ASSERT(result == VK_SUCCESS);

            begin();
        }

        VkCommandBuffer& CommandListImpl::native()
        {
            return m_commandList;
        }

        const VkCommandBuffer& CommandListImpl::native() const
        {
            return m_commandList;
        }

        void CommandListImpl::reset(implementation::PipelineImpl& /*pipelineState*/)
        {
            auto result = vkResetCommandBuffer(m_commandList, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
            ASSERT(result == VK_SUCCESS);
        }

        VkAccessFlags vulkanAccessFlags(ResourceState state)
        {
            switch (state)
            {
            case ResourceState::Common: return VK_ACCESS_SHADER_READ_BIT;
            case ResourceState::VertexAndConstantBuffer: return VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT | VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_UNIFORM_READ_BIT;
            case ResourceState::IndexBuffer: return VK_ACCESS_INDEX_READ_BIT;
            case ResourceState::RenderTarget: return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            case ResourceState::UnorderedAccess: return VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT;
            case ResourceState::DepthWrite: return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            case ResourceState::DepthRead: return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
            case ResourceState::NonPixelShaderResource: return VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT;
            case ResourceState::PixelShaderResource: return VK_ACCESS_SHADER_READ_BIT;
            case ResourceState::StreamOut: return VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_READ_BIT;
            case ResourceState::IndirectArgument: return VK_ACCESS_INDEX_READ_BIT;
            case ResourceState::CopyDest: return VK_ACCESS_TRANSFER_WRITE_BIT;
            case ResourceState::CopySource: return VK_ACCESS_TRANSFER_READ_BIT;
            case ResourceState::ResolveDest: return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            case ResourceState::ResolveSource: return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            case ResourceState::GenericRead: return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            case ResourceState::Present: return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            case ResourceState::Predication: return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            }
            return VK_ACCESS_SHADER_READ_BIT;
        }

        void CommandListImpl::transition(Texture& resource, ResourceState state)
        {
            TextureImpl* impl = TextureImplGet::impl(resource);
            if (impl->state() == state)
                return;

            VkImageMemoryBarrier barrier = {};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout = vulkanResourceStates(impl->state());
            barrier.newLayout = vulkanResourceStates(state);
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = TextureImplGet::impl(resource)->native();
            barrier.srcAccessMask = vulkanAccessFlags(impl->state());
            barrier.dstAccessMask = vulkanAccessFlags(state);
            barrier.subresourceRange.aspectMask = vulkanFormatAspects(resource.format());
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;

            vkCmdPipelineBarrier(
                m_commandList,
                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                0,
                0, nullptr,
                0, nullptr,
                1, &barrier
            );

            impl->state(state);
        }

        void CommandListImpl::transition(TextureRTV& resource, ResourceState state)
        {
            transition(resource.texture(), state);
        }

        void CommandListImpl::transition(TextureSRV& resource, ResourceState state)
        {
            transition(resource.texture(), state);
        }

        void CommandListImpl::transition(TextureDSV& resource, ResourceState state)
        {
            transition(resource.texture(), state);
        }

        void CommandListImpl::transition(Buffer& resource, ResourceState state)
        {
            BufferImpl* impl = BufferImplGet::impl(resource);
            if (impl->state() == state)
                return;

            VkBufferMemoryBarrier barrier = {};
            barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.srcAccessMask = vulkanAccessFlags(impl->state());
            barrier.dstAccessMask = vulkanAccessFlags(state);
            barrier.buffer = BufferImplGet::impl(resource)->native();

            vkCmdPipelineBarrier(
                m_commandList,
                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                0,
                0, nullptr,
                1, &barrier,
                0, nullptr
            );

            impl->state(state);
        }

        void CommandListImpl::transition(BufferSRV& resource, ResourceState state)
        {
            transition(resource.buffer(), state);
        }

        void CommandListImpl::transition(BufferIBV& resource, ResourceState state)
        {
            transition(resource.buffer(), state);
        }

        void CommandListImpl::transition(BufferCBV& resource, ResourceState state)
        {
            transition(resource.buffer(), state);
        }

        void CommandListImpl::transition(BufferVBV& resource, ResourceState state)
        {
            transition(resource.buffer(), state);
        }

        void CommandListImpl::setResourceBarrier(VkSubmitInfo barrier) const
        {
            m_barrier = barrier;
        }

        VkSubmitInfo& CommandListImpl::barrier()
        {
            return m_barrier;
        }

        const VkSubmitInfo& CommandListImpl::barrier() const
        {
            return m_barrier;
        }

        void CommandListImpl::setViewPorts(const std::vector<Viewport>& viewports)
        {
            for (auto&& viewport : viewports)
            {
                m_renderPassBeginInfo.renderArea.offset = { static_cast<int32_t>(viewport.topLeftX), static_cast<int32_t>(viewport.topLeftY) };
                m_renderPassBeginInfo.renderArea.extent = { static_cast<uint32_t>(viewport.width), static_cast<uint32_t>(viewport.height) };
            }
        }

        void CommandListImpl::setScissorRects(const std::vector<Rectangle>& rects)
        {

        }

        void CommandListImpl::setRenderTargets(std::vector<TextureRTV> /*targets*/)
        {
            /*D3D12_CPU_DESCRIPTOR_HANDLE* handles = new D3D12_CPU_DESCRIPTOR_HANDLE[targets.size()];
            for (size_t i = 0; i < targets.size(); ++i)
            {
                handles[i] = DescriptorHandleImplGet::impl(targets[i]).cpuHandle();
            }
            m_CommandList->OMSetRenderTargets(static_cast<UINT>(targets.size()), handles, FALSE, NULL);
            delete[] handles;*/
        }

        void CommandListImpl::setRenderTargets(std::vector<TextureRTV> targets, TextureDSV& dsv)
        {

        }

        void CommandListImpl::clearRenderTargetView(const TextureRTV& /*target*/, const Color4f& /*color*/)
        {
            //m_CommandList->ClearRenderTargetView(DescriptorHandleImplGet::impl(target).cpuHandle(), color.values, 0, NULL);
        }

        void CommandListImpl::clearDepthStencilView(const TextureDSV& target, float depth, uint8_t stencil)
        {

        }

        void CommandListImpl::beginRenderPass(implementation::PipelineImpl& pipeline, int frameBufferIndex)
        {
            //auto swapChain = m_device->currentSwapChain().lock();
            //ASSERT(swapChain);

            
            m_renderPassBeginInfo.renderPass = *pipeline.m_renderPass;
            m_renderPassBeginInfo.framebuffer = pipeline.m_framebuffers[static_cast<size_t>(frameBufferIndex)];
            /*m_renderPassBeginInfo.renderArea.offset = { 0, 0 };
            m_renderPassBeginInfo.renderArea.extent = SwapChainImplGet::impl(*swapChain).extent();*/

            std::array<VkClearValue, 1> clearValues = {};
            clearValues[0] = m_clearColor;
            //clearValues[1].depthStencil = { 1.0f, 0 };

            m_renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            m_renderPassBeginInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(m_commandList, &m_renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
        }

        void CommandListImpl::begin()
        {
            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = 0;// VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(m_commandList, &beginInfo);
        }

        void CommandListImpl::copyBuffer(
            const Buffer& srcBuffer,
            Buffer& dstBuffer,
            uint64_t elements,
            uint32_t srcStartElement,
            uint32_t dstStartElement)
        {
            VkBufferCopy copyRegion = {};
            copyRegion.size = static_cast<VkDeviceSize>(elements * srcBuffer.description().descriptor.elementSize);
            copyRegion.srcOffset = srcStartElement * srcBuffer.description().descriptor.elementSize;
            copyRegion.dstOffset = dstStartElement * dstBuffer.description().descriptor.elementSize;

            vkCmdCopyBuffer(
                m_commandList, 
                BufferImplGet::impl(srcBuffer)->native(), 
                BufferImplGet::impl(dstBuffer)->native(), 
                1, 
                &copyRegion);
        }

        void CommandListImpl::bindPipe(implementation::PipelineImpl& pipelineImpl,
            shaders::PipelineConfiguration* configuration)
        {
            pipelineImpl.configure(*this, configuration);
            pipelineImpl.finalize();
            vkCmdBindPipeline(
                m_commandList,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                *pipelineImpl.m_pipeline);

            beginRenderPass(pipelineImpl, 0);
            /*bindDescriptorSets(
                pipelineImpl,
                tools::make_impl<DescriptorHandleImpl>(
                    pipelineImpl.descriptorSet()
                ));*/
        }

        void CommandListImpl::bindVertexBuffer(const BufferVBV& buffer)
        {
            VkBuffer vertexBuffers[] = { BufferImplGet::impl(buffer.buffer())->native() };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(m_commandList, 0, 1, vertexBuffers, offsets);
        }

        void CommandListImpl::bindIndexBuffer(const BufferIBV& buffer)
        {
            vkCmdBindIndexBuffer(
                m_commandList, 
                BufferImplGet::impl(buffer.buffer())->native(),
                0,
                VK_INDEX_TYPE_UINT32);
        }

        void CommandListImpl::bindDescriptorSets(implementation::PipelineImpl& pipeline, const tools::impl_ptr<DescriptorHandleImpl> descriptor)
        {
            vkCmdBindDescriptorSets(
                m_commandList,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                *pipeline.m_pipelineLayout,
                0, 1,
                &descriptor->native(),
                0, nullptr);
        }

        void CommandListImpl::clearBuffer(Buffer& buffer, uint32_t value, uint32_t startElement, uint32_t numElements)
        {
            vkCmdFillBuffer(
                m_commandList,
                BufferImplGet::impl(buffer)->native(),
                startElement * buffer.description().descriptor.elementSize,
                numElements * buffer.description().descriptor.elementSize,
                value);
        }

        void CommandListImpl::clearTexture(
            Texture& texture, 
            const Color4f& color,
            const SubResource& subResource)
        {
            VkClearColorValue clearColor;
            float r = color.red();
            float g = color.green();
            float b = color.blue();
            float a = color.alpha();

            clearColor.float32[0] = r;
            clearColor.float32[1] = g;
            clearColor.float32[2] = b;
            clearColor.float32[3] = a;

            clearColor.int32[0] = *reinterpret_cast<int32_t*>(&r);
            clearColor.int32[1] = *reinterpret_cast<int32_t*>(&g);
            clearColor.int32[2] = *reinterpret_cast<int32_t*>(&b);
            clearColor.int32[3] = *reinterpret_cast<int32_t*>(&a);

            clearColor.uint32[0] = *reinterpret_cast<uint32_t*>(&r);
            clearColor.uint32[1] = *reinterpret_cast<uint32_t*>(&g);
            clearColor.uint32[2] = *reinterpret_cast<uint32_t*>(&b);
            clearColor.uint32[3] = *reinterpret_cast<uint32_t*>(&a);

            auto slices = subResource.arraySliceCount == AllArraySlices ? texture.description().descriptor.arraySlices : subResource.arraySliceCount;
            auto mips = subResource.mipCount == AllMipLevels ? texture.description().descriptor.mipLevels : subResource.mipCount;

            VkImageSubresourceRange subResourceRange[1];
            subResourceRange[0].baseArrayLayer = subResource.firstArraySlice;
            subResourceRange[0].baseMipLevel = subResource.firstMipLevel;
            subResourceRange[0].layerCount = slices;
            subResourceRange[0].levelCount = mips;
            subResourceRange[0].aspectMask = vulkanFormatAspects(texture.format());

            //vkCmdClearDepthStencilImage()
            //vkCmdClearAttachments
            

            vkCmdClearColorImage(
                m_commandList,
                TextureImplGet::impl(texture)->native(),
                VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                &clearColor,
                1, subResourceRange
            );
        }

        void CommandListImpl::draw(uint32_t vertexCount)
        {
            /*VkDrawIndirectCommand cmd;
            cmd.firstInstance = 0;
            cmd.firstVertex = 0;
            cmd.instanceCount = 1;
            cmd.vertexCount = 3;


            vkCmdDrawIndirect(
                m_commandList,

            );*/

            vkCmdDraw(
                m_commandList,
                vertexCount,
                1,
                0,
                0);
        }

        void CommandListImpl::drawIndexed(
            uint32_t indexCount, 
            uint32_t instanceCount,
            uint32_t firstIndex, 
            int32_t vertexOffset, 
            uint32_t firstInstance)
        {
            vkCmdDrawIndexed(
                m_commandList, 
                indexCount, 
                instanceCount, 
                firstIndex, 
                vertexOffset, 
                firstInstance);
        }

        void CommandListImpl::dispatch(
            uint32_t threadGroupCountX,
            uint32_t threadGroupCountY,
            uint32_t threadGroupCountZ)
        {

        }

        void CommandListImpl::executeIndirect(
            Buffer& argumentBuffer,
            uint64_t argumentBufferOffset)
        {

        }

        void CommandListImpl::endRenderPass()
        {
            vkCmdEndRenderPass(m_commandList);
        }

        void CommandListImpl::end()
        {
            auto result = vkEndCommandBuffer(m_commandList);
            ASSERT(result == VK_SUCCESS);
        }

        VkAccessFlags getAccessFlagsFromLayout(VkImageLayout layout)
        {
            switch (layout)
            {
            case VK_IMAGE_LAYOUT_UNDEFINED: return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            case VK_IMAGE_LAYOUT_GENERAL: return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL: return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL: return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            case VK_IMAGE_LAYOUT_PREINITIALIZED: return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
            }
            return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
        }

        /*
        VK_ACCESS_INDIRECT_COMMAND_READ_BIT = 0x00000001,
        VK_ACCESS_INDEX_READ_BIT = 0x00000002,
        VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT = 0x00000004,
        VK_ACCESS_UNIFORM_READ_BIT = 0x00000008,
        VK_ACCESS_INPUT_ATTACHMENT_READ_BIT = 0x00000010,
        VK_ACCESS_SHADER_READ_BIT = 0x00000020,
        VK_ACCESS_SHADER_WRITE_BIT = 0x00000040,
        VK_ACCESS_COLOR_ATTACHMENT_READ_BIT = 0x00000080,
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT = 0x00000100,
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT = 0x00000200,
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT = 0x00000400,
        VK_ACCESS_TRANSFER_READ_BIT = 0x00000800,
        VK_ACCESS_TRANSFER_WRITE_BIT = 0x00001000,
        VK_ACCESS_HOST_READ_BIT = 0x00002000,
        VK_ACCESS_HOST_WRITE_BIT = 0x00004000,
        VK_ACCESS_MEMORY_READ_BIT = 0x00008000,
        VK_ACCESS_MEMORY_WRITE_BIT = 0x00010000,
        VK_ACCESS_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
        */

        /*void CommandListImpl::transitionTexture(const Texture& image, ImageLayout from, ImageLayout to)
        {
            auto oldLayout = vulkanImageLayout(from);
            auto newLayout = vulkanImageLayout(to);

            VkImageMemoryBarrier barrier = {};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout = oldLayout;
            barrier.newLayout = newLayout;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = TextureImplGet::impl(image)->native();
            barrier.subresourceRange.aspectMask = vulkanFormatAspects(image.format());
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;*/

            /*VkSubresourceLayout layout;
            VkImageSubresource subResource;
            subResource.arrayLayer = 0;
            subResource.mipLevel = 0;
            subResource.aspectMask = vulkanFormatAspects(image.format());
            vkGetImageSubresourceLayout(
                DeviceImplGet::impl(m_device).device(),
                TextureImplGet::impl(image)->native(),
                &subResource,
                &layout);*/

            /*if (oldLayout == VK_IMAGE_LAYOUT_PREINITIALIZED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
                barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
                barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            }
            else if (oldLayout == VK_IMAGE_LAYOUT_PREINITIALIZED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
                barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
                barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            }
            else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
                barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            }
            else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
                barrier.srcAccessMask = 0;
                barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            }
            else {
                throw std::invalid_argument("unsupported layout transition!");
            }*/

            /*switch (oldLayout) {
            case VK_IMAGE_LAYOUT_PREINITIALIZED:
                barrier.srcAccessMask =
                    VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
                break;
            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
                barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                break;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
                barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                break;
            case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
                barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                break;
            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
                barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                break;
            case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
                barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            }

            switch (newLayout) {
            case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
                barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                break;
            case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
                barrier.srcAccessMask |= VK_ACCESS_TRANSFER_READ_BIT;
                barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                break;
            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
                barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                break;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
                barrier.dstAccessMask |=
                    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                break;
            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
                barrier.srcAccessMask =
                    VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
                barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                break;
            }*/

            // Undefined layout:
            //   Note: Only allowed as initial layout!
            //   Note: Make sure any writes to the image have been finished
         /*   if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED)
                barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;

            // Old layout is color attachment:
            //   Note: Make sure any writes to the color buffer have been finished
            if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
                barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            // Old layout is transfer source:
            //   Note: Make sure any reads from the image have been finished
            if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
                barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            // Old layout is shader read (sampler, input attachment):
            //   Note: Make sure any shader reads from the image have been finished
            if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
                barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;

            // New layout is transfer destination (copy, blit):
            //   Note: Make sure any copyies to the image have been finished
            if (newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
                barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            // New layout is transfer source (copy, blit):
            //   Note: Make sure any reads from and writes to the image have been finished
            if (newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
                barrier.srcAccessMask = barrier.srcAccessMask | VK_ACCESS_TRANSFER_READ_BIT;
                barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            }

            // New layout is color attachment:
            //   Note: Make sure any writes to the color buffer hav been finished
            if (newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
                barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            }

            // New layout is depth attachment:
            //   Note: Make sure any writes to depth/stencil buffer have been finished
            if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
                barrier.dstAccessMask = barrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            // New layout is shader read (sampler, input attachment):
            //   Note: Make sure any writes to the image have been finished
            if (newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
                barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
                barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            }

            vkCmdPipelineBarrier(
                m_commandList,
                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                0,
                0, nullptr,
                0, nullptr,
                1, &barrier
            );
        }*/

        void CommandListImpl::copyTexture(const Texture& src, Texture& dst)
        {
            VkImageSubresourceLayers subResource = {};
            subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            subResource.baseArrayLayer = 0;
            subResource.mipLevel = 0;
            subResource.layerCount = 1;

            VkImageCopy region = {};
            region.srcSubresource = subResource;
            region.dstSubresource = subResource;
            region.srcOffset = { 0, 0, 0 };
            region.dstOffset = { 0, 0, 0 };
            region.extent.width = static_cast<uint32_t>(src.width());
            region.extent.height = static_cast<uint32_t>(src.height());
            region.extent.depth = 1;

            vkCmdCopyImage(
                m_commandList,
                TextureImplGet::impl(src)->native(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                TextureImplGet::impl(dst)->native(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1, &region
            );
        }

        void CommandListImpl::copyTexture(const Texture& src, Buffer& dst)
        {

        }
    }
}
