#pragma once

#include "tools/SmartPimpl.h"
#include "tools/Codegen.h"
#include "engine/graphics/Format.h"
#include "engine/graphics/CommonNoDep.h"
#include "tools/ByteRange.h"
#include "shaders/ShaderTypes.h"
#include <vector>

namespace engine
{
    using ResourceKey = size_t;

    class Device;

    namespace implementation
    {
        class SwapChainImpl;
        class DeviceImpl;
        class BufferImpl;
    }

    struct SampleDescription
    {
        unsigned int count;
        unsigned int quality;
    };

    enum class TextureLayout
    {
        Unknown,
        RowMajor,
        UndefinedSwizzle64KB,
        StandardSwizzle64KB
    };

    enum class ResourceFlags
    {
        None,
        AllowRenderTarget,
        AllowDepthStencil,
        AllowUnorderedAccess,
        DenyShaderResource,
        AllowCrossAdapter,
        AllowSimultaneousAccess
    };

    enum class ResourceState;

    struct DepthStencilValue
    {
        float depth;
        unsigned char stencil;
    };

    struct ClearValue
    {
        Format format;
        union
        {
            float color[4];
            DepthStencilValue depthStencil;
        };
    };

    struct ResourceDescription
    {
        ResourceDimension dimension;
        unsigned long long alignment;
        unsigned long long width;
        unsigned int height;
        unsigned short depthOrArraySize;
        unsigned int mipLevels;
        Format format;
        SampleDescription sampleDescription;
        TextureLayout layout;
        ResourceFlags flags;
    };

    enum class ResourceUsage
    {
        GpuRead,
        GpuReadWrite,
        GpuRenderTargetRead,
        GpuRenderTargetReadWrite,
        DepthStencil,
        GpuToCpu,
        Upload
    };

    constexpr const size_t InvalidElementsValue = static_cast<size_t>(-1);
    constexpr const size_t InvalidElementSizeValue = static_cast<size_t>(-1);
    struct BufferDescription
    {
       
        struct Descriptor
        {
            Format format = Format::UNKNOWN;
            int32_t elements = -1;
            int32_t elementSize = -1;
            uint32_t firstElement = 0;
            ResourceUsage usage;
            bool structured = false;
            bool append = false;
            const char* name = nullptr;
        };
        Descriptor descriptor;

        BufferDescription& format(Format value)
        {
            descriptor.format = value;
            return *this;
        }
        BufferDescription& elements(uint32_t value)
        {
            descriptor.elements = value;
            return *this;
        }
        BufferDescription& elements(int32_t value)
        {
            descriptor.elements = value;
            return *this;
        }
        BufferDescription& elements(size_t value)
        {
            descriptor.elements = static_cast<int32_t>(value);
            return *this;
        }
        BufferDescription& elementSize(size_t value)
        {
            descriptor.elementSize = static_cast<int32_t>(value);
            return *this;
        }
        BufferDescription& elementSize(int32_t value)
        {
            descriptor.elementSize = value;
            return *this;
        }
        BufferDescription& firstElement(uint32_t value)
        {
            descriptor.firstElement = value;
            return *this;
        }
        BufferDescription& usage(ResourceUsage value)
        {
            descriptor.usage = value;
            return *this;
        }
        BufferDescription& structured(bool value)
        {
            descriptor.structured = value;
            return *this;
        }
        BufferDescription& append(bool value)
        {
            descriptor.append = value;
            return *this;
        }
        BufferDescription& name(const char* value)
        {
            descriptor.name = value;
            return *this;
        }

        struct InitialData
        {
            std::vector<uint8_t> data;
            uint32_t elementStart;
            int32_t elements;
            int32_t elementSize;
            explicit operator bool() const
            {
                return data.size() > 0;
            }

            InitialData()
                : elementStart{ 0 }
                , elements{ 0 }
                , elementSize{ 0 }
            {}

            template<typename T>
            InitialData(const std::vector<T>& srcdata, uint32_t elemStart = 0)
            {
                elements = static_cast<int32_t>(srcdata.size());
                elementSize = static_cast<int32_t>(sizeof(T));
                auto size = elements * elementSize;
                data = std::vector<uint8_t>(size, 0);
                memcpy(data.data(), reinterpret_cast<const uint8_t*>(srcdata.data()), size);
                elementStart = elemStart;
            }
            
