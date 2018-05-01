#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include "engine/graphics/dx12/DX12Headers.h"
#include "engine/graphics/dx12/DX12Common.h"
#include "tools/ComPtr.h"
#include "tools/ByteRange.h"

namespace engine
{
    namespace shaders
    {
        class PipelineConfiguration;
        class Shader;
    }

    namespace implementation
    {
        class DescriptorHeapImpl;
    }

    class Device;
    class Buffer;
    class BufferCBV;
    class TextureDSV;
    class TextureRTV;
    class BufferView;
    class RootSignature;
    struct BlendDescription;
    struct RasterizerDescription;
    struct DepthStencilDescription;
    enum class PrimitiveTopologyType;
    enum class Format;
    struct InputElementDescription;
    enum class IndexBufferStripCutValue;
    class ShaderBinary;
    class CommandList;
    class ShaderBinary;
    class Sampler;
    class TextureSRV;
    class ShaderStorage;
    class RootSignature;

    namespace implementation
    {
        class CommandListImpl;
        class PipelineImpl
        {
        public:
            PipelineImpl(
                Device& device,
                ShaderStorage& storage);

            ~PipelineImpl();
            PipelineImpl(const PipelineImpl&) = default;
            PipelineImpl(PipelineImpl&&) = default;
            PipelineImpl& operator=(const PipelineImpl&) = default;
            PipelineImpl& operator=(PipelineImpl&&) = default;

            void setBlendState(const BlendDescription& desc);
            void setRasterizerState(const RasterizerDescription& desc);
            void setDepthStencilState(const DepthStencilDescription& desc);
            void setSampleMask(unsigned int mask);
            void setPrimitiveTopologyType(PrimitiveTopologyType type, bool adjacency = false);
            void setRenderTargetFormat(Format RTVFormat, Format DSVFormat, unsigned int msaaCount = 1, unsigned int msaaQuality = 0);
            void setRenderTargetFormats(std::vector<Format> RTVFormats, Format DSVFormat, unsigned int msaaCount = 1, unsigned int msaaQuality = 0);
            void setInputLayout(unsigned int numElements, const InputElementDescription* inputElementDescs);
            void setPrimitiveRestart(IndexBufferStripCutValue value);
            void setDepthBufferView(std::shared_ptr<TextureDSV> view);

            void setUniformBuffer(const Buffer& buffer);
            void setTextureSRV(const TextureSRV& srv);
            void setSampler(const Sampler& sampler);

            void configure(CommandListImpl& commandList, shaders::PipelineConfiguration* configuration);

            ID3D12PipelineState* native() const;
            ID3D12RootSignature* rootSignature() const;

            struct TableRange
            {
                D3D12_DESCRIPTOR_RANGE1 range;
                D3D12_SHADER_VISIBILITY visibility;
                
                struct TableResource
                {
                    D3D_SRV_DIMENSION dimension;
                    std::string name;
                };
                std::vector<TableResource> resourceNames;
            };
            std::vector<TableRange>& descriptorRanges();
            D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDescription();
            D3D_PRIMITIVE_TOPOLOGY& topology();

            enum class RangeInfoType
            {
                SRV,
                UAV,
                CONSTANT,
                SAMPLER
            };
            struct ResourceRangeInfo
            {
                int resourceRange;
                D3D12_GPU_DESCRIPTOR_HANDLE resourceGpuHeapStart;
                bool srvUavCbvSet;
                RangeInfoType type;
            };
            struct SamplerRangeInfo
            {
                int resourceRange;
                D3D12_GPU_DESCRIPTOR_HANDLE samplerGpuHeapStart;
                bool samplerSet;
                RangeInfoType type;
            };
            const std::vector<ResourceRangeInfo>& resourceRanges() const
            {
                return m_resourceRanges;
            }
            const std::vector<SamplerRangeInfo>& samplerRanges() const
            {
                return m_samplerRanges;
            }

        private:
            void loadShaders(shaders::PipelineConfiguration* configuration);
            void generateBindings(shaders::PipelineConfiguration* configuration);

            //void generateBindings(const void* byteCode, size_t size, D3D12_SHADER_VISIBILITY visibility);
        private:
            Device& m_device;
            ShaderStorage& m_storage;

            void finalize(shaders::PipelineConfiguration* configuration);

            std::shared_ptr<RootSignature> m_rootSignature;
            tools::ComPtr<ID3D12PipelineState> m_pipelineState;
            std::shared_ptr<D3D12_GRAPHICS_PIPELINE_STATE_DESC> m_pipelineStateDesc;
            std::shared_ptr<D3D12_COMPUTE_PIPELINE_STATE_DESC> m_computePipelineStateDesc;
            std::vector<D3D12_INPUT_ELEMENT_DESC> m_inputLayouts;
            std::vector<char> m_semanticNames;
            uint32_t m_semanticIndex;

            std::vector<D3D12_INPUT_ELEMENT_DESC> m_elementDescs;
            D3D_PRIMITIVE_TOPOLOGY m_topology;

            std::shared_ptr<const ShaderBinary> m_cs;
            std::shared_ptr<const ShaderBinary> m_vs;
            std::shared_ptr<const ShaderBinary> m_ps;
            std::shared_ptr<const ShaderBinary> m_hs;
            std::shared_ptr<const ShaderBinary> m_gs;
            std::shared_ptr<const ShaderBinary> m_ds;

            std::vector<TableRange> m_range;
            std::vector<TableRange> m_compareRange;
            bool m_finalized;
            bool m_pipelineCreated;

            void createResourceBindingTables();
            void createSamplerBindingTables();

            uint32_t m_resourceCount;
            uint32_t m_samplerCount;

            DescriptorHandle m_resourceDescriptors;
            DescriptorHandle m_samplerDescriptors;

            std::vector<ResourceRangeInfo> m_resourceRanges;
            std::vector<SamplerRangeInfo> m_samplerRanges;
            
            bool m_descriptorResourceTablesInvalid;
            bool m_descriptorSamplerTablesInvalid;
            struct ResourceMap
            {
                size_t ptr;
                D3D12_CPU_DESCRIPTOR_HANDLE handle;
                std::function<uint64_t()> currentHandle;
            };
            std::vector<ResourceMap> m_resourceDescriptorMap;
            std::vector<ResourceMap> m_samplerDescriptorMap;

            std::vector<ConstantBufferUpdates> m_constantBufferUpdates;

            std::pair<bool, uint64_t> createPipelineResourceHash(size_t resourceCount) const;
            std::pair<bool, uint64_t> createPipelineSamplerHash() const;
            struct PipelineResourceHashStorage
            {
                DescriptorHandle resourceDescriptors;
                std::vector<ResourceRangeInfo> ranges;
                uint64_t lastUsedFrame;
            };
            struct PipelineSamplerHashStorage
            {
                DescriptorHandle samplerDescriptors;
                std::vector<SamplerRangeInfo> ranges;
                uint64_t lastUsedFrame;
            };
            std::unordered_map<uint64_t, PipelineResourceHashStorage> m_hashResourceStorage;
            std::unordered_map<uint64_t, PipelineSamplerHashStorage> m_hashSamplerStorage;

            struct BindlessBinding
            {
                shaders::Shader* shader;
                bool srv;
                bool texture;
                std::string name;
                size_t lastSize;
            };
            std::vector<BindlessBinding> m_bindlessBindings;

            class SamplerCacheItem
            {
            public:
            private:
                std::vector<uint64_t> m_ids;
            };
            std::unordered_map<uint64_t, DescriptorHandle> m_samplerCache;
        };
    }
}

