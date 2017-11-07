#pragma once

struct ID3D12Fence;

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

            ID3D12Fence* native();
            ID3D12Fence* native() const;

            void reset();
            bool signaled() const;
 
            void blockUntilSignaled();
            void blockUntilSignaled(unsigned long long value);

            unsigned long long value() const;
            unsigned long long nextValue();
        private:
            ID3D12Fence* m_fence;
            void* m_fenceEvent;
            unsigned long long m_fenceValue;
        };
    }
}

