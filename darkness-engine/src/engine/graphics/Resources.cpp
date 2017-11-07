#include "engine/graphics/Resources.h"
#include "engine/graphics/Barrier.h"

#if defined(_WIN32) && !defined(VULKAN)
#include "engine/graphics/dx12/DX12Resources.h"
#endif

#if defined(VULKAN)
#include "engine/graphics/vulkan/VulkanResources.h"
#endif

#ifdef __APPLE__
#include "engine/graphics/metal/MetalBuffer.h"
#endif


using namespace engine::implementation;
using namespace std;

namespace engine
{
    namespace implementation
    {
        const BufferImpl* BufferImplGet::impl(const Buffer& value)
        {
            return dynamic_cast<const BufferImpl*>(value.impl()); \
        }

        BufferImpl* BufferImplGet::impl(Buffer& value)
        {
            return dynamic_cast<BufferImpl*>(value.impl());
        }
    }

    Buffer::Buffer(const Device& device, const BufferDescription& desc)
        : m_impl{ make_shared<BufferImpl>(DeviceImplGet::impl(device), desc) }
    {
    }

    Buffer::Buffer(shared_ptr<implementation::BufferImpl> impl)
        : m_impl{ impl }
    {

    }

    const implementation::BufferImpl* Buffer::impl() const
    {
        return m_impl.get();
    }

    implementation::BufferImpl* Buffer::impl()
    {
        return m_impl.get();
    }

    void* Buffer::map(const Device& device)
    {
        return m_impl->map(DeviceImplGet::impl(device));
    }
    void Buffer::unmap(const Device& device)
    {
        m_impl->unmap(DeviceImplGet::impl(device));
    }

    BufferDescription Buffer::description() const
    {
        return{ m_impl->description() };
    }

    ResourceState Buffer::state() const
    {
        return m_impl->state();
    }

    void Buffer::state(ResourceState state)
    {
        m_impl->state(state);
    }

    namespace implementation
    {
        const BufferSRVImpl* BufferSRVImplGet::impl(const BufferSRV& value)
        {
            return dynamic_cast<const BufferSRVImpl*>(value.impl()); \
        }

        BufferSRVImpl* BufferSRVImplGet::impl(BufferSRV& value)
        {
            return dynamic_cast<BufferSRVImpl*>(value.impl());
        }
    }

    BufferSRV::BufferSRV(
        const Device& device,
        const Buffer& buffer,
        const BufferDescription& desc)
        : m_impl{ make_shared<BufferSRVImpl>(DeviceImplGet::impl(device), buffer, desc) }
    {
    }

    const implementation::BufferSRVImpl* BufferSRV::impl() const
    {
        return m_impl.get();
    }

    implementation::BufferSRVImpl* BufferSRV::impl()
    {
        return m_impl.get();
    }

    bool BufferSRV::valid() const { return m_impl.get(); };

    const BufferDescription::Descriptor& BufferSRV::desc() const
    {
        return m_impl->description();
    }

    Buffer& BufferSRV::buffer()
    {
        return m_impl->buffer();
    }

    const Buffer& BufferSRV::buffer() const
    {
        return m_impl->buffer();
    }

    namespace implementation
    {
        const BufferUAVImpl* BufferUAVImplGet::impl(const BufferUAV& value)
        {
            return dynamic_cast<const BufferUAVImpl*>(value.impl()); \
        }

        BufferUAVImpl* BufferUAVImplGet::impl(BufferUAV& value)
        {
            return dynamic_cast<BufferUAVImpl*>(value.impl());
        }
    }

    BufferUAV::BufferUAV(
        const Device& device,
        const Buffer& buffer,
        const BufferDescription& desc)
        : m_impl{ make_shared<BufferUAVImpl>(DeviceImplGet::impl(device), buffer, desc) }
    {
    }

    const implementation::BufferUAVImpl* BufferUAV::impl() const
    {
        return m_impl.get();
    }

    implementation::BufferUAVImpl* BufferUAV::impl()
    {
        return m_impl.get();
    }

    bool BufferUAV::valid() const { return m_impl.get(); };

    Buffer& BufferUAV::buffer()
    {
        return m_impl->buffer();
    }

    const Buffer& BufferUAV::buffer() const
    {
        return m_impl->buffer();
    }

    const BufferDescription::Descriptor& BufferUAV::desc() const
    {
        return m_impl->description();
    }

    void BufferUAV::setCounterValue(uint32_t value)
    {
        m_impl->setCounterValue(value);
    }
    uint32_t BufferUAV::getCounterValue()
    {
        return m_impl->getCounterValue();
    }

    namespace implementation
    {
        const BufferIBVImpl* BufferIBVImplGet::impl(const BufferIBV& value)
        {
            return dynamic_cast<const BufferIBVImpl*>(value.impl()); \
        }
        
