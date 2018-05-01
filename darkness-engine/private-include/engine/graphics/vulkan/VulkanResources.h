#pragma once

#include "engine/graphics/Resources.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/vulkan/VulkanConversions.h"
#include "engine/primitives/Color.h"
#include "tools/Codegen.h"
#include "tools/image/Image.h"

#include <memory>
#include <string>

namespace engine
{
    namespace implementation
    {
        class DeviceImpl;
        class BufferViewImpl;
        class BufferImpl
        {
        public:
            BufferImpl(
                const DeviceImpl& device,
                const BufferDescription& desc);

            void* map(const DeviceImpl& device);
            void unmap(const DeviceImpl& device);

            const BufferDescription::Descriptor& description() const;

            ResourceState state() const;
            void state(ResourceState state);

            VkBuffer& native();
            const VkBuffer& native() const;

        protected:
            friend class BufferViewImpl;
            friend class BufferSRVImpl;
            friend class BufferUAVImpl;
            friend class BufferIBVImpl;
            friend class BufferVBVImpl;
            friend class BufferCBVImpl;
            BufferDescription::Descriptor m_description;
            std::shared_ptr<VkBuffer> m_buffer{ nullptr };
            std::shared_ptr<VkDeviceMemory> m_memory{ nullptr };
            ResourceState m_state;
        };

        class BufferSRVImpl
        {
        public:
            BufferSRVImpl(
                const DeviceImpl& device,
                const Buffer& buffer,
                const BufferDescription& desc);

            const BufferDescription::Descriptor& description() const;

            VkBufferView& native();
            const VkBufferView& native() const;

            Buffer& buffer();
            const Buffer& buffer() const;

            uint64_t uniqueId() const
            {
                return 0;
            }

        protected:
            BufferDescription::Descriptor m_description;
            std::shared_ptr<VkBufferView> m_view{ nullptr };
            std::shared_ptr<VkDeviceMemory> m_memory{ nullptr };
            Buffer m_buffer;
        };

        class BufferUAVImpl
        {
        public:
            BufferUAVImpl(
                const DeviceImpl& device,
                const Buffer& buffer,
                const BufferDescription& desc);

            const BufferDescription::Descriptor& description() const;

            void setCounterValue(uint32_t value) {};
            uint32_t getCounterValue() { return 0; };

            VkBufferView& native();
            const VkBufferView& native() const;

            Buffer& buffer();
            const Buffer& buffer() const;

            uint64_t uniqueId() const
            {
                return 0;

            }
        protected:
            BufferDescription::Descriptor m_description;
            std::shared_ptr<VkBufferView> m_view{ nullptr };
            std::shared_ptr<VkDeviceMemory> m_memory{ nullptr };
            Buffer m_buffer;
        };

        class BufferIBVImpl
        {
        public:
            BufferIBVImpl(
                const DeviceImpl& device,
                const Buffer& buffer,
                const BufferDescription& desc);

            const BufferDescription::Descriptor& description() const;

            VkBufferView& native();
            const VkBufferView& native() const;

            Buffer& buffer();
            const Buffer& buffer() const;

            uint64_t uniqueId() const
            {
                return 0;
            }

        protected:
            BufferDescription::Descriptor m_description;
            std::shared_ptr<VkBufferView> m_view{ nullptr };
            std::shared_ptr<VkDeviceMemory> m_memory{ nullptr };
            Buffer m_buffer;
        };

        class BufferCBVImpl
        {
        public:
            BufferCBVImpl(
                const DeviceImpl& device,
                const Buffer& buffer,
                const BufferDescription& desc);

            const BufferDescription::Descriptor& description() const;

            VkBufferView& native();
            const VkBufferView& native() const;

            Buffer& buffer();
            const Buffer& buffer() const;

            uint64_t uniqueId() const
            {
                return 0;
            }

        protected:
            BufferDescription::Descriptor m_description;
            std::shared_ptr<VkBufferView> m_view{ nullptr };
            std::shared_ptr<VkDeviceMemory> m_memory{ nullptr };
            Buffer m_buffer;
        };

