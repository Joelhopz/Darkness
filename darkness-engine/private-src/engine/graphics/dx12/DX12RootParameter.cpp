#include "engine/graphics/dx12/DX12RootParameter.h"
#include "engine/graphics/dx12/DX12Headers.h"
#include "engine/graphics/dx12/DX12Conversions.h"

#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/RootParameter.h"

#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        RootParameterImpl::RootParameterImpl()
            : m_parameter{ new D3D12_ROOT_PARAMETER() }
        {
        }

        RootParameterImpl::~RootParameterImpl()
        {
            if (m_parameter)
                delete m_parameter;
        }

        void RootParameterImpl::binding(unsigned int /*index*/)
        {
            LOG("DX12 RootParameter binding function is not implemented");
        }

        unsigned int RootParameterImpl::binding() const
        {
            return 0;
        }

        void RootParameterImpl::visibility(ShaderVisibility /*visibility*/)
        {
            LOG("DX12 RootParameter visibility function is not implemented");
        }

        const ShaderVisibility RootParameterImpl::visibility() const
        {
            return static_cast<ShaderVisibility>(ShaderVisibilityBits::Vertex);
        }

        D3D12_ROOT_PARAMETER& RootParameterImpl::native()
        {
            return *m_parameter;
        }

        void RootParameterImpl::initAsConstants(unsigned int reg, unsigned int num32BitValues, ShaderVisibility visibility)
        {
            m_parameter->ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
            m_parameter->ShaderVisibility = dxShaderVisibility(visibility);
            m_parameter->Constants.Num32BitValues = num32BitValues;
            m_parameter->Constants.ShaderRegister = reg;
            m_parameter->Constants.RegisterSpace = 0;
        }

        void RootParameterImpl::initAsCBV(unsigned int reg, ShaderVisibility visibility)
        {
            m_parameter->ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            m_parameter->ShaderVisibility = dxShaderVisibility(visibility);
            m_parameter->Descriptor.ShaderRegister = reg;
            m_parameter->Descriptor.RegisterSpace = 0;
        }

        void RootParameterImpl::initAsSRV(unsigned int reg, ShaderVisibility visibility)
        {
            m_parameter->ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
            m_parameter->ShaderVisibility = dxShaderVisibility(visibility);
            m_parameter->Descriptor.ShaderRegister = reg;
            m_parameter->Descriptor.RegisterSpace = 0;
        }

        void RootParameterImpl::initAsUAV(unsigned int reg, ShaderVisibility visibility)
        {
            m_parameter->ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
            m_parameter->ShaderVisibility = dxShaderVisibility(visibility);
            m_parameter->Descriptor.ShaderRegister = reg;
            m_parameter->Descriptor.RegisterSpace = 0;
        }

        void RootParameterImpl::initAsDescriptorRange(DescriptorRangeType type, unsigned int reg, unsigned int count, ShaderVisibility visibility)
        {
            initAsDescriptorTable(1, visibility);
            setTableRange(0, type, reg, count);
        }

        void RootParameterImpl::initAsDescriptorTable(unsigned int rangeCount, ShaderVisibility visibility)
        {
            m_parameter->ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            m_parameter->ShaderVisibility = dxShaderVisibility(visibility);
            m_parameter->DescriptorTable.NumDescriptorRanges = rangeCount;
            m_parameter->DescriptorTable.pDescriptorRanges = new D3D12_DESCRIPTOR_RANGE[rangeCount];
        }

        void RootParameterImpl::setTableRange(
            unsigned int rangeIndex, 
            DescriptorRangeType type, 
            unsigned int reg, 
            unsigned int count, 
            unsigned int space)
        {
            D3D12_DESCRIPTOR_RANGE* range = const_cast<D3D12_DESCRIPTOR_RANGE*>(m_parameter->DescriptorTable.pDescriptorRanges + rangeIndex);
            range->RangeType = dxRangeType(type);
            range->NumDescriptors = count;
            range->BaseShaderRegister = reg;
            range->RegisterSpace = space;
            range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        }
    }
}