            InitialData(const tools::ByteRange& srcdata, uint32_t alignment, uint32_t elemStart = 0)
            {
                elements = (static_cast<int32_t>(srcdata.length()) + (alignment -1 )) & ~(alignment - 1);
                data = std::vector<uint8_t>(srcdata.sizeBytes(), 0);
                memcpy(data.data(), reinterpret_cast<const uint8_t*>(srcdata.start), srcdata.sizeBytes());
                elementStart = elemStart;
                elementSize = static_cast<int32_t>(srcdata.elementSize);
            }
        };
        InitialData initialData;
        BufferDescription& setInitialData(InitialData data)
        {
            initialData = data;
            descriptor.elements = data.elements;
            descriptor.elementSize = data.elementSize;
            descriptor.firstElement = data.elementStart;
            return *this;
        };
    };

    PIMPL_FWD_BUFFER(Buffer, BufferImpl)
    PIMPL_FWD_BUFFER(BufferView, BufferViewImpl)
    PIMPL_FWD_BUFFER(BufferSRV, BufferSRVImpl)
    PIMPL_FWD_BUFFER(BufferUAV, BufferUAVImpl)
    PIMPL_FWD_BUFFER(BufferIBV, BufferIBVImpl)
    PIMPL_FWD_BUFFER(BufferCBV, BufferCBVImpl)
    PIMPL_FWD_BUFFER(BufferVBV, BufferVBVImpl)

    class Buffer
    {
    public:
        Buffer() = default;
        Buffer(std::shared_ptr<implementation::BufferImpl> impl);

        void* map(const Device& device);
        void unmap(const Device& device);

        BufferDescription description() const;
        ResourceState state() const;
        void state(ResourceState state);

    protected:
        friend class implementation::SwapChainImpl;
        friend class Device;
        friend class implementation::DeviceImpl;
        Buffer(
            const Device& device, 
            const BufferDescription& desc);

    protected:
        friend struct implementation::BufferImplGet;
        const implementation::BufferImpl* impl() const;
        implementation::BufferImpl* impl();
        std::shared_ptr<implementation::BufferImpl> m_impl;
    };

    class BufferSRV
    {
    public:
        BufferSRV() = default;
        bool valid() const;
        const BufferDescription::Descriptor& desc() const;

        Buffer& buffer();
        const Buffer& buffer() const;
    protected:
        friend class implementation::SwapChainImpl;
        friend class Device;
        friend class implementation::DeviceImpl;
        BufferSRV(
            const Device& device,
            const Buffer& buffer,
            const BufferDescription& desc);

    protected:
        friend struct implementation::BufferSRVImplGet;
        const implementation::BufferSRVImpl* impl() const;
        implementation::BufferSRVImpl* impl();
        std::shared_ptr<implementation::BufferSRVImpl> m_impl;
    };

    class BufferUAV
    {
    public:
        BufferUAV() = default;

        /*void setCounterValue(uint32_t value);
        uint32_t getCounterValue();*/

        bool valid() const;
        const BufferDescription::Descriptor& desc() const;

        Buffer& buffer();
        const Buffer& buffer() const;
    protected:
        friend class implementation::SwapChainImpl;
        friend class Device;
        friend class implementation::DeviceImpl;
        BufferUAV(
            const Device& device,
            const Buffer& buffer,
            const BufferDescription& desc);

    protected:
        friend struct implementation::BufferUAVImplGet;
        const implementation::BufferUAVImpl* impl() const;
        implementation::BufferUAVImpl* impl();
        std::shared_ptr<implementation::BufferUAVImpl> m_impl;
    };

    class BufferIBV
    {
    public:
        BufferIBV() = default;
        bool valid() const;
        const BufferDescription::Descriptor& desc() const;

        Buffer& buffer();
        const Buffer& buffer() const;
    protected:
        friend class implementation::SwapChainImpl;
        friend class Device;
        friend class implementation::DeviceImpl;
        BufferIBV(
            const Device& device,
            const Buffer& buffer,
            const BufferDescription& desc);

    protected:
        friend struct implementation::BufferIBVImplGet;
        const implementation::BufferIBVImpl* impl() const;
        implementation::BufferIBVImpl* impl();
        std::shared_ptr<implementation::BufferIBVImpl> m_impl;
    };

