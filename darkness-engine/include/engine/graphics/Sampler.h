#pragma once

#include "tools/SmartPimpl.h"

namespace engine
{
    PIMPL_FWD_BUFFER(Sampler, SamplerImpl)

    struct SamplerDescription;
    class Device;
    class DescriptorHandle;

    class Sampler
    {
    public:
        INTERFACE_DEFAULT_CONSTRUCTORS(Sampler)
        Sampler() {};
        bool valid() const;
    private:
        friend class Device;
        Sampler(const Device& device, const SamplerDescription& desc);

        PIMPL_FRIEND_ACCESS_BASE_BUFFER(SamplerImpl)
    };
}
