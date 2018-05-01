#include "engine/graphics/dx12/DX12Pipeline.h"
#include "engine/graphics/dx12/DX12Headers.h"
#include "engine/graphics/dx12/DX12Resources.h"
#include "engine/graphics/dx12/DX12ShaderBinary.h"
#include "engine/graphics/dx12/DX12Conversions.h"
#include "engine/graphics/dx12/DX12RootSignature.h"
#include "engine/graphics/dx12/DX12Device.h"
#include "engine/graphics/dx12/DX12RootParameter.h"
#include "engine/graphics/dx12/DX12DescriptorHeap.h"
#include "engine/graphics/dx12/DX12Sampler.h"
#include "engine/graphics/dx12/DX12CommandList.h"

#include "engine/graphics/ShaderBinary.h"
#include "engine/graphics/RootSignature.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/Sampler.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/CommonNoDep.h"
#include "engine/graphics/Barrier.h"
#include "tools/Debug.h"
#include "tools/hash/Hash.h"
#include <memory>
#include <algorithm>

#include <D3d12shader.h>
#include <D3Dcompiler.h>

#include <sstream>
#include <inttypes.h>

using namespace tools;

namespace engine
{
    namespace implementation
    {
        PipelineImpl::PipelineImpl(
            Device& device,
            ShaderStorage& storage)
            : m_device{ device }
            , m_storage{ storage }
            , m_rootSignature{ new RootSignature(device, 4) }
            , m_pipelineState{}
            , m_pipelineStateDesc{ std::make_shared<D3D12_GRAPHICS_PIPELINE_STATE_DESC>() }
            , m_computePipelineStateDesc{ std::make_shared<D3D12_COMPUTE_PIPELINE_STATE_DESC>() }
            , m_semanticIndex{ 0u }
            , m_topology{ D3D_PRIMITIVE_TOPOLOGY_UNDEFINED }
            , m_finalized{ false }
            , m_pipelineCreated{ false }
            , m_resourceCount{ 0 }
            , m_samplerCount{ 0 }
            , m_resourceDescriptors{}
            , m_samplerDescriptors{}
            , m_descriptorResourceTablesInvalid{ true }
            , m_descriptorSamplerTablesInvalid{ true }
        {
            m_semanticNames.resize(4096);
            m_pipelineStateDesc->BlendState.AlphaToCoverageEnable = false;
            m_pipelineStateDesc->BlendState.IndependentBlendEnable = false;
            m_pipelineStateDesc->BlendState.RenderTarget[0].BlendEnable = false;
            m_pipelineStateDesc->BlendState.RenderTarget[0].LogicOpEnable = false;
            m_pipelineStateDesc->BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
            m_pipelineStateDesc->BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
            m_pipelineStateDesc->BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
            m_pipelineStateDesc->BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
            m_pipelineStateDesc->BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
            m_pipelineStateDesc->BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
            m_pipelineStateDesc->BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
            m_pipelineStateDesc->BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
            m_pipelineStateDesc->SampleMask = UINT_MAX;

            m_pipelineStateDesc->RasterizerState = dxRasterizerDesc(RasterizerDescription());

            //finalize();
        }

        PipelineImpl::~PipelineImpl()
        {
            if (m_cs) m_cs->unregisterForChange(this);
            if (m_vs) m_vs->unregisterForChange(this);
            if (m_ps) m_ps->unregisterForChange(this);
            if (m_hs) m_hs->unregisterForChange(this);
            if (m_gs) m_gs->unregisterForChange(this);
            if (m_ds) m_ds->unregisterForChange(this);
        }

#if 1
        void PipelineImpl::setBlendState(const BlendDescription& desc)
        {
            m_pipelineStateDesc->BlendState = dxBlendDesc(desc);
        }

        void PipelineImpl::setRasterizerState(const RasterizerDescription& desc)
        {
            m_pipelineStateDesc->RasterizerState = dxRasterizerDesc(desc);
        }

        void PipelineImpl::setDepthStencilState(const DepthStencilDescription& desc)
        {
            m_pipelineStateDesc->DepthStencilState = dxDepthStencilDesc(desc);
        }

        void PipelineImpl::setSampleMask(unsigned int mask)
        {
            m_pipelineStateDesc->SampleMask = mask;
        }

        void PipelineImpl::setPrimitiveTopologyType(PrimitiveTopologyType type, bool adjacency)
        {
            ASSERT(dxPrimitiveTopologyType(type) != D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED);
            m_pipelineStateDesc->PrimitiveTopologyType = dxPrimitiveTopologyType(dxPrimitiveTopologyType(type, adjacency));
            m_topology = dxPrimitiveTopologyType(type, adjacency);
        }

        D3D_PRIMITIVE_TOPOLOGY& PipelineImpl::topology()
        {
            return m_topology;
        }

        D3D12_GRAPHICS_PIPELINE_STATE_DESC& PipelineImpl::pipelineDescription()
        {
            return *m_pipelineStateDesc;
        }

        void PipelineImpl::setRenderTargetFormat(Format RTVFormat, Format DSVFormat, unsigned int msaaCount, unsigned int msaaQuality)
        {
            setRenderTargetFormats({ RTVFormat }, DSVFormat, msaaCount, msaaQuality);
        }

        void PipelineImpl::setRenderTargetFormats(std::vector<Format> RTVFormats, Format DSVFormat, unsigned int msaaCount, unsigned int msaaQuality)
        {
            // Null format array conflicts with non-zero length
            uint32_t index = 0;
            for (auto&& format : RTVFormats)
            {
                m_pipelineStateDesc->RTVFormats[index] = dxFormat(format);
                ++index;
            }
            m_pipelineStateDesc->NumRenderTargets = static_cast<UINT>(RTVFormats.size());
            m_pipelineStateDesc->DSVFormat = dxFormat(DSVFormat);
            m_pipelineStateDesc->SampleDesc.Count = msaaCount;
            m_pipelineStateDesc->SampleDesc.Quality = msaaQuality;
        }
#endif

        void PipelineImpl::setInputLayout(unsigned int /*numElements*/, const InputElementDescription* /*inputElementDescs*/)
        {
            LOG("DX12 Pipeline setInputLayout not implemented");
            /*m_pipelineStateDesc->InputLayout.NumElements = numElements;
            if (numElements > 0)
            {
                std::unique_ptr<D3D12_INPUT_ELEMENT_DESC[]> NewElements(new D3D12_INPUT_ELEMENT_DESC[numElements]);
                if(NewElements)
                    memcpy(NewElements.get(), inputElementDescs, numElements * sizeof(D3D12_INPUT_ELEMENT_DESC));
                m_inputLayouts.reset((const D3D12_INPUT_ELEMENT_DESC*)NewElements.get());
            }
            else
            {
                m_inputLayouts = nullptr;
            }*/
        }

        void PipelineImpl::setPrimitiveRestart(IndexBufferStripCutValue value)
        {
            m_pipelineStateDesc->IBStripCutValue = dxIndexBufferStripCutValue(value);
        }

#if 1
        void PipelineImpl::setDepthBufferView(std::shared_ptr<TextureDSV> /*view*/)
        {
            LOG("DX12 Pipeline setDepthBufferView not implemented");
        }

        void PipelineImpl::setUniformBuffer(const Buffer& /*buffer*/)
        {
            LOG("DX12 Pipeline setUniformBuffer not implemented");
        }

        void PipelineImpl::setTextureSRV(const TextureSRV& /*srv*/)
        {
            LOG("DX12 Pipeline setTextureSRV not implemented");
        }

        void PipelineImpl::setSampler(const Sampler& /*sampler*/)
        {
            LOG("DX12 Pipeline setSampler not implemented");
        }

#endif
        void PipelineImpl::loadShaders(shaders::PipelineConfiguration* configuration)
        {
            if (configuration->hasComputeShader())
            {
                m_cs = configuration->computeShader()->load(m_device, m_storage);
                auto desc = m_computePipelineStateDesc.get();
                auto bin = m_cs.get();
                m_cs->registerForChange(this, [this, desc, bin, configuration]()
                {
                    desc->CS = ShaderBinaryImplGet::impl(*bin).native();
                    this->generateBindings(configuration);
                    m_pipelineState.Reset();
                    auto res = DeviceImplGet::impl(this->m_device).device()->CreateComputePipelineState(
                        m_computePipelineStateDesc.get(),
                        DARKNESS_IID_PPV_ARGS(m_pipelineState.GetAddressOf()));
                    ASSERT(SUCCEEDED(res));
                });
                m_computePipelineStateDesc->CS = ShaderBinaryImplGet::impl(*m_cs).native();
            }
            if (configuration->hasVertexShader())
            {
                m_vs = configuration->vertexShader()->load(m_device, m_storage);
                auto desc = m_pipelineStateDesc.get();
                auto bin = m_vs.get();
                m_vs->registerForChange(this, [this, desc, bin, configuration]()
                { 
                    desc->VS = ShaderBinaryImplGet::impl(*bin).native();
                    this->generateBindings(configuration);
                    m_pipelineState.Reset();
                    auto res = DeviceImplGet::impl(this->m_device).device()->CreateGraphicsPipelineState(
                        this->m_pipelineStateDesc.get(),
                        DARKNESS_IID_PPV_ARGS(this->m_pipelineState.GetAddressOf()));
                    ASSERT(SUCCEEDED(res));
                });
                m_pipelineStateDesc->VS = ShaderBinaryImplGet::impl(*m_vs).native();
            }
            if (configuration->hasPixelShader())
            {
                m_ps = configuration->pixelShader()->load(m_device, m_storage);
                auto desc = m_pipelineStateDesc.get();
                auto bin = m_ps.get();
                m_ps->registerForChange(this, [this, desc, bin, configuration]()
                { 
                    desc->PS = ShaderBinaryImplGet::impl(*bin).native();
                    this->generateBindings(configuration);
                    m_pipelineState.Reset();
                    auto res = DeviceImplGet::impl(this->m_device).device()->CreateGraphicsPipelineState(
                        this->m_pipelineStateDesc.get(),
                        DARKNESS_IID_PPV_ARGS(this->m_pipelineState.GetAddressOf()));
                    ASSERT(SUCCEEDED(res));
                });
                m_pipelineStateDesc->PS = ShaderBinaryImplGet::impl(*m_ps).native();
            }
            if (configuration->hasHullShader())
            {
                m_hs = configuration->hullShader()->load(m_device, m_storage);
                auto desc = m_pipelineStateDesc.get();
                auto bin = m_hs.get();
                m_hs->registerForChange(this, [this, desc, bin, configuration]()
                {
                    desc->HS = ShaderBinaryImplGet::impl(*bin).native();
                    this->generateBindings(configuration);
                    m_pipelineState.Reset();
                    auto res = DeviceImplGet::impl(this->m_device).device()->CreateGraphicsPipelineState(
                        this->m_pipelineStateDesc.get(),
                        DARKNESS_IID_PPV_ARGS(this->m_pipelineState.GetAddressOf()));
                    ASSERT(SUCCEEDED(res));
                });
                m_pipelineStateDesc->HS = ShaderBinaryImplGet::impl(*m_hs).native();
            }
            if (configuration->hasGeometryShader())
            {
                m_gs = configuration->geometryShader()->load(m_device, m_storage);
                auto desc = m_pipelineStateDesc.get();
                auto bin = m_gs.get();
                m_gs->registerForChange(this, [this, desc, bin, configuration]()
                {
                    desc->GS = ShaderBinaryImplGet::impl(*bin).native();
                    this->generateBindings(configuration);
                    m_pipelineState.Reset();
                    auto res = DeviceImplGet::impl(this->m_device).device()->CreateGraphicsPipelineState(
                        this->m_pipelineStateDesc.get(),
                        DARKNESS_IID_PPV_ARGS(this->m_pipelineState.GetAddressOf()));
                    ASSERT(SUCCEEDED(res));
                });
                m_pipelineStateDesc->GS = ShaderBinaryImplGet::impl(*m_gs).native();
            }
            if (configuration->hasDomainShader())
            {
                m_ds = configuration->domainShader()->load(m_device, m_storage);
                auto desc = m_pipelineStateDesc.get();
                auto bin = m_ds.get();
                m_ds->registerForChange(this, [this, desc, bin, configuration]()
                {
                    desc->DS = ShaderBinaryImplGet::impl(*bin).native();
                    this->generateBindings(configuration);
                    m_pipelineState.Reset();
                    auto res = DeviceImplGet::impl(this->m_device).device()->CreateGraphicsPipelineState(
                        this->m_pipelineStateDesc.get(),
                        DARKNESS_IID_PPV_ARGS(this->m_pipelineState.GetAddressOf()));
                    ASSERT(SUCCEEDED(res));
                });
                m_pipelineStateDesc->DS = ShaderBinaryImplGet::impl(*m_ds).native();
            }
        }