    class BufferCBV
    {
    public:
        BufferCBV() = default;
        BufferCBV(std::shared_ptr<implementation::BufferCBVImpl> impl);
        bool valid() const;
        const BufferDescription::Descriptor& desc() const;

        Buffer& buffer();
        const Buffer& buffer() const;
    protected:
        friend class implementation::SwapChainImpl;
        friend class Device;
        friend class implementation::DeviceImpl;
        BufferCBV(
            const Device& device,
            const Buffer& buffer,
            const BufferDescription& desc);

    protected:
        friend struct implementation::BufferCBVImplGet;
        const implementation::BufferCBVImpl* impl() const;
        implementation::BufferCBVImpl* impl();
        std::shared_ptr<implementation::BufferCBVImpl> m_impl;
    };

    class BufferVBV
    {
    public:
        BufferVBV() = default;
        bool valid() const;
        const BufferDescription::Descriptor& desc() const;

        Buffer& buffer();
        const Buffer& buffer() const;
    protected:
        friend class implementation::SwapChainImpl;
        friend class Device;
        friend class implementation::DeviceImpl;
        BufferVBV(
            const Device& device,
            const Buffer& buffer,
            const BufferDescription& desc);

    protected:
        friend struct implementation::BufferVBVImplGet;
        const implementation::BufferVBVImpl* impl() const;
        implementation::BufferVBVImpl* impl();
        std::shared_ptr<implementation::BufferVBVImpl> m_impl;
    };

    struct TextureDescription
    {
        struct Descriptor
        {
            Format format = Format::UNKNOWN;
            uint32_t width = 0;
            uint32_t height = 0;
            uint32_t depth = 1;
            uint32_t arraySlices = 1;
            uint32_t mipLevels = 1;
            uint32_t samples = 1;
            ResourceDimension dimension = ResourceDimension::Texture2D;
            ResourceUsage usage = ResourceUsage::GpuRead;
            bool append = false;
            Float4 optimizedClearValue = { 0.0f, 0.0f, 0.0f, 0.0f };
            float optimizedDepthClearValue = 1.0f;
            uint8_t optimizedStencilClearValue = 0;
            const char* name;
        };
        Descriptor descriptor;

        TextureDescription& format(Format value)
        {
            descriptor.format = value;
            return *this;
        }
        TextureDescription& width(uint32_t value)
        {
            descriptor.width = value;
            return *this;
        }
        TextureDescription& height(uint32_t value)
        {
            descriptor.height = value;
            return *this;
        }
        TextureDescription& depth(uint32_t value)
        {
            descriptor.depth = value;
            return *this;
        }
        TextureDescription& arraySlices(uint32_t value)
        {
            descriptor.arraySlices = value;
            return *this;
        }
        TextureDescription& mipLevels(uint32_t value)
        {
            descriptor.mipLevels = value;
            return *this;
        }
        TextureDescription& samples(uint32_t value)
        {
            descriptor.samples = value;
            return *this;
        }
        TextureDescription& dimension(ResourceDimension value)
        {
            descriptor.dimension = value;
            if (value == ResourceDimension::TextureCubemap)
                descriptor.arraySlices = 6;
            return *this;
        }
        TextureDescription& usage(ResourceUsage value)
        {
            descriptor.usage = value;
            return *this;
        }
        TextureDescription& optimizedClearValue(Float4 value)
        {
            descriptor.optimizedClearValue = value;
            return *this;
        }
        TextureDescription& optimizedDepthClearValue(float value)
        {
            descriptor.optimizedDepthClearValue = value;
            return *this;
        }
        TextureDescription& optimizedStencilClearValue(uint8_t value)
        {
            descriptor.optimizedStencilClearValue = value;
            return *this;
        }
        TextureDescription& name(const char* value)
        {
            descriptor.name = value;
            return *this;
        }

        struct InitialData
        {
            std::vector<uint8_t> data;
            uint32_t pitch;
            uint32_t slicePitch;
            explicit operator bool() const
            {
                return data.size() > 0;
            }

            InitialData()
                : pitch{ 0 }
                , slicePitch{ 0 }
            {}

