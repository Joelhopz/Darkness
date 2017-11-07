#include "engine/graphics/dx12/DX12RootSignature.h"
#include "engine/graphics/dx12/DX12Headers.h"
#include "engine/graphics/dx12/DX12RootParameter.h"
#include "engine/graphics/dx12/DX12Sampler.h"
#include "engine/graphics/dx12/DX12Device.h"
#include "engine/graphics/dx12/DX12Conversions.h"

#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/RootParameter.h"
#include "engine/graphics/RootSignature.h"
#include "engine/graphics/Device.h"

#include "tools/Debug.h"
#include "tools/ComPtr.h"

namespace engine
{
    namespace implementation
    {
        RootSignatureImpl::RootSignatureImpl(const Device& device, int rootParameterCount, int staticSamplerCount)
            : m_device{ device }
            , m_signature{ nullptr }
            , m_numInitializedStaticSamplers{ 0 }
            , m_parameters{ nullptr }
            , m_samplers{ nullptr }
            , m_parameterCount{ 0 }
            , m_samplerCount{ 0 }
        {
            reset(rootParameterCount, staticSamplerCount);
        }

        RootSignatureImpl::~RootSignatureImpl()
        {
            if (m_signature)
            {
                m_signature->Release();
                m_signature = nullptr;
            }
        }

        void RootSignatureImpl::reset(int rootParameterCount, int staticSamplerCount)
        {
            m_parameterCount = rootParameterCount;
            m_samplerCount = staticSamplerCount;

            if (rootParameterCount > 0)
                m_parameters.reset(new RootParameter[static_cast<size_t>(rootParameterCount)]);
            else
                m_parameters = nullptr;

            if (staticSamplerCount > 0)
                m_samplers.reset(new StaticSamplerDescription[static_cast<size_t>(staticSamplerCount)]);
            else
                m_samplers = nullptr;
        }

        void RootSignatureImpl::initStaticSampler(int samplerNum, const SamplerDescription& description, ShaderVisibility visibility)
        {
            StaticSamplerDescription& staticSamplerDesc = m_samplers[m_numInitializedStaticSamplers++];
            staticSamplerDesc.desc.filter = description.desc.filter;
            staticSamplerDesc.desc.addressU = description.desc.addressU;
            staticSamplerDesc.desc.addressV = description.desc.addressV;
            staticSamplerDesc.desc.addressW = description.desc.addressW;
            staticSamplerDesc.desc.mipLODBias = description.desc.mipLODBias;
            staticSamplerDesc.desc.maxAnisotrophy = description.desc.maxAnisotrophy;
            staticSamplerDesc.desc.comparisonFunc = description.desc.comparisonFunc;
            staticSamplerDesc.desc.borderColor = StaticBorderColor::OpaqueWhite;
            staticSamplerDesc.desc.minLOD = description.desc.minLOD;
            staticSamplerDesc.desc.maxLOD = description.desc.maxLOD;
            staticSamplerDesc.desc.shaderRegister = static_cast<uint32_t>(samplerNum);
            staticSamplerDesc.desc.registerSpace = 0;
            staticSamplerDesc.desc.shaderVisibility = visibility;

            if (staticSamplerDesc.desc.addressU == TextureAddressMode::Border ||
                staticSamplerDesc.desc.addressV == TextureAddressMode::Border ||
                staticSamplerDesc.desc.addressW == TextureAddressMode::Border)
            {
                if (description.desc.borderColor[3] == 1.0f)
                {
                    if (description.desc.borderColor[0] == 1.0f)
                        staticSamplerDesc.desc.borderColor = StaticBorderColor::OpaqueWhite;
                    else
                        staticSamplerDesc.desc.borderColor = StaticBorderColor::OpaqueBlack;
                }
                else
                    staticSamplerDesc.desc.borderColor = StaticBorderColor::TransparentBlack;
            }
        }