        DXGI_FORMAT formatFromSignature(D3D_REGISTER_COMPONENT_TYPE componentType, BYTE mask)
        {
            ASSERT(mask);
            /*if ((mask & 0x1000) |
                (mask & 0x0100) |
                (mask & 0x0010) |
                (mask & 0x0001))*/
            if(mask == 15)
            {
                // four components
                switch (componentType)
                {
                case D3D_REGISTER_COMPONENT_UINT32: return DXGI_FORMAT_R32G32B32A32_UINT;
                case D3D_REGISTER_COMPONENT_SINT32: return DXGI_FORMAT_R32G32B32A32_SINT;
                case D3D_REGISTER_COMPONENT_FLOAT32: return DXGI_FORMAT_R32G32B32A32_FLOAT;
                default: return DXGI_FORMAT_UNKNOWN;
                }
            }
            /*else if ((mask & 0x0100) |
                     (mask & 0x0010) |
                     (mask & 0x0001))*/
            else if (mask == 7)
            {
                // four components
                switch (componentType)
                {
                case D3D_REGISTER_COMPONENT_UINT32: return DXGI_FORMAT_R32G32B32_UINT;
                case D3D_REGISTER_COMPONENT_SINT32: return DXGI_FORMAT_R32G32B32_SINT;
                case D3D_REGISTER_COMPONENT_FLOAT32: return DXGI_FORMAT_R32G32B32_FLOAT;
                default: return DXGI_FORMAT_UNKNOWN;
                }
            }
            /*else if ((mask & 0x0010) |
                     (mask & 0x0001))*/
            else if (mask == 3)
            {
                // four components
                switch (componentType)
                {
                case D3D_REGISTER_COMPONENT_UINT32: return DXGI_FORMAT_R32G32_UINT;
                case D3D_REGISTER_COMPONENT_SINT32: return DXGI_FORMAT_R32G32_SINT;
                case D3D_REGISTER_COMPONENT_FLOAT32: return DXGI_FORMAT_R32G32_FLOAT;
                default: return DXGI_FORMAT_UNKNOWN;
                }
            }
            //else if (mask & 0x0001)
            else if (mask == 1)
            {
                // four components
                switch (componentType)
                {
                case D3D_REGISTER_COMPONENT_UINT32: return DXGI_FORMAT_R32_UINT;
                case D3D_REGISTER_COMPONENT_SINT32: return DXGI_FORMAT_R32_SINT;
                case D3D_REGISTER_COMPONENT_FLOAT32: return DXGI_FORMAT_R32_FLOAT;
                default: return DXGI_FORMAT_UNKNOWN;
                }
            }
            ASSERT(false);
            return DXGI_FORMAT_UNKNOWN;
        }

        const char* dimensionToString(D3D_SRV_DIMENSION dimension)
        {
            switch (dimension)
            {
            case D3D_SRV_DIMENSION_UNKNOWN:            return "D3D_SRV_DIMENSION_UNKNOWN    ";
            case D3D_SRV_DIMENSION_BUFFER:            return "D3D_SRV_DIMENSION_BUFFER     ";
            case D3D_SRV_DIMENSION_TEXTURE1D:        return "D3D_SRV_DIMENSION_TEXTURE1D  ";
            case D3D_SRV_DIMENSION_TEXTURE1DARRAY:    return "D3D_SRV_DIMENSION_TEXTURE1DARRAY";
            case D3D_SRV_DIMENSION_TEXTURE2D:        return "D3D_SRV_DIMENSION_TEXTURE2D  ";
            case D3D_SRV_DIMENSION_TEXTURE2DARRAY:    return "D3D_SRV_DIMENSION_TEXTURE2DARRAY";
            case D3D_SRV_DIMENSION_TEXTURE2DMS:        return "D3D_SRV_DIMENSION_TEXTURE2DMS";
            case D3D_SRV_DIMENSION_TEXTURE2DMSARRAY:return "D3D_SRV_DIMENSION_TEXTURE2DMSARRAY";
            case D3D_SRV_DIMENSION_TEXTURE3D:        return "D3D_SRV_DIMENSION_TEXTURE3D  ";
            case D3D_SRV_DIMENSION_TEXTURECUBE:        return "D3D_SRV_DIMENSION_TEXTURECUBE";
            case D3D_SRV_DIMENSION_TEXTURECUBEARRAY:return "D3D_SRV_DIMENSION_TEXTURECUBEARRAY";
            case D3D_SRV_DIMENSION_BUFFEREX:        return "D3D_SRV_DIMENSION_BUFFEREX   ";
            default: return "";
            }
        }

        const char* returnTypeToString(D3D_RESOURCE_RETURN_TYPE type)
        {
            switch (type)
            {
            case D3D_RETURN_TYPE_UNORM:        return "D3D_RETURN_TYPE_UNORM    ";
            case D3D_RETURN_TYPE_SNORM:        return "D3D_RETURN_TYPE_SNORM    ";
            case D3D_RETURN_TYPE_SINT:        return "D3D_RETURN_TYPE_SINT     ";
            case D3D_RETURN_TYPE_UINT:        return "D3D_RETURN_TYPE_UINT     ";
            case D3D_RETURN_TYPE_FLOAT:        return "D3D_RETURN_TYPE_FLOAT    ";
            case D3D_RETURN_TYPE_MIXED:        return "D3D_RETURN_TYPE_MIXED    ";
            case D3D_RETURN_TYPE_DOUBLE:    return "D3D_RETURN_TYPE_DOUBLE   ";
            case D3D_RETURN_TYPE_CONTINUED:    return "D3D_RETURN_TYPE_CONTINUED";
            default: return "";
            }
        }

        const char* inputTypeToString(D3D_SHADER_INPUT_TYPE type)
        {
            switch (type)
            {
            case D3D_SIT_CBUFFER:                        return "D3D_SIT_CBUFFER               ";
            case D3D_SIT_TBUFFER:                        return "D3D_SIT_TBUFFER               ";
            case D3D_SIT_TEXTURE:                        return "D3D_SIT_TEXTURE               ";
            case D3D_SIT_SAMPLER:                        return "D3D_SIT_SAMPLER               ";
            case D3D_SIT_UAV_RWTYPED:                    return "D3D_SIT_UAV_RWTYPED           ";
            case D3D_SIT_STRUCTURED:                    return "D3D_SIT_STRUCTURED            ";
            case D3D_SIT_UAV_RWSTRUCTURED:                return "D3D_SIT_UAV_RWSTRUCTURED      ";
            case D3D_SIT_BYTEADDRESS:                    return "D3D_SIT_BYTEADDRESS           ";
            case D3D_SIT_UAV_RWBYTEADDRESS:                return "D3D_SIT_UAV_RWBYTEADDRESS     ";
            case D3D_SIT_UAV_APPEND_STRUCTURED:            return "D3D_SIT_UAV_APPEND_STRUCTURED ";
            case D3D_SIT_UAV_CONSUME_STRUCTURED:        return "D3D_SIT_UAV_CONSUME_STRUCTURED";
            case D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER:    return "D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER";
            default: return "";
            }
        }

        D3D12_DESCRIPTOR_RANGE_TYPE rangeTypeFromInputType(D3D_SHADER_INPUT_TYPE type)
        {
            switch (type)
            {
            case D3D_SIT_CBUFFER:                        return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
            case D3D_SIT_TBUFFER:                        return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            case D3D_SIT_TEXTURE:                        return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            case D3D_SIT_SAMPLER:                        return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
            case D3D_SIT_UAV_RWTYPED:                    return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
            case D3D_SIT_STRUCTURED:                    return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            case D3D_SIT_UAV_RWSTRUCTURED:                return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
            case D3D_SIT_BYTEADDRESS:                    return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            case D3D_SIT_UAV_RWBYTEADDRESS:                return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
            case D3D_SIT_UAV_APPEND_STRUCTURED:            return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
            case D3D_SIT_UAV_CONSUME_STRUCTURED:        return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
            case D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER:    return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
            default: return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            }
        }

        void printElementDesc(const D3D12_INPUT_ELEMENT_DESC& desc)
        {
            LOG("SemanticName: %s, SemanticIndex: %u, Format: %s, InputSlot: %u, AlignedByteOffset: %u, InstanceDataStepRate: %u",
                desc.SemanticName,
                desc.SemanticIndex,
                formatToString(fromDXFormat(desc.Format)).c_str(),
                desc.InputSlot,
                desc.AlignedByteOffset,
                desc.InstanceDataStepRate);
        }

#if 0
        void PipelineImpl::generateBindings(const void* byteCode, size_t size, D3D12_SHADER_VISIBILITY visibility)
        {

            ID3D12ShaderReflection* reflector;
            auto res = D3DReflect(
                byteCode,
                size,
                IID_ID3D12ShaderReflection,
                (void**)&reflector
            );
            ASSERT(SUCCEEDED(res));

            D3D12_SHADER_DESC shaderDesc;
            res = reflector->GetDesc(&shaderDesc);

            // INPUT PARAMETERS
#if 0
            for (unsigned int i = 0; i < shaderDesc.InputParameters; ++i)
            {
                D3D12_SIGNATURE_PARAMETER_DESC input_desc;
                reflector->GetInputParameterDesc(i, &input_desc);

                bool semanticExists = false;
                for (auto&& d : m_inputLayouts)
                {
                    if (strcmp(d.SemanticName, input_desc.SemanticName) == 0)
                    {
                        semanticExists = true;
                        break;
                    }
                }

                if (!semanticExists)
                {
                    D3D12_INPUT_ELEMENT_DESC desc;
                    desc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
                    desc.Format = formatFromSignature(input_desc.ComponentType, input_desc.Mask);
                    desc.InputSlot = input_desc.Stream;
                    desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
                    desc.InstanceDataStepRate = 0;
                    desc.SemanticIndex = input_desc.SemanticIndex;
                    desc.SemanticName = input_desc.SemanticName;
                    m_inputLayouts.emplace_back(desc);
                    printElementDesc(desc);
                }
            }

            m_pipelineStateDesc->InputLayout.NumElements = static_cast<UINT>(m_inputLayouts.size());
            m_pipelineStateDesc->InputLayout.pInputElementDescs = m_inputLayouts.data();
#endif

            // CONSTANT BUFFERS
#if 0
            struct ConstantBufferLayout
            {
                D3D12_SHADER_BUFFER_DESC description;
                std::vector<D3D12_SHADER_VARIABLE_DESC> variables;
                std::vector<D3D12_SHADER_TYPE_DESC> types;
            };
            std::vector<ConstantBufferLayout> constantBuffers;
            for (unsigned int i = 0; i < shaderDesc.ConstantBuffers; ++i)
            {

                ConstantBufferLayout bufferLayout;
                ID3D12ShaderReflectionConstantBuffer* constBuffer = reflector->GetConstantBufferByIndex(i);
                constBuffer->GetDesc(&bufferLayout.description);

                for (unsigned int a = 0; a < bufferLayout.description.Variables; ++a)
                {
                    ID3D12ShaderReflectionVariable* variable = constBuffer->GetVariableByIndex(a);
                    D3D12_SHADER_VARIABLE_DESC variableDesc;
                    variable->GetDesc(&variableDesc);
                    bufferLayout.variables.emplace_back(variableDesc);

                    ID3D12ShaderReflectionType* type = variable->GetType();
                    D3D12_SHADER_TYPE_DESC typeDesc;
                    type->GetDesc(&typeDesc);
                    bufferLayout.types.emplace_back(typeDesc);
                }
                constantBuffers.emplace_back(bufferLayout);
            }
#endif

            // RESOURCE BINDINGS
#if 1
            for (unsigned int i = 0; i < shaderDesc.BoundResources; ++i)
            {
                D3D12_SHADER_INPUT_BIND_DESC desc;
                reflector->GetResourceBindingDesc(i, &desc);

                auto inputType = rangeTypeFromInputType(desc.Type);

                auto search = std::find_if(
                    m_compareRange.begin(),
                    m_compareRange.end(),
                    [inputType, visibility](const TableRange& r) {
                    return (inputType == r.range.RangeType) && (r.visibility == visibility); });

                if (search == m_compareRange.end())
                {
                    D3D12_DESCRIPTOR_RANGE1 new_range;
                    new_range.BaseShaderRegister = 0;
                    new_range.NumDescriptors = 1;
                    new_range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    new_range.RangeType = inputType;
                    new_range.RegisterSpace = 0;
                    new_range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
                    /*if (inputType == D3D12_DESCRIPTOR_RANGE_TYPE_UAV)
                    {
                        new_range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE;
                    }*/
                    TableRange::TableResource tableResource{ desc.Dimension, std::string(desc.Name) };
                    std::vector<TableRange::TableResource> vec{ std::move(tableResource) };
                    auto tableRange = TableRange{ new_range, visibility, std::move(vec) };
                    m_compareRange.emplace_back(std::move(tableRange));
                }
                else
                {
                    (*search).range.NumDescriptors++;
                    TableRange::TableResource tableResource{ desc.Dimension, std::string(desc.Name) };
                    (*search).resourceNames.emplace_back(std::move(tableResource));
                }
            }
#endif
        }
#endif

