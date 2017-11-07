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
            shaders::PipelineConfiguration* configuration,
            ShaderStorage& storage)
            : m_device{ device }
            , m_storage{ storage }
            , m_rootSignature{ new RootSignature(device, 4) }
            , m_pipelineState{}
            , m_pipelineStateDesc{ std::make_shared<D3D12_GRAPHICS_PIPELINE_STATE_DESC>() }
            , m_computePipelineStateDesc{ std::make_shared<D3D12_COMPUTE_PIPELINE_STATE_DESC>() }
            , m_configuration{ configuration }
            , m_finalized{ false }
            , m_pipelineCreated{ false }
            , m_descriptorTablesInvalid{ true }
        {
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

            finalize();
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

        void PipelineImpl::setRenderTarget(TextureRTV /*rtv*/)
        {
            LOG("DX12 Pipeline setRenderTarget. this function is silly");
            m_pipelineStateDesc->NumRenderTargets = 1;
        }
#endif
        void PipelineImpl::loadShaders()
        {
            if (m_configuration->hasComputeShader())
            {
                m_cs = m_configuration->computeShader()->load(m_device, m_storage);
                auto desc = m_computePipelineStateDesc.get();
                auto bin = m_cs.get();
                m_cs->registerForChange(this, [desc, bin]() { desc->CS = ShaderBinaryImplGet::impl(*bin).native(); });
                m_computePipelineStateDesc->CS = ShaderBinaryImplGet::impl(*m_cs).native();
            }
            if (m_configuration->hasVertexShader())
            {
                m_vs = m_configuration->vertexShader()->load(m_device, m_storage);
                auto desc = m_pipelineStateDesc.get();
                auto bin = m_vs.get();
                m_vs->registerForChange(this, [this, desc, bin]() 
                { 
                    desc->VS = ShaderBinaryImplGet::impl(*bin).native();
                    this->generateBindings();
                    m_pipelineState.Reset();
                    auto res = DeviceImplGet::impl(this->m_device).device()->CreateGraphicsPipelineState(
                        this->m_pipelineStateDesc.get(),
                        DARKNESS_IID_PPV_ARGS(this->m_pipelineState.GetAddressOf()));
                    ASSERT(SUCCEEDED(res));
                });
                m_pipelineStateDesc->VS = ShaderBinaryImplGet::impl(*m_vs).native();
            }
            if (m_configuration->hasPixelShader())
            {
                m_ps = m_configuration->pixelShader()->load(m_device, m_storage);
                auto desc = m_pipelineStateDesc.get();
                auto bin = m_ps.get();
                m_ps->registerForChange(this, [this, desc, bin]()
                { 
                    desc->PS = ShaderBinaryImplGet::impl(*bin).native();
                    this->generateBindings();
                    m_pipelineState.Reset();
                    auto res = DeviceImplGet::impl(this->m_device).device()->CreateGraphicsPipelineState(
                        this->m_pipelineStateDesc.get(),
                        DARKNESS_IID_PPV_ARGS(this->m_pipelineState.GetAddressOf()));
                    ASSERT(SUCCEEDED(res));
                });
                m_pipelineStateDesc->PS = ShaderBinaryImplGet::impl(*m_ps).native();
            }
            if (m_configuration->hasHullShader())
            {
                m_hs = m_configuration->hullShader()->load(m_device, m_storage);
                auto desc = m_pipelineStateDesc.get();
                auto bin = m_hs.get();
                m_hs->registerForChange(this, [this, desc, bin]()
                {
                    desc->HS = ShaderBinaryImplGet::impl(*bin).native();
                    this->generateBindings();
                    m_pipelineState.Reset();
                    auto res = DeviceImplGet::impl(this->m_device).device()->CreateGraphicsPipelineState(
                        this->m_pipelineStateDesc.get(),
                        DARKNESS_IID_PPV_ARGS(this->m_pipelineState.GetAddressOf()));
                    ASSERT(SUCCEEDED(res));
                });
                m_pipelineStateDesc->HS = ShaderBinaryImplGet::impl(*m_hs).native();
            }
            if (m_configuration->hasGeometryShader())
            {
                m_gs = m_configuration->geometryShader()->load(m_device, m_storage);
                auto desc = m_pipelineStateDesc.get();
                auto bin = m_gs.get();
                m_gs->registerForChange(this, [this, desc, bin]()
                {
                    desc->GS = ShaderBinaryImplGet::impl(*bin).native();
                    this->generateBindings();
                    m_pipelineState.Reset();
                    auto res = DeviceImplGet::impl(this->m_device).device()->CreateGraphicsPipelineState(
                        this->m_pipelineStateDesc.get(),
                        DARKNESS_IID_PPV_ARGS(this->m_pipelineState.GetAddressOf()));
                    ASSERT(SUCCEEDED(res));
                });
                m_pipelineStateDesc->GS = ShaderBinaryImplGet::impl(*m_gs).native();
            }
            if (m_configuration->hasDomainShader())
            {
                m_ds = m_configuration->domainShader()->load(m_device, m_storage);
                auto desc = m_pipelineStateDesc.get();
                auto bin = m_ds.get();
                m_ds->registerForChange(this, [this, desc, bin]()
                {
                    desc->DS = ShaderBinaryImplGet::impl(*bin).native();
                    this->generateBindings();
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

        void PipelineImpl::generateBindings(const void* byteCode, size_t size, D3D12_SHADER_VISIBILITY visibility)
        {
            ID3D12ShaderReflection* reflector;
            auto res = D3DReflect(
                byteCode,
                size,
                IID_ID3D11ShaderReflection,
                (void**)&reflector
            );
            ASSERT(SUCCEEDED(res));

            D3D12_SHADER_DESC shaderDesc;
            res = reflector->GetDesc(&shaderDesc);

            // VS
            for (unsigned int i = 0; i < shaderDesc.InputParameters; ++i)
            {
                D3D12_SIGNATURE_PARAMETER_DESC input_desc;
                reflector->GetInputParameterDesc(i, &input_desc);
                //auto temp = input_desc.ComponentType;

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
                }
            }

            m_pipelineStateDesc->InputLayout.NumElements = static_cast<UINT>(m_inputLayouts.size());
            m_pipelineStateDesc->InputLayout.pInputElementDescs = m_inputLayouts.data();

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

            for (unsigned int i = 0; i < shaderDesc.BoundResources; ++i)
            {
                D3D12_SHADER_INPUT_BIND_DESC desc;
                reflector->GetResourceBindingDesc(i, &desc);

                auto inputType = rangeTypeFromInputType(desc.Type);

                auto search = std::find_if(
                    m_range.begin(),
                    m_range.end(),
                    [inputType, visibility](const TableRange& r) {
                    return (inputType == r.range.RangeType) && (r.visibility == visibility); });

                if (search == m_range.end())
                {
                    D3D12_DESCRIPTOR_RANGE new_range;
                    new_range.BaseShaderRegister = 0;
                    new_range.NumDescriptors = 1;
                    new_range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    new_range.RangeType = inputType;
                    new_range.RegisterSpace = 0;
                    TableRange::TableResource tableResource{ desc.Dimension, std::string(desc.Name) };
                    std::vector<TableRange::TableResource> vec{ std::move(tableResource) };
                    auto tableRange = TableRange{ new_range, visibility, std::move(vec) };
                    m_range.emplace_back(std::move(tableRange));
                }
                else
                {
                    (*search).range.NumDescriptors++;
                    TableRange::TableResource tableResource{ desc.Dimension, std::string(desc.Name) };
                    (*search).resourceNames.emplace_back(std::move(tableResource));
                }
            }
        }

        void PipelineImpl::generateBindings()
        {
            m_inputLayouts.clear();
            m_range.clear();

            if (m_cs)
            {
                generateBindings(
                    ShaderBinaryImplGet::impl(*m_cs).native().pShaderBytecode,
                    ShaderBinaryImplGet::impl(*m_cs).native().BytecodeLength,
                    D3D12_SHADER_VISIBILITY_ALL
                );
            }

            if (m_vs)
            {
                generateBindings(
                    ShaderBinaryImplGet::impl(*m_vs).native().pShaderBytecode,
                    ShaderBinaryImplGet::impl(*m_vs).native().BytecodeLength,
                    D3D12_SHADER_VISIBILITY_VERTEX
                );
            }

            if (m_ps)
            {
                generateBindings(
                    ShaderBinaryImplGet::impl(*m_ps).native().pShaderBytecode,
                    ShaderBinaryImplGet::impl(*m_ps).native().BytecodeLength,
                    D3D12_SHADER_VISIBILITY_PIXEL
                );
            }

            if (m_gs)
            {
                generateBindings(
                    ShaderBinaryImplGet::impl(*m_gs).native().pShaderBytecode,
                    ShaderBinaryImplGet::impl(*m_gs).native().BytecodeLength,
                    D3D12_SHADER_VISIBILITY_GEOMETRY
                );
            }

            if (m_ds)
            {
                generateBindings(
                    ShaderBinaryImplGet::impl(*m_ds).native().pShaderBytecode,
                    ShaderBinaryImplGet::impl(*m_ds).native().BytecodeLength,
                    D3D12_SHADER_VISIBILITY_DOMAIN
                );
            }

            if (m_hs)
            {
                generateBindings(
                    ShaderBinaryImplGet::impl(*m_hs).native().pShaderBytecode,
                    ShaderBinaryImplGet::impl(*m_hs).native().BytecodeLength,
                    D3D12_SHADER_VISIBILITY_HULL
                );
            }

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
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX: { shader = const_cast<shaders::Shader*>(m_configuration->vertexShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL: { shader = const_cast<shaders::Shader*>(m_configuration->pixelShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_HULL: { shader = const_cast<shaders::Shader*>(m_configuration->hullShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_DOMAIN: { shader = const_cast<shaders::Shader*>(m_configuration->domainShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_GEOMETRY: { shader = const_cast<shaders::Shader*>(m_configuration->geometryShader()); break; }
                            case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL: { shader = const_cast<shaders::Shader*>(m_configuration->computeShader()); break; }
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
                                m_range[resourceRange].range.NumDescriptors = static_cast<UINT>(max(1, bindlessSize));
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

            m_descriptorTablesInvalid = true;
        }

        void PipelineImpl::createBindingTables()
        {
            DeviceImpl& device = DeviceImplGet::impl(m_device);

            if (m_resourceCount > 0)
                m_resourceDescriptors = device.heaps().shaderVisible_cbv_srv_uav->getDescriptor(m_resourceCount);
            else
                m_resourceDescriptors = {};
            if (m_samplerCount > 0)
                m_samplerDescriptors = device.heaps().shaderVisible_sampler->getDescriptor(m_samplerCount);
            else
                m_samplerDescriptors = {};
        }

        static uint64_t lastDebugFrame = 0;
        static std::map<const void*, std::vector<std::string>> pipes;

        std::pair<bool, uint64_t> PipelineImpl::createPipelineHash(size_t resourceCount, size_t samplerCount) const
        {
            if (m_resourceDescriptorMap.size() > 0 || m_samplerDescriptorMap.size() > 0)
            {
                std::vector<uint64_t> pipelineHashData(m_resourceDescriptorMap.size() + m_samplerDescriptorMap.size() + 2 + m_range.size());
                uint64_t* ptr = &pipelineHashData[0];
                for (auto&& res : m_resourceDescriptorMap)
                {
                    *ptr = res.second.currentHandle();
                    ++ptr;
                }
                for (auto&& res : m_samplerDescriptorMap)
                {
                    *ptr = res.second.currentHandle();
                    ++ptr;
                }
                *ptr = resourceCount; ++ptr;
                *ptr = samplerCount; ++ptr;
                for (auto&& range : m_range)
                {
                    *ptr = static_cast<uint64_t>(range.range.RangeType);
                    ++ptr;
                }

                std::pair<bool, uint64_t> res = { true, static_cast<uint64_t>(tools::hash(reinterpret_cast<uint8_t*>(pipelineHashData.data()), static_cast<unsigned int>(pipelineHashData.size() * sizeof(uint64_t)))) };


                /*std::ostringstream pipeStr;
                pipeStr << "HASH: ";
                pipeStr << res.second;
                pipeStr << ". ";
                for (auto val : pipelineHashData)
                {
                    pipeStr << val;
                    pipeStr << ", ";
                }
                if (pipes.find(static_cast<const void*>(this)) == pipes.end())
                {
                    pipes[static_cast<const void*>(this)] = std::vector<std::string>();
                }
                pipes[static_cast<const void*>(this)].emplace_back(pipeStr.str());*/

                
                return res;
            }
            return { false, 0 };
        }

        void PipelineImpl::configure(CommandListImpl& commandList, shaders::PipelineConfiguration* configuration)
        {
            /*if (m_device.frameNumber() != lastDebugFrame)
            {
                lastDebugFrame = m_device.frameNumber();

                LOG("===================================== START PIPELINE BINDS =====================================");
                for (auto&& pipe : pipes)
                {
                    for (auto&& str : pipe.second)
                    {
                        LOG("Pipeline [%p]: %s", pipe.first, str.c_str());
                    }
                }

                pipes.clear();
            }*/

            auto currentFrame = m_device.frameNumber();
            if (!m_pipelineCreated)
            {
                if (!configuration->hasComputeShader())
                {
                    auto res = DeviceImplGet::impl(m_device).device()->CreateGraphicsPipelineState(
                        m_pipelineStateDesc.get(),
                        DARKNESS_IID_PPV_ARGS(m_pipelineState.GetAddressOf()));
                    ASSERT(SUCCEEDED(res));
                }
                else
                {
                    auto res = DeviceImplGet::impl(m_device).device()->CreateComputePipelineState(
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
                    this->generateBindings();
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

            auto pipelineHash = createPipelineHash(m_resourceCount, m_samplerCount);

            //LOG("Pipeline [%p]: LOOKING FOR: %" PRIu64 "", static_cast<const void*>(this), pipelineHash.second);

            bool postGeneratePipelineHash = false;
            if (pipelineHash.first)
            {
                auto storedPipeline = m_hashStorage.find(pipelineHash.second);
                if (storedPipeline != m_hashStorage.end())
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

                    samplerDescriptorCount = storedPipeline->second.samplerDescriptors.count();
                    if (samplerDescriptorCount > 0)
                    {
                        samplerCpuDescriptor = storedPipeline->second.samplerDescriptors.cpuHandle();
                        samplerGpuDescriptor = storedPipeline->second.samplerDescriptors.gpuHandle();
                        samplerDescriptorHandleSize = storedPipeline->second.samplerDescriptors.handleSize();
                    }
                    m_resourceRanges = storedPipeline->second.ranges;

                    storedPipeline->second.lastUsedFrame = currentFrame;

                    //LOG("Pipeline [%p]: USING: %" PRIu64 "", static_cast<const void*>(this), pipelineHash.second);
                }
                else
                {
                    // something has changed in the pipeline configuration
                    // creating a new table and post hash
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

                    postGeneratePipelineHash = true;
                    m_descriptorTablesInvalid = true;
                }
            }
            else
            {
                LOG("could not create pipeline");
                // could not create hash at all. usually first run for this pipeline
                // we generate the hash after the table has been created
                createBindingTables();

                resourceDescriptorCount = m_resourceDescriptors.count();
                if (resourceDescriptorCount)
                {
                    resourceCpuDescriptor = m_resourceDescriptors.cpuHandle();
                    resourceGpuDescriptor = m_resourceDescriptors.gpuHandle();
                    resourceDescriptorHandleSize = m_resourceDescriptors.handleSize();
                }

                samplerDescriptorCount = m_samplerDescriptors.count();
                if (samplerDescriptorCount)
                {
                    samplerCpuDescriptor = m_samplerDescriptors.cpuHandle();
                    samplerGpuDescriptor = m_samplerDescriptors.gpuHandle();
                    samplerDescriptorHandleSize = m_samplerDescriptors.handleSize();
                }

                postGeneratePipelineHash = true;
                m_descriptorTablesInvalid = true;
            }

            

            if (m_descriptorTablesInvalid)
            {
                m_descriptorTablesInvalid = false;
                DeviceImpl& device = DeviceImplGet::impl(m_device);
            
                m_resourceDescriptorMap.clear();
                m_samplerDescriptorMap.clear();
                m_constantBufferUpdates.clear();
                m_resourceRanges.clear();

                if (resourceDescriptorCount > 0 || samplerDescriptorCount > 0)
                {
                    for (int resourceRange = 0; resourceRange < m_range.size(); ++resourceRange)
                    {
                        RangeInfo rangeInfo;
                        rangeInfo.resourceGpuHeapStart = resourceGpuDescriptor;
                        rangeInfo.samplerGpuHeapStart = samplerGpuDescriptor;
                        rangeInfo.srvUavCbvSet = false;
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

                                            m_resourceDescriptorMap[resourceCpuDescriptor.ptr] = {
                                                TextureSRVImplGet::impl(resource)->native(),
                                                [shader, resname]()->uint64_t
                                                {
                                                    auto res = shader->textureSrv(resname);
                                                    if (res.valid())
                                                        return TextureSRVImplGet::impl(res)->uniqueId();
                                                    return 0;
                                                } };

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

                                            m_resourceDescriptorMap[resourceCpuDescriptor.ptr] = {
                                                TextureSRVImplGet::impl(resource[i])->native(),
                                                [shader, resname]()->uint64_t
                                            {
                                                auto res = shader->bindlessTextureSrv(resname);
                                                std::vector<uint64_t> uids;
                                                for (int a = 0; a < res.size(); ++a)
                                                {
                                                    uids.emplace_back(TextureSRVImplGet::impl(res[a])->uniqueId());
                                                }
                                                return static_cast<uint64_t>(tools::hash(reinterpret_cast<uint8_t*>(uids.data()), static_cast<unsigned int>(uids.size() * sizeof(uint64_t))));
                                            } };

                                            //LOG("setting TextureSrv: %s", resname.c_str());

                                            resourceCpuDescriptor.ptr += resourceDescriptorHandleSize;
                                            resourceGpuDescriptor.ptr += resourceDescriptorHandleSize;
                                            rangeInfo.srvUavCbvSet = true;
                                            rangeInfo.type = RangeInfoType::SRV;
                                        }
                                        /*if (resource.size() == 0)
                                        {
                                            // null texture
                                            device.device()->CopyDescriptorsSimple(
                                                1,
                                                resourceCpuDescriptor,
                                                TextureSRVImplGet::impl(device.nullResources().textureSRV)->native(),
                                                D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

                                            m_resourceDescriptorMap[resourceCpuDescriptor.ptr] = {
                                                TextureSRVImplGet::impl(device.nullResources().textureSRV)->native(),
                                                [shader, resname]()->uint64_t
                                            {
                                                return 0;
                                            } };

                                            //LOG("setting TextureSrv: %s", resname.c_str());

                                            resourceCpuDescriptor.ptr += resourceDescriptorHandleSize;
                                            resourceGpuDescriptor.ptr += resourceDescriptorHandleSize;
                                            rangeInfo.srvUavCbvSet = true;
                                            rangeInfo.type = RangeInfoType::SRV;
                                        }*/
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

                                        m_resourceDescriptorMap[resourceCpuDescriptor.ptr] = {
                                            TextureUAVImplGet::impl(resource)->native(),
                                            [shader, resname]()->uint64_t
                                            {
                                                auto res = shader->textureUav(resname);
                                                if (res.valid())
                                                    return TextureUAVImplGet::impl(res)->uniqueId();
                                                return 0;
                                            } };

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

                                        m_resourceDescriptorMap[resourceCpuDescriptor.ptr] = {
                                            BufferSRVImplGet::impl(resource)->native(),
                                            [shader, resname]()->uint64_t
                                            {
                                                auto res = shader->bufferSrv(resname);
                                                if (res.valid())
                                                    return BufferSRVImplGet::impl(res)->uniqueId();
                                                return 0;
                                            } };

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

                                        m_resourceDescriptorMap[resourceCpuDescriptor.ptr] = {
                                            BufferUAVImplGet::impl(resource)->native(),
                                            [shader, resname]()->uint64_t
                                            {
                                                auto res = shader->bufferUav(resname);
                                                if (res.valid())
                                                    return BufferUAVImplGet::impl(res)->uniqueId();
                                                return 0;
                                            } };

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

                                                // create a CBV for the buffer
                                                std::shared_ptr<BufferCBV> cbv = std::make_shared<BufferCBV>(
                                                    std::make_shared<BufferCBVImpl>(
                                                        device,
                                                        cbvBuffer,
                                                        BufferDescription()
                                                        .elements(initial.elements)
                                                        .elementSize(initial.elementSize)
                                                        .format(Format::Format_R32_UINT)));

                                                constantrange.buffer = cbv;
                                            }
                                            device.uploadBuffer(commandList, *constantrange.buffer, constantrange.range, 0);
                                            m_constantBufferUpdates.emplace_back(ConstantBufferUpdates{ constantrange.buffer, constantrange.range });

                                            device.device()->CopyDescriptorsSimple(
                                                1,
                                                resourceCpuDescriptor,
                                                BufferCBVImplGet::impl(*constantrange.buffer)->native(),
                                                D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

                                            /*std::string resname = tableResource.name;
                                            m_resourceDescriptorMap[resourceCpuDescriptor.ptr] = {
                                                BufferCBVImplGet::impl(*constantrange.buffer)->native(),
                                                [shader, resname]()->uint64_t
                                                {
                                                    auto& constantranges = shader->constants();
                                                    for (auto&& constantrange : constantranges)
                                                    {
                                                        if (constantrange.name == resname)
                                                        {
                                                            return BufferCBVImplGet::impl(*constantrange.buffer)->uniqueId();
                                                        }
                                                    }
                                                    return 0;
                                                }
                                            };*/

                                            //LOG("setting Constant range: %s", constantrange.name);

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

                                        m_resourceDescriptorMap[resourceCpuDescriptor.ptr] = {
                                            SamplerImplGet::impl(resource)->native(),
                                            [shader, resname]()->uint64_t
                                            {
                                                auto res = shader->sampler(resname);
                                                if (res.valid())
                                                    return SamplerImplGet::impl(res)->uniqueId();
                                                return 0;
                                            } };

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

                                if (!validResource &&
                                    range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SRV &&
                                    tableResource.dimension != D3D_SRV_DIMENSION_BUFFER)
                                {
                                    auto& srv = device.nullResources().textureSRV;
                                    device.device()->CopyDescriptorsSimple(
                                        1,
                                        resourceCpuDescriptor,
                                        TextureSRVImplGet::impl(srv)->native(),
                                        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

                                    m_resourceDescriptorMap[resourceCpuDescriptor.ptr] = {
                                        TextureSRVImplGet::impl(srv)->native(),
                                        [shader, resname]()->uint64_t
                                    {
                                        const TextureSRV& srv = shader->textureSrv(resname);
                                        if (srv.valid())
                                            return TextureSRVImplGet::impl(srv)->uniqueId();
                                        return 0;
                                    } };

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

                                    m_resourceDescriptorMap[resourceCpuDescriptor.ptr] = {
                                        TextureUAVImplGet::impl(uav)->native(),
                                        [shader, resname]()->uint64_t
                                    {
                                        const TextureUAV& uav = shader->textureUav(resname);
                                        if (uav.valid())
                                            return TextureUAVImplGet::impl(uav)->uniqueId();
                                        return 0;
                                    } };

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

                                    m_resourceDescriptorMap[resourceCpuDescriptor.ptr] = {
                                        BufferSRVImplGet::impl(srv)->native(),
                                        [shader, resname]()->uint64_t
                                    {
                                        const BufferSRV& srv = shader->bufferSrv(resname);
                                        if (srv.valid())
                                            return BufferSRVImplGet::impl(srv)->uniqueId();
                                        return 0;
                                    } };

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

                                    m_resourceDescriptorMap[resourceCpuDescriptor.ptr] = {
                                        BufferUAVImplGet::impl(uav)->native(),
                                        [shader, resname]()->uint64_t
                                    {
                                        const BufferUAV& uav = shader->bufferUav(resname);
                                        if (uav.valid())
                                            return BufferUAVImplGet::impl(uav)->uniqueId();
                                        return 0;
                                    } };

                                    resourceCpuDescriptor.ptr += resourceDescriptorHandleSize;
                                    resourceGpuDescriptor.ptr += resourceDescriptorHandleSize;
                                    rangeInfo.srvUavCbvSet = true;
                                    rangeInfo.type = RangeInfoType::UAV;
                                    //LOG_INFO("Using null buffer uav resource for: %s", tableResource.name.c_str());
                                }
                                else if (!validResource && range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_CBV)
                                {
                                    //ASSERT(validResource, "Unset resource: %s", tableResource.name.c_str());
                                    rangeInfo.type = RangeInfoType::CONSTANT;
                                }
                                else if (!validResource && range.range.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
                                {
                                    auto& sampler = device.nullResources().sampler;
                                    device.device()->CopyDescriptorsSimple(
                                        1,
                                        samplerCpuDescriptor,
                                        SamplerImplGet::impl(sampler)->native(),
                                        D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

                                    m_resourceDescriptorMap[resourceCpuDescriptor.ptr] = {
                                        SamplerImplGet::impl(sampler)->native(),
                                        [shader, resname]()->uint64_t
                                    {
                                        const Sampler& sampler = shader->sampler(resname);
                                        if (sampler.valid())
                                            return SamplerImplGet::impl(sampler)->uniqueId();
                                        return 0;
                                    } };

                                    samplerCpuDescriptor.ptr += samplerDescriptorHandleSize;
                                    samplerGpuDescriptor.ptr += samplerDescriptorHandleSize;
                                    rangeInfo.samplerSet = true;
                                    rangeInfo.type = RangeInfoType::SAMPLER;
                                    //LOG_INFO("Using null sampler resource for: %s", tableResource.name.c_str());
                                }
                            }
                        }

                        ASSERT(!(rangeInfo.srvUavCbvSet && rangeInfo.samplerSet), "Can not set both SRV, UAV, CBV ranges and SAMPLER range at the same time");

                        m_resourceRanges.emplace_back(rangeInfo);
                    }
                }
            }
            else
            {
                DeviceImpl& device = DeviceImplGet::impl(m_device);
                for (auto&& update : m_constantBufferUpdates)
                {
                    device.uploadBuffer(commandList, *update.buffer, update.range, 0);
                }
            }

            if (postGeneratePipelineHash)
            {
                pipelineHash = createPipelineHash(m_resourceCount, m_samplerCount);
                if (pipelineHash.first)
                {
                    m_hashStorage[pipelineHash.second] = { std::move(m_resourceDescriptors), std::move(m_samplerDescriptors), m_resourceRanges, currentFrame };

                    //LOG("Pipeline [%p]: ADDING HASH: %" PRIu64 "", static_cast<const void*>(this), pipelineHash.second);
                    //LOG("generated hash: %ull", pipelineHash.second);
                }
                else
                {
                    //LOG("We were not able to create pipeline hash. Resource and sampler descriptor maps are empty. This is unlikely but can happen if there are no resources at all bound to shader.");
                }
            }

            // remove cached pipelines that have not been seen in BackBufferCount frames
            auto cachedPipeline = m_hashStorage.begin();
            do
            {
                if (cachedPipeline != m_hashStorage.end())
                {
                    if (currentFrame - (*cachedPipeline).second.lastUsedFrame > BackBufferCount + 1)
                    {
                        auto toRemove = cachedPipeline;

                        //LOG("Pipeline [%p]: REMOVING THIS HASH: %" PRIu64 "", static_cast<const void*>(this), (*toRemove).first);

                        ++cachedPipeline;
                        m_hashStorage.erase(toRemove);
                    }
                    else
                    {
                        ++cachedPipeline;
                    }
                }
            } while (cachedPipeline != m_hashStorage.end());
        }

        std::vector<PipelineImpl::TableRange>& PipelineImpl::descriptorRanges()
        {
            return m_range;
        }

        void PipelineImpl::finalize()
        {
            if (!m_finalized)
            {
                loadShaders();
                generateBindings();

                //createBindingTables();

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