        void RootSignatureImpl::finalize(RootSignatureFlags flags)
        {
            if (m_signature)
                return;

            ASSERT(static_cast<int>(m_numInitializedStaticSamplers) == m_samplerCount);

            D3D12_ROOT_PARAMETER* parameters = nullptr;
            if (m_parameterCount > 0)
            {
                parameters = new D3D12_ROOT_PARAMETER[static_cast<size_t>(m_parameterCount)];
                for (size_t i = 0; i < static_cast<size_t>(m_parameterCount); ++i)
                {
                    parameters[i] = RootParameterImplGet::impl(m_parameters[i]).native();
                }
            }

            D3D12_STATIC_SAMPLER_DESC* samplers = nullptr;
            if (m_samplerCount > 0)
            {
                samplers = new D3D12_STATIC_SAMPLER_DESC[static_cast<size_t>(m_samplerCount)];
                for (size_t i = 0; i < static_cast<size_t>(m_samplerCount); ++i)
                {
                    samplers[i] = dxStaticSamplerDesc(m_samplers[i]);
                }
            }

            D3D12_ROOT_SIGNATURE_DESC rootDesc;
            rootDesc.NumParameters = static_cast<UINT>(m_parameterCount);
            rootDesc.pParameters = (const D3D12_ROOT_PARAMETER*)parameters;
            rootDesc.NumStaticSamplers = static_cast<UINT>(m_samplerCount);
            rootDesc.pStaticSamplers = (const D3D12_STATIC_SAMPLER_DESC*)samplers;
            rootDesc.Flags = dxSignatureFlags(flags) | D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

            m_descriptorTableBitMap = 0;
            m_maxDescriptorCacheHandleCount = 0;

            for (int param = 0; param < m_parameterCount; ++param)
            {
                const D3D12_ROOT_PARAMETER& rootParam = rootDesc.pParameters[param];
                m_descriptorTableSize[param] = 0;

                if (rootParam.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
                {
                    ASSERT(rootParam.DescriptorTable.pDescriptorRanges != nullptr);

                    /*HashCode = Utility::HashState(rootParam.DescriptorTable.pDescriptorRanges,
                        rootParam.DescriptorTable.NumDescriptorRanges, HashCode);*/

                    // We don't care about sampler descriptor tables.  We don't manage them in DescriptorCache
                    if (rootParam.DescriptorTable.pDescriptorRanges->RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
                        continue;

                    m_descriptorTableBitMap |= (1 << param);
                    for (UINT TableRange = 0; TableRange < rootParam.DescriptorTable.NumDescriptorRanges; ++TableRange)
                        m_descriptorTableSize[param] += rootParam.DescriptorTable.pDescriptorRanges[TableRange].NumDescriptors;

                    m_maxDescriptorCacheHandleCount += m_descriptorTableSize[param];
                }
            }

            tools::ComPtr<ID3DBlob> pOutBlob, pErrorBlob;

            ASSERT(SUCCEEDED(D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1,
                pOutBlob.GetAddressOf(), pErrorBlob.GetAddressOf())));

            ASSERT(SUCCEEDED(implementation::DeviceImplGet::impl(m_device).device()->CreateRootSignature(1, pOutBlob->GetBufferPointer(), pOutBlob->GetBufferSize(),
                DARKNESS_IID_PPV_ARGS(&m_signature))));
        }

        size_t RootSignatureImpl::rootParameterCount() const
        {
            return 0;
        }

        RootParameter& RootSignatureImpl::operator[](int index)
        {
            ASSERT(index >= 0 && index < m_parameterCount);
            return m_parameters[static_cast<size_t>(index)];
        }

        const RootParameter& RootSignatureImpl::operator[](int index) const
        {
            ASSERT(index >= 0 && index < m_parameterCount);
            return m_parameters[static_cast<size_t>(index)];
        }

        ID3D12RootSignature* RootSignatureImpl::native()
        {
            return m_signature;
        }

        ID3D12RootSignature* RootSignatureImpl::native() const
        {
            return m_signature;
        }
    }
}
