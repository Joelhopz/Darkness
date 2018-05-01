#include "engine/graphics/dx12/DX12Resources.h"
#include "engine/graphics/dx12/DX12Headers.h"
#include "engine/graphics/dx12/DX12Device.h"
#include "engine/graphics/dx12/DX12DescriptorHeap.h"
#include "engine/graphics/Barrier.h"

#include "tools/Debug.h"

#include <inttypes.h>

using namespace tools;

namespace engine
{
    namespace implementation
    {
        D3D12_HEAP_PROPERTIES getHeapPropertiesFromUsage(ResourceUsage usage)
        {
            D3D12_HEAP_PROPERTIES heapProperties{ D3D12_HEAP_TYPE_DEFAULT };
            if (usage == ResourceUsage::Upload)
            {
                heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
            }
            else if (usage == ResourceUsage::GpuToCpu)
            {
                heapProperties.Type = D3D12_HEAP_TYPE_READBACK;
            }
            else if (usage == ResourceUsage::GpuRead)
            {
                heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
            }
            return heapProperties;
        }

        ResourceState getResourceStateFromUsage(ResourceUsage usage)
        {
            ResourceState resState;
            if (usage == ResourceUsage::Upload)
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
        
        D3D12_RESOURCE_FLAGS flagsFromUsage(ResourceUsage usage)
        {
            switch (usage)
            {
            case ResourceUsage::Upload: return D3D12_RESOURCE_FLAG_NONE;
            case ResourceUsage::DepthStencil: return D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
            case ResourceUsage::GpuRead: return D3D12_RESOURCE_FLAG_NONE;
            case ResourceUsage::GpuReadWrite: return D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            case ResourceUsage::GpuRenderTargetRead: return D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
            case ResourceUsage::GpuRenderTargetReadWrite: return D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            case ResourceUsage::GpuToCpu: return D3D12_RESOURCE_FLAG_NONE;
            }
            return D3D12_RESOURCE_FLAG_NONE;
        }

        static uint64_t debugResourceAllocation = 0;
        static uint64_t debugBufferAllocation = 0;
        static uint64_t debugTextureAllocation = 0;
        uint64_t bytesToMB(uint64_t bytes) { return bytes / 1024u / 1024u; }
        uint64_t bytesToKB(uint64_t bytes) { return bytes / 1024u; }

        void debugMemory(const char* name, uint64_t newBytes, bool buffer)
        {
            debugResourceAllocation += newBytes;
            if (buffer)
                debugBufferAllocation += newBytes;
            else
                debugTextureAllocation += newBytes;

            if(newBytes > 1024u * 1024u)
                LOG_WARNING("[RESOURCES: %" PRIu64 " MB, BUFFERS: %" PRIu64 " MB, TEXTURES: %" PRIu64 " MB] new allocation: %" PRIu64 " MB, %s",
                    bytesToMB(debugResourceAllocation),
                    bytesToMB(debugBufferAllocation),
                    bytesToMB(debugTextureAllocation),
                    bytesToMB(newBytes),
                    name);
            else if(newBytes > 1024u)
                LOG_WARNING("[RESOURCES: %" PRIu64 " MB, BUFFERS: %" PRIu64 " MB, TEXTURES: %" PRIu64 " MB] new allocation: %" PRIu64 " KB, %s",
                    bytesToMB(debugResourceAllocation),
                    bytesToMB(debugBufferAllocation),
                    bytesToMB(debugTextureAllocation),
                    bytesToKB(newBytes),
                    name);
            else
                LOG_WARNING("[RESOURCES: %" PRIu64 " MB, BUFFERS: %" PRIu64 " MB, TEXTURES: %" PRIu64 " MB] new allocation: %" PRIu64 " Bytes, %s",
                    bytesToMB(debugResourceAllocation),
                    bytesToMB(debugBufferAllocation),
                    bytesToMB(debugTextureAllocation),
                    newBytes,
                    name);
        }

        BufferImpl::BufferImpl(const DeviceImpl& device, const BufferDescription& desc)
            : m_description(desc.descriptor)
            , m_state{ ResourceState::Common }
            //, m_state{ getResourceStateFromUsage(m_description.usage) }
        {
            auto elementSize = m_description.elementSize == -1 ? formatBytes(m_description.format) : m_description.elementSize;
            m_bufferSize = m_description.elements * elementSize;

            debugMemory(m_description.name, m_bufferSize, true);

            ASSERT(m_description.name);
            D3D12_RESOURCE_DESC res = {};
            res.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
            res.DepthOrArraySize = 1;
            res.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
            res.Flags = flagsFromUsage(m_description.usage);
            res.Format = DXGI_FORMAT_UNKNOWN;
            res.Height = 1;
            res.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            res.MipLevels = 1;
            res.SampleDesc.Count = 1;
            res.SampleDesc.Quality = 0;
            if(m_description.append)
                res.Width = roundUpToMultiple(m_bufferSize, D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT) + sizeof(uint32_t);
            else
                res.Width = m_bufferSize;

            D3D12_HEAP_PROPERTIES heapProperties = getHeapPropertiesFromUsage(m_description.usage);
            
            if (heapProperties.Type == D3D12_HEAP_TYPE_UPLOAD)
            {
                m_state = ResourceState::GenericRead;
            }
            if (heapProperties.Type == D3D12_HEAP_TYPE_READBACK)
            {
                m_state = ResourceState::CopyDest;
            }
            
            D3D12_RESOURCE_STATES resState = dxResourceStates(m_state);

            auto success = device.device()->CreateCommittedResource(
                &heapProperties, 
                D3D12_HEAP_FLAG_NONE,
                &res,
                resState,
                nullptr, 
                DARKNESS_IID_PPV_ARGS(m_buffer.GetAddressOf()));

            ASSERT(SUCCEEDED(success));

#ifndef RELEASE

#ifdef _UNICODE
            static TCHAR resourceName[1024] = {};
            size_t numCharacters;
            if(m_description.name)
                mbstowcs_s(&numCharacters, resourceName, m_description.name, 1024);
#else
            wchar_t resourceName[1024] = {};
            size_t numCharacters;
            mbstowcs_s(&numCharacters, resourceName, m_description.name, 1024);
#endif

            m_buffer->SetName(resourceName);
#endif

            /*if (desc.initialData)
            {
                void* data;
                auto mapres = m_buffer->Map(0, nullptr, &data);
                ASSERT(SUCCEEDED(mapres));
                memcpy(data, desc.initialData.data.data(), desc.initialData.data.size());
                m_buffer->Unmap(0, nullptr);
            }*/
        }

        void* BufferImpl::map(const DeviceImpl& /*device*/)
        {
            void* data = nullptr;
            D3D12_RANGE range;
            range.Begin = 0;
            range.End = m_bufferSize;
            auto mapres = m_buffer->Map(0, &range, &data);
            ASSERT(SUCCEEDED(mapres));
            return data;
        }

        void BufferImpl::unmap(const DeviceImpl& /*device*/)
        {
            D3D12_RANGE range;
            range.Begin = 0;
            range.End = 0;
            m_buffer->Unmap(0, &range);
        }

        const BufferDescription::Descriptor& BufferImpl::description() const
        {
            return m_description;
        }

        ID3D12Resource* BufferImpl::native() const
        {
            return m_buffer.Get();
        }

        ResourceState BufferImpl::state() const
        {
            return m_state;
        }

        void BufferImpl::state(ResourceState _state)
        {
            m_state = _state;
        }

        BufferSRVImpl::BufferSRVImpl(
            const DeviceImpl& device,
            const Buffer& buffer,
            const BufferDescription& desc)
            : m_description(desc.descriptor)
            , m_viewHandle{ device.heaps().cbv_srv_uav->getDescriptor() }
            , m_buffer{ buffer }
        {
            auto elements = m_description.elements != -1 ? m_description.elements : buffer.description().descriptor.elements;
            auto elementSize = m_description.elementSize != -1 ? m_description.elementSize : buffer.description().descriptor.elementSize;

            D3D12_SHADER_RESOURCE_VIEW_DESC viewdesc;
            viewdesc.Format = desc.descriptor.structured ? DXGI_FORMAT_UNKNOWN : dxFormat(m_description.format);
            viewdesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
            viewdesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            viewdesc.Buffer.FirstElement = m_description.firstElement;
            viewdesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
            viewdesc.Buffer.NumElements = static_cast<UINT>(elements);
            viewdesc.Buffer.StructureByteStride = desc.descriptor.structured ? static_cast<UINT>(elementSize) : 0;

            device.device()->CreateShaderResourceView(
                BufferImplGet::impl(buffer)->native(),
                &viewdesc,
                m_viewHandle.cpuHandle());
        }

        const BufferDescription::Descriptor& BufferSRVImpl::description() const
        {
            return m_description;
        }

        D3D12_CPU_DESCRIPTOR_HANDLE& BufferSRVImpl::native()
        {
            return m_viewHandle.cpuHandle();
        }

        const D3D12_CPU_DESCRIPTOR_HANDLE& BufferSRVImpl::native() const
        {
            return m_viewHandle.cpuHandle();
        }

        Buffer& BufferSRVImpl::buffer()
        {
            return m_buffer;
        }

        const Buffer& BufferSRVImpl::buffer() const
        {
            return m_buffer;
        }

        uint32_t BufferUAVImpl::structureCounterOffsetBytes() const
        {
            return roundUpToMultiple(m_description.elements * m_description.elementSize, D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT);
        }

        BufferUAVImpl::BufferUAVImpl(
            const DeviceImpl& device,
            const Buffer& buffer,
            const BufferDescription& desc)
            : m_description(desc.descriptor)
            , m_viewHandle{ device.heaps().cbv_srv_uav->getDescriptor() }
            , m_buffer{ buffer }
        {
            m_description.elements = m_description.elements != -1 ? m_description.elements : buffer.description().descriptor.elements;
            m_description.elementSize = m_description.elementSize != -1 ? m_description.elementSize : static_cast<int32_t>(formatBytes(m_description.format));
            if (m_description.elementSize == -1)
                m_description.elementSize = buffer.description().descriptor.elementSize;
            if (m_description.elementSize == -1)
                m_description.elementSize = static_cast<int32_t>(formatBytes(buffer.description().descriptor.format));

            /*if (m_description.append)
            {
                D3D12_RESOURCE_DESC res = {};
                res.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;// D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT;
                res.DepthOrArraySize = 1;
                res.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
                res.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
                res.Format = dxFormat(m_description.format);
                res.Height = 1;
                res.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
                res.MipLevels = 1;
                res.SampleDesc.Count = 1;
                res.SampleDesc.Quality = 0;
                res.Width = sizeof(uint32_t);

                D3D12_HEAP_PROPERTIES heapProperties{ D3D12_HEAP_TYPE_READBACK };
                auto success = device.device()->CreateCommittedResource(
                    &heapProperties,
                    D3D12_HEAP_FLAG_NONE,
                    &res,
                    D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
                    nullptr,
                    DARKNESS_IID_PPV_ARGS(m_counterBuffer.GetAddressOf()));

                ASSERT(SUCCEEDED(success));

                setCounterValue(0);
            }*/

            D3D12_UNORDERED_ACCESS_VIEW_DESC viewdesc;
            viewdesc.Format = dxFormat(m_description.format);
            viewdesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
            viewdesc.Buffer.FirstElement = m_description.firstElement;
            viewdesc.Buffer.NumElements = static_cast<UINT>(m_description.elements);
            viewdesc.Buffer.StructureByteStride = (m_description.structured || m_description.append) ? static_cast<UINT>(m_description.elementSize) : 0;
            if (m_description.append)
            {
                viewdesc.Buffer.CounterOffsetInBytes = structureCounterOffsetBytes();
                viewdesc.Format = DXGI_FORMAT_UNKNOWN;
            }
            else
                viewdesc.Buffer.CounterOffsetInBytes = 0;
            viewdesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

            device.device()->CreateUnorderedAccessView(
                BufferImplGet::impl(buffer)->native(),
                m_description.append ? BufferImplGet::impl(buffer)->native() : nullptr,
                //m_description.append ? m_counterBuffer.Get() : nullptr,
                &viewdesc,
                m_viewHandle.cpuHandle());
        }

        /*void BufferUAVImpl::setCounterValue(uint32_t value)
        {
            uint8_t* counterData = nullptr;
            D3D12_RANGE counterRange = { 
                static_cast<SIZE_T>(roundUpToMultiple(m_description.elements * m_description.elementSize, D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT)),
                static_cast<SIZE_T>(roundUpToMultiple(m_description.elements * m_description.elementSize, D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT) + sizeof(uint32_t)) };
            BufferImplGet::impl(m_buffer)->native()->Map(0, &counterRange, reinterpret_cast<void**>(&counterData));
            memcpy(counterData, &value, sizeof(uint32_t));
            BufferImplGet::impl(m_buffer)->native()->Unmap(0, nullptr);
        }

        uint32_t BufferUAVImpl::getCounterValue()
        {
            uint32_t res = 0;
            uint8_t* counterData = nullptr;
            D3D12_RANGE counterRange = {
                static_cast<SIZE_T>(roundUpToMultiple(m_description.elements * m_description.elementSize, D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT)),
                static_cast<SIZE_T>(roundUpToMultiple(m_description.elements * m_description.elementSize, D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT) + sizeof(uint32_t)) };
            BufferImplGet::impl(m_buffer)->native()->Map(0, &counterRange, reinterpret_cast<void**>(&counterData));
            memcpy(&res, counterData, sizeof(uint32_t));
            BufferImplGet::impl(m_buffer)->native()->Unmap(0, nullptr);
            return res;
        }*/

        const BufferDescription::Descriptor& BufferUAVImpl::description() const
        {
            return m_description;
        }

        D3D12_CPU_DESCRIPTOR_HANDLE& BufferUAVImpl::native()
        {
            return m_viewHandle.cpuHandle();
        }

        const D3D12_CPU_DESCRIPTOR_HANDLE& BufferUAVImpl::native() const
        {
            return m_viewHandle.cpuHandle();
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
            , m_viewHandle{ device.heaps().cbv_srv_uav->getDescriptor() }
            , m_buffer{ buffer }
        {
            auto elements = m_description.elements != -1 ? m_description.elements : buffer.description().descriptor.elements;
            auto elementSize = m_description.elementSize != -1 ? m_description.elementSize : buffer.description().descriptor.elementSize;

            D3D12_SHADER_RESOURCE_VIEW_DESC viewdesc;
            viewdesc.Format = dxFormat(m_description.format);
            if (viewdesc.Format == DXGI_FORMAT_UNKNOWN)
                viewdesc.Format = dxFormat(buffer.description().descriptor.format);
            if (viewdesc.Format == DXGI_FORMAT_UNKNOWN)
                viewdesc.Format = m_description.elementSize == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
            viewdesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
            viewdesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            viewdesc.Buffer.FirstElement = m_description.firstElement;
            viewdesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
            viewdesc.Buffer.NumElements = static_cast<UINT>(elements);
            viewdesc.Buffer.StructureByteStride = desc.descriptor.structured ? static_cast<UINT>(elementSize) : 0;

            m_view.BufferLocation = BufferImplGet::impl(buffer)->native()->GetGPUVirtualAddress();
            m_view.SizeInBytes = elements * elementSize;
            m_view.Format = viewdesc.Format;

            device.device()->CreateShaderResourceView(
                BufferImplGet::impl(buffer)->native(),
                &viewdesc,
                m_viewHandle.cpuHandle());
        }

        const BufferDescription::Descriptor& BufferIBVImpl::description() const
        {
            return m_description;
        }

        D3D12_CPU_DESCRIPTOR_HANDLE& BufferIBVImpl::native()
        {
            return m_viewHandle.cpuHandle();
        }

        const D3D12_CPU_DESCRIPTOR_HANDLE& BufferIBVImpl::native() const
        {
            return m_viewHandle.cpuHandle();
        }

        Buffer& BufferIBVImpl::buffer()
        {
            return m_buffer;
        }

        const Buffer& BufferIBVImpl::buffer() const
        {
            return m_buffer;
        }

        const D3D12_INDEX_BUFFER_VIEW* BufferIBVImpl::view() const
        {
            return &m_view;
        }

        BufferCBVImpl::BufferCBVImpl(
            const DeviceImpl& device,
            const Buffer& buffer,
            const BufferDescription& desc)
            : m_description(desc.descriptor)
            , m_viewHandle{ device.heaps().cbv_srv_uav->getDescriptor() }
            , m_buffer{ buffer }
        {
            auto elements = m_description.elements != -1 ? m_description.elements : buffer.description().descriptor.elements;
            auto elementSize = m_description.elementSize != -1 ? m_description.elementSize : buffer.description().descriptor.elementSize;

            D3D12_CONSTANT_BUFFER_VIEW_DESC view;
            view.BufferLocation = BufferImplGet::impl(buffer)->native()->GetGPUVirtualAddress();
            view.SizeInBytes = (elements * elementSize);
            device.device()->CreateConstantBufferView(&view, m_viewHandle.cpuHandle());
        }

        const BufferDescription::Descriptor& BufferCBVImpl::description() const
        {
            return m_description;
        }

        D3D12_CPU_DESCRIPTOR_HANDLE& BufferCBVImpl::native()
        {
            return m_viewHandle.cpuHandle();
        }

        const D3D12_CPU_DESCRIPTOR_HANDLE& BufferCBVImpl::native() const
        {
            return m_viewHandle.cpuHandle();
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
            , m_viewHandle{ device.heaps().cbv_srv_uav->getDescriptor() }
            , m_buffer{ buffer }
        {
            auto elements = m_description.elements != -1 ? m_description.elements : buffer.description().descriptor.elements;
            auto elementSize = m_description.elementSize != -1 ? m_description.elementSize : buffer.description().descriptor.elementSize;

            m_view.BufferLocation = BufferImplGet::impl(buffer)->native()->GetGPUVirtualAddress();
            m_view.SizeInBytes = elements * elementSize;
            m_view.StrideInBytes = desc.descriptor.structured ? static_cast<UINT>(elementSize) : 0;

            D3D12_SHADER_RESOURCE_VIEW_DESC viewdesc;
            viewdesc.Format = dxFormat(m_description.format);

            viewdesc.Format = dxFormat(m_description.format);
            if (viewdesc.Format == DXGI_FORMAT_UNKNOWN)
                viewdesc.Format = dxFormat(buffer.description().descriptor.format);
            if (viewdesc.Format == DXGI_FORMAT_UNKNOWN)
                viewdesc.Format = DXGI_FORMAT_R32_UINT;

            if (desc.descriptor.structured)
                viewdesc.Format = DXGI_FORMAT_UNKNOWN;

            viewdesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
            viewdesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            viewdesc.Buffer.FirstElement = m_description.firstElement;
            viewdesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
            viewdesc.Buffer.NumElements = static_cast<UINT>(elements);
            viewdesc.Buffer.StructureByteStride = desc.descriptor.structured ? static_cast<UINT>(elementSize) : 0;

            device.device()->CreateShaderResourceView(
                BufferImplGet::impl(buffer)->native(),
                &viewdesc,
                m_viewHandle.cpuHandle());
        }

        const BufferDescription::Descriptor& BufferVBVImpl::description() const
        {
            return m_description;
        }

        D3D12_CPU_DESCRIPTOR_HANDLE& BufferVBVImpl::native()
        {
            return m_viewHandle.cpuHandle();
        }

        const D3D12_CPU_DESCRIPTOR_HANDLE& BufferVBVImpl::native() const
        {
            return m_viewHandle.cpuHandle();
        }

        Buffer& BufferVBVImpl::buffer()
        {
            return m_buffer;
        }

        const Buffer& BufferVBVImpl::buffer() const
        {
            return m_buffer;
        }

        const D3D12_VERTEX_BUFFER_VIEW* BufferVBVImpl::view() const
        {
            return &m_view;
        }

        typedef enum D3D11_STANDARD_MULTISAMPLE_QUALITY_LEVELS {
            D3D11_STANDARD_MULTISAMPLE_PATTERN = 0xffffffff,
            D3D11_CENTER_MULTISAMPLE_PATTERN = 0xfffffffe
        } D3D11_STANDARD_MULTISAMPLE_QUALITY_LEVELS;

        TextureImpl::TextureImpl(
            const DeviceImpl& device,
            const TextureDescription& desc)
            : m_description(desc.descriptor)
        {
            for (int slice = 0; slice < static_cast<int>(m_description.arraySlices); ++slice)
            {
                for (int mip = 0; mip < static_cast<int>(m_description.mipLevels); ++mip)
                {
                    m_state.emplace_back(ResourceState::Common);
                }
            }

            auto bytesAllocated = imageBytes(m_description.format, m_description.width, m_description.height, m_description.arraySlices, m_description.mipLevels);
            debugMemory(m_description.name, bytesAllocated, false);

            ASSERT(m_description.name);
            D3D12_RESOURCE_DESC res = {};
            res.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
            res.DepthOrArraySize = static_cast<UINT16>(m_description.arraySlices);
            res.Dimension = dxResourceDimension(m_description.dimension);
            res.Flags = flagsFromUsage(desc.descriptor.usage);
            res.Format = dxTypelessFormat(m_description.format);
            
            if (m_description.format == Format::D32_FLOAT)
                res.Format = DXGI_FORMAT_R32_TYPELESS;

            if ((m_description.usage == ResourceUsage::GpuRead) ||
                (m_description.usage == ResourceUsage::GpuReadWrite) ||
                (m_description.usage == ResourceUsage::GpuRenderTargetRead) ||
                (m_description.usage == ResourceUsage::GpuRenderTargetReadWrite) ||
                (m_description.usage == ResourceUsage::DepthStencil))
                res.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
            else
                res.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            //res.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
            
            if (m_description.dimension == ResourceDimension::TextureCubemap)
            {
                ASSERT(m_description.width <= D3D12_REQ_TEXTURECUBE_DIMENSION, "Too big texture for cubemap");
                ASSERT(m_description.height <= D3D12_REQ_TEXTURECUBE_DIMENSION, "Too big texture for cubemap");
                ASSERT(res.DepthOrArraySize <= D3D12_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION, "Too many array slices for cubemap");
            }

            
            res.SampleDesc.Count = m_description.samples;
            res.SampleDesc.Quality = m_description.samples > 1 ? D3D11_STANDARD_MULTISAMPLE_PATTERN : 0;
            res.Width = m_description.width;
            res.Height = m_description.height;
            res.MipLevels = static_cast<UINT16>(m_description.mipLevels);

            D3D12_HEAP_PROPERTIES heapProperties = {};// = getHeapPropertiesFromUsage(m_description.usage);
            heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
            heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            heapProperties.CreationNodeMask = 0;
            heapProperties.VisibleNodeMask = 0;

            D3D12_RESOURCE_STATES resState = dxResourceStates(ResourceState::Common);

            D3D12_CLEAR_VALUE clearValue = {};
            if ((m_description.usage == ResourceUsage::GpuRenderTargetRead) ||
                (m_description.usage == ResourceUsage::GpuRenderTargetReadWrite))
            {
                clearValue.Color[0] = m_description.optimizedClearValue.x;
                clearValue.Color[1] = m_description.optimizedClearValue.y;
                clearValue.Color[2] = m_description.optimizedClearValue.z;
                clearValue.Color[3] = m_description.optimizedClearValue.w;
            }
            else if (m_description.usage == ResourceUsage::DepthStencil)
            {
                clearValue.DepthStencil.Depth = m_description.optimizedDepthClearValue;
                clearValue.DepthStencil.Stencil = m_description.optimizedStencilClearValue;
            }
            clearValue.Format = dxFormat(m_description.format);

            bool setClearValue =
                ((res.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) == D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) ||
                ((res.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) == D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

            auto success = device.device()->CreateCommittedResource(
                &heapProperties,
                D3D12_HEAP_FLAG_NONE,
                &res,
                resState,
                setClearValue ? &clearValue : nullptr,
                DARKNESS_IID_PPV_ARGS(m_texture.GetAddressOf()));

            ASSERT(SUCCEEDED(success));

#ifndef RELEASE

#ifdef _UNICODE
            static TCHAR resourceName[1024] = {};
            size_t numCharacters;
            if(m_description.name)
                mbstowcs_s(&numCharacters, resourceName, m_description.name, 1024);
#else
            wchar_t resourceName[1024] = {};
            size_t numCharacters;
            mbstowcs_s(&numCharacters, resourceName, m_description.name, 1024);
#endif

            m_texture->SetName(resourceName);
#endif

            if (desc.initialData)
            {
                /*void* data;
                D3D12_RANGE range;
                range.Begin = desc.initialData.elementStart * desc.initialData.elementSize;
                range.End = desc.initialData.data.size();
                auto mapres = m_buffer->Map(0, &range, &data);
                ASSERT(SUCCEEDED(mapres));
                memcpy(data, desc.initialData.data.data(), desc.initialData.data.size());
                m_buffer->Unmap(0, &range);*/
            }
        }

        TextureImpl::TextureImpl(
            const DeviceImpl& /*device*/,
            const TextureDescription& desc,
            ID3D12Resource* resource,
            ResourceState currentState)
            : m_description(desc.descriptor)
            , m_state{ currentState }
        {
            m_texture.Attach(resource);
        }

        void* TextureImpl::map(const DeviceImpl& /*device*/)
        {
            void* data;
            D3D12_RANGE range;
            range.Begin = 0;
            range.End = 0;
            auto mapres = m_texture->Map(0, &range, &data);
            ASSERT(SUCCEEDED(mapres));
            return data;
        }

        void TextureImpl::unmap(const DeviceImpl& /*device*/)
        {
            D3D12_RANGE range;
            range.Begin = 0;
            range.End = 0;
            m_texture->Unmap(0, nullptr);
        }

        const TextureDescription::Descriptor& TextureImpl::description() const
        {
            return m_description;
        }

        ID3D12Resource* TextureImpl::native() const
        {
            return m_texture.Get();
        }

        ResourceState TextureImpl::state(int slice, int mip) const
        {
            return m_state[mip + (slice * m_description.mipLevels)];
        }

        void TextureImpl::state(int slice, int mip, ResourceState _state)
        {
            m_state[mip + (slice * m_description.mipLevels)] = _state;
        }

        TextureSRVImpl::TextureSRVImpl(
            const DeviceImpl& device,
            const Texture& texture,
            const TextureDescription& desc,
            SubResource subResources)
            : m_description(desc.descriptor)
            , m_viewHandle{ device.heaps().cbv_srv_uav->getDescriptor() }
            , m_texture{ texture }
            , m_subResources{ subResources }
        {
            D3D12_SHADER_RESOURCE_VIEW_DESC viewdesc;
            viewdesc.Format = dxFormat(m_description.format);

            // D3D12_ENCODE_SHADER_4_COMPONENT_MAPPING(2,1,0,3);
            viewdesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            switch (m_description.dimension)
            {
                case ResourceDimension::Texture1D:
                {
                    viewdesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
                    viewdesc.Texture1D.MipLevels = static_cast<UINT>(subResources.mipCount != AllMipLevels ? subResources.mipCount : texture.mipLevels());
                    viewdesc.Texture1D.MostDetailedMip = static_cast<UINT>(subResources.firstMipLevel);
                    viewdesc.Texture1D.ResourceMinLODClamp = 0.0f;
                    break;
                }
                case ResourceDimension::Texture1DArray:
                {
                    viewdesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
                    viewdesc.Texture1DArray.MipLevels = static_cast<UINT>(subResources.mipCount != AllMipLevels ? subResources.mipCount : texture.mipLevels());
                    viewdesc.Texture1DArray.MostDetailedMip = static_cast<UINT>(subResources.firstMipLevel);
                    viewdesc.Texture1DArray.ResourceMinLODClamp = 0.0f;
                    viewdesc.Texture1DArray.ArraySize = static_cast<UINT>(subResources.arraySliceCount != AllArraySlices ? subResources.arraySliceCount : texture.arraySlices());
                    viewdesc.Texture1DArray.FirstArraySlice = static_cast<UINT>(subResources.firstArraySlice);
                    break;
                }
                case ResourceDimension::Texture2D:
                {
                    viewdesc.ViewDimension = m_description.samples > 1 ? D3D12_SRV_DIMENSION_TEXTURE2DMS : D3D12_SRV_DIMENSION_TEXTURE2D;
                    viewdesc.Texture2D.MipLevels = static_cast<UINT>(subResources.mipCount != AllMipLevels ? subResources.mipCount : texture.mipLevels());
                    viewdesc.Texture2D.MostDetailedMip = static_cast<UINT>(subResources.firstMipLevel);
                    viewdesc.Texture2D.ResourceMinLODClamp = 0.0f;
                    viewdesc.Texture2D.PlaneSlice = 0u;
                    break;
                }
                case ResourceDimension::Texture2DArray:
                {
                    viewdesc.ViewDimension = m_description.samples > 1 ? D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY : D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
                    viewdesc.Texture2DArray.MipLevels = static_cast<UINT>(subResources.mipCount != AllMipLevels ? subResources.mipCount : texture.mipLevels());
                    viewdesc.Texture2DArray.MostDetailedMip = static_cast<UINT>(subResources.firstMipLevel);
                    viewdesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
                    viewdesc.Texture2DArray.ArraySize = static_cast<UINT>(subResources.arraySliceCount != AllArraySlices ? subResources.arraySliceCount : texture.arraySlices());
                    viewdesc.Texture2DArray.FirstArraySlice = static_cast<UINT>(subResources.firstArraySlice);
                    viewdesc.Texture2DArray.PlaneSlice = 0;
                    break;
                }
                case ResourceDimension::Texture3D:
                {
                    viewdesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
                    viewdesc.Texture3D.MipLevels = static_cast<UINT>(subResources.mipCount != AllMipLevels ? subResources.mipCount : texture.mipLevels());
                    viewdesc.Texture3D.MostDetailedMip = static_cast<UINT>(subResources.firstMipLevel);
                    viewdesc.Texture3D.ResourceMinLODClamp = 0.0f;
                    break;
                }
                case ResourceDimension::TextureCubemap:
                {
                    viewdesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
                    viewdesc.TextureCube.MipLevels = static_cast<UINT>(subResources.mipCount != AllMipLevels ? subResources.mipCount : texture.mipLevels());
                    viewdesc.TextureCube.MostDetailedMip = static_cast<UINT>(subResources.firstMipLevel);
                    viewdesc.TextureCube.ResourceMinLODClamp = 0.0f;
                    break;
                }
                case ResourceDimension::TextureCubemapArray:
                {
                    viewdesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
                    viewdesc.TextureCubeArray.MipLevels = static_cast<UINT>(subResources.mipCount != AllMipLevels ? subResources.mipCount : texture.mipLevels());
                    viewdesc.TextureCubeArray.MostDetailedMip = static_cast<UINT>(subResources.firstMipLevel);
                    viewdesc.TextureCubeArray.ResourceMinLODClamp = 0.0f;
                    viewdesc.TextureCubeArray.First2DArrayFace = static_cast<UINT>(subResources.firstArraySlice);
                    viewdesc.TextureCubeArray.NumCubes = static_cast<UINT>(subResources.arraySliceCount != AllArraySlices ? subResources.arraySliceCount : texture.arraySlices());
                    break;
                }
            }

            device.device()->CreateShaderResourceView(
                TextureImplGet::impl(texture)->native(),
                &viewdesc,
                m_viewHandle.cpuHandle());
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

        uint32_t TextureSRVImpl::depth() const
        {
            return m_description.depth;
        }

        D3D12_CPU_DESCRIPTOR_HANDLE& TextureSRVImpl::native()
        {
            return m_viewHandle.cpuHandle();
        }

        const D3D12_CPU_DESCRIPTOR_HANDLE& TextureSRVImpl::native() const
        {
            return m_viewHandle.cpuHandle();
        }

        const SubResource& TextureSRVImpl::subResource() const
        {
            return m_subResources;
        }

        TextureUAVImpl::TextureUAVImpl(
            const DeviceImpl& device,
            const Texture& texture,
            const TextureDescription& desc,
            SubResource subResources)
            : m_description(desc.descriptor)
            , m_viewHandle{ device.heaps().cbv_srv_uav->getDescriptor() }
            , m_texture{ texture }
            , m_subResources{ subResources }
        {
            if (m_description.append)
            {
                D3D12_RESOURCE_DESC res = {};
                res.Alignment = D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT;
                res.DepthOrArraySize = 1;
                res.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
                res.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET |
                    D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL |
                    D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS |
                    D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER |
                    D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS;
                res.Format = dxFormat(m_description.format);
                res.Height = 1;
                res.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
                res.MipLevels = 1;
                res.SampleDesc.Count = 1;
                res.SampleDesc.Quality = 0;
                res.Width = sizeof(uint32_t);

                D3D12_HEAP_PROPERTIES heapProperties{ D3D12_HEAP_TYPE_UPLOAD };
                auto success = device.device()->CreateCommittedResource(
                    &heapProperties,
                    D3D12_HEAP_FLAG_NONE,
                    &res,
                    D3D12_RESOURCE_STATE_GENERIC_READ,
                    nullptr,
                    DARKNESS_IID_PPV_ARGS(m_counterBuffer.GetAddressOf()));

                ASSERT(SUCCEEDED(success));

                //setCounterValue(0);
            }

            D3D12_UNORDERED_ACCESS_VIEW_DESC viewdesc;
            viewdesc.Format = dxFormat(m_description.format);
            switch (m_description.dimension)
            {
            case ResourceDimension::Texture1D:
            {
                viewdesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
                viewdesc.Texture1D.MipSlice = subResources.firstMipLevel;
                break;
            }
            case ResourceDimension::Texture1DArray:
            {
                viewdesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
                viewdesc.Texture1DArray.MipSlice = static_cast<UINT>(subResources.firstMipLevel);
                viewdesc.Texture1DArray.ArraySize = static_cast<UINT>(subResources.arraySliceCount != AllArraySlices ? subResources.arraySliceCount : texture.arraySlices());
                viewdesc.Texture1DArray.FirstArraySlice = static_cast<UINT>(subResources.firstArraySlice);
                break;
            }
            case ResourceDimension::Texture2D:
            {
                viewdesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
                viewdesc.Texture2D.MipSlice = static_cast<UINT>(subResources.firstMipLevel);
                viewdesc.Texture2D.PlaneSlice = 0;
                break;
            }
            case ResourceDimension::Texture2DArray:
            {
                viewdesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
                viewdesc.Texture2DArray.MipSlice = static_cast<UINT>(subResources.firstMipLevel);
                viewdesc.Texture2DArray.ArraySize = static_cast<UINT>(subResources.arraySliceCount != AllArraySlices ? subResources.arraySliceCount : texture.arraySlices());
                viewdesc.Texture2DArray.FirstArraySlice = static_cast<UINT>(subResources.firstArraySlice);
                viewdesc.Texture2DArray.PlaneSlice = 0;
                break;
            }
            case ResourceDimension::Texture3D:
            {
                viewdesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
                viewdesc.Texture3D.MipSlice = static_cast<UINT>(subResources.firstMipLevel);
                viewdesc.Texture3D.FirstWSlice = static_cast<UINT>(subResources.firstArraySlice);
                viewdesc.Texture3D.WSize = static_cast<UINT>(subResources.arraySliceCount != AllArraySlices ? subResources.arraySliceCount : texture.arraySlices());
                break;
            }
            }

            device.device()->CreateUnorderedAccessView(
                TextureImplGet::impl(texture)->native(),
                m_description.append ? m_counterBuffer.Get() : nullptr,
                &viewdesc,
                m_viewHandle.cpuHandle());
        }

        const TextureDescription::Descriptor& TextureUAVImpl::description() const
        {
            return m_description;
        }

        /*void TextureUAVImpl::setCounterValue(uint32_t value)
        {
            uint8_t* counterData = nullptr;
            D3D12_RANGE counterRange = { 0, 0 };
            m_counterBuffer->Map(0, &counterRange, reinterpret_cast<void**>(&counterData));
            memcpy(counterData, &value, sizeof(uint32_t));
            m_counterBuffer->Unmap(0, nullptr);
        }

        uint32_t TextureUAVImpl::getCounterValue()
        {
            uint32_t res = 0;
            uint8_t* counterData = nullptr;
            D3D12_RANGE counterRange = { 0, 0 };
            m_counterBuffer->Map(0, &counterRange, reinterpret_cast<void**>(&counterData));
            memcpy(&res, counterData, sizeof(uint32_t));
            m_counterBuffer->Unmap(0, nullptr);
            return res;
        }*/

        Texture& TextureUAVImpl::texture()
        {
            return m_texture;
        }

        const Texture& TextureUAVImpl::texture() const
        {
            return m_texture;
        }

        Format TextureUAVImpl::format() const
        {
            return m_description.format;
        }

        uint32_t TextureUAVImpl::width() const
        {
            return m_description.width;
        }

        uint32_t TextureUAVImpl::height() const
        {
            return m_description.height;
        }

        uint32_t TextureUAVImpl::depth() const
        {
            return m_description.depth;
        }

        D3D12_CPU_DESCRIPTOR_HANDLE& TextureUAVImpl::native()
        {
            return m_viewHandle.cpuHandle();
        }

        const D3D12_CPU_DESCRIPTOR_HANDLE& TextureUAVImpl::native() const
        {
            return m_viewHandle.cpuHandle();
        }

        const SubResource& TextureUAVImpl::subResource() const
        {
            return m_subResources;
        }

        TextureDSVImpl::TextureDSVImpl(
            const DeviceImpl& device,
            const Texture& texture,
            const TextureDescription& desc,
            SubResource subResources)
            : m_description(desc.descriptor)
            , m_viewHandle{ device.heaps().dsv->getDescriptor() }
            , m_texture{ texture }
            , m_subResources{ subResources }
        {
            D3D12_DEPTH_STENCIL_VIEW_DESC viewdesc;
            viewdesc.Format = dxFormat(m_description.format);
            viewdesc.Flags = D3D12_DSV_FLAG_NONE;
            switch (m_description.dimension)
            {
            case ResourceDimension::Texture1D:
            {
                viewdesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1D;
                viewdesc.Texture1D.MipSlice = static_cast<UINT>(subResources.firstMipLevel);
                break;
            }
            case ResourceDimension::Texture1DArray:
            {
                viewdesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1DARRAY;
                viewdesc.Texture1DArray.MipSlice = static_cast<UINT>(subResources.firstMipLevel);
                viewdesc.Texture1DArray.FirstArraySlice = static_cast<UINT>(subResources.firstArraySlice);
                viewdesc.Texture1DArray.ArraySize = static_cast<UINT>(subResources.arraySliceCount != AllArraySlices ? subResources.arraySliceCount : texture.arraySlices());
                break;
            }
            case ResourceDimension::Texture2D:
            {
                viewdesc.ViewDimension = m_description.samples > 1 ? D3D12_DSV_DIMENSION_TEXTURE2DMS : D3D12_DSV_DIMENSION_TEXTURE2D;
                viewdesc.Texture2D.MipSlice = static_cast<UINT>(subResources.firstMipLevel);
                break;
            }
            case ResourceDimension::Texture2DArray:
            {
                viewdesc.ViewDimension = m_description.samples > 1 ? D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY : D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
                viewdesc.Texture2DArray.MipSlice = static_cast<UINT>(subResources.firstMipLevel);
                viewdesc.Texture2DArray.ArraySize = static_cast<UINT>(subResources.arraySliceCount != AllArraySlices ? subResources.arraySliceCount : texture.arraySlices());
                viewdesc.Texture2DArray.FirstArraySlice = static_cast<UINT>(subResources.firstArraySlice);
                break;
            }
            }

            device.device()->CreateDepthStencilView(
                TextureImplGet::impl(texture)->native(),
                &viewdesc,
                m_viewHandle.cpuHandle());
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

        D3D12_CPU_DESCRIPTOR_HANDLE& TextureDSVImpl::native()
        {
            return m_viewHandle.cpuHandle();
        }

        const D3D12_CPU_DESCRIPTOR_HANDLE& TextureDSVImpl::native() const
        {
            return m_viewHandle.cpuHandle();
        }

        const SubResource& TextureDSVImpl::subResource() const
        {
            return m_subResources;
        }

        TextureRTVImpl::TextureRTVImpl(
            const DeviceImpl& device,
            const Texture& texture,
            const TextureDescription& desc,
            SubResource subResources)
            : m_description(desc.descriptor)
            , m_viewHandle{ device.heaps().rtv->getDescriptor() }
            , m_texture{ texture }
            , m_subResources{ subResources }
        {
            D3D12_RENDER_TARGET_VIEW_DESC viewdesc;
            viewdesc.Format = dxFormat(m_description.format);

            m_description.width >>= subResources.firstMipLevel;
            m_description.height >>= subResources.firstMipLevel;
            if (m_description.width < 1)
                m_description.width = 1;
            if (m_description.height < 1)
                m_description.height = 1;

            if(subResources.mipCount != AllMipLevels)
                m_description.mipLevels = subResources.mipCount;

            switch (m_description.dimension)
            {
            case ResourceDimension::Texture1D:
            {
                viewdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1D;
                viewdesc.Texture1D.MipSlice = subResources.firstMipLevel;
                break;
            }
            case ResourceDimension::Texture1DArray:
            {
                viewdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1DARRAY;
                viewdesc.Texture1DArray.MipSlice = static_cast<UINT>(subResources.firstMipLevel);
                viewdesc.Texture1DArray.FirstArraySlice = static_cast<UINT>(subResources.firstArraySlice);
                viewdesc.Texture1DArray.ArraySize = static_cast<UINT>(subResources.arraySliceCount != AllArraySlices ? subResources.arraySliceCount : texture.arraySlices());
                break;
            }
            case ResourceDimension::Texture2D:
            {
                viewdesc.ViewDimension = m_description.samples > 1 ? D3D12_RTV_DIMENSION_TEXTURE2DMS : D3D12_RTV_DIMENSION_TEXTURE2D;
                viewdesc.Texture2D.MipSlice = static_cast<UINT>(subResources.firstMipLevel);
                viewdesc.Texture2D.PlaneSlice = 0;
                break;
            }
            case ResourceDimension::Texture2DArray:
            {
                viewdesc.ViewDimension = m_description.samples > 1 ? D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY : D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
                viewdesc.Texture2DArray.MipSlice = static_cast<UINT>(subResources.firstMipLevel);
                viewdesc.Texture2DArray.ArraySize = static_cast<UINT>(subResources.arraySliceCount != AllArraySlices ? subResources.arraySliceCount : texture.arraySlices());
                viewdesc.Texture2DArray.FirstArraySlice = static_cast<UINT>(subResources.firstArraySlice);
                viewdesc.Texture2DArray.PlaneSlice = 0;
                break;
            }
            case ResourceDimension::Texture3D:
            {
                viewdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE3D;
                viewdesc.Texture3D.MipSlice = static_cast<UINT>(subResources.firstMipLevel);
                viewdesc.Texture3D.FirstWSlice = static_cast<UINT>(subResources.firstArraySlice);
                viewdesc.Texture3D.WSize = static_cast<UINT>(subResources.arraySliceCount != AllArraySlices ? subResources.arraySliceCount : texture.arraySlices());
                break;
            }
            case ResourceDimension::TextureCubemap:
            {
                viewdesc.ViewDimension = m_description.samples > 1 ? D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY : D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
                viewdesc.Texture2DArray.MipSlice = static_cast<UINT>(subResources.firstMipLevel);
                viewdesc.Texture2DArray.ArraySize = static_cast<UINT>(subResources.arraySliceCount != AllArraySlices ? subResources.arraySliceCount : texture.arraySlices());
                viewdesc.Texture2DArray.FirstArraySlice = static_cast<UINT>(subResources.firstArraySlice);
                viewdesc.Texture2DArray.PlaneSlice = 0;
                break;
            }
            }

            auto tex = TextureImplGet::impl(texture)->native();
            auto vdesc = &viewdesc;
            auto handle = m_viewHandle.cpuHandle();
            auto dev = device.device();
            dev->CreateRenderTargetView(
                tex,
                vdesc,
                handle);
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

        D3D12_CPU_DESCRIPTOR_HANDLE& TextureRTVImpl::native()
        {
            return m_viewHandle.cpuHandle();
        }

        const D3D12_CPU_DESCRIPTOR_HANDLE& TextureRTVImpl::native() const
        {
            return m_viewHandle.cpuHandle();
        }

        const SubResource& TextureRTVImpl::subResource() const
        {
            return m_subResources;
        }
    }
}