        BufferIBVImpl* BufferIBVImplGet::impl(BufferIBV& value)
        {
            return dynamic_cast<BufferIBVImpl*>(value.impl());
        }
    }

    BufferIBV::BufferIBV(
        const Device& device,
        const Buffer& buffer,
        const BufferDescription& desc)
        : m_impl{ make_shared<BufferIBVImpl>(DeviceImplGet::impl(device), buffer, desc) }
    {
    }

    const implementation::BufferIBVImpl* BufferIBV::impl() const
    {
        return m_impl.get();
    }

    implementation::BufferIBVImpl* BufferIBV::impl()
    {
        return m_impl.get();
    }

    bool BufferIBV::valid() const { return m_impl.get(); };

    Buffer& BufferIBV::buffer()
    {
        return m_impl->buffer();
    }

    const Buffer& BufferIBV::buffer() const
    {
        return m_impl->buffer();
    }

    const BufferDescription::Descriptor& BufferIBV::desc() const
    {
        return m_impl->description();
    }

    namespace implementation
    {
        const BufferCBVImpl* BufferCBVImplGet::impl(const BufferCBV& value)
        {
            return dynamic_cast<const BufferCBVImpl*>(value.impl()); \
        }

        BufferCBVImpl* BufferCBVImplGet::impl(BufferCBV& value)
        {
            return dynamic_cast<BufferCBVImpl*>(value.impl());
        }
    }

    BufferCBV::BufferCBV(
            const Device& device,
            const Buffer& buffer,
            const BufferDescription& desc)
        : m_impl{ make_shared<BufferCBVImpl>(DeviceImplGet::impl(device), buffer, desc) }
    {
    }

    BufferCBV::BufferCBV(shared_ptr<implementation::BufferCBVImpl> impl)
        : m_impl{ impl }
    {
    }

    const implementation::BufferCBVImpl* BufferCBV::impl() const
    {
        return m_impl.get();
    }

    implementation::BufferCBVImpl* BufferCBV::impl()
    {
        return m_impl.get();
    }

    bool BufferCBV::valid() const { return m_impl.get(); };

    Buffer& BufferCBV::buffer()
    {
        return m_impl->buffer();
    }

    const Buffer& BufferCBV::buffer() const
    {
        return m_impl->buffer();
    }

    const BufferDescription::Descriptor& BufferCBV::desc() const
    {
        return m_impl->description();
    }

    namespace implementation
    {
        const BufferVBVImpl* BufferVBVImplGet::impl(const BufferVBV& value)
        {
            return dynamic_cast<const BufferVBVImpl*>(value.impl()); \
        }

        BufferVBVImpl* BufferVBVImplGet::impl(BufferVBV& value)
        {
            return dynamic_cast<BufferVBVImpl*>(value.impl());
        }
    }
    
    BufferVBV::BufferVBV(
            const Device& device,
            const Buffer& buffer,
            const BufferDescription& desc)
        : m_impl{ make_shared<BufferVBVImpl>(DeviceImplGet::impl(device), buffer, desc) }
    {
    }

    const implementation::BufferVBVImpl* BufferVBV::impl() const
    {
        return m_impl.get();
    }

    implementation::BufferVBVImpl* BufferVBV::impl()
    {
        return m_impl.get();
    }

    bool BufferVBV::valid() const { return m_impl.get(); };

    const BufferDescription::Descriptor& BufferVBV::desc() const
    {
        return m_impl->description();
    }

    Buffer& BufferVBV::buffer()
    {
        return m_impl->buffer();
    }

    const Buffer& BufferVBV::buffer() const
    {
        return m_impl->buffer();
    }

    namespace implementation
    {
        const TextureImpl* TextureImplGet::impl(const Texture& value)
        {
            return dynamic_cast<const TextureImpl*>(value.impl()); \
        }

        TextureImpl* TextureImplGet::impl(Texture& value)
        {
            return dynamic_cast<TextureImpl*>(value.impl());
        }
    }

    Texture::Texture(shared_ptr<implementation::TextureImpl> texture)
        : m_impl{ texture }
    {
    }

    const implementation::TextureImpl* Texture::impl() const
    {
        return m_impl.get();
    }

    implementation::TextureImpl* Texture::impl()
    {
        return m_impl.get();
    }

    void* Texture::map(const Device& device)
    {
        return m_impl->map(DeviceImplGet::impl(device));
    }
    void Texture::unmap(const Device& device)
    {
        m_impl->unmap(DeviceImplGet::impl(device));
    }

    Format Texture::format() const
    {
        return m_impl->description().format;
    }

    uint32_t Texture::width() const
    {
        return m_impl->description().width;
    }

    uint32_t Texture::height() const
    {
        return m_impl->description().height;
    }

    uint32_t Texture::depth() const
    {
        return m_impl->description().depth;
    }