        class BufferVBVImpl
        {
        public:
            BufferVBVImpl(
                const DeviceImpl& device,
                const Buffer& buffer,
                const BufferDescription& desc);

            const BufferDescription::Descriptor& description() const;

            VkBufferView& native();
            const VkBufferView& native() const;

            Buffer& buffer();
            const Buffer& buffer() const;

            uint64_t uniqueId() const
            {
                return 0;
            }

        protected:
            BufferDescription::Descriptor m_description;
            std::shared_ptr<VkBufferView> m_view{ nullptr };
            std::shared_ptr<VkDeviceMemory> m_memory{ nullptr };
            Buffer m_buffer;
        };

        class TextureImpl
        {
        public:
            TextureImpl(
                const DeviceImpl& device,
                const TextureDescription& desc);
            TextureImpl(
                std::shared_ptr<VkImage> image,
                const TextureDescription& desc);

            void* map(const DeviceImpl& device);
            void unmap(const DeviceImpl& device);

            const TextureDescription::Descriptor& description() const;

            VkImage& native();
            const VkImage& native() const;
            
            ResourceState state() const;
            void state(ResourceState state);
        protected:
            const TextureDescription::Descriptor m_description;
            std::shared_ptr<VkImage> m_image{ nullptr };
            std::shared_ptr<VkDeviceMemory> m_memory{ nullptr };
            ResourceState m_state;
        };

        class TextureSRVImpl
        {
        public:
            TextureSRVImpl(
                const DeviceImpl& device,
                const Texture& texture, 
                const TextureDescription& desc,
                SubResource subResources = SubResource());

            const TextureDescription::Descriptor& description() const;

            Texture& texture();
            const Texture& texture() const;

            Format format() const;
            uint32_t width() const;
            uint32_t height() const;

            VkImageView& native();
            const VkImageView& native() const;
            
            uint64_t uniqueId() const
            {
                return 0;
            }
        private:
            const TextureDescription::Descriptor m_description;
            std::shared_ptr<VkImageView> m_view{ nullptr };
            Texture m_texture;
        };

        class TextureUAVImpl
        {
        public:
            TextureUAVImpl(
                const DeviceImpl& device,
                const Texture& texture,
                const TextureDescription& desc,
                SubResource subResources = SubResource());

            const TextureDescription::Descriptor& description() const;

            void setCounterValue(uint32_t value) {};
            uint32_t getCounterValue() {};

            Texture& texture();
            const Texture& texture() const;

            Format format() const;
            uint32_t width() const;
            uint32_t height() const;

            VkImageView& native();
            const VkImageView& native() const;

            uint64_t uniqueId() const
            {
                return 0;
            }

        private:
            const TextureDescription::Descriptor m_description;
            std::shared_ptr<VkImageView> m_view{ nullptr };
            Texture m_texture;
        };

        class TextureDSVImpl
        {
        public:
            TextureDSVImpl(
                const DeviceImpl& device,
                const Texture& texture,
                const TextureDescription& desc,
                SubResource subResources = SubResource());

            const TextureDescription::Descriptor& description() const;

            Texture& texture();
            const Texture& texture() const;

            Format format() const;
            uint32_t width() const;
            uint32_t height() const;

            VkImageView& native();
            const VkImageView& native() const;

            uint64_t uniqueId() const
            {
                return 0;
            }
        private:
            const TextureDescription::Descriptor m_description;
            std::shared_ptr<VkImageView> m_view{ nullptr };
            Texture m_texture;
        };

        class TextureRTVImpl
        {
        public:
            TextureRTVImpl(
                const DeviceImpl& device,
                const Texture& texture,
                const TextureDescription& desc,
                SubResource subResources = SubResource());

            const TextureDescription::Descriptor& description() const;

            Texture& texture();
            const Texture& texture() const;

            Format format() const;
            uint32_t width() const;
            uint32_t height() const;

            VkImageView& native();
            const VkImageView& native() const;

            uint64_t uniqueId() const
            {
                return 0;
            }
        private:
            const TextureDescription::Descriptor m_description;
            std::shared_ptr<VkImageView> m_view{ nullptr };
            Texture m_texture;
        };
    }
}

