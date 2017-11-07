#pragma once

#include "tools/SmartPimpl.h"

namespace engine
{
    PIMPL_FWD(Semaphore, SemaphoreImpl)

    class Device;

    namespace implementation
    {
        class CommandListImpl;
    }

    class Semaphore
    {
    private:
        friend class Device;
        friend class implementation::CommandListImpl;
        Semaphore(const Device& device);

        PIMPL_FRIEND_ACCESS(SemaphoreImpl)
    };
}