            template<typename T>
            InitialData(const std::vector<T>& srcdata, uint32_t _pitch, uint32_t _slicePitch)
                : pitch{ _pitch }
                , slicePitch{ _slicePitch }
            {
                auto size = srcdata.size() * sizeof(T);
                data = std::vector<uint8_t>(size, 0);
                memcpy(data.data(), reinterpret_cast<const uint8_t*>(srcdata.data()), size);
            }

            InitialData(const tools::ByteRange& srcdata, uint32_t _pitch, uint32_t _slicePitch)
                : pitch{ _pitch }
                , slicePitch{ _slicePitch }
                , data( srcdata.sizeBytes() )
            {
                memcpy(data.data(), reinterpret_cast<const uint8_t*>(srcdata.start), srcdata.sizeBytes());
            }
        };
        InitialData initialData;
        TextureDescription& setInitialData(const InitialData& data)
        {
            initialData = data;
            return *this;
        };
    };

    PIMPL_FWD_BUFFER(Texture, TextureImpl)
    PIMPL_FWD_BUFFER(TextureSRV, TextureSRVImpl)
    PIMPL_FWD_BUFFER(TextureUAV, TextureUAVImpl)
    PIMPL_FWD_BUFFER(TextureDSV, TextureDSVImpl)
    PIMPL_FWD_BUFFER(TextureRTV, TextureRTVImpl)

    class Texture
    {
    public:
        Texture() = default;

        void* map(const Device& device);
        void unmap(const Device& device);

        Format format() const;
        uint32_t width() const;
        uint32_t height() const;
        uint32_t depth() const;
        uint32_t arraySlices() const;
        uint32_t mipLevels() const;
        uint32_t samples() const;
        ResourceDimension dimension() const;

        TextureDescription description() const;
        ResourceState state(int slice, int mip) const;
        void state(int slice, int mip, ResourceState state);

    protected:
        friend class implementation::SwapChainImpl;
        friend class Device;
        friend class implementation::DeviceImpl;
        Texture(std::shared_ptr<implementation::TextureImpl> texture);

    protected:
        friend struct implementation::TextureImplGet;
        const implementation::TextureImpl* impl() const;
        implementation::TextureImpl* impl();
        std::shared_ptr<implementation::TextureImpl> m_impl;
    };

    class TextureSRV
    {
    public:
        TextureSRV() = default;

        bool valid() const;

        Format format() const;
        uint32_t width() const;
        uint32_t height() const;
        uint32_t depth() const;

        Texture& texture();
        const Texture& texture() const;

        const SubResource& subResource() const;

    protected:
        friend class implementation::SwapChainImpl;
        friend class Device;
        friend class implementation::DeviceImpl;
        TextureSRV(std::shared_ptr<implementation::TextureSRVImpl> view);
        
        friend struct implementation::TextureSRVImplGet;
        const implementation::TextureSRVImpl* impl() const;
        implementation::TextureSRVImpl* impl();
        std::shared_ptr<implementation::TextureSRVImpl> m_impl;
    };

    class TextureUAV
    {
    public:
        TextureUAV() = default;
        
        bool valid() const;

        Format format() const;
        uint32_t width() const;
        uint32_t height() const;
        uint32_t depth() const;

        Texture& texture();
        const Texture& texture() const;

        const SubResource& subResource() const;

    protected:
        friend class implementation::SwapChainImpl;
        friend class Device;
        friend class implementation::DeviceImpl;
        TextureUAV(std::shared_ptr<implementation::TextureUAVImpl> view);

        friend struct implementation::TextureUAVImplGet;
        const implementation::TextureUAVImpl* impl() const;
        implementation::TextureUAVImpl* impl();
        std::shared_ptr<implementation::TextureUAVImpl> m_impl;
    };

    class TextureDSV
    {
    public:
        TextureDSV() = default;

        bool valid() const;

        Format format() const;
        uint32_t width() const;
        uint32_t height() const;

        Texture& texture();
        const Texture& texture() const;

        const SubResource& subResource() const;

    protected:
        friend class implementation::SwapChainImpl;
        friend class Device;
        friend class implementation::DeviceImpl;
        TextureDSV(std::shared_ptr<implementation::TextureDSVImpl> view);

        friend struct implementation::TextureDSVImplGet;
        const implementation::TextureDSVImpl* impl() const;
        implementation::TextureDSVImpl* impl();
        std::shared_ptr<implementation::TextureDSVImpl> m_impl;

    };

    class TextureRTV
    {
    public:
        TextureRTV() = default;