        std::string rangeTypeToString(D3D12_DESCRIPTOR_RANGE_TYPE type)
        {
            switch (type)
            {
            case D3D12_DESCRIPTOR_RANGE_TYPE_SRV: { return std::string("D3D12_DESCRIPTOR_RANGE_TYPE_SRV"); }
            case D3D12_DESCRIPTOR_RANGE_TYPE_UAV: { return std::string("D3D12_DESCRIPTOR_RANGE_TYPE_UAV"); }
            case D3D12_DESCRIPTOR_RANGE_TYPE_CBV: { return std::string("D3D12_DESCRIPTOR_RANGE_TYPE_CBV"); }
            case D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER: { return std::string("D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER"); }
            }
        }

        std::string rangeFlagsToString(D3D12_DESCRIPTOR_RANGE_FLAGS flags)
        {
            std::string res = "";
            if (flags & D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE) res += " | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE";
            if (flags & D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE) res += " | D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE";
            if (flags & D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE) res += " | D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE";
            if (flags & D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC) res += " | D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC";
            return res;
        }

        std::string rangeVisibilityToString(D3D12_SHADER_VISIBILITY visibility)
        {
            switch (visibility)
            {
            case D3D12_SHADER_VISIBILITY_ALL: return "D3D12_SHADER_VISIBILITY_ALL";
            case D3D12_SHADER_VISIBILITY_VERTEX: return "D3D12_SHADER_VISIBILITY_VERTEX";
            case D3D12_SHADER_VISIBILITY_HULL: return "D3D12_SHADER_VISIBILITY_HULL";
            case D3D12_SHADER_VISIBILITY_DOMAIN: return "D3D12_SHADER_VISIBILITY_DOMAIN";
            case D3D12_SHADER_VISIBILITY_GEOMETRY: return "D3D12_SHADER_VISIBILITY_GEOMETRY";
            case D3D12_SHADER_VISIBILITY_PIXEL: return "D3D12_SHADER_VISIBILITY_PIXEL";
            }
        }

        void printRange(const std::vector<PipelineImpl::TableRange>& range)
        {
            LOG(" PIPELINE START ");
            for (auto&& tableRange : range)
            {
                LOG("RANGE START");
                LOG("RangeType: %s", rangeTypeToString(tableRange.range.RangeType).c_str());
                LOG("NumDescriptors: %u", tableRange.range.NumDescriptors);
                LOG("BaseShaderRegister: %u", tableRange.range.BaseShaderRegister);
                LOG("RegisterSpace: %u", tableRange.range.RegisterSpace);
                LOG("Flags: %s", rangeFlagsToString(tableRange.range.Flags).c_str());
                LOG("OffsetInDescriptorsFromTableStart: %u", tableRange.range.OffsetInDescriptorsFromTableStart);
                LOG("visibility: %s", rangeVisibilityToString(tableRange.visibility).c_str());
                int i = 0;
                for (auto&& res : tableRange.resourceNames)
                {
                    LOG("TableResource[%i].dimension: %s", i, dimensionToString(res.dimension));
                    LOG("TableResource[%i].name: %s", i, res.name.c_str());
                    ++i;
                }
                LOG("RANGE STOP");
            }
            LOG(" PIPELINE STOP ");
        }

        struct DebugOutFile
        {
            std::ofstream file;
        };
        static std::shared_ptr<DebugOutFile> MyLog = nullptr;
        static std::shared_ptr<DebugOutFile> RefLog = nullptr;

