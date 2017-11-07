#include "tools/RingBuffer.h"
#include "tools/ToolsCommon.h"
#include "tools/Debug.h"

namespace tools
{
    RingBuffer::RingBuffer(tools::ByteRange range)
        : m_range{ range }
        , m_currentPosition{ 0 }
        , m_oldestAllocationPosition{ 0 }
        , m_tailSize{ 0 }
    {
        LOG("RingBuffer allocated: %zu", range.size() / 1024 / 1024);
    }

    void* RingBuffer::allocate(size_t bytes)
    {
        return allocate(bytes, 4);
    }

    static size_t RingBytesAllocated = 0;
    static size_t RingBytesAllocatedNextTarget = 1024 * 1024;
    void* RingBuffer::allocate(size_t bytes, size_t align)
    {
        RingBytesAllocated += bytes;
        if (RingBytesAllocated > RingBytesAllocatedNextTarget)
        {
            //LOG("Ring currently: %zu", RingBytesAllocated / 1024 / 1024);
            RingBytesAllocatedNextTarget += 1024 * 1024;
        }

        uintptr_t allocationPosition = roundUpToMultiple(
            reinterpret_cast<uintptr_t>(const_cast<uint8_t*>(m_range.start)) + m_currentPosition, align);

        uintptr_t allocationEndPosition = allocationPosition + roundUpToMultiple(bytes, align);

        if (allocationEndPosition <= reinterpret_cast<uintptr_t>(m_range.stop))
        {
            void* result = reinterpret_cast<void*>(allocationPosition);
            m_currentPosition = allocationEndPosition - reinterpret_cast<uintptr_t>(const_cast<uint8_t*>(m_range.start));
            ASSERT(m_currentPosition <= m_range.size(), "Ran out of memory from RingBuffer");
            return result;
        }
        else
        {
            //m_tailSize = reinterpret_cast<uintptr_t>(m_range.stop) - allocationPosition;
            allocationPosition = roundUpToMultiple(reinterpret_cast<uintptr_t>(const_cast<uint8_t*>(m_range.start)), align);
            allocationEndPosition = allocationPosition + roundUpToMultiple(bytes, align);
            
            //if (allocationEndPosition <= m_oldestAllocationPosition)
            {
                void* result = reinterpret_cast<void*>(allocationPosition);
                m_currentPosition = allocationEndPosition - reinterpret_cast<uintptr_t>(const_cast<uint8_t*>(m_range.start));
                ASSERT(m_currentPosition <= m_range.size(), "Ran out of memory from RingBuffer");
                return result;
            }
        }
        
        ASSERT(false, "Ran out of memory from RingBuffer");
        return nullptr;
    }

    void RingBuffer::free(void* /*ptr*/)
    {
        // nop
    }

    size_t RingBuffer::offset(void* ptr) const
    {
        return static_cast<size_t>(reinterpret_cast<uintptr_t>(ptr) -
            reinterpret_cast<uintptr_t>(const_cast<uint8_t*>(m_range.start)));
    }
}
