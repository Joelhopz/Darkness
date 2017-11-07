#pragma once

#include "tools/SmartPimpl.h"

namespace engine
{
    PIMPL_FWD(Fence, FenceImpl)

    namespace implementation
    {
        class DeviceImpl;
    }

    class Device;

    class Fence
    {
    public:
        void reset();
        bool signaled() const;
        void blockUntilSignaled();
        void blockUntilSignaled(unsigned long long value);
        unsigned long long value() const;
    private:
        friend class Device;
        friend class implementation::DeviceImpl;
        Fence(const Device& device);

        PIMPL_FRIEND_ACCESS(FenceImpl)
    };
}