        void PipelineImpl::generateBindings(shaders::PipelineConfiguration* configuration)
        {
            m_inputLayouts.clear();
            m_range.clear();
            m_compareRange.clear();
            m_semanticIndex = 0;
            memset(&m_semanticNames[0], '\0', m_semanticNames.size());

            auto shaderTypeToDXGI = [](const std::string& type)->DXGI_FORMAT
            {
                if (type == "uint") return DXGI_FORMAT_R32_UINT;
                else if (type == "uint2") return DXGI_FORMAT_R32G32_UINT;
                else if (type == "uint3") return DXGI_FORMAT_R32G32B32_UINT;
                else if (type == "uint4") return DXGI_FORMAT_R32G32B32A32_UINT;
                else if (type == "float") return DXGI_FORMAT_R32_FLOAT;
                else if (type == "float2") return DXGI_FORMAT_R32G32_FLOAT;
                else if (type == "float3") return DXGI_FORMAT_R32G32B32_FLOAT;
                else if (type == "float4") return DXGI_FORMAT_R32G32B32A32_FLOAT;
                else
                    ASSERT(false, "Unhandled shader input type");
                return DXGI_FORMAT_UNKNOWN;
            };
            auto extractSemanticIndex = [](const std::string& semanticName)->uint32_t
            {
                std::string index = "";
                //for (auto&& ch : semanticName)
                for(auto ch = semanticName.rbegin(); ch != semanticName.rend(); ++ch)
                {
                    if ((*ch) >= 48 && (*ch) <= 57)
                    {
                        std::string temp = "";
                        temp += *ch;
                        index.insert(0, temp);
                    }
                    else break;
                }
                if(index.size() > 0)
                	return static_cast<uint32_t>(std::stoi(index));
                else return 0u;
            };
            auto extractSemanticName = [](const std::string& semanticName)->std::string
            {
                int indexLength = 0;
                for (auto ch = semanticName.rbegin(); ch != semanticName.rend(); ++ch)
                {
                    if ((*ch) >= 48 && (*ch) <= 57)
                    {
                        ++indexLength;
                    }
                    else break;
                }
                return semanticName.substr(0, semanticName.length() - indexLength);
            };
            auto addInputsToLayout = [shaderTypeToDXGI, extractSemanticIndex, extractSemanticName]
                (std::vector<D3D12_INPUT_ELEMENT_DESC>& layouts, 
                 const std::vector<shaders::ShaderInputParameter>& inputs,
                    std::vector<char>& semanticNames,
                    uint32_t& semanticIndex)
            {
                for(int i = 0; i < inputs.size(); ++i)
                {
                    D3D12_INPUT_ELEMENT_DESC desc;
                    desc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
                    desc.Format = shaderTypeToDXGI(inputs[i].type);
                    desc.InputSlot = 0;
                    desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
                    desc.InstanceDataStepRate = 0;
                    desc.SemanticIndex = extractSemanticIndex(inputs[i].semanticName);

                    auto semanticNameWithoutNumber = extractSemanticName(inputs[i].semanticName);
                    auto currentSemanticIndex = semanticIndex;
                    ASSERT(currentSemanticIndex + semanticNameWithoutNumber.size() < semanticNames.size(), "DX12 Pipeline ran out of semantics namespace. (Shader parameter input names are so long that we can't store them within 4kb)");
                    memcpy(&semanticNames[currentSemanticIndex], semanticNameWithoutNumber.data(), semanticNameWithoutNumber.size());
                    semanticIndex += static_cast<uint32_t>(semanticNameWithoutNumber.size()) + 1u;
                    desc.SemanticName = &semanticNames[currentSemanticIndex];
                    
                    printElementDesc(desc);

                    bool identicalSemanticFound = false;
                    for (auto&& l : layouts)
                    {
                        if ((l.SemanticIndex == desc.SemanticIndex) &&
                            (strcmp(l.SemanticName, desc.SemanticName) == 0))
                        {
                            identicalSemanticFound = true;
                            break;
                        }
                    }
                    if(!identicalSemanticFound)
                        layouts.emplace_back(desc);
                }
            };

            auto addSamplers = [](std::vector<TableRange>& tableRange, const shaders::Shader* shader, D3D12_SHADER_VISIBILITY visibility)
            {
                auto uavCount = shader->samplerNames().size();
                if (uavCount > 0)
                {
                    bool found = false;
                    TableRange* range = nullptr;
                    for(int i = 0; i < tableRange.size(); ++i)
                    {
                        if ((tableRange[i].range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER) &&
                            (tableRange[i].visibility == visibility))
                        {
                            range = &tableRange[i];
                            range->range.NumDescriptors += uavCount;
                            break;
                        }
                    }
                    if (!range)
                    {
                        tableRange.emplace_back(TableRange{});
                        range = &tableRange.back();
                        range->range.NumDescriptors = uavCount;
                    }
                    range->range.BaseShaderRegister = 0;
                    range->range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    range->range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
                    range->range.RegisterSpace = 0;
                    range->range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
                    range->visibility = visibility;

                    for (auto&& sname : shader->samplerNames())
                    {
                        TableRange::TableResource tableResource;
                        tableResource.dimension = D3D_SRV_DIMENSION_UNKNOWN;
                        tableResource.name = sname;
                        range->resourceNames.emplace_back(tableResource);
                    }
                }
            };
            /*auto addBufferSRVs = [](std::vector<TableRange>& tableRange, const shaders::Shader* shader, D3D12_SHADER_VISIBILITY visibility)
            {
                auto srvCount = shader->bufferSrvNames().size();
                if (srvCount > 0)
                {
                    bool found = false;
                    TableRange* range = nullptr;
                    for (int i = 0; i < tableRange.size(); ++i)
                    {
                        if ((tableRange[i].range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SRV) &&
                            (tableRange[i].visibility == visibility))
                        {
                            range = &tableRange[i];
                            range->range.NumDescriptors += srvCount;
                            break;
                        }
                    }
                    if (!range)
                    {
                        tableRange.emplace_back(TableRange{});
                        range = &tableRange.back();
                        range->range.NumDescriptors = srvCount;
                    }
                    range->range.BaseShaderRegister = 0;
                    range->range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    range->range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                    range->range.RegisterSpace = 0;
                    range->range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
                    range->visibility = visibility;

                    for (auto&& sname : shader->bufferSrvNames())
                    {
                        TableRange::TableResource tableResource;
                        tableResource.dimension = D3D_SRV_DIMENSION_BUFFER;
                        tableResource.name = sname;
                        range->resourceNames.emplace_back(tableResource);
                    }
                }
            };
            auto addTextureSRVs = [](std::vector<TableRange>& tableRange, const shaders::Shader* shader, D3D12_SHADER_VISIBILITY visibility)
            {
                auto srvCount = shader->textureSrvNames().size();
                if (srvCount > 0)
                {
                    bool found = false;
                    TableRange* range = nullptr;
                    for (int i = 0; i < tableRange.size(); ++i)
                    {
                        if ((tableRange[i].range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SRV) &&
                            (tableRange[i].visibility == visibility))
                        {
                            range = &tableRange[i];
                            range->range.NumDescriptors += srvCount;
                            break;
                        }
                    }
                    if (!range)
                    {
                        tableRange.emplace_back(TableRange{});
                        range = &tableRange.back();
                        range->range.NumDescriptors = srvCount;
                    }
                    range->range.BaseShaderRegister = 0;
                    range->range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    range->range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                    range->range.RegisterSpace = 0;
                    range->range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
                    range->visibility = visibility;

                    for (auto&& sname : shader->textureSrvNames())
                    {
                        TableRange::TableResource tableResource;
                        auto res = shader->textureSrv(sname);
                        if (res.valid())
                        {
                            switch (res.texture().dimension())
                            {
                            case ResourceDimension::Texture1D: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE1D; break; }
                            case ResourceDimension::Texture2D: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE2D; break; }
                            case ResourceDimension::Texture3D: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE3D; break; }
                            case ResourceDimension::Texture1DArray: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE1DARRAY; break; }
                            case ResourceDimension::Texture2DArray: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE2DARRAY; break; }
                            case ResourceDimension::TextureCubemap: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURECUBE; break; }
                            case ResourceDimension::TextureCubemapArray: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURECUBEARRAY; break; }
                            }
                        }
                        else
                        {
                            tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE2D;
                        }
                        tableResource.name = sname;
                        range->resourceNames.emplace_back(tableResource);
                    }
                }
            };
            auto addBufferUAVs = [](std::vector<TableRange>& tableRange, const shaders::Shader* shader, D3D12_SHADER_VISIBILITY visibility)
            {
                auto uavCount = shader->bufferUavNames().size();
                if (uavCount > 0)
                {
                    bool found = false;
                    TableRange* range = nullptr;
                    for (int i = 0; i < tableRange.size(); ++i)
                    {
                        if ((tableRange[i].range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_UAV) &&
                            (tableRange[i].visibility == visibility))
                        {
                            range = &tableRange[i];
                            range->range.NumDescriptors += uavCount;
                            break;
                        }
                    }
                    if (!range)
                    {
                        tableRange.emplace_back(TableRange{});
                        range = &tableRange.back();
                        range->range.NumDescriptors = uavCount;
                    }
                    range->range.BaseShaderRegister = 0;
                    range->range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    range->range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
                    range->range.RegisterSpace = 0;
                    range->range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
                    range->visibility = visibility;

                    for (auto&& sname : shader->bufferUavNames())
                    {
                        TableRange::TableResource tableResource;
                        tableResource.dimension = D3D_SRV_DIMENSION_BUFFER;
                        tableResource.name = sname;
                        range->resourceNames.emplace_back(tableResource);
                    }
                }
            };
            auto addTextureUAVs = [](std::vector<TableRange>& tableRange, const shaders::Shader* shader, D3D12_SHADER_VISIBILITY visibility)
            {
                auto uavCount = shader->textureUavNames().size();
                if (uavCount > 0)
                {
                    bool found = false;
                    TableRange* range = nullptr;
                    for (int i = 0; i < tableRange.size(); ++i)
                    {
                        if ((tableRange[i].range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_UAV) &&
                            (tableRange[i].visibility == visibility))
                        {
                            range = &tableRange[i];
                            range->range.NumDescriptors += uavCount;
                            break;
                        }
                    }
                    if (!range)
                    {
                        tableRange.emplace_back(TableRange{});
                        range = &tableRange.back();
                        range->range.NumDescriptors = uavCount;
                    }
                    range->range.BaseShaderRegister = 0;
                    range->range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    range->range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
                    range->range.RegisterSpace = 0;
                    range->range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
                    range->visibility = visibility;

                    for (auto&& sname : shader->textureUavNames())
                    {
                        TableRange::TableResource tableResource;
                        auto res = shader->textureUav(sname);
                        if (res.valid())
                        {
                            switch (res.texture().dimension())
                            {
                            case ResourceDimension::Texture1D: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE1D; break; }
                            case ResourceDimension::Texture2D: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE2D; break; }
                            case ResourceDimension::Texture3D: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE3D; break; }
                            case ResourceDimension::Texture1DArray: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE1DARRAY; break; }
                            case ResourceDimension::Texture2DArray: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE2DARRAY; break; }
                            case ResourceDimension::TextureCubemap: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURECUBE; break; }
                            case ResourceDimension::TextureCubemapArray: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURECUBEARRAY; break; }
                            }
                        }
                        else
                        {
                            tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE2D;
                        }
                        tableResource.name = sname;
                        range->resourceNames.emplace_back(tableResource);
                    }
                }
            };*/

            auto addSRVs = [](std::vector<TableRange>& tableRange, const shaders::Shader* shader, D3D12_SHADER_VISIBILITY visibility)
            {
                auto srvCount = shader->srvNames().size();
                if (srvCount > 0)
                {
                    bool found = false;
                    TableRange* range = nullptr;
                    for (int i = 0; i < tableRange.size(); ++i)
                    {
                        if ((tableRange[i].range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SRV) &&
                            (tableRange[i].visibility == visibility))
                        {
                            range = &tableRange[i];
                            range->range.NumDescriptors += srvCount;
                            break;
                        }
                    }
                    if (!range)
                    {
                        tableRange.emplace_back(TableRange{});
                        range = &tableRange.back();
                        range->range.NumDescriptors = srvCount;
                    }
                    range->range.BaseShaderRegister = 0;
                    range->range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    range->range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                    range->range.RegisterSpace = 0;
                    range->range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
                    range->visibility = visibility;

                    for (auto&& sname : shader->srvNames())
                    {
                        TableRange::TableResource tableResource;
                        if (shader->hasTextureSrv(sname) ||
                            shader->hasBindlessTextureSrv(sname))
                        {
                            switch (shader->textureDimension(sname))
                            {
                            case ResourceDimension::Texture1D: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE1D; break; }
                            case ResourceDimension::Texture2D: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE2D; break; }
                            case ResourceDimension::Texture3D: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE3D; break; }
                            case ResourceDimension::Texture1DArray: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE1DARRAY; break; }
                            case ResourceDimension::Texture2DArray: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE2DARRAY; break; }
                            case ResourceDimension::TextureCubemap: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURECUBE; break; }
                            case ResourceDimension::TextureCubemapArray: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURECUBEARRAY; break; }
                            }
                        }
                        if (shader->hasBufferSrv(sname) ||
                            shader->hasBindlessBufferSrv(sname))
                        {
                            tableResource.dimension = D3D_SRV_DIMENSION_BUFFER;
                        }
                        tableResource.name = sname;
                        range->resourceNames.emplace_back(tableResource);
                    }
                }
            };
            auto addUAVs = [](std::vector<TableRange>& tableRange, const shaders::Shader* shader, D3D12_SHADER_VISIBILITY visibility)
            {
                auto uavCount = shader->uavNames().size();
                if (uavCount > 0)
                {
                    bool found = false;
                    TableRange* range = nullptr;
                    for (int i = 0; i < tableRange.size(); ++i)
                    {
                        if ((tableRange[i].range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_UAV) &&
                            (tableRange[i].visibility == visibility))
                        {
                            range = &tableRange[i];
                            range->range.NumDescriptors += uavCount;
                            break;
                        }
                    }
                    if (!range)
                    {
                        tableRange.emplace_back(TableRange{});
                        range = &tableRange.back();
                        range->range.NumDescriptors = uavCount;
                    }
                    range->range.BaseShaderRegister = 0;
                    range->range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    range->range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
                    range->range.RegisterSpace = 0;
                    range->range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
                    range->visibility = visibility;

                    for (auto&& sname : shader->uavNames())
                    {
                        TableRange::TableResource tableResource;
                        if (shader->hasTextureUav(sname) ||
                            shader->hasBindlessTextureUav(sname))
                        {
                            switch (shader->textureDimension(sname))
                            {
                            case ResourceDimension::Texture1D: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE1D; break; }
                            case ResourceDimension::Texture2D: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE2D; break; }
                            case ResourceDimension::Texture3D: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE3D; break; }
                            case ResourceDimension::Texture1DArray: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE1DARRAY; break; }
                            case ResourceDimension::Texture2DArray: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURE2DARRAY; break; }
                            case ResourceDimension::TextureCubemap: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURECUBE; break; }
                            case ResourceDimension::TextureCubemapArray: { tableResource.dimension = D3D_SRV_DIMENSION_TEXTURECUBEARRAY; break; }
                            }
                        }
                        if (shader->hasBufferUav(sname) ||
                            shader->hasBindlessBufferUav(sname))
                        {
                            tableResource.dimension = D3D_SRV_DIMENSION_BUFFER;
                        }
                        tableResource.name = sname;
                        range->resourceNames.emplace_back(tableResource);
                    }
                }
            };

            auto addConstants = [](std::vector<TableRange>& tableRange, shaders::Shader* shader, D3D12_SHADER_VISIBILITY visibility)
            {
                auto constantCount = shader->constants().size();
                if (constantCount > 0)
                {
                    tableRange.emplace_back(TableRange{});
                    TableRange& range = tableRange.back();
                    range.range.BaseShaderRegister = 0;
                    range.range.NumDescriptors = constantCount;
                    range.range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    range.range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
                    range.range.RegisterSpace = 0;
                    range.range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
                    range.visibility = visibility;

                    for (auto&& cons : shader->constants())
                    {
                        TableRange::TableResource tableResource;
                        tableResource.dimension = D3D_SRV_DIMENSION_UNKNOWN;
                        tableResource.name = cons.name;
                        range.resourceNames.emplace_back(tableResource);
                    }
                }
            };

            auto addResources = [
                /*addTextureSRVs, 
                addTextureUAVs, 
                addBufferSRVs, 
                addBufferUAVs, */
                addSRVs,
                addUAVs,
                addSamplers,
                addConstants]
                (std::vector<TableRange>& tableRange, const shaders::Shader* shader, D3D12_SHADER_VISIBILITY visibility)
            {
                addSamplers(tableRange, shader, visibility);

                addSRVs(tableRange, shader, visibility);
                addUAVs(tableRange, shader, visibility);
                /*addBufferSRVs(tableRange, shader, visibility);
                addTextureSRVs(tableRange, shader, visibility);

                addBufferUAVs(tableRange, shader, visibility);
                addTextureUAVs(tableRange, shader, visibility);*/
                
                addConstants(tableRange, const_cast<shaders::Shader*>(shader), visibility);
            };

            if (m_cs)
            {
                LOG("shader: %s", ShaderBinaryImplGet::impl(*m_cs).supportPath().c_str());
                addInputsToLayout(m_inputLayouts, configuration->computeShader()->inputParameters(), m_semanticNames, m_semanticIndex);
                addResources(m_range, configuration->computeShader(), D3D12_SHADER_VISIBILITY_ALL);
                /*generateBindings(
                    ShaderBinaryImplGet::impl(*m_cs).native().pShaderBytecode,
                    ShaderBinaryImplGet::impl(*m_cs).native().BytecodeLength,
                    D3D12_SHADER_VISIBILITY_ALL
                );*/
            }

            if (m_vs)
            {
                LOG("shader: %s", ShaderBinaryImplGet::impl(*m_vs).supportPath().c_str());
                addInputsToLayout(m_inputLayouts, configuration->vertexShader()->inputParameters(), m_semanticNames, m_semanticIndex);
                addResources(m_range, configuration->vertexShader(), D3D12_SHADER_VISIBILITY_VERTEX);
                /*generateBindings(
                    ShaderBinaryImplGet::impl(*m_vs).native().pShaderBytecode,
                    ShaderBinaryImplGet::impl(*m_vs).native().BytecodeLength,
                    D3D12_SHADER_VISIBILITY_VERTEX
                );*/
            }

            if (m_ps)
            {
                addInputsToLayout(m_inputLayouts, configuration->pixelShader()->inputParameters(), m_semanticNames, m_semanticIndex);
                addResources(m_range, configuration->pixelShader(), D3D12_SHADER_VISIBILITY_PIXEL);
                /*generateBindings(
                    ShaderBinaryImplGet::impl(*m_ps).native().pShaderBytecode,
                    ShaderBinaryImplGet::impl(*m_ps).native().BytecodeLength,
                    D3D12_SHADER_VISIBILITY_PIXEL
                );*/
            }

            if (m_gs)
            {
                addInputsToLayout(m_inputLayouts, configuration->geometryShader()->inputParameters(), m_semanticNames, m_semanticIndex);
                addResources(m_range, configuration->geometryShader(), D3D12_SHADER_VISIBILITY_GEOMETRY);
                /*generateBindings(
                    ShaderBinaryImplGet::impl(*m_gs).native().pShaderBytecode,
                    ShaderBinaryImplGet::impl(*m_gs).native().BytecodeLength,
                    D3D12_SHADER_VISIBILITY_GEOMETRY
                );*/
            }

            if (m_ds)
            {
                addInputsToLayout(m_inputLayouts, configuration->domainShader()->inputParameters(), m_semanticNames, m_semanticIndex);
                addResources(m_range, configuration->domainShader(), D3D12_SHADER_VISIBILITY_DOMAIN);
                /*generateBindings(
                    ShaderBinaryImplGet::impl(*m_ds).native().pShaderBytecode,
                    ShaderBinaryImplGet::impl(*m_ds).native().BytecodeLength,
                    D3D12_SHADER_VISIBILITY_DOMAIN
                );*/
            }

            if (m_hs)
            {
                addInputsToLayout(m_inputLayouts, configuration->hullShader()->inputParameters(), m_semanticNames, m_semanticIndex);
                addResources(m_range, configuration->hullShader(), D3D12_SHADER_VISIBILITY_HULL);
                /*generateBindings(
                    ShaderBinaryImplGet::impl(*m_hs).native().pShaderBytecode,
                    ShaderBinaryImplGet::impl(*m_hs).native().BytecodeLength,
                    D3D12_SHADER_VISIBILITY_HULL
                );*/
            }