    uint32_t Texture::arraySlices() const
    {
        return m_impl->description().arraySlices;
    }

    uint32_t Texture::mipLevels() const
    {
        return m_impl->description().mipLevels;
    }

    uint32_t Texture::samples() const
    {
        return m_impl->description().samples;
    }

    ResourceDimension Texture::dimension() const
    {
        return m_impl->description().dimension;
    }

    TextureDescription Texture::description() const
    {
        return{ m_impl->description() };
    }

    ResourceState Texture::state() const
    {
        return m_impl->state();
    }

    void Texture::state(ResourceState state)
    {
        m_impl->state(state);
    }

    namespace implementation
    {
        const TextureSRVImpl* TextureSRVImplGet::impl(const TextureSRV& value)
        {
            return dynamic_cast<const TextureSRVImpl*>(value.impl()); \
        }

        TextureSRVImpl* TextureSRVImplGet::impl(TextureSRV& value)
        {
            return dynamic_cast<TextureSRVImpl*>(value.impl());
        }
    }

    TextureSRV::TextureSRV(shared_ptr<implementation::TextureSRVImpl> view)
        : m_impl{ view }
    { }

    const implementation::TextureSRVImpl* TextureSRV::impl() const
    {
        return m_impl.get();
    }

    implementation::TextureSRVImpl* TextureSRV::impl()
    {
        return m_impl.get();
    }

    Texture& TextureSRV::texture()
    {
        return m_impl->texture();
    }

    const Texture& TextureSRV::texture() const
    {
        return m_impl->texture();
    }

    bool TextureSRV::valid() const { return m_impl.get(); };

    
    namespace implementation
    {
        const TextureUAVImpl* TextureUAVImplGet::impl(const TextureUAV& value)
        {
            return dynamic_cast<const TextureUAVImpl*>(value.impl()); \
        }

        TextureUAVImpl* TextureUAVImplGet::impl(TextureUAV& value)
        {
            return dynamic_cast<TextureUAVImpl*>(value.impl());
        }
    }

    TextureUAV::TextureUAV(shared_ptr<implementation::TextureUAVImpl> view)
        : m_impl{ view }
    {
    }

    const implementation::TextureUAVImpl* TextureUAV::impl() const
    {
        return m_impl.get();
    }

    implementation::TextureUAVImpl* TextureUAV::impl()
    {
        return m_impl.get();
    }

    bool TextureUAV::valid() const { return m_impl.get(); };

    Texture& TextureUAV::texture()
    {
        return m_impl->texture();
    }

    const Texture& TextureUAV::texture() const
    {
        return m_impl->texture();
    }

    namespace implementation
    {
        const TextureDSVImpl* TextureDSVImplGet::impl(const TextureDSV& value)
        {
            return dynamic_cast<const TextureDSVImpl*>(value.impl()); \
        }

        TextureDSVImpl* TextureDSVImplGet::impl(TextureDSV& value)
        {
            return dynamic_cast<TextureDSVImpl*>(value.impl());
        }
    }

    TextureDSV::TextureDSV(shared_ptr<implementation::TextureDSVImpl> view)
        : m_impl{ view }
    {
    }

    const implementation::TextureDSVImpl* TextureDSV::impl() const
    {
        return m_impl.get();
    }

    implementation::TextureDSVImpl* TextureDSV::impl()
    {
        return m_impl.get();
    }

    bool TextureDSV::valid() const { return m_impl.get(); };

    Texture& TextureDSV::texture()
    {
        return m_impl->texture();
    }

    const Texture& TextureDSV::texture() const
    {
        return m_impl->texture();
    }

    namespace implementation
    {
        const TextureRTVImpl* TextureRTVImplGet::impl(const TextureRTV& value)
        {
            return dynamic_cast<const TextureRTVImpl*>(value.impl()); \
        }

        TextureRTVImpl* TextureRTVImplGet::impl(TextureRTV& value)
        {
            return dynamic_cast<TextureRTVImpl*>(value.impl());
        }
    }

    TextureRTV::TextureRTV(shared_ptr<implementation::TextureRTVImpl> view)
        : m_impl{ view }
    {
    }

    const implementation::TextureRTVImpl* TextureRTV::impl() const
    {
        return m_impl.get();
    }

    implementation::TextureRTVImpl* TextureRTV::impl()
    {
        return m_impl.get();
    }

    bool TextureRTV::valid() const { return m_impl.get(); };

    uint32_t TextureRTV::width() const
    {
        return m_impl->description().width;
    }

    uint32_t TextureRTV::height() const
    {
        return m_impl->description().height;
    }

    Texture& TextureRTV::texture()
    {
        return m_impl->texture();
    }

    const Texture& TextureRTV::texture() const
    {
        return m_impl->texture();
    }

}
