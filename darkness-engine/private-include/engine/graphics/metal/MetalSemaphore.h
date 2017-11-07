#pragma once

#include "engine/graphics/metal/MetalHeaders.h"
#include <vector>

struct MetalSemaphore{};

namespace engine
{
    class Device;
    namespace implementation
    {
        class SemaphoreImpl
        {
        public:
            SemaphoreImpl(const Device& device);
            
            MetalSemaphore& native();
            const MetalSemaphore& native() const;
        private:
            std::shared_ptr<MetalSemaphore> m_semaphore;
        };
    }
}