        bool valid() const;

        Format format() const;
        uint32_t width() const;
        uint32_t height() const;

        Texture& texture();
        const Texture& texture() const;

        const SubResource& subResource() const;

    protected:
        friend class implementation::SwapChainImpl;
        friend class Device;
        friend class implementation::DeviceImpl;
        friend class implementation::CommandListImpl;
        TextureRTV(std::shared_ptr<implementation::TextureRTVImpl> view);

        friend struct implementation::TextureRTVImplGet;
        const implementation::TextureRTVImpl* impl() const;
        implementation::TextureRTVImpl* impl();
        std::shared_ptr<implementation::TextureRTVImpl> m_impl;

    };

    class TextureBindlessSRV
    {
    public:
        int32_t id(const ResourceKey& key)
        {
            for(int i = 0; i < m_textures.size(); ++i)
            {
                if (m_textures[i].key == key)
                    return i;
            }
            return -1;
        }
        uint32_t push(const ResourceKey& key, TextureSRV texture)
        {
            auto texId = id(key);
            if (texId == -1)
            {
                m_textures.emplace_back(Container{ key, texture });
                return static_cast<uint32_t>(m_textures.size() - 1);
            }
            return static_cast<uint32_t>(texId);
        }
        
        TextureSRV& operator[](int index)
        {
            return m_textures[index].texture;
        }

        const TextureSRV& operator[](int index) const
        {
            return m_textures[index].texture;
        }

        size_t size() const
        {
            return m_textures.size();
        }
    private:
        struct Container
        {
            ResourceKey key;
            TextureSRV texture;
        };
        std::vector<Container> m_textures;
    };

    class TextureBindlessUAV
    {
    public:
        int32_t id(const ResourceKey& key)
        {
            for (int i = 0; i < m_textures.size(); ++i)
            {
                if (m_textures[i].key == key)
                    return i;
            }
            return -1;
        }
        uint32_t push(const ResourceKey& key, TextureUAV& texture)
        {
            auto texId = id(key);
            if (texId == -1)
            {
                m_textures.emplace_back(Container{ key, texture });
                return static_cast<uint32_t>(m_textures.size() - 1);
            }
            return static_cast<uint32_t>(texId);
        }
        TextureUAV& operator[](int index)
        {
            return m_textures[index].texture;
        }

        size_t size() const
        {
            return m_textures.size();
        }
    private:
        struct Container
        {
            ResourceKey key;
            TextureUAV texture;
        };
        std::vector<Container> m_textures;
    };

    class BufferBindlessSRV
    {
    public:
        int32_t id(const ResourceKey& key)
        {
            for (int i = 0; i < m_buffers.size(); ++i)
            {
                if (m_buffers[i].key == key)
                    return i;
            }
            return -1;
        }
        uint32_t push(const ResourceKey& key, BufferSRV& buffer)
        {
            auto bufId = id(key);
            if (bufId == -1)
            {
                m_buffers.emplace_back(Container{ key, buffer });
                return static_cast<uint32_t>(m_buffers.size() - 1);
            }
            return static_cast<uint32_t>(bufId);
        }
        BufferSRV& operator[](int index)
        {
            return m_buffers[index].buffer;
        }

        size_t size() const
        {
            return m_buffers.size();
        }
    private:
        struct Container
        {
            ResourceKey key;
            BufferSRV buffer;
        };
        std::vector<Container> m_buffers;
    };

    class BufferBindlessUAV
    {
    public:
        int32_t id(const ResourceKey& key)
        {
            for (int i = 0; i < m_buffers.size(); ++i)
            {
                if (m_buffers[i].key == key)
                    return i;
            }
            return -1;
        }
        uint32_t push(const ResourceKey& key, BufferUAV& buffer)
        {
            auto bufId = id(key);
            if (bufId == -1)
            {
                m_buffers.emplace_back(Container{ key, buffer });
                return static_cast<uint32_t>(m_buffers.size() - 1);
            }
            return static_cast<uint32_t>(bufId);
        }
        BufferUAV& operator[](int index)
        {
            return m_buffers[index].buffer;
        }

        size_t size() const
        {
            return m_buffers.size();
        }
    private:
        struct Container
        {
            ResourceKey key;
            BufferUAV buffer;
        };
        std::vector<Container> m_buffers;
    };
}
