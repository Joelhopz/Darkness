#include "engine/graphics/vulkan/VulkanResources.h"
#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/vulkan/VulkanDevice.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/Barrier.h"

#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        uint32_t findMemoryType(const DeviceImpl& device, uint32_t typeFilter, VkMemoryPropertyFlags properties)
        {
            VkPhysicalDeviceMemoryProperties memProperties;
            vkGetPhysicalDeviceMemoryProperties(device.physicalDevice(), &memProperties);

            for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
                if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                {
                    return i;
                }
            }
            ASSERT(false);
            return 0;
        }

        void updateDescFromInitialData(const BufferDescription& origdesc, BufferDescription::Descriptor& desc)
        {
            auto elements = origdesc.descriptor.elements;
            auto elementSize = origdesc.descriptor.elementSize;
            if (elements == InvalidElementsValue)
            {
                if (origdesc.initialData)
                {
                    elements = origdesc.initialData.elements;
                }
                else ASSERT(false);
            }
            if (elementSize == InvalidElementSizeValue)
            {
                if (origdesc.initialData)
                {
                    elementSize = origdesc.initialData.elementSize;
                }
                else ASSERT(false);
            }
            desc.elements = elements;
            desc.elementSize = elementSize;
        }

        VkBufferUsageFlags vulkanBufferUsageFlagsFromUsage(ResourceUsage usage)
        {
            switch (usage)
            {
            case ResourceUsage::CpuToGpu: return VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            case ResourceUsage::GpuRead: return VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
            case ResourceUsage::GpuReadWrite: return VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
            case ResourceUsage::GpuToCpu: return VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
            case ResourceUsage::GpuRenderTargetRead: return VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
            case ResourceUsage::GpuRenderTargetReadWrite: return VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
            case ResourceUsage::DepthStencil: return VkBufferUsageFlagBits::VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
            default: return VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
            }
        }

        VkMemoryPropertyFlags vulkanMemoryPropertyFlagsFromUsage(ResourceUsage usage)
        {
            switch (usage)
            {
            case ResourceUsage::CpuToGpu: return VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            case ResourceUsage::GpuRead: return VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            case ResourceUsage::GpuReadWrite: return VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            case ResourceUsage::GpuToCpu: return VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            case ResourceUsage::GpuRenderTargetRead: return VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            case ResourceUsage::GpuRenderTargetReadWrite: return VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            case ResourceUsage::DepthStencil: return VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            default: return VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            }
        }

        ResourceState getResourceStateFromUsage(ResourceUsage usage)
        {
            ResourceState resState;
            if (usage == ResourceUsage::CpuToGpu)
            {
                resState = ResourceState::GenericRead;
            }
            else if (usage == ResourceUsage::GpuToCpu)
            {
                resState = ResourceState::CopyDest;
            }
            else if (usage == ResourceUsage::GpuRead)
            {
                resState = ResourceState::CopyDest;
            }
            else if (usage == ResourceUsage::DepthStencil)
            {
                resState = ResourceState::DepthWrite;
            }
            else if (usage == ResourceUsage::GpuRenderTargetReadWrite)
            {
                resState = ResourceState::RenderTarget;
            }
            else if (usage == ResourceUsage::GpuRenderTargetRead)
            {
                resState = ResourceState::RenderTarget;
            }
            else
            {
                resState = ResourceState::Common;
            }
            return resState;
        }

        BufferImpl::BufferImpl(const DeviceImpl& device, const BufferDescription& desc)
            : m_description( desc.descriptor )
            , m_buffer{ vulkanPtr<VkBuffer>(device.device(), vkDestroyBuffer) }
            , m_memory{ vulkanPtr<VkDeviceMemory>(device.device(), vkFreeMemory) }
            , m_state{ getResourceStateFromUsage(m_description.usage) }
        {
            updateDescFromInitialData(desc, m_description);

            const VkDevice& dev = device.device();

            VkBufferCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = static_cast<VkDeviceSize>(m_description.elements * m_description.elementSize);
            bufferInfo.usage = vulkanBufferUsageFlagsFromUsage(m_description.usage);// | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            //bufferInfo.usage = vulkanBufferUsageFlags(m_description.usage) | VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            auto result = vkCreateBuffer(dev, &bufferInfo, nullptr, m_buffer.get());
            ASSERT(result == VK_SUCCESS);

            VkMemoryRequirements memReq;
            vkGetBufferMemoryRequirements(dev, *m_buffer, &memReq);

            VkMemoryAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memReq.size;
            allocInfo.memoryTypeIndex = findMemoryType(
                device,
                memReq.memoryTypeBits,
                vulkanMemoryPropertyFlagsFromUsage(m_description.usage));

            result = vkAllocateMemory(device.device(), &allocInfo, nullptr, m_memory.get());
            ASSERT(result == VK_SUCCESS);

            vkBindBufferMemory(device.device(), *m_buffer, *m_memory, 0);

            if (desc.initialData)
            {
                void* data;
                vkMapMemory(
                    device.device(), 
                    (*m_memory.get()), 
                    desc.initialData.elementStart * desc.initialData.elementSize, 
                    desc.initialData.data.size(), 
                    0, &data);
                memcpy(data, desc.initialData.data.data(), desc.initialData.data.size());
                vkUnmapMemory(device.device(), *m_memory.get());
            }
        }

        void* BufferImpl::map(const DeviceImpl& device)
        {
            void* data;
            vkMapMemory(device.device(), (*m_memory.get()), 0, m_description.elements * m_description.elementSize, 0, &data);
            return data;
        }

        void BufferImpl::unmap(const DeviceImpl& device)
        {
            vkUnmapMemory(device.device(), *m_memory.get());
        }

        const BufferDescription::Descriptor& BufferImpl::description() const
        {
            return m_description;
        }

        ResourceState BufferImpl::state() const
        {
            return m_state;
        }

        void BufferImpl::state(ResourceState state)
        {
            m_state = state;
        }

        VkBuffer& BufferImpl::native()
        {
            return *m_buffer;
        }

        const VkBuffer& BufferImpl::native() const
        {
            return *m_buffer;
        }

        /*BufferViewImpl::BufferViewImpl(
            const Device& device, 
            const Buffer& buffer, 
            const BufferDescription& desc)
            : m_description(desc.descriptor)
            , m_view{ vulkanPtr<VkBufferView>(device.device(), vkDestroyBufferView) }
            , m_memory{ BufferImplGet::impl(buffer)->m_memory }
        {
            const VkDevice& dev = device.device();

            auto elements = m_description.elements != InvalidElementsValue ? m_description.elements : buffer.description().descriptor.elements;
            auto elementSize = m_description.elementSize != InvalidElementSizeValue ? m_description.elementSize : buffer.description().descriptor.elementSize;

            m_description.elements = elements;
            m_description.elementSize = elementSize;

            VkBufferViewCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
            bufferInfo.buffer = BufferImplGet::impl(buffer)->native();
            bufferInfo.flags = 0;// vulkanBufferUsageFlags(desc.descriptor.usage) | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            bufferInfo.format = vulkanFormat(desc.descriptor.format);
            bufferInfo.offset = desc.descriptor.firstElement * elementSize;
            bufferInfo.range = elements * elementSize;

            auto result = vkCreateBufferView(dev, &bufferInfo, nullptr, m_view.get());
            ASSERT(result == VK_SUCCESS);
        }

        void* BufferViewImpl::map(const Device& device)
        {
            void* data;
            vkMapMemory(device.device(), (*m_memory.get()), 0, m_description.elements * m_description.elementSize, 0, &data);
            return data;
        }

        void BufferViewImpl::unmap(const Device& device)
        {
            vkUnmapMemory(device.device(), *m_memory.get());
        }

        const BufferDescription::Descriptor& BufferViewImpl::description() const
        {
            return m_description;
        }

        VkBufferView& BufferViewImpl::native()
        {
            return *m_view;
        }

        const VkBufferView& BufferViewImpl::native() const
        {
            return *m_view;
        }*/

        //------------------------
        BufferSRVImpl::BufferSRVImpl(
            const DeviceImpl& device,
            const Buffer& buffer,
            const BufferDescription& desc)
            : m_description(desc.descriptor)
            , m_view{ vulkanPtr<VkBufferView>(device.device(), vkDestroyBufferView) }
            , m_memory{ BufferImplGet::impl(buffer)->m_memory }
            , m_buffer{ buffer }
        {
            const VkDevice& dev = device.device();

            int32_t elements = m_description.elements != InvalidElementsValue ? m_description.elements : buffer.description().descriptor.elements;
            int32_t elementSize = m_description.elementSize != InvalidElementSizeValue ? m_description.elementSize : buffer.description().descriptor.elementSize;

            m_description.elements = elements;
            m_description.elementSize = elementSize;

            VkBufferViewCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
            bufferInfo.buffer = BufferImplGet::impl(buffer)->native();
            bufferInfo.flags = 0;// vulkanBufferUsageFlags(desc.descriptor.usage) | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            bufferInfo.format = vulkanFormat(desc.descriptor.format);
            bufferInfo.offset = desc.descriptor.firstElement * elementSize;
            bufferInfo.range = elements * elementSize;

            auto result = vkCreateBufferView(dev, &bufferInfo, nullptr, m_view.get());
            ASSERT(result == VK_SUCCESS);
        }

        const BufferDescription::Descriptor& BufferSRVImpl::description() const
        {
            return m_description;
        }

        VkBufferView& BufferSRVImpl::native()
        {
            return *m_view;
        }

        const VkBufferView& BufferSRVImpl::native() const
        {
            return *m_view;
        }

        Buffer& BufferSRVImpl::buffer()
        {
            return m_buffer;
        }

        const Buffer& BufferSRVImpl::buffer() const
        {
            return m_buffer;
        }

        BufferUAVImpl::BufferUAVImpl(
            const DeviceImpl& device,
            const Buffer& buffer,
            const BufferDescription& desc)
            : m_description(desc.descriptor)
            , m_view{ vulkanPtr<VkBufferView>(device.device(), vkDestroyBufferView) }
            , m_memory{ BufferImplGet::impl(buffer)->m_memory }
            , m_buffer{ buffer }
        {
            const VkDevice& dev = device.device();

            int32_t elements = m_description.elements != InvalidElementsValue ? m_description.elements : buffer.description().descriptor.elements;
            int32_t elementSize = m_description.elementSize != InvalidElementSizeValue ? m_description.elementSize : buffer.description().descriptor.elementSize;

            m_description.elements = elements;
            m_description.elementSize = elementSize;

            VkBufferViewCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
            bufferInfo.buffer = BufferImplGet::impl(buffer)->native();
            bufferInfo.flags = 0;// vulkanBufferUsageFlags(desc.descriptor.usage) | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            bufferInfo.format = vulkanFormat(buffer.description().descriptor.format);// vulkanFormat(desc.descriptor.format);
            bufferInfo.offset = desc.descriptor.firstElement * elementSize;
            bufferInfo.range = elements * elementSize;

            auto result = vkCreateBufferView(dev, &bufferInfo, nullptr, m_view.get());
            ASSERT(result == VK_SUCCESS);
        }

        const BufferDescription::Descriptor& BufferUAVImpl::description() const
        {
            return m_description;
        }

        VkBufferView& BufferUAVImpl::native()
        {
            return *m_view;
        }

        const VkBufferView& BufferUAVImpl::native() const
        {
            return *m_view;
        }

        Buffer& BufferUAVImpl::buffer()
        {
            return m_buffer;
        }
        const Buffer& BufferUAVImpl::buffer() const
        {
            return m_buffer;
        }

        BufferIBVImpl::BufferIBVImpl(
            const DeviceImpl& device,
            const Buffer& buffer,
            const BufferDescription& desc)
            : m_description(desc.descriptor)
            , m_view{ vulkanPtr<VkBufferView>(device.device(), vkDestroyBufferView) }
            , m_memory{ BufferImplGet::impl(buffer)->m_memory }
            , m_buffer{ buffer }
        {
            const VkDevice& dev = device.device();

            int32_t elements = m_description.elements != InvalidElementsValue ? m_description.elements : buffer.description().descriptor.elements;
            int32_t elementSize = m_description.elementSize != InvalidElementSizeValue ? m_description.elementSize : buffer.description().descriptor.elementSize;

            m_description.elements = elements;
            m_description.elementSize = elementSize;

            VkBufferViewCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
            bufferInfo.buffer = BufferImplGet::impl(buffer)->native();
            bufferInfo.flags = 0;// vulkanBufferUsageFlags(desc.descriptor.usage) | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            bufferInfo.format = vulkanFormat(desc.descriptor.format);
            bufferInfo.offset = desc.descriptor.firstElement * elementSize;
            bufferInfo.range = elements * elementSize;

            auto result = vkCreateBufferView(dev, &bufferInfo, nullptr, m_view.get());
            ASSERT(result == VK_SUCCESS);
        }

        const BufferDescription::Descriptor& BufferIBVImpl::description() const
        {
            return m_description;
        }

        VkBufferView& BufferIBVImpl::native()
        {
            return *m_view;
        }

        const VkBufferView& BufferIBVImpl::native() const
        {
            return *m_view;
        }

        Buffer& BufferIBVImpl::buffer()
        {
            return m_buffer;
        }

        const Buffer& BufferIBVImpl::buffer() const
        {
            return m_buffer;
        }

        BufferCBVImpl::BufferCBVImpl(
            const DeviceImpl& device,
            const Buffer& buffer,
            const BufferDescription& desc)
            : m_description(desc.descriptor)
            , m_view{ vulkanPtr<VkBufferView>(device.device(), vkDestroyBufferView) }
            , m_memory{ BufferImplGet::impl(buffer)->m_memory }
            , m_buffer{ buffer }
        {
            const VkDevice& dev = device.device();

            int32_t elements = m_description.elements != InvalidElementsValue ? m_description.elements : buffer.description().descriptor.elements;
            int32_t elementSize = m_description.elementSize != InvalidElementSizeValue ? m_description.elementSize : buffer.description().descriptor.elementSize;

            m_description.elements = elements;
            m_description.elementSize = elementSize;

            VkBufferViewCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
            bufferInfo.buffer = BufferImplGet::impl(buffer)->native();
            bufferInfo.flags = 0;// vulkanBufferUsageFlags(desc.descriptor.usage) | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            bufferInfo.format = vulkanFormat(desc.descriptor.format);
            bufferInfo.offset = desc.descriptor.firstElement * elementSize;
            bufferInfo.range = elements * elementSize;

            auto result = vkCreateBufferView(dev, &bufferInfo, nullptr, m_view.get());
            ASSERT(result == VK_SUCCESS);
        }

        const BufferDescription::Descriptor& BufferCBVImpl::description() const
        {
            return m_description;
        }

        VkBufferView& BufferCBVImpl::native()
        {
            return *m_view;
        }

        const VkBufferView& BufferCBVImpl::native() const
        {
            return *m_view;
        }

        Buffer& BufferCBVImpl::buffer()
        {
            return m_buffer;
        }

        const Buffer& BufferCBVImpl::buffer() const
        {
            return m_buffer;
        }

        BufferVBVImpl::BufferVBVImpl(
            const DeviceImpl& device,
            const Buffer& buffer,
            const BufferDescription& desc)
            : m_description(desc.descriptor)
            , m_view{ vulkanPtr<VkBufferView>(device.device(), vkDestroyBufferView) }
            , m_memory{ BufferImplGet::impl(buffer)->m_memory }
            , m_buffer{ buffer }
        {
            const VkDevice& dev = device.device();

            int32_t elements = m_description.elements != InvalidElementsValue ? m_description.elements : buffer.description().descriptor.elements;
            int32_t elementSize = m_description.elementSize != InvalidElementSizeValue ? m_description.elementSize : buffer.description().descriptor.elementSize;

            m_description.elements = elements;
            m_description.elementSize = elementSize;

            VkBufferViewCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
            bufferInfo.buffer = BufferImplGet::impl(buffer)->native();
            bufferInfo.flags = 0;// vulkanBufferUsageFlags(desc.descriptor.usage) | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            bufferInfo.format = vulkanFormat(desc.descriptor.format);
            bufferInfo.offset = desc.descriptor.firstElement * elementSize;
            bufferInfo.range = elements * elementSize;

            auto result = vkCreateBufferView(dev, &bufferInfo, nullptr, m_view.get());
            ASSERT(result == VK_SUCCESS);
        }

        const BufferDescription::Descriptor& BufferVBVImpl::description() const
        {
            return m_description;
        }

        VkBufferView& BufferVBVImpl::native()
        {
            return *m_view;
        }

        const VkBufferView& BufferVBVImpl::native() const
        {
            return *m_view;
        }

        Buffer& BufferVBVImpl::buffer()
        {
            return m_buffer;
        }

        const Buffer& BufferVBVImpl::buffer() const
        {
            return m_buffer;
        }

        VkSampleCountFlagBits vulkanSamples(uint32_t samples)
        {
            switch (samples)
            {
                case 1: return VK_SAMPLE_COUNT_1_BIT;
                case 2: return VK_SAMPLE_COUNT_2_BIT;
                case 4: return VK_SAMPLE_COUNT_4_BIT;
                case 8: return VK_SAMPLE_COUNT_8_BIT;
                case 16: return VK_SAMPLE_COUNT_16_BIT;
                case 32: return VK_SAMPLE_COUNT_32_BIT;
                case 64: return VK_SAMPLE_COUNT_64_BIT;
            }
            return VK_SAMPLE_COUNT_1_BIT;
        }

        VkImageTiling vulkanImageTilingFromUsage(ResourceUsage usage)
        {
            switch (usage)
            {
            case ResourceUsage::CpuToGpu: return VkImageTiling::VK_IMAGE_TILING_LINEAR;
            case ResourceUsage::GpuToCpu: return VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
            case ResourceUsage::GpuRead: return VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
            case ResourceUsage::GpuReadWrite: return VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
            case ResourceUsage::GpuRenderTargetRead: return VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
            case ResourceUsage::GpuRenderTargetReadWrite: return VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
            case ResourceUsage::DepthStencil: return VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
            default: return VkImageTiling::VK_IMAGE_TILING_LINEAR;
            }
        }

        TextureImpl::TextureImpl(const DeviceImpl& device, const TextureDescription& desc)
            : m_description( desc.descriptor )
            , m_image{ vulkanPtr<VkImage>(device.device(), vkDestroyImage) }
            , m_memory{ vulkanPtr<VkDeviceMemory>(device.device(), vkFreeMemory) }
            , m_state{ getResourceStateFromUsage(m_description.usage) }
        {
            const VkDevice& dev = device.device();

            VkImageCreateInfo imageInfo = {};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType = vulkanDimension(m_description.dimension);
            imageInfo.extent.width = m_description.width;
            imageInfo.extent.height = m_description.height;
            imageInfo.extent.depth = m_description.depth;
            imageInfo.mipLevels = m_description.mipLevels;
            imageInfo.arrayLayers = m_description.arraySlices;

            imageInfo.format = vulkanFormat(m_description.format);
            imageInfo.tiling = vulkanImageTilingFromUsage(m_description.usage);
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
            imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageInfo.samples = vulkanSamples(m_description.samples);
            imageInfo.flags = (desc.descriptor.dimension == ResourceDimension::TextureCubemap) ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0;

            VkImageFormatProperties imageFormatProperties = {};
            vkGetPhysicalDeviceImageFormatProperties(device.physicalDevice(), imageInfo.format, imageInfo.imageType, imageInfo.tiling, imageInfo.usage, imageInfo.flags, &imageFormatProperties);

            auto result = vkCreateImage(device.device(), &imageInfo, nullptr, m_image.get());
            ASSERT(result == VK_SUCCESS);

            VkMemoryRequirements memReq;
            vkGetImageMemoryRequirements(dev, *m_image, &memReq);

            VkMemoryAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memReq.size;
            allocInfo.memoryTypeIndex = findMemoryType(
                device,
                memReq.memoryTypeBits,
                vulkanMemoryPropertyFlagsFromUsage(desc.descriptor.usage));

            result = vkAllocateMemory(device.device(), &allocInfo, nullptr, m_memory.get());
            ASSERT(result == VK_SUCCESS);

            vkBindImageMemory(device.device(), *m_image, *m_memory, 0);

            if (desc.initialData)
            {
                ASSERT(allocInfo.allocationSize == desc.initialData.data.size());
                void* data;
                vkMapMemory(
                    device.device(), 
                    *m_memory, 
                    0,
                    desc.initialData.data.size(),
                    0, 
                    &data);

                memcpy(data, desc.initialData.data.data(), desc.initialData.data.size());
                vkUnmapMemory(device.device(), *m_memory.get());
            }
        }

        TextureImpl::TextureImpl(
            std::shared_ptr<VkImage> image,
            const TextureDescription& desc)
            : m_description(desc.descriptor)
            , m_image{ image }
            , m_memory{ nullptr }
            , m_state{ ResourceState::RenderTarget }
        {
        }

        void* TextureImpl::map(const DeviceImpl& device)
        {
            void* data = nullptr;
            LOG_WARNING("TODO: TextureImpl::map not implemented");
            //vkMapMemory(device.device(), (*m_memory.get()), 0, m_description.elements * m_description.elementSize, 0, &data);
            return data;
        }

        void TextureImpl::unmap(const DeviceImpl& device)
        {
            LOG_WARNING("TODO: TextureImpl::unmap not implemented");
            //vkUnmapMemory(device.device(), *m_memory.get());
        }

        const TextureDescription::Descriptor& TextureImpl::description() const
        {
            return m_description;
        }

        VkImage& TextureImpl::native()
        {
            return *m_image;
        }

        const VkImage& TextureImpl::native() const
        {
            return *m_image;
        }

        ResourceState TextureImpl::state() const
        {
            return m_state;
        }

        void TextureImpl::state(ResourceState state)
        {
            m_state = state;
        }

        TextureSRVImpl::TextureSRVImpl(
            const DeviceImpl& device,
            const Texture& texture, 
            const TextureDescription& desc,
            SubResource subResources)
            : m_description(desc.descriptor)
            , m_view{ vulkanPtr<VkImageView>(device.device(), vkDestroyImageView) }
            , m_texture{ texture }
        {
            const VkDevice& dev = device.device();

            VkImageViewCreateInfo viewInfo = {};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = TextureImplGet::impl(texture)->native();
            viewInfo.flags = 0; // vulkanBufferUsageFlags(desc.descriptor.usage) | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            viewInfo.format = vulkanFormat(desc.descriptor.format);
            viewInfo.subresourceRange = vulkanSubResource(texture, subResources);
            viewInfo.viewType = vulkanViewType(m_description.dimension);

            auto result = vkCreateImageView(dev, &viewInfo, nullptr, m_view.get());
            ASSERT(result == VK_SUCCESS);
        }

        const TextureDescription::Descriptor& TextureSRVImpl::description() const
        {
            return m_description;
        }

        Texture& TextureSRVImpl::texture()
        {
            return m_texture;
        }

        const Texture& TextureSRVImpl::texture() const
        {
            return m_texture;
        }

        Format TextureSRVImpl::format() const
        {
            return m_description.format;
        }

        uint32_t TextureSRVImpl::width() const
        {
            return m_description.width;
        }

        uint32_t TextureSRVImpl::height() const
        {
            return m_description.height;
        }

        VkImageView& TextureSRVImpl::native()
        {
            return *m_view;
        }

        const VkImageView& TextureSRVImpl::native() const
        {
            return *m_view;
        }

        TextureUAVImpl::TextureUAVImpl(
            const DeviceImpl& device,
            const Texture& texture,
            const TextureDescription& desc,
            SubResource subResources)
            : m_description(desc.descriptor)
            , m_view{ vulkanPtr<VkImageView>(device.device(), vkDestroyImageView) }
            , m_texture{ texture }
        {
            const VkDevice& dev = device.device();

            VkImageViewCreateInfo viewInfo = {};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = TextureImplGet::impl(texture)->native();
            viewInfo.flags = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            viewInfo.format = vulkanFormat(desc.descriptor.format);
            viewInfo.subresourceRange = vulkanSubResource(texture, subResources);
            viewInfo.viewType = vulkanViewType(m_description.dimension);

            auto result = vkCreateImageView(dev, &viewInfo, nullptr, m_view.get());
            ASSERT(result == VK_SUCCESS);
        }

        const TextureDescription::Descriptor& TextureUAVImpl::description() const
        {
            return m_description;
        }

        Texture& TextureUAVImpl::texture()
        {
            return m_texture;
        }

        const Texture& TextureUAVImpl::texture() const
        {
            return m_texture;
        }

        VkImageView& TextureUAVImpl::native()
        {
            return *m_view;
        }

        const VkImageView& TextureUAVImpl::native() const
        {
            return *m_view;
        }

        TextureDSVImpl::TextureDSVImpl(
            const DeviceImpl& device,
            const Texture& texture,
            const TextureDescription& desc,
            SubResource subResources)
            : m_description(desc.descriptor)
            , m_view{ vulkanPtr<VkImageView>(device.device(), vkDestroyImageView) }
            , m_texture{ texture }
        {
            const VkDevice& dev = device.device();

            VkImageViewCreateInfo viewInfo = {};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = TextureImplGet::impl(texture)->native();
            viewInfo.flags = 0;// vulkanBufferUsageFlags(desc.descriptor.usage) | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            viewInfo.format = vulkanFormat(desc.descriptor.format);
            viewInfo.subresourceRange = vulkanSubResource(texture, subResources);
            viewInfo.viewType = vulkanViewType(m_description.dimension);

            auto result = vkCreateImageView(dev, &viewInfo, nullptr, m_view.get());
            ASSERT(result == VK_SUCCESS);
        }

        const TextureDescription::Descriptor& TextureDSVImpl::description() const
        {
            return m_description;
        }

        Texture& TextureDSVImpl::texture()
        {
            return m_texture;
        }

        const Texture& TextureDSVImpl::texture() const
        {
            return m_texture;
        }

        Format TextureDSVImpl::format() const
        {
            return m_description.format;
        }

        uint32_t TextureDSVImpl::width() const
        {
            return m_description.width;
        }

        uint32_t TextureDSVImpl::height() const
        {
            return m_description.height;
        }

        VkImageView& TextureDSVImpl::native()
        {
            return *m_view;
        }

        const VkImageView& TextureDSVImpl::native() const
        {
            return *m_view;
        }

        TextureRTVImpl::TextureRTVImpl(
            const DeviceImpl& device,
            const Texture& texture,
            const TextureDescription& desc,
            SubResource subResources)
            : m_description(desc.descriptor)
            , m_view{ vulkanPtr<VkImageView>(device.device(), vkDestroyImageView) }
            , m_texture{ texture }
        {
            const VkDevice& dev = device.device();

            VkImageViewCreateInfo viewInfo = {};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = TextureImplGet::impl(texture)->native();
            viewInfo.flags = 0;// vulkanBufferUsageFlags(desc.descriptor.usage) | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            viewInfo.format = vulkanFormat(desc.descriptor.format);
            viewInfo.subresourceRange = vulkanSubResource(texture, subResources);
            viewInfo.viewType = vulkanViewType(m_description.dimension);

            auto result = vkCreateImageView(dev, &viewInfo, nullptr, m_view.get());
            ASSERT(result == VK_SUCCESS);
        }

        const TextureDescription::Descriptor& TextureRTVImpl::description() const
        {
            return m_description;
        }

        Texture& TextureRTVImpl::texture()
        {
            return m_texture;
        }

        const Texture& TextureRTVImpl::texture() const
        {
            return m_texture;
        }

        Format TextureRTVImpl::format() const
        {
            return m_description.format;
        }

        uint32_t TextureRTVImpl::width() const
        {
            return m_description.width;
        }

        uint32_t TextureRTVImpl::height() const
        {
            return m_description.height;
        }

        VkImageView& TextureRTVImpl::native()
        {
            return *m_view;
        }

        const VkImageView& TextureRTVImpl::native() const
        {
            return *m_view;
        }
    }
}
