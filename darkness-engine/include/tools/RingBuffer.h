#pragma once

#include "tools/Allocator.h"
#include "tools/ByteRange.h"

namespace tools
{
    class RingBuffer : public OffsetAllocatorInterface
    {
    public:
        RingBuffer() = default;
        RingBuffer(ByteRange range, size_t align = 4);

        void* allocate(size_t bytes) override;
        void* allocate(size_t bytes, size_t align) override;
        void free(void* ptr) override;
        size_t offset(void* ptr) const override;
        void* ptrFromOffset(size_t offset) const override;
    private:
        size_t m_alignment;
        ByteRange m_range;
        uintptr_t m_currentPosition;
        uintptr_t m_oldestAllocationPosition;
        size_t m_tailSize;
    };
}