#if 0
            customDebugMessageHandler = [](const std::string& msg)
            {
                if (!MyLog)
                {
                    MyLog = std::shared_ptr<DebugOutFile>(new DebugOutFile(), [](DebugOutFile* f) { f->file.close(); delete f; });
                    MyLog->file.open("C:\\work\\pipelog_my.txt", std::ios::out);
                    ASSERT(MyLog->file.is_open(), "could not open");
                }
                auto outmsg = msg;
                outmsg += '\n';
                MyLog->file.write(outmsg.data(), outmsg.size());
            };
            if (m_cs) { LOG("shader: %s", ShaderBinaryImplGet::impl(*m_cs).supportPath().c_str()); }
            if (m_vs) { LOG("shader: %s", ShaderBinaryImplGet::impl(*m_vs).supportPath().c_str()); }
            if (m_ps) { LOG("shader: %s", ShaderBinaryImplGet::impl(*m_ps).supportPath().c_str()); }
            if (m_gs) { LOG("shader: %s", ShaderBinaryImplGet::impl(*m_gs).supportPath().c_str()); }
            if (m_ds) { LOG("shader: %s", ShaderBinaryImplGet::impl(*m_ds).supportPath().c_str()); }
            if (m_hs) { LOG("shader: %s", ShaderBinaryImplGet::impl(*m_hs).supportPath().c_str()); }
            printRange(m_range);

            customDebugMessageHandler = [](const std::string& msg)
            {
                if (!RefLog)
                {
                    RefLog = std::shared_ptr<DebugOutFile>(new DebugOutFile(), [](DebugOutFile* f) { f->file.close(); delete f; });
                    RefLog->file.open("C:\\work\\pipelog_ref.txt", std::ios::out);
                    ASSERT(RefLog->file.is_open(), "could not open");
                }
                auto outmsg = msg;
                outmsg += '\n';
                RefLog->file.write(outmsg.data(), outmsg.size());
            };
            if (m_cs) { LOG("shader: %s", ShaderBinaryImplGet::impl(*m_cs).supportPath().c_str()); }
            if (m_vs) { LOG("shader: %s", ShaderBinaryImplGet::impl(*m_vs).supportPath().c_str()); }
            if (m_ps) { LOG("shader: %s", ShaderBinaryImplGet::impl(*m_ps).supportPath().c_str()); }
            if (m_gs) { LOG("shader: %s", ShaderBinaryImplGet::impl(*m_gs).supportPath().c_str()); }
            if (m_ds) { LOG("shader: %s", ShaderBinaryImplGet::impl(*m_ds).supportPath().c_str()); }
            if (m_hs) { LOG("shader: %s", ShaderBinaryImplGet::impl(*m_hs).supportPath().c_str()); }
            printRange(m_compareRange);

            customDebugMessageHandler = {};
