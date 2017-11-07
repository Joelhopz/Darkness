#pragma once

#include "tools/Allocator.h"
#include "tools/ByteRange.h"

namespace tools
{
    class RingBuffer : public OffsetAllocatorInterface
    {
    public:
        RingBuffer() = default;
        RingBuffer(ByteRange range);

        void* allocate(size_t bytes) override;
        void* allocate(size_t bytes, size_t align) override;
        void free(void* ptr) override;
        size_t offset(void* ptr) const override;
    private:
        ByteRange m_range;
        uintptr_t m_currentPosition;
        uintptr_t m_oldestAllocationPosition;
        size_t m_tailSize;
    };
}
