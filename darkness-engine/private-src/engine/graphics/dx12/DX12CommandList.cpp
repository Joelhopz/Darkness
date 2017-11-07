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
        CommandListImpl::CommandListImpl(DeviceImpl* device)
            : m_device{ device }
            , m_open{ false }
            , m_descriptorHeaps{ device->heaps() }
            , m_lastPipelineState{ nullptr }
        {
            //m_allocator = std::make_shared<CommandAllocatorImpl>(*device);
            m_allocator = device->createCommandAllocator();
            auto res = device->device()->CreateCommandList(
                0,
                D3D12_COMMAND_LIST_TYPE_DIRECT,
                m_allocator->native(),
                NULL,
                __uuidof(ID3D12GraphicsCommandList),
                (void**)m_commandList.GetAddressOf());

            ASSERT(SUCCEEDED(res));

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

        void CommandListImpl::transition(Texture& resource, ResourceState state)
        {
            TextureImpl* impl = TextureImplGet::impl(resource);
            if (impl->state() == state)
                return;

            D3D12_RESOURCE_BARRIER barrier = {};
            barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            barrier.Transition.pResource = impl->native();
            barrier.Transition.StateBefore = dxResourceStates(impl->state());
            barrier.Transition.StateAfter = dxResourceStates(state);
            barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            m_barriers.emplace_back(barrier);

            m_commandList->ResourceBarrier(1, &m_barriers[m_barriers.size() - 1]);

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

        static int hack_normals = 1;
        static int hack_tangents = 1;
        static int hack_uv = 1;
        void CommandListImpl::transition(Buffer& resource, ResourceState state)
        {
            BufferImpl* impl = BufferImplGet::impl(resource);
            if (impl->state() == state)
                return;

            

            bool perform = true;
            /*if (strcmp(resource.description().descriptor.name, "SubMesh Normals") == 0)
            {
                --hack_normals;
                if (hack_normals < 0)
                    perform = false;
            }
            if (strcmp(resource.description().descriptor.name, "SubMesh Tangents") == 0)
            {
                --hack_tangents;
                if (hack_tangents < 0)
                    perform = false;
            }
            if (strcmp(resource.description().descriptor.name, "SubMesh UV") == 0)
            {
                --hack_uv;
                if (hack_uv < 0)
                    perform = false;
            }*/

            if (perform)
            {
                /*if ((strcmp(resource.description().descriptor.name, "Constants") != 0) &&
                    (strcmp(resource.description().descriptor.name, "Pick buffer") != 0) &&
                    (strcmp(resource.description().descriptor.name, "ImGui vbv") != 0) &&
                    (strcmp(resource.description().descriptor.name, "ImGui ibv") != 0) &&
                    (strcmp(resource.description().descriptor.name, "constants_struct") != 0) &&
                    (strcmp(resource.description().descriptor.name, "ConstData") != 0))
                {

                    LOG("transitioning resource(%p): %s, from: %s, to: %s",
                        impl->native(),
                        resource.description().descriptor.name,
                        resourceStateToString(impl->state()),
                        resourceStateToString(state)
                    );
                }*/

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
        }

        void CommandListImpl::clearRenderTargetView(const TextureRTV& target, const Color4f& color)
        {
            m_commandList->ClearRenderTargetView(TextureRTVImplGet::impl(target)->native(), color.get(), 0, NULL);
        }

        void CommandListImpl::clearDepthStencilView(const TextureDSV& target, float depth, uint8_t stencil)
        {
            std::vector<D3D12_RECT> rects;
            rects.emplace_back(D3D12_RECT{ 0, 0, 
                static_cast<LONG>(target.texture().width()), 
                static_cast<LONG>(target.texture().height()) });

            bool hasStencil = target.texture().format() == Format::Format_D24_UNORM_S8_UINT;

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
            m_commandList->CopyBufferRegion(
                BufferImplGet::impl(dstBuffer)->native(),
                dstStartElement * dstBuffer.description().descriptor.elementSize,
                BufferImplGet::impl(srcBuffer)->native(),
                srcStartElement * srcBuffer.description().descriptor.elementSize,
                elements * srcBuffer.description().descriptor.elementSize);
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

            

            std::vector<ID3D12DescriptorHeap*> heaps{
                m_device->heaps().shaderVisible_cbv_srv_uav->native(),
                m_device->heaps().shaderVisible_sampler->native()
            };

            m_commandList->SetDescriptorHeaps(static_cast<UINT>(heaps.size()), heaps.data());

            //m_argumentDescs.clear();
            if (!configuration->hasComputeShader())
            {
                for (auto&& range : pipelineImpl.resourceRanges())
                {
                    if (range.srvUavCbvSet)
                    {
                        /*D3D12_INDIRECT_ARGUMENT_DESC desc;
                        if(range.type == PipelineImpl::RangeInfoType::SRV)
                            desc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_SHADER_RESOURCE_VIEW;
                        else if (range.type == PipelineImpl::RangeInfoType::UAV)
                            desc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW;
                        else if (range.type == PipelineImpl::RangeInfoType::CONSTANT)
                            desc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
                        desc.ConstantBufferView.RootParameterIndex = range.resourceRange;
                        m_argumentDescs.emplace_back(desc);*/

                        m_commandList->SetGraphicsRootDescriptorTable(range.resourceRange, range.resourceGpuHeapStart);
                    }
                    if (range.samplerSet)
                        m_commandList->SetGraphicsRootDescriptorTable(range.resourceRange, range.samplerGpuHeapStart);
                }
            }
            else
            {
                for (auto&& range : pipelineImpl.resourceRanges())
                {
                    if (range.srvUavCbvSet)
                    {
                        /*D3D12_INDIRECT_ARGUMENT_DESC desc;
                        if (range.type == PipelineImpl::RangeInfoType::SRV)
                            desc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_SHADER_RESOURCE_VIEW;
                        else if (range.type == PipelineImpl::RangeInfoType::UAV)
                            desc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW;
                        else if (range.type == PipelineImpl::RangeInfoType::CONSTANT)
                            desc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
                        desc.ConstantBufferView.RootParameterIndex = range.resourceRange;
                        m_argumentDescs.emplace_back(desc);*/

                        m_commandList->SetComputeRootDescriptorTable(range.resourceRange, range.resourceGpuHeapStart);
                    }
                    if (range.samplerSet)
                        m_commandList->SetComputeRootDescriptorTable(range.resourceRange, range.samplerGpuHeapStart);
                }
            }

            m_commandList->IASetPrimitiveTopology(pipelineImpl.topology());
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

        void CommandListImpl::clearBuffer(Buffer& /*buffer*/, uint32_t /*value*/, uint32_t /*startElement*/, uint32_t /*numElements*/)
        {
            LOG("DX12 CommandList clearBuffer is unimplemented");
        }

        void CommandListImpl::clearTexture(
            Texture& /*texture*/,
            const Color4f& /*color*/,
            const SubResource& /*subResource*/)
        {
            LOG("DX12 CommandList clearTexture is unimplemented");
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

        void CommandListImpl::draw(uint32_t vertexCount)
        {
            m_commandList->DrawInstanced(vertexCount, 1, 0, 0);
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

        void CommandListImpl::executeIndirect(
            Buffer& argumentBuffer,
            uint64_t argumentBufferOffset
        )
        {
            /*D3D12_INDIRECT_ARGUMENT_DESC argDesc;
            argDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;
            m_argumentDescs.emplace_back(argDesc);*/

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
            if (argumentBuffer.description().descriptor.append)
            {
                transition(argumentBuffer, ResourceState::IndirectArgument);
                countBuffer = BufferImplGet::impl(argumentBuffer)->native();
                countBufferOffset = roundUpToMultiple(
                    argumentBuffer.description().descriptor.elements * 
                    argumentBuffer.description().descriptor.elementSize, 
                    D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT);
            }
            
            ID3D12CommandSignature* signature;
            m_device->device()->CreateCommandSignature(
                &desc,
                m_rootSignature,
                __uuidof(ID3D12CommandSignature),
                (void**)&signature);
            m_commandSignatures.emplace_back(signature);

            m_commandList->ExecuteIndirect(
                signature,                                              // ID3D12CommandSignature* pCommandSignature
                0,                                                      // UINT MaxCommandCount
                BufferImplGet::impl(argumentBuffer)->native(),          // ID3D12Resource* pArgumentBuffer
                argumentBufferOffset,                                   // UINT64 ArgumentBufferOffset
                countBuffer,                                            // ID3D12Resource* pCountBuffer
                countBufferOffset                                       // UINT64 CountBufferOffset
            );
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

        void CommandListImpl::copyTexture(const Texture& src, Texture& dst)
        {
            D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
            srcLocation.pResource = TextureImplGet::impl(src)->native();
            srcLocation.SubresourceIndex = 0;
            srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
            dstLocation.pResource = TextureImplGet::impl(dst)->native();
            dstLocation.SubresourceIndex = 0;
            dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            D3D12_BOX box = {};
            box.left = 0;
            box.top = 0;
            box.right = dst.width();
            box.bottom = dst.height();
            box.front = 0;
            box.back = 1;
            m_commandList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, &box);
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
