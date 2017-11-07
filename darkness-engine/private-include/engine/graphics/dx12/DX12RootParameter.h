#pragma once

#include "engine/graphics/CommonNoDep.h"

struct D3D12_ROOT_PARAMETER;

namespace engine
{
    enum class DescriptorRangeType;

    namespace implementation
    {
        class RootParameterImpl
        {
        public:
            RootParameterImpl();
            ~RootParameterImpl();

            void binding(unsigned int index);
            unsigned int binding() const;

            void visibility(ShaderVisibility visibility);
            const ShaderVisibility visibility() const;

            void initAsConstants(unsigned int reg, unsigned int num32BitValues, ShaderVisibility visibility);
            void initAsCBV(unsigned int reg, ShaderVisibility visibility);
            void initAsSRV(unsigned int reg, ShaderVisibility visibility);
            void initAsUAV(unsigned int reg, ShaderVisibility visibility);
            void initAsDescriptorRange(DescriptorRangeType type, unsigned int reg, unsigned int count, ShaderVisibility visibility);
            void initAsDescriptorTable(unsigned int rangeCount, ShaderVisibility visibility);
            void setTableRange(unsigned int rangeIndex, DescriptorRangeType type, unsigned int reg, unsigned int count, unsigned int space = 0);

            D3D12_ROOT_PARAMETER& native();
        private:
            D3D12_ROOT_PARAMETER* m_parameter;
        };
    }
}

