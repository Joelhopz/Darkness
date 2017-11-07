#pragma once

struct D3D12_CPU_DESCRIPTOR_HANDLE;
struct D3D12_GPU_DESCRIPTOR_HANDLE;

namespace engine
{
    enum class DescriptorHandleType;
    class Device;

    namespace implementation
    {
        class DescriptorHandleImpl
        {
        public:
            DEFAULT_CONSTRUCTORS(DescriptorHandleImpl)
            //DescriptorHandleImpl(const D3D12_CPU_DESCRIPTOR_HANDLE& handle, unsigned int incrementSize);
            //DescriptorHandleImpl(const D3D12_GPU_DESCRIPTOR_HANDLE& handle, unsigned int incrementSize);

            DescriptorHandleType type() const;

            DescriptorHandleImpl& operator++();
            DescriptorHandleImpl operator++(int);
            DescriptorHandleImpl& operator+=(int count);
        private:
            D3D12_CPU_DESCRIPTOR_HANDLE* m_cpuHandle;
            D3D12_GPU_DESCRIPTOR_HANDLE* m_gpuHandle;
            unsigned int m_incrementSize;
        };
    }
}
