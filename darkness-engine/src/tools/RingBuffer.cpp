#include "tools/RingBuffer.h"
#include "tools/ToolsCommon.h"
#include "tools/Debug.h"

namespace tools
{
    RingBuffer::RingBuffer(tools::ByteRange range, size_t align)
        : m_alignment{ align }
        , m_range{ range }
        , m_currentPosition{ 0 }
        , m_oldestAllocationPosition{ 0 }
        , m_tailSize{ 0 }
    {
        LOG("RingBuffer allocated: %zu", range.sizeBytes() / 1024 / 1024);
    }

    void* RingBuffer::allocate(size_t bytes)
    {
        return allocate(bytes, m_alignment);
    }

    void* RingBuffer::allocate(size_t bytes, size_t align)
    {
        uintptr_t allocationPosition = roundUpToMultiple(
            m_range.start + m_currentPosition, align);

        uintptr_t allocationEndPosition = allocationPosition + roundUpToMultiple(bytes, align);

        if (allocationEndPosition <= m_range.stop)
        {
            void* result = reinterpret_cast<void*>(allocationPosition);
            m_currentPosition = allocationEndPosition - m_range.start;
            ASSERT(m_currentPosition <= m_range.sizeBytes(), "Ran out of memory from RingBuffer");
            return result;
        }
        else
        {
            //m_tailSize = reinterpret_cast<uintptr_t>(m_range.stop) - allocationPosition;
            allocationPosition = roundUpToMultiple(m_range.start, align);
            allocationEndPosition = allocationPosition + roundUpToMultiple(bytes, align);
            
            //if (allocationEndPosition <= m_oldestAllocationPosition)
            {
                void* result = reinterpret_cast<void*>(allocationPosition);
                m_currentPosition = allocationEndPosition - m_range.start;
                ASSERT(m_currentPosition <= m_range.sizeBytes(), "Ran out of memory from RingBuffer");
                return result;
            }
        }
    }

    void RingBuffer::free(void* /*ptr*/)
    {
        // nop
    }

    size_t RingBuffer::offset(void* ptr) const
    {
        return static_cast<size_t>(reinterpret_cast<uintptr_t>(ptr) - m_range.start);
    }

    void* RingBuffer::ptrFromOffset(size_t offset) const
    {
        return reinterpret_cast<void*>(m_range.start + static_cast<uintptr_t>(offset));
    }
}
