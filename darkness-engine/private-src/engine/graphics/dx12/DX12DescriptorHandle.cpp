#include "engine/graphics/dx12/DX12DescriptorHandle.h"
#include "engine/graphics/dx12/DX12Headers.h"

#include "engine/graphics/DescriptorHandle.h"
#include "tools/Debug.h"

namespace engine
{
    namespace implementation
    {
        DescriptorHandleImpl::DescriptorHandleImpl()
            : m_cpuHandle{ nullptr }
            , m_gpuHandle{ nullptr }
            , m_incrementSize{ 0 }
        {
        }

        DescriptorHandleImpl::DescriptorHandleImpl(
            const D3D12_CPU_DESCRIPTOR_HANDLE& handle,
            unsigned int incrementSize)
            : m_cpuHandle{ new D3D12_CPU_DESCRIPTOR_HANDLE(handle) }
            , m_gpuHandle{ nullptr }
            , m_incrementSize{ incrementSize }
        {
        }

        DescriptorHandleImpl::DescriptorHandleImpl(
            const D3D12_GPU_DESCRIPTOR_HANDLE& handle,
            unsigned int incrementSize)
            : m_cpuHandle{ nullptr }
            , m_gpuHandle{ new D3D12_GPU_DESCRIPTOR_HANDLE(handle) }
            , m_incrementSize{ incrementSize }
        {
        }

        DescriptorHandleImpl::DescriptorHandleImpl(const DescriptorHandleImpl& handle)
            : m_cpuHandle{ handle.m_cpuHandle ? new D3D12_CPU_DESCRIPTOR_HANDLE(*handle.m_cpuHandle) : nullptr }
            , m_gpuHandle{ handle.m_gpuHandle ? new D3D12_GPU_DESCRIPTOR_HANDLE(*handle.m_gpuHandle) : nullptr }
            , m_incrementSize{ handle.m_incrementSize }
        {
        }

        DescriptorHandleImpl& DescriptorHandleImpl::operator=(const DescriptorHandleImpl& handle)
        {
            m_cpuHandle = handle.m_cpuHandle ? new D3D12_CPU_DESCRIPTOR_HANDLE(*handle.m_cpuHandle) : nullptr;
            m_gpuHandle = handle.m_gpuHandle ? new D3D12_GPU_DESCRIPTOR_HANDLE(*handle.m_gpuHandle) : nullptr;
            m_incrementSize = handle.m_incrementSize;
            return *this;
        }

        DescriptorHandleImpl::DescriptorHandleImpl(DescriptorHandleImpl&& handle)
            : m_cpuHandle{ std::move(handle.m_cpuHandle) }
            , m_gpuHandle{ std::move(handle.m_gpuHandle )}
            , m_incrementSize{ std::move(handle.m_incrementSize) }
        {
            handle.m_cpuHandle = nullptr;
            handle.m_gpuHandle = nullptr;
            handle.setIncrementSize(0);
        }

        DescriptorHandleImpl& DescriptorHandleImpl::operator=(DescriptorHandleImpl&& handle)
        {
            m_cpuHandle = std::move(handle.m_cpuHandle);
            m_gpuHandle = std::move(handle.m_gpuHandle);
            m_incrementSize = std::move(handle.m_incrementSize);
            handle.m_cpuHandle = nullptr;
            handle.m_gpuHandle = nullptr;
            handle.setIncrementSize(0);
            return *this;
        }

        DescriptorHandleImpl::~DescriptorHandleImpl()
        {
            
            
        }

        DescriptorHandleType DescriptorHandleImpl::type() const
        {
            return m_cpuHandle ? DescriptorHandleType::CPU : DescriptorHandleType::GPU;
        }

        const D3D12_CPU_DESCRIPTOR_HANDLE& DescriptorHandleImpl::cpuHandle() const
        {
            return *m_cpuHandle;
        }

        const D3D12_GPU_DESCRIPTOR_HANDLE& DescriptorHandleImpl::gpuHandle() const
        {
            return *m_gpuHandle;
        }

        DescriptorHandleImpl& DescriptorHandleImpl::operator++()
        {
            if (m_cpuHandle)
                m_cpuHandle->ptr += m_incrementSize;
            else
                m_gpuHandle->ptr += m_incrementSize;
            return *this;
        }

        DescriptorHandleImpl DescriptorHandleImpl::operator++(int)
        {
            DescriptorHandleImpl temp(*this);
            operator++();
            return temp;
        }

        DescriptorHandleImpl& DescriptorHandleImpl::operator+=(int count)
        {
            if (m_cpuHandle)
                m_cpuHandle->ptr += m_incrementSize * count;
            else
                m_gpuHandle->ptr += m_incrementSize * count;
            return *this;
        }

        unsigned int DescriptorHandleImpl::incrementSize() const
        {
            return m_incrementSize;
        }

        void DescriptorHandleImpl::setIncrementSize(unsigned int size)
        {
            m_incrementSize = size;
        }
    }
}
