#include "engine/graphics/dx12/DX12CommandList.h"
#include "engine/graphics/dx12/DX12Headers.h"
#include "engine/graphics/dx12/DX12Pipeline.h"
#include "engine/graphics/dx12/DX12Device.h"
#include "engine/graphics/dx12/DX12Resources.h"
#include "engine/graphics/dx12/DX12Sampler.h"
#include "engine/graphics/dx12/DX12Semaphore.h"
#include "engine/graphics/dx12/DX12CommandAllocator.h"
#include "engine/graphics/dx12/DX12DescriptorHandle.h"
#include "engine/graphics/dx12/DX12DescriptorHeap.h"

#include "engine/graphics/Device.h"
#include "engine/graphics/Barrier.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/Semaphore.h"
#include "engine/graphics/Pipeline.h"
#include "engine/graphics/Sampler.h"
#include "engine/graphics/Viewport.h"
#include "engine/graphics/Rect.h"
#include "engine/primitives/Color.h"
#include "shaders/ShaderTypes.h"

#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        CommandListImpl::CommandListImpl(DeviceImpl* device, CommandListType type)
            : m_device{ device }
            , m_open{ false }
            , m_descriptorHeaps{ device->heaps() }
            , m_lastPipelineState{ nullptr }
            , m_lastTopology{ D3D_PRIMITIVE_TOPOLOGY_UNDEFINED }
        {
            //m_allocator = std::make_shared<CommandAllocatorImpl>(*device);
            m_allocator = device->createCommandAllocator(type);
            auto res = device->device()->CreateCommandList(
                0,
                dxCommandListType(type),
                m_allocator->native(),
                NULL,
                DARKNESS_IID_PPV_ARGS(m_commandList.GetAddressOf()));

            ASSERT(SUCCEEDED(res));

            if (type != CommandListType::Copy)
            {
                std::vector<ID3D12DescriptorHeap*> heaps{
                    m_device->heaps().shaderVisible_cbv_srv_uav->native(),
                    m_device->heaps().shaderVisible_sampler->native()
                };

                m_commandList->SetDescriptorHeaps(static_cast<UINT>(heaps.size()), heaps.data());
            }

            m_open = true;
        }

        CommandListImpl::~CommandListImpl()
        {
            m_device->freeCommandAllocator(m_allocator);
            for (auto&& sig : m_commandSignatures)
            {
                sig->Release();
            }
            m_commandSignatures.clear();
        }

        void CommandListImpl::reset(implementation::PipelineImpl& pipelineState)
        {
            auto res = m_commandList->Reset(m_allocator->native(), pipelineState.native());
            ASSERT(SUCCEEDED(res));
        }

        void CommandListImpl::executeBundle(CommandListImpl& commandList)
        {
            m_commandList->ExecuteBundle(commandList.native());
        }

        void CommandListImpl::transition(Texture& resource, ResourceState state, const SubResource& subResource)
        {
            TextureImpl* impl = TextureImplGet::impl(resource);

            auto localSubRes = subResource;

            if ((localSubRes.arraySliceCount == AllArraySlices) &&
                (localSubRes.mipCount == AllMipLevels))
            {
                bool forceIndividual = false;
                auto firstState = impl->state(0, 0);
                for (int slice = 0; slice < static_cast<int>(resource.arraySlices()); ++slice)
                {
                    for (int mip = 0; mip < static_cast<int>(resource.mipLevels()); ++mip)
                    {
                        if (firstState != impl->state(slice, mip))
                        {
                            forceIndividual = true;
                            break;
                        }
                    }
                    if (forceIndividual)
                        break;
                }

                if (forceIndividual)
                {
                    localSubRes.firstArraySlice = 0;
                    localSubRes.firstMipLevel = 0;
                    localSubRes.arraySliceCount = resource.arraySlices();
                    localSubRes.mipCount = resource.mipLevels();
                }
            }

            if ((localSubRes.arraySliceCount == AllArraySlices) &&
                (localSubRes.mipCount == AllMipLevels))
            {
                ResourceState anystate = state;
                for (int slice = 0; slice < static_cast<int>(resource.arraySlices()); ++slice)
                {
                    for (int mip = 0; mip < static_cast<int>(resource.mipLevels()); ++mip)
                    {
                        if (impl->state(slice, mip) != anystate)
                        {
                            anystate = impl->state(slice, mip);
                            break;
                        }
                    }
                }
                if (anystate != state)
                {
                    D3D12_RESOURCE_BARRIER barrier = {};
                    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                    barrier.Transition.pResource = impl->native();
                    barrier.Transition.StateBefore = dxResourceStates(anystate);
                    barrier.Transition.StateAfter = dxResourceStates(state);
                    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
                    m_barriers.emplace_back(barrier);
                    m_commandList->ResourceBarrier(1, &m_barriers[m_barriers.size() - 1]);

                    for (int slice = 0; slice < static_cast<int>(resource.arraySlices()); ++slice)
                    {
                        for (int mip = 0; mip < static_cast<int>(resource.mipLevels()); ++mip)
                        {
                            impl->state(slice, mip, state);
                        }
                    }
                }
            }
            else
            {
                uint32_t sliceCount = localSubRes.arraySliceCount == AllArraySlices ?
                    resource.arraySlices() :
                    min(localSubRes.arraySliceCount, static_cast<int>(resource.arraySlices() - localSubRes.firstArraySlice));

                uint32_t mipCount = localSubRes.mipCount == AllMipLevels ?
                    resource.mipLevels() :
                    min(localSubRes.mipCount, static_cast<int>(resource.mipLevels() - localSubRes.firstMipLevel));

                auto firstIndex = m_barriers.size();
                for(int slice = static_cast<int>(localSubRes.firstArraySlice); slice < static_cast<int>(localSubRes.firstArraySlice + sliceCount); ++slice)
                {
                    for (int mip = static_cast<int>(localSubRes.firstMipLevel); mip < static_cast<int>(localSubRes.firstMipLevel + mipCount); ++mip)
                    {
                        if (impl->state(slice, mip) != state)
                        {
                            D3D12_RESOURCE_BARRIER barrier = {};
                            barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                            barrier.Transition.pResource = impl->native();
                            barrier.Transition.StateBefore = dxResourceStates(impl->state(slice, mip));
                            barrier.Transition.StateAfter = dxResourceStates(state);
                            barrier.Transition.Subresource = mip + (slice * resource.mipLevels());
                            m_barriers.emplace_back(barrier);
                            impl->state(slice, mip, state);
                        }
                    }
                }
                auto barrierCount = m_barriers.size() - firstIndex;
                if(barrierCount > 0)
                    m_commandList->ResourceBarrier(static_cast<UINT>(barrierCount), &m_barriers[firstIndex]);
            }
        }

        void CommandListImpl::transition(TextureRTV& resource, ResourceState state)
        {
            transition(resource.texture(), state, resource.subResource());
        }

        void CommandListImpl::transition(TextureSRV& resource, ResourceState state)
        {
            transition(resource.texture(), state, resource.subResource());
        }

        void CommandListImpl::transition(TextureDSV& resource, ResourceState state)
        {
            transition(resource.texture(), state, resource.subResource());
        }

        void CommandListImpl::transition(Buffer& resource, ResourceState state)
        {
            BufferImpl* impl = BufferImplGet::impl(resource);
            if (impl->state() == state)
                return;

            D3D12_RESOURCE_BARRIER barrier = {};
            barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            barrier.Transition.pResource = impl->native();
            barrier.Transition.StateBefore = dxResourceStates(impl->state());
            barrier.Transition.StateAfter = dxResourceStates(state);
            barrier.Transition.Subresource = 0;// D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            m_barriers.emplace_back(barrier);

            m_commandList->ResourceBarrier(1, &m_barriers[m_barriers.size() - 1]);

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

        void CommandListImpl::setRenderTargets(std::vector<TextureRTV> targets)
        {
            std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> handles(targets.size());
            for (size_t i = 0; i < targets.size(); ++i)
            {
                handles[i] = TextureRTVImplGet::impl(targets[i])->native();
            }
            m_commandList->OMSetRenderTargets(
                static_cast<UINT>(targets.size()), 
                handles.data(), 
                FALSE, 
                NULL);

            // we could also set these manually, but it's only a minor
            // optimization and a major pain in the ass to remember.
            // so we'll set them right away
            if (targets.size() > 0)
            {
                setViewPorts({
                    engine::Viewport{
                    0, 0,
                    static_cast<float>(targets[0].width()),
                    static_cast<float>(targets[0].height()),
                    0.0f, 1.0f } });
                setScissorRects({ engine::Rectangle{ 0, 0,
                    static_cast<unsigned int>(targets[0].width()),
                    static_cast<unsigned int>(targets[0].height()) } });
            }
        }

        void CommandListImpl::setRenderTargets(std::vector<TextureRTV> targets, TextureDSV& dsv)
        {
            std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> handles;
            for (size_t i = 0; i < targets.size(); ++i)
            {
                if(targets[i].valid())
                    handles.emplace_back(TextureRTVImplGet::impl(targets[i])->native());
            }
            if (handles.size() > 0)
            {
                m_commandList->OMSetRenderTargets(
                    static_cast<UINT>(handles.size()),
                    handles.data(),
                    FALSE,
                    &TextureDSVImplGet::impl(dsv)->native());
            }
            else
            {
                m_commandList->OMSetRenderTargets(
                    0,
                    nullptr,
                    FALSE,
                    &TextureDSVImplGet::impl(dsv)->native());
            }

            // we could also set these manually, but it's only a minor
            // optimization and a major pain in the ass to remember.
            // so we'll set them right away
            if (targets.size() > 0)
            {
                setViewPorts({
                    engine::Viewport{
                    0, 0,
                    static_cast<float>(targets[0].width()),
                    static_cast<float>(targets[0].height()),
                    0.0f, 1.0f } });
                setScissorRects({ engine::Rectangle{ 0, 0,
                    static_cast<unsigned int>(targets[0].width()),
                    static_cast<unsigned int>(targets[0].height()) } });
            }
            else
            {
                setViewPorts({
                    engine::Viewport{
                    0, 0,
                    static_cast<float>(dsv.texture().width()),
                    static_cast<float>(dsv.texture().height()),
                    0.0f, 1.0f } });
                setScissorRects({ engine::Rectangle{ 0, 0,
                    static_cast<unsigned int>(dsv.texture().width()),
                    static_cast<unsigned int>(dsv.texture().height()) } });
            }
        }

        void CommandListImpl::clearRenderTargetView(const TextureRTV& target, const Color4f& color)
        {
            std::vector<D3D12_RECT> rects;
            rects.emplace_back(D3D12_RECT{ 0, 0,
                static_cast<LONG>(target.texture().width()),
                static_cast<LONG>(target.texture().height()) });
            m_commandList->ClearRenderTargetView(
                TextureRTVImplGet::impl(target)->native(), 
                color.get(), 
                static_cast<UINT>(rects.size()),
                rects.data());
        }

        void CommandListImpl::clearDepthStencilView(const TextureDSV& target, float depth, uint8_t stencil)
        {
            std::vector<D3D12_RECT> rects;
            rects.emplace_back(D3D12_RECT{ 0, 0, 
                static_cast<LONG>(target.texture().width()), 
                static_cast<LONG>(target.texture().height()) });

            bool hasStencil = target.texture().format() == Format::D24_UNORM_S8_UINT;

            D3D12_CLEAR_FLAGS flags = D3D12_CLEAR_FLAG_DEPTH;
            if (hasStencil)
                flags |= D3D12_CLEAR_FLAG_STENCIL;

            m_commandList->ClearDepthStencilView(
                TextureDSVImplGet::impl(target)->native(), 
                flags,
                depth, 
                stencil, 
                static_cast<UINT>(rects.size()),
                rects.data());
        }

        void CommandListImpl::copyBuffer(const Buffer& srcBuffer,
            Buffer& dstBuffer,
            uint64_t elements,
            uint32_t srcStartElement,
            uint32_t dstStartElement)
        {
            auto dstElementSize = dstBuffer.description().descriptor.elementSize;
            if (dstElementSize == -1)
                dstElementSize = static_cast<int32_t>(formatBytes(dstBuffer.description().descriptor.format));

            auto srcElementSize = srcBuffer.description().descriptor.elementSize;
            if (srcElementSize == -1)
                srcElementSize = static_cast<int32_t>(formatBytes(srcBuffer.description().descriptor.format));

            m_commandList->CopyBufferRegion(
                BufferImplGet::impl(dstBuffer)->native(),
                dstStartElement * dstElementSize,
                BufferImplGet::impl(srcBuffer)->native(),
                srcStartElement * srcElementSize,
                elements * srcElementSize);
        }

        static ID3D12PipelineState* lastPipelineState = nullptr;

        void CommandListImpl::bindPipe(
            implementation::PipelineImpl& pipelineImpl,
            shaders::PipelineConfiguration* configuration)
        {
            pipelineImpl.configure(*this, configuration);

            auto pipelineState = pipelineImpl.native();
            if(m_lastPipelineState != pipelineState)
            {
                m_lastPipelineState = pipelineState;
                m_commandList->SetPipelineState(pipelineState);

                m_rootSignature = pipelineImpl.rootSignature();
                if (!configuration->hasComputeShader())
                    m_commandList->SetGraphicsRootSignature(pipelineImpl.rootSignature());
                else
                    m_commandList->SetComputeRootSignature(pipelineImpl.rootSignature());
            }

            //m_argumentDescs.clear();
            if (m_lastRootDescriptor.size() != pipelineImpl.resourceRanges().size())
                m_lastRootDescriptor.clear();
            m_lastRootDescriptor.resize(pipelineImpl.resourceRanges().size());

            if (!configuration->hasComputeShader())
            {
                for (auto&& range : pipelineImpl.resourceRanges())
                {
                    if (range.srvUavCbvSet)
                    {
                        if (m_lastRootDescriptor[range.resourceRange] != range.resourceGpuHeapStart.ptr)
                        {
                            m_commandList->SetGraphicsRootDescriptorTable(range.resourceRange, range.resourceGpuHeapStart);
                            m_lastRootDescriptor[range.resourceRange] = range.resourceGpuHeapStart.ptr;
                        }
                    }
                }
                for (auto&& range : pipelineImpl.samplerRanges())
                {
                    if (range.samplerSet)
                    {
                        if (m_lastRootDescriptor[range.resourceRange] != range.samplerGpuHeapStart.ptr)
                        {
                            m_commandList->SetGraphicsRootDescriptorTable(range.resourceRange, range.samplerGpuHeapStart);
                            m_lastRootDescriptor[range.resourceRange] = range.samplerGpuHeapStart.ptr;
                        }
                    }
                }

                auto pipeTopology = pipelineImpl.topology();
                if (m_lastTopology != pipeTopology)
                {
                    m_commandList->IASetPrimitiveTopology(pipeTopology);
                    m_lastTopology = pipeTopology;
                }
            }
            else
            {
                for (auto&& range : pipelineImpl.resourceRanges())
                {
                    if (range.srvUavCbvSet)
                    {
                        if (m_lastRootDescriptor[range.resourceRange] != range.resourceGpuHeapStart.ptr)
                        {
                            m_commandList->SetComputeRootDescriptorTable(range.resourceRange, range.resourceGpuHeapStart);
                            m_lastRootDescriptor[range.resourceRange] = range.resourceGpuHeapStart.ptr;
                        }
                    }
                }
                for (auto&& range : pipelineImpl.samplerRanges())
                {
                    if (range.samplerSet)
                    {
                        if (m_lastRootDescriptor[range.resourceRange] != range.samplerGpuHeapStart.ptr)
                        {
                            m_commandList->SetComputeRootDescriptorTable(range.resourceRange, range.samplerGpuHeapStart);
                            m_lastRootDescriptor[range.resourceRange] = range.samplerGpuHeapStart.ptr;
                        }
                    }
                }
            }
        }

        void CommandListImpl::setViewPorts(const std::vector<Viewport>& viewports)
        {
            std::vector<D3D12_VIEWPORT> viewPorts;
            for (auto&& viewport : viewports)
            {
                D3D12_VIEWPORT vport;
                vport.Width = viewport.width;
                vport.Height = viewport.height;
                vport.MinDepth = viewport.minDepth;
                vport.MaxDepth = viewport.maxDepth;
                vport.TopLeftX = viewport.topLeftX;
                vport.TopLeftY = viewport.topLeftY;
                viewPorts.emplace_back(vport);
            }
            m_commandList->RSSetViewports(static_cast<UINT>(viewPorts.size()), viewPorts.data());
        }

        void CommandListImpl::setScissorRects(const std::vector<Rectangle>& rects)
        {
            std::vector<D3D12_RECT> scissorRects;
            for (auto&& rect : rects)
            {
                D3D12_RECT scissorRect;
                scissorRect.top = rect.top;
                scissorRect.left = rect.left;
                scissorRect.right = rect.right;
                scissorRect.bottom = rect.bottom;
                scissorRects.emplace_back(scissorRect);
            }
            m_commandList->RSSetScissorRects(static_cast<UINT>(scissorRects.size()), scissorRects.data());
        }


        void CommandListImpl::bindVertexBuffer(const BufferVBV& buffer)
        {
            m_commandList->IASetVertexBuffers(0, 1, BufferVBVImplGet::impl(buffer)->view());
        }

        void CommandListImpl::bindIndexBuffer(const BufferIBV& buffer)
        {
            m_commandList->IASetIndexBuffer(BufferIBVImplGet::impl(buffer)->view());
        }

        void CommandListImpl::clearBuffer(Buffer& buffer, uint32_t value, uint32_t /*startElement*/, uint32_t /*numElements*/)
        {
            const uint32_t values[4]{ value, value, value, value };
            BufferUAVImpl bufferUAV(*m_device, buffer, buffer.description());
            m_commandList->ClearUnorderedAccessViewUint(bufferUAV.m_viewHandle.gpuHandle(), bufferUAV.m_viewHandle.cpuHandle(), BufferImplGet::impl(buffer)->native(), values, 0, nullptr);
            m_bufferUAVClearTargets.emplace_back(std::move(bufferUAV));
        }

        void CommandListImpl::clearTexture(
            Texture& texture,
            const Color4f& color,
            const SubResource& subResource)
        {
            TextureRTV target = TextureRTV{ std::make_shared<TextureRTVImpl>(*m_device, texture, texture.description(), subResource) };
            std::vector<D3D12_RECT> rects;
            rects.emplace_back(D3D12_RECT{ 0, 0,
                static_cast<LONG>(target.texture().width()),
                static_cast<LONG>(target.texture().height()) });
            m_commandList->ClearRenderTargetView(
                TextureRTVImplGet::impl(target)->native(),
                color.get(),
                static_cast<UINT>(rects.size()),
                rects.data());
            m_clearTargetsAlive.emplace_back(std::move(target));
        }

        void CommandListImpl::clearBufferUAV(BufferUAV& /*buffer*/, uint32_t /*value*/, uint32_t /*startElement*/, uint32_t /*numElements*/)
        {
            //m_commandList->ClearUnorderedAccessViewUint(BufferImplGet::impl(BufferUAVImplGet::impl(buffer)->buffer())->native, )
            LOG("DX12 CommandList clearBufferUAV is unimplemented");
        }

        void CommandListImpl::clearTextureUAV(TextureUAV& /*texture*/, const Color4f& /*color*/, const SubResource& /*subResource*/)
        {
            LOG("DX12 CommandList clearTextureUAV is unimplemented");
        }

        void CommandListImpl::clearTextureDSV(TextureDSV& /*texture*/, const Color4f& /*color*/, const SubResource& /*subResource*/)
        {
            LOG("DX12 CommandList clearTextureDSV is unimplemented");
        }

        void CommandListImpl::clearTextureRTV(TextureRTV& /*texture*/, const Color4f& /*color*/, const SubResource& /*subResource*/)
        {
            //m_commandList->ClearRenderTargetView()
            LOG("DX12 CommandList clearTextureRTV is unimplemented");
        }

        void CommandListImpl::setStructureCounter(BufferUAV& buffer, uint32_t value)
        {
            m_device->uploadRawBuffer(
                *this,
                buffer.buffer(),
                tools::ByteRange(
                    reinterpret_cast<uint8_t*>(&value),
                    reinterpret_cast<uint8_t*>(&value) + sizeof(uint32_t)),
                BufferUAVImplGet::impl(buffer)->structureCounterOffsetBytes());
        }

        void CommandListImpl::copyStructureCounter(BufferUAV& srcBuffer, Buffer& dst, uint32_t dstByteOffset)
        {
            m_commandList->CopyBufferRegion(
                BufferImplGet::impl(dst)->native(),
                dstByteOffset,
                BufferImplGet::impl(srcBuffer.buffer())->native(),
                BufferUAVImplGet::impl(srcBuffer)->structureCounterOffsetBytes(), sizeof(uint32_t));
        }

        void CommandListImpl::draw(uint32_t vertexCount)
        {
            m_commandList->DrawInstanced(vertexCount, 1, 0, 0);
        }

        void CommandListImpl::drawIndirect(Buffer& indirectArguments, uint64_t argumentBufferOffset)
        {
            m_commandList->ExecuteIndirect(m_device->drawIndirectSignature(), 1, BufferImplGet::impl(indirectArguments)->native(), argumentBufferOffset, nullptr, 0);
        }

        void CommandListImpl::dispatch(
            uint32_t threadGroupCountX,
            uint32_t threadGroupCountY,
            uint32_t threadGroupCountZ)
        {
            m_commandList->Dispatch(
                threadGroupCountX,
                threadGroupCountY,
                threadGroupCountZ
            );
        }

        void CommandListImpl::drawIndexed(
            uint32_t indexCount,
            uint32_t instanceCount,
            uint32_t firstIndex,
            int32_t vertexOffset,
            uint32_t firstInstance)
        {
            m_commandList->DrawIndexedInstanced(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
        }

        void CommandListImpl::drawIndexedIndirect(Buffer& indirectArguments, uint64_t argumentBufferOffset)
        {
            m_commandList->ExecuteIndirect(m_device->drawIndexedIndirectSignature(), 1, BufferImplGet::impl(indirectArguments)->native(), argumentBufferOffset, nullptr, 0);
#if 0
            D3D12_INDIRECT_ARGUMENT_DESC argDesc[1];
            argDesc[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

            D3D12_COMMAND_SIGNATURE_DESC desc;
            ZeroMemory(&desc, sizeof(D3D12_COMMAND_SIGNATURE_DESC));

            desc.pArgumentDescs = &argDesc[0];
            desc.ByteStride = sizeof(D3D12_INDIRECT_ARGUMENT_DESC);
            desc.NodeMask = 0;
            desc.NumArgumentDescs = 1;// static_cast<UINT>(m_argumentDescs.size());

                                      /*desc.ByteStride = argumentBuffer.description().descriptor.elementSize;
                                      desc.NumArgumentDescs = argumentBuffer.description().descriptor.elements;
                                      desc.pArgumentDescs = &argDesc;
                                      desc.*/

            ID3D12Resource* countBuffer = nullptr;
            UINT64 countBufferOffset = 0;
            if (indirectArguments.description().descriptor.append)
            {
                transition(indirectArguments, ResourceState::IndirectArgument);
                countBuffer = BufferImplGet::impl(indirectArguments)->native();
                countBufferOffset = roundUpToMultiple(
                    indirectArguments.description().descriptor.elements *
                    indirectArguments.description().descriptor.elementSize,
                    D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT);
            }

            ID3D12CommandSignature* signature;
            m_device->device()->CreateCommandSignature(
                &desc,
                m_rootSignature,
                DARKNESS_IID_PPV_ARGS(&signature));
            m_commandSignatures.emplace_back(signature);

            m_commandList->ExecuteIndirect(
                signature,                                              // ID3D12CommandSignature* pCommandSignature
                0,                                                      // UINT MaxCommandCount
                BufferImplGet::impl(indirectArguments)->native(),          // ID3D12Resource* pArgumentBuffer
                argumentBufferOffset,                                   // UINT64 ArgumentBufferOffset
                countBuffer,                                            // ID3D12Resource* pCountBuffer
                countBufferOffset                                       // UINT64 CountBufferOffset
            );
#endif
        }

        void CommandListImpl::dispatchIndirect(
            Buffer& indirectArguments,
            uint64_t argumentBufferOffset
        )
        {
            m_commandList->ExecuteIndirect(m_device->dispatchIndirectSignature(), 1, BufferImplGet::impl(indirectArguments)->native(), argumentBufferOffset, nullptr, 0);
#if 0
            /*D3D12_INDIRECT_ARGUMENT_DESC argDesc;
            argDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;
            m_argumentDescs.emplace_back(argDesc);*/

            D3D12_INDIRECT_ARGUMENT_DESC argDesc[1];
            argDesc[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH;

            D3D12_COMMAND_SIGNATURE_DESC desc;
            ZeroMemory(&desc, sizeof(D3D12_COMMAND_SIGNATURE_DESC));

            desc.pArgumentDescs = &argDesc[0];
            desc.ByteStride = sizeof(D3D12_INDIRECT_ARGUMENT_DESC);
            desc.NodeMask = 0;
            desc.NumArgumentDescs = 1;// static_cast<UINT>(m_argumentDescs.size());

            /*desc.ByteStride = argumentBuffer.description().descriptor.elementSize;
            desc.NumArgumentDescs = argumentBuffer.description().descriptor.elements;
            desc.pArgumentDescs = &argDesc;
            desc.*/

            ID3D12Resource* countBuffer = nullptr;
            UINT64 countBufferOffset = 0;
            if (indirectArguments.description().descriptor.append)
            {
                transition(indirectArguments, ResourceState::IndirectArgument);
                countBuffer = BufferImplGet::impl(indirectArguments)->native();
                countBufferOffset = roundUpToMultiple(
                    indirectArguments.description().descriptor.elements * 
                    indirectArguments.description().descriptor.elementSize, 
                    D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT);
            }
            
            ID3D12CommandSignature* signature;
            m_device->device()->CreateCommandSignature(
                &desc,
                m_rootSignature,
                DARKNESS_IID_PPV_ARGS(&signature));
            m_commandSignatures.emplace_back(signature);

            m_commandList->ExecuteIndirect(
                signature,                                              // ID3D12CommandSignature* pCommandSignature
                0,                                                      // UINT MaxCommandCount
                BufferImplGet::impl(indirectArguments)->native(),          // ID3D12Resource* pArgumentBuffer
                argumentBufferOffset,                                   // UINT64 ArgumentBufferOffset
                countBuffer,                                            // ID3D12Resource* pCountBuffer
                countBufferOffset                                       // UINT64 CountBufferOffset
            );
#endif
        }

        void CommandListImpl::begin()
        {
        }

        void CommandListImpl::end()
        {
            m_commandList->Close();
            m_open = false;
        }

        void CommandListImpl::beginRenderPass(implementation::PipelineImpl& /*pipeline*/, int /*frameBufferIndex*/)
        {
            LOG("DX12 CommandList beginRenderPass is unimplemented");
        }

        void CommandListImpl::endRenderPass()
        {
            LOG("DX12 CommandList endRenderPass is unimplemented");
        }

        /*void CommandListImpl::transitionTexture(const Texture& image, ImageLayout from, ImageLayout to)
        {
            LOG("DX12 CommandList transitionTexture is unimplemented");
        }*/

        void CommandListImpl::copyTexture(const TextureSRV& src, TextureUAV& dst)
        {
            if (src.width() == dst.width() &&
                src.height() == dst.height())
            {
                D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
                srcLocation.pResource = TextureImplGet::impl(src.texture())->native();
                srcLocation.SubresourceIndex = 0;
                srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
                D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
                dstLocation.pResource = TextureImplGet::impl(dst.texture())->native();
                dstLocation.SubresourceIndex = 0;
                dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
                D3D12_BOX box = {};
                box.left = 0;
                box.top = 0;
                box.right = src.width();
                box.bottom = src.height();
                box.front = 0;
                box.back = 1;
                m_commandList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, &box);
            }
            else
            {
                /*D3D12_SUBRESOURCE_FOOTPRINT srcfprint;
                srcfprint.Depth = 1;
                srcfprint.Format = dxFormat(src.format());
                srcfprint.Height = src.height();
                srcfprint.Width = src.width();
                srcfprint.RowPitch = roundUpToMultiple(src.width() * formatBytes(src.format()), D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
                D3D12_PLACED_SUBRESOURCE_FOOTPRINT srcFootprint;
                srcFootprint.Footprint = srcfprint;
                srcFootprint.Offset = 0;
                D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
                srcLocation.pResource = TextureImplGet::impl(src.texture())->native();
                srcLocation.PlacedFootprint = srcFootprint;
                srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;*/

                D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
                srcLocation.pResource = TextureImplGet::impl(src.texture())->native();
                srcLocation.SubresourceIndex = 0;
                srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;


                D3D12_SUBRESOURCE_FOOTPRINT dstfprint;
                dstfprint.Depth = 1;
                dstfprint.Format = dxFormat(dst.format());
                dstfprint.Height = dst.height();
                dstfprint.Width = dst.width();
                dstfprint.RowPitch = static_cast<UINT>(roundUpToMultiple(dst.width() * formatBytes(dst.format()), D3D12_TEXTURE_DATA_PITCH_ALIGNMENT));
                D3D12_PLACED_SUBRESOURCE_FOOTPRINT dstFootprint;
                dstFootprint.Footprint = dstfprint;
                dstFootprint.Offset = 0;
                D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
                dstLocation.pResource = TextureImplGet::impl(dst.texture())->native();
                dstLocation.PlacedFootprint = dstFootprint;
                dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;


                D3D12_BOX srcBox = {};
                srcBox.left = 0;
                srcBox.top = 0;
                srcBox.right = src.width();
                srcBox.bottom = src.height();
                srcBox.front = 0;
                srcBox.back = 1;

                UINT dstX = 0;
                UINT dstY = 0;
                UINT dstZ = 0;

                m_commandList->CopyTextureRegion(
                    &dstLocation, 
                    dstX, dstY, dstZ,
                    &srcLocation, 
                    &srcBox);
            }
        }

        void CommandListImpl::copyTexture(const TextureSRV& src, TextureSRV& dst)
        {
            D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
            srcLocation.pResource = TextureImplGet::impl(src.texture())->native();
            srcLocation.SubresourceIndex = 0;
            srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
            dstLocation.pResource = TextureImplGet::impl(dst.texture())->native();
            dstLocation.SubresourceIndex = 0;
            dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            D3D12_BOX box = {};
            box.left = 0;
            box.top = 0;
            box.right = src.width();
            box.bottom = src.height();
            box.front = 0;
            box.back = 1;
            m_commandList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, &box);
        }

        void CommandListImpl::copyTexture(const TextureSRV& src, BufferUAV& dst)
        {
            D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
            srcLocation.pResource = TextureImplGet::impl(src.texture())->native();
            srcLocation.SubresourceIndex = 0;
            srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
            dstLocation.pResource = BufferImplGet::impl(dst.buffer())->native();
            dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
            dstLocation.PlacedFootprint.Offset = 0;
            dstLocation.PlacedFootprint.Footprint.Depth = 1;
            dstLocation.PlacedFootprint.Footprint.Format = dxFormat(dst.buffer().description().descriptor.format);
            dstLocation.PlacedFootprint.Footprint.Width = src.width();
            dstLocation.PlacedFootprint.Footprint.Height = src.height();
            dstLocation.PlacedFootprint.Footprint.RowPitch = dst.buffer().description().descriptor.elementSize * src.width();
            m_commandList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, nullptr);
        }

        void CommandListImpl::copyTexture(const TextureSRV& src, BufferSRV& dst)
        {
            D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
            srcLocation.pResource = TextureImplGet::impl(src.texture())->native();
            srcLocation.SubresourceIndex = 0;
            srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
            dstLocation.pResource = BufferImplGet::impl(dst.buffer())->native();
            dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
            dstLocation.PlacedFootprint.Offset = 0;
            dstLocation.PlacedFootprint.Footprint.Depth = 1;
            dstLocation.PlacedFootprint.Footprint.Format = dxFormat(dst.buffer().description().descriptor.format);
            dstLocation.PlacedFootprint.Footprint.Width = src.width();
            dstLocation.PlacedFootprint.Footprint.Height = src.height();
            dstLocation.PlacedFootprint.Footprint.RowPitch = dst.buffer().description().descriptor.elementSize * src.width();
            m_commandList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, nullptr);
        }

        ID3D12GraphicsCommandList* CommandListImpl::native()
        {
            return m_commandList.Get();
        }

        ID3D12GraphicsCommandList* CommandListImpl::native() const
        {
            return m_commandList.Get();
        }

        bool CommandListImpl::isOpen() const
        {
            return m_open;
        }
    }
}