#endif

            m_pipelineStateDesc->InputLayout.NumElements = static_cast<UINT>(m_inputLayouts.size());
            m_pipelineStateDesc->InputLayout.pInputElementDescs = m_inputLayouts.data();

            m_rootSignature.reset(new RootSignature(m_device, static_cast<int>(m_range.size())));
            RootSignature& rs = *m_rootSignature;

            for (int i = 0; i < m_range.size(); ++i)
            {
                RootParameterImplGet::impl(rs[i]).native().ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                RootParameterImplGet::impl(rs[i]).native().ShaderVisibility = m_range[i].visibility;
                RootParameterImplGet::impl(rs[i]).native().DescriptorTable.NumDescriptorRanges = 1;
                RootParameterImplGet::impl(rs[i]).native().DescriptorTable.pDescriptorRanges = &m_range[i].range;
            }

            

            // get descriptor count
            m_resourceCount = 0;
            m_samplerCount = 0;
            m_bindlessBindings.clear();
            for (int resourceRange = 0; resourceRange < m_range.size(); ++resourceRange)
            {
                auto range = m_range[resourceRange];
                for (int resourceName = 0; resourceName < range.resourceNames.size(); ++resourceName)
                {
                    if ((range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SRV) ||
                        (range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_UAV) ||
                        (range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_CBV))
                    {
                        auto tableResource = range.resourceNames[resourceName];

                        shaders::Shader* shader = nullptr;
                        switch (m_range[resourceRange].visibility)
                        {
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX: { shader = const_cast<shaders::Shader*>(configuration->vertexShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL: { shader = const_cast<shaders::Shader*>(configuration->pixelShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_HULL: { shader = const_cast<shaders::Shader*>(configuration->hullShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_DOMAIN: { shader = const_cast<shaders::Shader*>(configuration->domainShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_GEOMETRY: { shader = const_cast<shaders::Shader*>(configuration->geometryShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL: { shader = const_cast<shaders::Shader*>(configuration->computeShader()); break; }
                            default: ASSERT(false, "Could not match shader visibility");
                        }


                        if (range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SRV &&
                            tableResource.dimension != D3D_SRV_DIMENSION_BUFFER)
                        {
                            std::string resname = tableResource.name;
                            if (shader->hasBindlessTextureSrv(resname))
                            {
                                auto bindlessSize = shader->bindlessTextureSrv(resname).size();
                                m_bindlessBindings.emplace_back(BindlessBinding{ shader, true, true, resname, bindlessSize });
                                m_range[resourceRange].range.NumDescriptors = m_range[resourceRange].range.NumDescriptors - 1 + static_cast<UINT>(max(1, bindlessSize));
                                m_resourceCount += static_cast<uint32_t>(bindlessSize);
                            }
                            else
                                ++m_resourceCount;
                        }
                        else
                            ++m_resourceCount;
                    }
                    else if (range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
                        ++m_samplerCount;
                }
            }

            rs.finalize();

            if (!m_cs)
                m_pipelineStateDesc->pRootSignature = RootSignatureImplGet::impl(rs).native();
            else
                m_computePipelineStateDesc->pRootSignature = RootSignatureImplGet::impl(rs).native();

            m_descriptorResourceTablesInvalid = true;
            m_descriptorSamplerTablesInvalid = true;
        }

        void PipelineImpl::createResourceBindingTables()
        {
            DeviceImpl& device = DeviceImplGet::impl(m_device);

            if (m_resourceCount > 0)
            {
                m_resourceDescriptors = device.heaps().shaderVisible_cbv_srv_uav->getDescriptor(m_resourceCount);
            }
            else
            {
                m_resourceDescriptors = {};
            }
        }

        void PipelineImpl::createSamplerBindingTables()
        {
            DeviceImpl& device = DeviceImplGet::impl(m_device);

            if (m_samplerCount > 0)
                m_samplerDescriptors = device.heaps().shaderVisible_sampler->getDescriptor(m_samplerCount);
            else
                m_samplerDescriptors = {};
        }

        static uint64_t lastDebugFrame = 0;
        static std::map<const void*, std::vector<std::string>> pipes;

        std::pair<bool, uint64_t> PipelineImpl::createPipelineResourceHash(size_t resourceCount) const
        {
            if (m_resourceDescriptorMap.size() > 0)
            {
                std::vector<uint64_t> pipelineHashData(m_resourceDescriptorMap.size() + 1 + m_range.size());
                uint64_t* ptr = &pipelineHashData[0];
                for (auto&& res : m_resourceDescriptorMap)
                {
                    *ptr = res.currentHandle();
                    ++ptr;
                }
                *ptr = resourceCount; ++ptr;
                for (auto&& range : m_range)
                {
                    *ptr = static_cast<uint64_t>(range.range.RangeType);
                    ++ptr;
                }

                std::pair<bool, uint64_t> res = { true, static_cast<uint64_t>(tools::hash(reinterpret_cast<uint8_t*>(pipelineHashData.data()), static_cast<unsigned int>(pipelineHashData.size() * sizeof(uint64_t)))) };
                
                return res;
            }
            return { false, 0 };
        }

        std::pair<bool, uint64_t> PipelineImpl::createPipelineSamplerHash() const
        {
            if (m_samplerDescriptorMap.size() > 0)
            {
                std::vector<uint64_t> pipelineHashData(m_samplerDescriptorMap.size());
                uint64_t* ptr = &pipelineHashData[0];
                for (auto&& res : m_samplerDescriptorMap)
                {
                    *ptr = res.currentHandle();
                    ++ptr;
                }
                std::pair<bool, uint64_t> res = { true, static_cast<uint64_t>(tools::hash(reinterpret_cast<uint8_t*>(pipelineHashData.data()), static_cast<unsigned int>(pipelineHashData.size() * sizeof(uint64_t)))) };

                return res;
            }
            return { false, 0 };
        }

#undef BINDING_DEBUG

        void PipelineImpl::configure(CommandListImpl& commandList, shaders::PipelineConfiguration* configuration)
        {
            finalize(configuration);

            DeviceImpl& device = DeviceImplGet::impl(m_device);

            auto currentFrame = m_device.frameNumber();
            if (!m_pipelineCreated)
            {
                if (!configuration->hasComputeShader())
                {
                    auto res = device.device()->CreateGraphicsPipelineState(
                        m_pipelineStateDesc.get(),
                        DARKNESS_IID_PPV_ARGS(m_pipelineState.GetAddressOf()));
                    ASSERT(SUCCEEDED(res));
                }
                else
                {
                    auto res = device.device()->CreateComputePipelineState(
                        m_computePipelineStateDesc.get(),
                        DARKNESS_IID_PPV_ARGS(m_pipelineState.GetAddressOf()));
                    ASSERT(SUCCEEDED(res));
                }
                m_pipelineCreated = true;
            }

            for (auto&& bindlessCheck : m_bindlessBindings)
            {
                bool change = false;
                if (bindlessCheck.srv && bindlessCheck.texture)
                {
                    auto newSize = bindlessCheck.shader->bindlessTextureSrv(bindlessCheck.name).size();
                    change = newSize != bindlessCheck.lastSize;
                    if (change)
                        bindlessCheck.lastSize = newSize;
                }
                if (change)
                {
                    this->generateBindings(configuration);
                    m_pipelineState.Reset();
                    auto res = DeviceImplGet::impl(this->m_device).device()->CreateGraphicsPipelineState(
                        this->m_pipelineStateDesc.get(),
                        DARKNESS_IID_PPV_ARGS(this->m_pipelineState.GetAddressOf()));
                    ASSERT(SUCCEEDED(res));
                }
            }

            D3D12_CPU_DESCRIPTOR_HANDLE resourceCpuDescriptor = {};
            D3D12_GPU_DESCRIPTOR_HANDLE resourceGpuDescriptor = {};
            D3D12_CPU_DESCRIPTOR_HANDLE samplerCpuDescriptor = {};
            D3D12_GPU_DESCRIPTOR_HANDLE samplerGpuDescriptor = {};
            size_t resourceDescriptorCount = {};
            size_t resourceDescriptorHandleSize = {};
            size_t samplerDescriptorCount = {};
            size_t samplerDescriptorHandleSize = {};

            //LOG("Pipeline [%p]: LOOKING FOR: %" PRIu64 "", static_cast<const void*>(this), pipelineHash.second);

#if 1
            //LOG_INFO("PIPELINE HASH START");
            auto pipelineResourceHash = createPipelineResourceHash(m_resourceCount);
            auto pipelineSamplerHash = createPipelineSamplerHash();
            
            bool postGenerateResourcePipelineHash = false;
            if (pipelineResourceHash.first)
            {
                auto storedPipeline = m_hashResourceStorage.find(pipelineResourceHash.second);
                if (storedPipeline != m_hashResourceStorage.end())
                {
                    // existing pipeline was found
                    // taking into use
                    resourceDescriptorCount = storedPipeline->second.resourceDescriptors.count();
                    if (resourceDescriptorCount > 0)
                    {
                        resourceCpuDescriptor = storedPipeline->second.resourceDescriptors.cpuHandle();
                        resourceGpuDescriptor = storedPipeline->second.resourceDescriptors.gpuHandle();
                        resourceDescriptorHandleSize = storedPipeline->second.resourceDescriptors.handleSize();
                    }
                    m_resourceRanges = storedPipeline->second.ranges;

                    storedPipeline->second.lastUsedFrame = currentFrame;

                    //LOG("Pipeline [%p]: USING: %" PRIu64 "", static_cast<const void*>(this), pipelineHash.second);
                }
                else
                {
                    //if (m_ps) LOG("Changed pipeline: %s", ShaderBinaryImplGet::impl(*m_ps).supportPath().c_str());
                    //if (m_cs) LOG("Changed pipeline: %s", ShaderBinaryImplGet::impl(*m_cs).supportPath().c_str());
                    // something has changed in the pipeline configuration
                    // creating a new table and post hash
                    createResourceBindingTables();

                    resourceDescriptorCount = m_resourceDescriptors.count();
                    if (resourceDescriptorCount > 0)
                    {
                        resourceCpuDescriptor = m_resourceDescriptors.cpuHandle();
                        resourceGpuDescriptor = m_resourceDescriptors.gpuHandle();
                        resourceDescriptorHandleSize = m_resourceDescriptors.handleSize();
                    }

                    postGenerateResourcePipelineHash = true;
                    m_descriptorResourceTablesInvalid = true;
                }
            }
            else
            {
                // could not create hash at all. usually first run for this pipeline
                // we generate the hash after the table has been created
                createResourceBindingTables();

                resourceDescriptorCount = m_resourceDescriptors.count();
                if (resourceDescriptorCount)
                {
                    resourceCpuDescriptor = m_resourceDescriptors.cpuHandle();
                    resourceGpuDescriptor = m_resourceDescriptors.gpuHandle();
                    resourceDescriptorHandleSize = m_resourceDescriptors.handleSize();
                }

                postGenerateResourcePipelineHash = true;
                m_descriptorResourceTablesInvalid = true;
            }

            bool postGenerateSamplerPipelineHash = false;
            if (m_samplerCount > 0)
            {
                if (pipelineSamplerHash.first)
                {
                    auto storedPipeline = m_hashSamplerStorage.find(pipelineSamplerHash.second);
                    if (storedPipeline != m_hashSamplerStorage.end())
                    {
                        // existing pipeline was found
                        // taking into use

                        samplerDescriptorCount = storedPipeline->second.samplerDescriptors.count();
                        if (samplerDescriptorCount > 0)
                        {
                            samplerCpuDescriptor = storedPipeline->second.samplerDescriptors.cpuHandle();
                            samplerGpuDescriptor = storedPipeline->second.samplerDescriptors.gpuHandle();
                            samplerDescriptorHandleSize = storedPipeline->second.samplerDescriptors.handleSize();
                        }

                        storedPipeline->second.lastUsedFrame = currentFrame;

                        //LOG("Pipeline [%p]: USING: %" PRIu64 "", static_cast<const void*>(this), pipelineHash.second);
                    }
                    else
                    {
                        // something has changed in the pipeline configuration
                        // creating a new table and post hash
                        createSamplerBindingTables();

                        samplerDescriptorCount = m_samplerDescriptors.count();
                        if (samplerDescriptorCount > 0)
                        {
                            samplerCpuDescriptor = m_samplerDescriptors.cpuHandle();
                            samplerGpuDescriptor = m_samplerDescriptors.gpuHandle();
                            samplerDescriptorHandleSize = m_samplerDescriptors.handleSize();
                        }

                        postGenerateSamplerPipelineHash = true;
                        m_descriptorSamplerTablesInvalid = true;
                    }
                }
                else
                {
                    // could not create hash at all. usually first run for this pipeline
                    // we generate the hash after the table has been created
                    createSamplerBindingTables();

                    samplerDescriptorCount = m_samplerDescriptors.count();
                    if (samplerDescriptorCount)
                    {
                        samplerCpuDescriptor = m_samplerDescriptors.cpuHandle();
                        samplerGpuDescriptor = m_samplerDescriptors.gpuHandle();
                        samplerDescriptorHandleSize = m_samplerDescriptors.handleSize();
                    }

                    postGenerateSamplerPipelineHash = true;
                    m_descriptorSamplerTablesInvalid = true;
                }
            }
#else
            createBindingTables();

            resourceDescriptorCount = m_resourceDescriptors.count();
            if (resourceDescriptorCount > 0)
            {
                resourceCpuDescriptor = m_resourceDescriptors.cpuHandle();
                resourceGpuDescriptor = m_resourceDescriptors.gpuHandle();
                resourceDescriptorHandleSize = m_resourceDescriptors.handleSize();
            }

            samplerDescriptorCount = m_samplerDescriptors.count();
            if (samplerDescriptorCount > 0)
            {
                samplerCpuDescriptor = m_samplerDescriptors.cpuHandle();
                samplerGpuDescriptor = m_samplerDescriptors.gpuHandle();
                samplerDescriptorHandleSize = m_samplerDescriptors.handleSize();
            }
#endif

            if (m_descriptorResourceTablesInvalid)
            {
                m_descriptorResourceTablesInvalid = false;
            
                m_resourceDescriptorMap.clear();
                m_constantBufferUpdates.clear();
                m_resourceRanges.clear();

                if (resourceDescriptorCount > 0)
                {
                    for (int resourceRange = 0; resourceRange < m_range.size(); ++resourceRange)
                    {
                        ResourceRangeInfo rangeInfo;
                        rangeInfo.resourceGpuHeapStart = resourceGpuDescriptor;
                        rangeInfo.srvUavCbvSet = false;
                        rangeInfo.resourceRange = resourceRange;

                        auto range = m_range[resourceRange];
                        for (int resourceName = 0; resourceName < range.resourceNames.size(); ++resourceName)
                        {
                            auto tableResource = range.resourceNames[resourceName];
                            bool validResource = false;

                            shaders::Shader* shader = nullptr;
                            switch (m_range[resourceRange].visibility)
                            {
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX: { shader = const_cast<shaders::Shader*>(configuration->vertexShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL: { shader = const_cast<shaders::Shader*>(configuration->pixelShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_HULL: { shader = const_cast<shaders::Shader*>(configuration->hullShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_DOMAIN: { shader = const_cast<shaders::Shader*>(configuration->domainShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_GEOMETRY: { shader = const_cast<shaders::Shader*>(configuration->geometryShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL: { shader = const_cast<shaders::Shader*>(configuration->computeShader()); break; }
                            default: ASSERT(false, "Could not match shader visibility");
                            }

                            if (resourceDescriptorCount > 0)
                            {
                                if (!validResource &&
                                    range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SRV &&
                                    tableResource.dimension != D3D_SRV_DIMENSION_BUFFER)
                                {
                                    std::string resname = tableResource.name;
                                    if (shader->hasTextureSrv(resname))
                                    {
                                        auto& resource = shader->textureSrv(resname);
                                        validResource = resource.valid();
                                        if (validResource)
                                        {
                                            // found it
                                            //LOG_INFO("found resource: %s", name.data());
                                            device.device()->CopyDescriptorsSimple(
                                                1,
                                                resourceCpuDescriptor,
                                                TextureSRVImplGet::impl(resource)->native(),
                                                D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

#ifdef BINDING_DEBUG
                                            auto& heap = *device.heaps().shaderVisible_cbv_srv_uav;
                                            auto start = heap.getCpuHeapStart();
                                            auto diff = resourceCpuDescriptor.ptr - start.ptr;
                                            auto ind = diff / resourceDescriptorHandleSize;
                                            LOG("DescriptorIndex[%u]: TEXTURE SRV: %s",
                                                ind,
                                                resname.c_str());
#endif

                                            m_resourceDescriptorMap.emplace_back(ResourceMap{
                                                resourceCpuDescriptor.ptr,
                                                TextureSRVImplGet::impl(resource)->native(),
                                                [shader, resname]()->uint64_t
                                                {
                                                    auto res = shader->textureSrv(resname);
                                                    uint64_t id = 0;
                                                    if (res.valid())
                                                        id = TextureSRVImplGet::impl(res)->uniqueId();

                                                    //LOG("PIPELINE RESOURCE TEXTURE SRV (%s): %u", resname.c_str(), id);
                                                    return id;
                                                } });

                                            //LOG("setting TextureSrv: %s", resname.c_str());

                                            resourceCpuDescriptor.ptr += resourceDescriptorHandleSize;
                                            resourceGpuDescriptor.ptr += resourceDescriptorHandleSize;
                                            rangeInfo.srvUavCbvSet = true;
                                            rangeInfo.type = RangeInfoType::SRV;
                                        }
                                    }
                                    else if (shader->hasBindlessTextureSrv(resname))
                                    {
                                        auto& resource = shader->bindlessTextureSrv(resname);
                                        validResource = true;
                                        for (int i = 0; i < resource.size(); ++i)
                                        {
                                            // found it
                                            //LOG_INFO("found resource: %s", name.data());
                                            device.device()->CopyDescriptorsSimple(
                                                1,
                                                resourceCpuDescriptor,
                                                TextureSRVImplGet::impl(resource[i])->native(),
                                                D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

#ifdef BINDING_DEBUG
                                            auto& heap = *device.heaps().shaderVisible_cbv_srv_uav;
                                            auto start = heap.getCpuHeapStart();
                                            auto diff = resourceCpuDescriptor.ptr - start.ptr;
                                            auto ind = diff / resourceDescriptorHandleSize;
                                            LOG("DescriptorIndex[%u]: BINDLESS TEXTURE SRV: %s",
                                                ind,
                                                resname.c_str());
#endif

                                            m_resourceDescriptorMap.emplace_back(ResourceMap{
                                                resourceCpuDescriptor.ptr,
                                                TextureSRVImplGet::impl(resource[i])->native(),
                                                [shader, resname]()->uint64_t
                                            {
                                                auto res = shader->bindlessTextureSrv(resname);
                                                uint64_t id = 0;
                                                std::vector<uint64_t> uids;
                                                for (int a = 0; a < res.size(); ++a)
                                                {
                                                    uids.emplace_back(TextureSRVImplGet::impl(res[a])->uniqueId());
                                                }
                                                id = static_cast<uint64_t>(tools::hash(reinterpret_cast<uint8_t*>(uids.data()), static_cast<unsigned int>(uids.size() * sizeof(uint64_t))));
                                                //LOG("PIPELINE RESOURCE BINDLESS TEXTURE SRV (%s): %u", resname.c_str(), id);
                                                return id;
                                            } });

                                            resourceCpuDescriptor.ptr += resourceDescriptorHandleSize;
                                            resourceGpuDescriptor.ptr += resourceDescriptorHandleSize;
                                            rangeInfo.srvUavCbvSet = true;
                                            rangeInfo.type = RangeInfoType::SRV;
                                        }
                                    }
                                    else
                                        ASSERT(false, "Unbound resource");
                                }
                                if (!validResource &&
                                    range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_UAV &&
                                    tableResource.dimension != D3D_SRV_DIMENSION_BUFFER)
                                {
                                    std::string resname = tableResource.name;
                                    auto& resource = shader->textureUav(resname);
                                    validResource = resource.valid();
                                    if (validResource)
                                    {
                                        // found it
                                        //LOG_INFO("found resource: %s", name.data());

                                        device.device()->CopyDescriptorsSimple(
                                            1,
                                            resourceCpuDescriptor,
                                            TextureUAVImplGet::impl(resource)->native(),
                                            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

#ifdef BINDING_DEBUG
                                        auto& heap = *device.heaps().shaderVisible_cbv_srv_uav;
                                        auto start = heap.getCpuHeapStart();
                                        auto diff = resourceCpuDescriptor.ptr - start.ptr;
                                        auto ind = diff / resourceDescriptorHandleSize;
                                        LOG("DescriptorIndex[%u]: TEXTURE UAV: %s",
                                            ind,
                                            resname.c_str());
#endif

                                        m_resourceDescriptorMap.emplace_back(ResourceMap{
                                            resourceCpuDescriptor.ptr,
                                            TextureUAVImplGet::impl(resource)->native(),
                                            [shader, resname]()->uint64_t
                                            {
                                                auto res = shader->textureUav(resname);
                                                uint64_t id = 0;
                                                if (res.valid())
                                                    id = TextureUAVImplGet::impl(res)->uniqueId();
                                                //LOG("PIPELINE RESOURCE BINDLESS TEXTURE UAV (%s): %u", resname.c_str(), id);
                                                return id;
                                            } });

                                        //LOG("setting TextureUav: %s", resname.c_str());

                                        resourceCpuDescriptor.ptr += resourceDescriptorHandleSize;
                                        resourceGpuDescriptor.ptr += resourceDescriptorHandleSize;
                                        rangeInfo.srvUavCbvSet = true;
                                        rangeInfo.type = RangeInfoType::UAV;
                                    }
                                }
                                if (!validResource &&
                                    range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SRV &&
                                    tableResource.dimension == D3D_SRV_DIMENSION_BUFFER)
                                {
                                    std::string resname = tableResource.name;
                                    auto& resource = shader->bufferSrv(resname);
                                    validResource = resource.valid();
                                    if (validResource)
                                    {
                                        // found it
                                        //LOG_INFO("found resource: %s", name.data());

                                        device.device()->CopyDescriptorsSimple(
                                            1,
                                            resourceCpuDescriptor,
                                            BufferSRVImplGet::impl(resource)->native(),
                                            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

#ifdef BINDING_DEBUG
                                        auto& heap = *device.heaps().shaderVisible_cbv_srv_uav;
                                        auto start = heap.getCpuHeapStart();
                                        auto diff = resourceCpuDescriptor.ptr - start.ptr;
                                        auto ind = diff / resourceDescriptorHandleSize;
                                        LOG("DescriptorIndex[%u]: BUFFER SRV: %s",
                                            ind,
                                            resname.c_str());
#endif

                                        m_resourceDescriptorMap.emplace_back(ResourceMap{
                                            resourceCpuDescriptor.ptr,
                                            BufferSRVImplGet::impl(resource)->native(),
                                            [shader, resname]()->uint64_t
                                            {
                                                auto res = shader->bufferSrv(resname);
                                                uint64_t id = 0;
                                                if (res.valid())
                                                    id = BufferSRVImplGet::impl(res)->uniqueId();
                                                //LOG("PIPELINE RESOURCE BUFFER SRV (%s): %u", resname.c_str(), id);
                                                return id;
                                            } });

                                        //LOG("setting BufferSrv: %s", resname.c_str());

                                        resourceCpuDescriptor.ptr += resourceDescriptorHandleSize;
                                        resourceGpuDescriptor.ptr += resourceDescriptorHandleSize;
                                        rangeInfo.srvUavCbvSet = true;
                                        rangeInfo.type = RangeInfoType::SRV;
                                    }
                                }
                                if (!validResource &&
                                    range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_UAV &&
                                    tableResource.dimension == D3D_SRV_DIMENSION_BUFFER)
                                {
                                    std::string resname = tableResource.name;
                                    auto& resource = shader->bufferUav(resname);
                                    validResource = resource.valid();
                                    if (validResource)
                                    {
                                        // found it
                                        //LOG_INFO("found resource: %s", name.data());

                                        device.device()->CopyDescriptorsSimple(
                                            1,
                                            resourceCpuDescriptor,
                                            BufferUAVImplGet::impl(resource)->native(),
                                            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

#ifdef BINDING_DEBUG
                                        auto& heap = *device.heaps().shaderVisible_cbv_srv_uav;
                                        auto start = heap.getCpuHeapStart();
                                        auto diff = resourceCpuDescriptor.ptr - start.ptr;
                                        auto ind = diff / resourceDescriptorHandleSize;
                                        LOG("DescriptorIndex[%u]: BUFFER UAV: %s",
                                            ind,
                                            resname.c_str());
#endif

                                        m_resourceDescriptorMap.emplace_back(ResourceMap{
                                            resourceCpuDescriptor.ptr,
                                            BufferUAVImplGet::impl(resource)->native(),
                                            [shader, resname]()->uint64_t
                                            {
                                                auto res = shader->bufferUav(resname);
                                                uint64_t id = 0;
                                                if (res.valid())
                                                    id = BufferUAVImplGet::impl(res)->uniqueId();
                                                //LOG("PIPELINE RESOURCE BUFFER UAV (%s): %u", resname.c_str(), id);
                                                return id;
                                            } });

                                        //LOG("setting BufferUav: %s", resname.c_str());

                                        resourceCpuDescriptor.ptr += resourceDescriptorHandleSize;
                                        resourceGpuDescriptor.ptr += resourceDescriptorHandleSize;
                                        rangeInfo.srvUavCbvSet = true;
                                        rangeInfo.type = RangeInfoType::UAV;
                                    }
                                }
                                if (!validResource && range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_CBV)
                                {
                                    auto& constantranges = shader->constants();
                                    for (auto&& constantrange : constantranges)
                                    {
                                        if (constantrange.name == tableResource.name)
                                        {
                                            if (!constantrange.buffer)
                                            {
                                                auto initial = BufferDescription::InitialData(
                                                    constantrange.range,
                                                    D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);

                                                Buffer cbvBuffer(std::make_shared<BufferImpl>(
                                                    device,
                                                    BufferDescription()
                                                    .name(constantrange.name)
                                                    .usage(ResourceUsage::GpuRead)
                                                    .elementSize(initial.elementSize)
                                                    .elements(initial.elements)
                                                    ));

#ifdef BINDING_DEBUG
                                                auto& heap = *device.heaps().shaderVisible_cbv_srv_uav;
                                                auto start = heap.getCpuHeapStart();
                                                auto diff = resourceCpuDescriptor.ptr - start.ptr;
                                                auto ind = diff / resourceDescriptorHandleSize;
                                                LOG("DescriptorIndex[%u]: CONSTANTS: %s",
                                                    ind,
                                                    constantrange.name);
#endif

                                                // create a CBV for the buffer
                                                std::shared_ptr<BufferCBV> cbv = std::make_shared<BufferCBV>(
                                                    std::make_shared<BufferCBVImpl>(
                                                        device,
                                                        cbvBuffer,
                                                        BufferDescription()
                                                        .elements(initial.elements)
                                                        .elementSize(initial.elementSize)
                                                        .format(Format::R32_UINT)));

                                                constantrange.buffer = cbv;
                                            }
                                            device.uploadBuffer(commandList, *constantrange.buffer, constantrange.range, 0);
                                            commandList.transition(*constantrange.buffer, ResourceState::VertexAndConstantBuffer);

                                            device.device()->CopyDescriptorsSimple(
                                                1,
                                                resourceCpuDescriptor,
                                                BufferCBVImplGet::impl(*constantrange.buffer)->native(),
                                                D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

                                            m_constantBufferUpdates.emplace_back(ConstantBufferUpdates{ constantrange.buffer, constantrange.range });

                                            resourceCpuDescriptor.ptr += resourceDescriptorHandleSize;
                                            resourceGpuDescriptor.ptr += resourceDescriptorHandleSize;
                                            //resourceFound = true;
                                            validResource = true;
                                            rangeInfo.srvUavCbvSet = true;
                                            rangeInfo.type = RangeInfoType::CONSTANT;
                                        }
                                    }
                                }
                            }

                            if (!validResource)
                            {
                                // falling back to null resources
                                std::string resname = tableResource.name;

                                if (!validResource &&
                                    range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SRV &&
                                    tableResource.dimension != D3D_SRV_DIMENSION_BUFFER)
                                {
                                    TextureSRV* srv = nullptr;
                                    if (tableResource.dimension == D3D_SRV_DIMENSION_TEXTURECUBE)
                                        srv = &device.nullResources().textureCubeSRV;
                                    else
                                        srv = &device.nullResources().textureSRV;

                                    device.device()->CopyDescriptorsSimple(
                                        1,
                                        resourceCpuDescriptor,
                                        TextureSRVImplGet::impl(*srv)->native(),
                                        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

#ifdef BINDING_DEBUG
                                    auto& heap = *device.heaps().shaderVisible_cbv_srv_uav;
                                    auto start = heap.getCpuHeapStart();
                                    auto diff = resourceCpuDescriptor.ptr - start.ptr;
                                    auto ind = diff / resourceDescriptorHandleSize;
                                    LOG("DescriptorIndex[%u]: NULL TEXTURE SRV: %s",
                                        ind,
                                        resname.c_str());
#endif

                                    m_resourceDescriptorMap.emplace_back(ResourceMap{
                                        resourceCpuDescriptor.ptr,
                                        TextureSRVImplGet::impl(*srv)->native(),
                                        [shader, resname]()->uint64_t
                                    {
                                        const TextureSRV& srv = shader->textureSrv(resname);
                                        uint64_t id = 0;
                                        if (srv.valid())
                                            id = TextureSRVImplGet::impl(srv)->uniqueId();
                                        //LOG("PIPELINE RESOURCE NULL TEXTURE SRV (%s): %u", resname.c_str(), id);
                                        return id;
                                    } });

                                    resourceCpuDescriptor.ptr += resourceDescriptorHandleSize;
                                    resourceGpuDescriptor.ptr += resourceDescriptorHandleSize;
                                    rangeInfo.srvUavCbvSet = true;
                                    rangeInfo.type = RangeInfoType::SRV;

                                    //LOG_INFO("Using null texture srv resource for: %s", tableResource.name.c_str());
                                }
                                else if (!validResource &&
                                    range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_UAV &&
                                    tableResource.dimension != D3D_SRV_DIMENSION_BUFFER)
                                {
                                    auto& uav = device.nullResources().textureUAV;
                                    device.device()->CopyDescriptorsSimple(
                                        1,
                                        resourceCpuDescriptor,
                                        TextureUAVImplGet::impl(uav)->native(),
                                        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

#ifdef BINDING_DEBUG
                                    auto& heap = *device.heaps().shaderVisible_cbv_srv_uav;
                                    auto start = heap.getCpuHeapStart();
                                    auto diff = resourceCpuDescriptor.ptr - start.ptr;
                                    auto ind = diff / resourceDescriptorHandleSize;
                                    LOG("DescriptorIndex[%u]: NULL TEXTURE UAV: %s",
                                        ind,
                                        resname.c_str());
#endif

                                    m_resourceDescriptorMap.emplace_back(ResourceMap{
                                        resourceCpuDescriptor.ptr,
                                        TextureUAVImplGet::impl(uav)->native(),
                                        [shader, resname]()->uint64_t
                                    {
                                        const TextureUAV& uav = shader->textureUav(resname);
                                        uint64_t id = 0;
                                        if (uav.valid())
                                            id = TextureUAVImplGet::impl(uav)->uniqueId();
                                        //LOG("PIPELINE RESOURCE NULL TEXTURE UAV (%s): %u", resname.c_str(), id);
                                        return id;
                                    } });

                                    resourceCpuDescriptor.ptr += resourceDescriptorHandleSize;
                                    resourceGpuDescriptor.ptr += resourceDescriptorHandleSize;
                                    rangeInfo.srvUavCbvSet = true;
                                    rangeInfo.type = RangeInfoType::UAV;

                                    //LOG_INFO("Using null texture uav resource for: %s", tableResource.name.c_str());
                                }
                                else if (!validResource &&
                                    range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SRV &&
                                    tableResource.dimension == D3D_SRV_DIMENSION_BUFFER)
                                {
                                    auto& srv = device.nullResources().bufferSRV;
                                    device.device()->CopyDescriptorsSimple(
                                        1,
                                        resourceCpuDescriptor,
                                        BufferSRVImplGet::impl(srv)->native(),
                                        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

#ifdef BINDING_DEBUG
                                    auto& heap = *device.heaps().shaderVisible_cbv_srv_uav;
                                    auto start = heap.getCpuHeapStart();
                                    auto diff = resourceCpuDescriptor.ptr - start.ptr;
                                    auto ind = diff / resourceDescriptorHandleSize;
                                    LOG("DescriptorIndex[%u]: NULL BUFFER SRV: %s",
                                        ind,
                                        resname.c_str());
#endif

                                    m_resourceDescriptorMap.emplace_back(ResourceMap{
                                        resourceCpuDescriptor.ptr,
                                        BufferSRVImplGet::impl(srv)->native(),
                                        [shader, resname]()->uint64_t
                                    {
                                        const BufferSRV& srv = shader->bufferSrv(resname);
                                        uint64_t id = 0;
                                        if (srv.valid())
                                            id = BufferSRVImplGet::impl(srv)->uniqueId();
                                        //LOG("PIPELINE RESOURCE NULL BUFFER SRV (%s): %u", resname.c_str(), id);
                                        return id;
                                    } });

                                    resourceCpuDescriptor.ptr += resourceDescriptorHandleSize;
                                    resourceGpuDescriptor.ptr += resourceDescriptorHandleSize;
                                    rangeInfo.srvUavCbvSet = true;
                                    rangeInfo.type = RangeInfoType::SRV;
                                    //LOG_INFO("Using null buffer srv resource for: %s", tableResource.name.c_str());
                                }
                                else if (!validResource &&
                                    range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_UAV &&
                                    tableResource.dimension == D3D_SRV_DIMENSION_BUFFER)
                                {
                                    auto& uav = device.nullResources().bufferUAV;
                                    device.device()->CopyDescriptorsSimple(
                                        1,
                                        resourceCpuDescriptor,
                                        BufferUAVImplGet::impl(uav)->native(),
                                        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

#ifdef BINDING_DEBUG
                                    auto& heap = *device.heaps().shaderVisible_cbv_srv_uav;
                                    auto start = heap.getCpuHeapStart();
                                    auto diff = resourceCpuDescriptor.ptr - start.ptr;
                                    auto ind = diff / resourceDescriptorHandleSize;
                                    LOG("DescriptorIndex[%u]: NULL BUFFER UAV: %s",
                                        ind,
                                        resname.c_str());
#endif

                                    m_resourceDescriptorMap.emplace_back(ResourceMap{
                                        resourceCpuDescriptor.ptr,
                                        BufferUAVImplGet::impl(uav)->native(),
                                        [shader, resname]()->uint64_t
                                    {
                                        const BufferUAV& uav = shader->bufferUav(resname);
                                        uint64_t id = 0;
                                        if (uav.valid())
                                            id = BufferUAVImplGet::impl(uav)->uniqueId();
                                        //LOG("PIPELINE RESOURCE NULL BUFFER UAV (%s): %u", resname.c_str(), id);
                                        return id;
                                    } });

                                    resourceCpuDescriptor.ptr += resourceDescriptorHandleSize;
                                    resourceGpuDescriptor.ptr += resourceDescriptorHandleSize;
                                    rangeInfo.srvUavCbvSet = true;
                                    rangeInfo.type = RangeInfoType::UAV;
                                    //LOG_INFO("Using null buffer uav resource for: %s", tableResource.name.c_str());
                                }
                                else if (!validResource && range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_CBV)
                                {
                                    LOG_ERROR("Unset resource: %s", tableResource.name.c_str());
                                    rangeInfo.type = RangeInfoType::CONSTANT;
                                }
                            }
                        }

                        m_resourceRanges.emplace_back(rangeInfo);
                    }
                }
            }
#if 1
            else
            {
                for (auto&& update : m_constantBufferUpdates)
                {
                    device.uploadBuffer(commandList, *update.buffer, update.range, 0);
                    commandList.transition(*update.buffer, ResourceState::VertexAndConstantBuffer);
                }
            }

            if (m_samplerCount > 0 && m_descriptorSamplerTablesInvalid)
            {
                m_descriptorSamplerTablesInvalid = false;

                m_samplerDescriptorMap.clear();
                m_samplerRanges.clear();

                if (samplerDescriptorCount > 0)
                {
                    for (int resourceRange = 0; resourceRange < m_range.size(); ++resourceRange)
                    {
                        SamplerRangeInfo rangeInfo;
                        rangeInfo.samplerGpuHeapStart = samplerGpuDescriptor;
                        rangeInfo.samplerSet = false;
                        rangeInfo.resourceRange = resourceRange;

                        auto range = m_range[resourceRange];
                        for (int resourceName = 0; resourceName < range.resourceNames.size(); ++resourceName)
                        {
                            auto tableResource = range.resourceNames[resourceName];
                            bool validResource = false;

                            shaders::Shader* shader = nullptr;
                            switch (m_range[resourceRange].visibility)
                            {
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX: { shader = const_cast<shaders::Shader*>(configuration->vertexShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL: { shader = const_cast<shaders::Shader*>(configuration->pixelShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_HULL: { shader = const_cast<shaders::Shader*>(configuration->hullShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_DOMAIN: { shader = const_cast<shaders::Shader*>(configuration->domainShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_GEOMETRY: { shader = const_cast<shaders::Shader*>(configuration->geometryShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL: { shader = const_cast<shaders::Shader*>(configuration->computeShader()); break; }
                            default: ASSERT(false, "Could not match shader visibility");
                            }

                            if (samplerDescriptorCount)
                            {
                                if (!validResource && range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
                                {
                                    std::string resname = tableResource.name;
                                    auto& resource = shader->sampler(resname);
                                    validResource = resource.valid();
                                    if (validResource)
                                    {
                                        device.device()->CopyDescriptorsSimple(
                                            1,
                                            samplerCpuDescriptor,
                                            SamplerImplGet::impl(resource)->native(),
                                            D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

                                        /*auto& samplerHeap = *device.heaps().shaderVisible_sampler;
                                        auto start = samplerHeap.getCpuHeapStart();
                                        auto diff = samplerCpuDescriptor.ptr - start.ptr;
                                        auto ind = diff / samplerDescriptorHandleSize;
                                        LOG("DescriptorIndex[%u]: SAMPLER: %s", 
                                            ind,
                                            resname.c_str());*/

                                        m_samplerDescriptorMap.emplace_back(ResourceMap{
                                            samplerCpuDescriptor.ptr,
                                            SamplerImplGet::impl(resource)->native(),
                                            [shader, resname]()->uint64_t
                                        {
                                            auto res = shader->sampler(resname);
                                            uint64_t id = 0;
                                            if (res.valid())
                                                id = SamplerImplGet::impl(res)->uniqueId();
                                            //LOG("PIPELINE RESOURCE SAMPLER (%s): %u", resname.c_str(), id);
                                            return id;
                                        } });

                                        //LOG("sampler: %s", resname.c_str());

                                        samplerCpuDescriptor.ptr += samplerDescriptorHandleSize;
                                        samplerGpuDescriptor.ptr += samplerDescriptorHandleSize;
                                        rangeInfo.samplerSet = true;
                                        rangeInfo.type = RangeInfoType::SAMPLER;
                                    }
                                }
                            }

                            if (!validResource)
                            {
                                // falling back to null resources
                                std::string resname = tableResource.name;

                                if (!validResource && range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
                                {
                                    auto& sampler = device.nullResources().sampler;
                                    device.device()->CopyDescriptorsSimple(
                                        1,
                                        samplerCpuDescriptor,
                                        SamplerImplGet::impl(sampler)->native(),
                                        D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

                                    /*LOG("DescriptorIndex[%u]: NULL SAMPLER: %s",
                                        samplerCpuDescriptor.ptr - device.heaps().shaderVisible_sampler->getCpuHeapStart().ptr /
                                        samplerDescriptorHandleSize,
                                        resname.c_str());*/

                                    m_resourceDescriptorMap.emplace_back(ResourceMap{
                                        resourceCpuDescriptor.ptr,
                                        SamplerImplGet::impl(sampler)->native(),
                                        [shader, resname]()->uint64_t
                                    {
                                        const Sampler& sampler = shader->sampler(resname);
                                        uint64_t id = 0;
                                        if (sampler.valid())
                                            id = SamplerImplGet::impl(sampler)->uniqueId();
                                        //LOG("PIPELINE RESOURCE NULL SAMPLER (%s): %u", resname.c_str(), id);
                                        return id;
                                    } });

                                    samplerCpuDescriptor.ptr += samplerDescriptorHandleSize;
                                    samplerGpuDescriptor.ptr += samplerDescriptorHandleSize;
                                    rangeInfo.samplerSet = true;
                                    rangeInfo.type = RangeInfoType::SAMPLER;
                                    //LOG_INFO("Using null sampler resource for: %s", tableResource.name.c_str());
                                }
                            }
                        }

                        m_samplerRanges.emplace_back(rangeInfo);
                    }
                }
            }




            if (postGenerateResourcePipelineHash)
            {
                pipelineResourceHash = createPipelineResourceHash(m_resourceCount);
                if (pipelineResourceHash.first)
                {
                    m_hashResourceStorage[pipelineResourceHash.second] = { std::move(m_resourceDescriptors), m_resourceRanges, currentFrame };
                }
                else
                {
                    //LOG("We were not able to create pipeline hash. Resource and sampler descriptor maps are empty. This is unlikely but can happen if there are no resources at all bound to shader.");
                }
            }

            if (m_samplerCount > 0 && postGenerateSamplerPipelineHash)
            {
                pipelineSamplerHash = createPipelineSamplerHash();
                if (pipelineSamplerHash.first)
                {
                    m_hashSamplerStorage[pipelineSamplerHash.second] = { std::move(m_samplerDescriptors), m_samplerRanges, currentFrame };
                }
                else
                {
                    //LOG("We were not able to create pipeline hash. Resource and sampler descriptor maps are empty. This is unlikely but can happen if there are no resources at all bound to shader.");
                }
            }

            // remove cached pipelines that have not been seen in BackBufferCount frames
#if 1
            auto cachedPipeline = m_hashResourceStorage.begin();
            do
            {
                if (cachedPipeline != m_hashResourceStorage.end())
                {
                    if (currentFrame - (*cachedPipeline).second.lastUsedFrame > BackBufferCount + 1)
                    {
                        auto toRemove = cachedPipeline;
                        ++cachedPipeline;
                        m_hashResourceStorage.erase(toRemove);
                    }
                    else
                    {
                        ++cachedPipeline;
                    }
                }
            } while (cachedPipeline != m_hashResourceStorage.end());
#endif
#else
            device.m_frameDescriptors.emplace_back(std::move(m_resourceDescriptors));
            device.m_frameDescriptors.emplace_back(std::move(m_samplerDescriptors));
#endif
        }

        std::vector<PipelineImpl::TableRange>& PipelineImpl::descriptorRanges()
        {
            return m_range;
        }

        void PipelineImpl::finalize(shaders::PipelineConfiguration* configuration)
        {
            if (!m_finalized)
            {
                loadShaders(configuration);
                generateBindings(configuration);

                m_finalized = true;
            }
        }

        ID3D12PipelineState* PipelineImpl::native() const
        {
            return m_pipelineState.Get();
        }

        ID3D12RootSignature* PipelineImpl::rootSignature() const
        {
            return RootSignatureImplGet::impl(*m_rootSignature).native();
        }
    }
}
