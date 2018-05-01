#pragma once

#include "engine/graphics/Resources.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/dx12/DX12Headers.h"
#include "engine/graphics/dx12/DX12Common.h"
#include "engine/graphics/dx12/DX12Conversions.h"
#include "engine/primitives/Color.h"
#include "engine/graphics/dx12/DX12FormatSetups.h"
#include "tools/ComPtr.h"

struct ID3D12Resource;

namespace engine
{
    namespace implementation
    {
        class DeviceImpl;
        class BufferViewImpl;
        class CommandListImpl;
        class BufferImpl
        {
        public:
            BufferImpl();
            BufferImpl(
                const DeviceImpl& device,
                const BufferDescription& desc);

            void* map(const DeviceImpl& device);
            void unmap(const DeviceImpl& device);

            const BufferDescription::Descriptor& description() const;
            ResourceState state() const;
            void state(ResourceState _state);
            ID3D12Resource* native() const;
        protected:
            const BufferDescription::Descriptor m_description;
            tools::ComPtr<ID3D12Resource> m_buffer;
            ResourceState m_state;
            size_t m_bufferSize;
        };

        class BufferSRVImpl
        {
        public:
            BufferSRVImpl(
                const DeviceImpl& device,
                const Buffer& buffer,
                const BufferDescription& desc);

            const BufferDescription::Descriptor& description() const;

            D3D12_CPU_DESCRIPTOR_HANDLE& native();
            const D3D12_CPU_DESCRIPTOR_HANDLE& native() const;

            Buffer& buffer();
            const Buffer& buffer() const;

            uint64_t uniqueId() const
            {
                return m_viewHandle.uniqueId();
            }

        protected:
            BufferDescription::Descriptor m_description;
            DescriptorHandle m_viewHandle;
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

            /*void setCounterValue(uint32_t value);
            uint32_t getCounterValue();*/

            D3D12_CPU_DESCRIPTOR_HANDLE& native();
            const D3D12_CPU_DESCRIPTOR_HANDLE& native() const;

            Buffer& buffer();
            const Buffer& buffer() const;

            uint64_t uniqueId() const
            {
                return m_viewHandle.uniqueId();
            }

            uint32_t structureCounterOffsetBytes() const;

        protected:
            friend class CommandListImpl;
            BufferDescription::Descriptor m_description;
            DescriptorHandle m_viewHandle;
            //tools::ComPtr<ID3D12Resource> m_counterBuffer;
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

            D3D12_CPU_DESCRIPTOR_HANDLE& native();
            const D3D12_CPU_DESCRIPTOR_HANDLE& native() const;

            Buffer& buffer();
            const Buffer& buffer() const;
            const D3D12_INDEX_BUFFER_VIEW* view() const;

            uint64_t uniqueId() const
            {
                return m_viewHandle.uniqueId();
            }
        protected:
            BufferDescription::Descriptor m_description;
            DescriptorHandle m_viewHandle;
            tools::ComPtr<ID3D12Resource> m_counterBuffer;
            Buffer m_buffer;

        private:
            D3D12_INDEX_BUFFER_VIEW m_view;
        };

        class BufferCBVImpl
        {
        public:
            BufferCBVImpl(
                const DeviceImpl& device,
                const Buffer& buffer,
                const BufferDescription& desc);

            const BufferDescription::Descriptor& description() const;

            D3D12_CPU_DESCRIPTOR_HANDLE& native();
            const D3D12_CPU_DESCRIPTOR_HANDLE& native() const;

            Buffer& buffer();
            const Buffer& buffer() const;

            uint64_t uniqueId() const
            {
                return m_viewHandle.uniqueId();
            }

        protected:
            BufferDescription::Descriptor m_description;
            DescriptorHandle m_viewHandle;
            tools::ComPtr<ID3D12Resource> m_counterBuffer;
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

            D3D12_CPU_DESCRIPTOR_HANDLE& native();
            const D3D12_CPU_DESCRIPTOR_HANDLE& native() const;

            Buffer& buffer();
            const Buffer& buffer() const;
            const D3D12_VERTEX_BUFFER_VIEW* view() const;

            uint64_t uniqueId() const
            {
                return m_viewHandle.uniqueId();
            }
        protected:
            BufferDescription::Descriptor m_description;
            DescriptorHandle m_viewHandle;
            tools::ComPtr<ID3D12Resource> m_counterBuffer;
            Buffer m_buffer;

        private:
            D3D12_VERTEX_BUFFER_VIEW m_view;
        };

        class TextureImpl
        {
        public:
            TextureImpl(
                const DeviceImpl& device,
                const TextureDescription& desc);

            TextureImpl(
                const DeviceImpl& device,
                const TextureDescription& desc,
                ID3D12Resource* resource,
                ResourceState currentState);

            void* map(const DeviceImpl& device);
            void unmap(const DeviceImpl& device);

            const TextureDescription::Descriptor& description() const;

            ID3D12Resource* native() const;

            ResourceState state(int slice, int mip) const;
            void state(int slice, int mip, ResourceState _state);

        protected:
            TextureDescription::Descriptor m_description;
            tools::ComPtr<ID3D12Resource> m_texture;
            std::vector<ResourceState> m_state;
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
            uint32_t depth() const;

            D3D12_CPU_DESCRIPTOR_HANDLE& native();
            const D3D12_CPU_DESCRIPTOR_HANDLE& native() const;

            uint64_t uniqueId() const
            {
                return m_viewHandle.uniqueId();
            }

            const SubResource& subResource() const;

        protected:
            TextureDescription::Descriptor m_description;
            DescriptorHandle m_viewHandle;
            Texture m_texture;
            SubResource m_subResources;
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

            /*void setCounterValue(uint32_t value);
            uint32_t getCounterValue();*/

            Texture& texture();
            const Texture& texture() const;

            Format format() const;
            uint32_t width() const;
            uint32_t height() const;
            uint32_t depth() const;

            D3D12_CPU_DESCRIPTOR_HANDLE& native();
            const D3D12_CPU_DESCRIPTOR_HANDLE& native() const;

            uint64_t uniqueId() const
            {
                return m_viewHandle.uniqueId();
            }

            const SubResource& subResource() const;
        protected:
            TextureDescription::Descriptor m_description;
            DescriptorHandle m_viewHandle;
            tools::ComPtr<ID3D12Resource> m_counterBuffer;
            Texture m_texture;
            SubResource m_subResources;
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

            D3D12_CPU_DESCRIPTOR_HANDLE& native();
            const D3D12_CPU_DESCRIPTOR_HANDLE& native() const;

            uint64_t uniqueId() const
            {
                return m_viewHandle.uniqueId();
            }
            const SubResource& subResource() const;
        protected:
            TextureDescription::Descriptor m_description;
            DescriptorHandle m_viewHandle;
            Texture m_texture;
            SubResource m_subResources;
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

            D3D12_CPU_DESCRIPTOR_HANDLE& native();
            const D3D12_CPU_DESCRIPTOR_HANDLE& native() const;

            uint64_t uniqueId() const
            {
                return m_viewHandle.uniqueId();
            }
            const SubResource& subResource() const;
        protected:
            TextureDescription::Descriptor m_description;
            DescriptorHandle m_viewHandle;
            Texture m_texture;
            SubResource m_subResources;
        };

    }
}

