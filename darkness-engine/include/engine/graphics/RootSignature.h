#pragma once

#include "tools/SmartPimpl.h"
#include "engine/graphics/CommonNoDep.h"

namespace engine
{
    PIMPL_FWD(RootSignature, RootSignatureImpl)
    
    struct SamplerDescription;
    class RootParameter;
    class Device;
    enum class RootSignatureFlags;

    namespace implementation
    {
        class PipelineImpl;
    }

    enum class RootSignatureFlags
    {
        None,
        AllowInputAssemblerInputLayout,
        DenyVertexShaderRootAccess,
        DenyHullShaderRootAccess,
        DenyDomainShaderRootAccess,
        DenyGeometryShaderRootAccess,
        DenyPixelShaderRootAccess,
        AllowStreamOutput
    };

    class RootSignature
    {
    public:
        void reset(int rootParameterCount, int staticSamplerCount);
        void initStaticSampler(int samplerNum, const SamplerDescription& description, ShaderVisibility visibility);
        void finalize(RootSignatureFlags flags = RootSignatureFlags::None);
        size_t rootParameterCount() const;
        RootParameter& operator[](int index);
        const RootParameter& operator[](int index) const;

    private:
        friend class Device;
        friend class implementation::PipelineImpl;
        RootSignature(const Device& device, int rootParameterCount = 0, int staticSamplerCount = 0);

        PIMPL_FRIEND_ACCESS(RootSignatureImpl)
    };
}
