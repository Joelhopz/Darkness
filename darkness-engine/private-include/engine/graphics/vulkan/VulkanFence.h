#pragma once

#include "engine/graphics/vulkan/VulkanHeaders.h"

namespace engine
{
    namespace implementation
    {
        class DeviceImpl;
        class FenceImpl
        {
        public:
            FenceImpl(const DeviceImpl& device);
            ~FenceImpl();

            FenceImpl(const FenceImpl&) = delete;
            FenceImpl(FenceImpl&&) = delete;
            FenceImpl& operator=(const FenceImpl&) = delete;
            FenceImpl& operator=(FenceImpl&&) = delete;

            VkFence& native();
            const VkFence& native() const;

            void reset();
            bool signaled() const;

            void blockUntilSignaled() const;
            void blockUntilSignaled(unsigned long long value);

            unsigned long long value() const;
            unsigned long long nextValue();
        private:
            const DeviceImpl& m_device;
            VkFence m_fence;
            unsigned long long m_fenceValue;
        };
    }
}

