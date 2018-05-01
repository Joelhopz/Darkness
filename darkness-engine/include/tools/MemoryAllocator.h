#pragma once

#include "tools/Allocator.h"
#include "tools/ByteRange.h"
#include "tools/ToolsCommon.h"
#include <vector>
#include <functional>
#include <map>
#include <unordered_map>

namespace tools
{
    constexpr size_t BiggestAlignment = pow<20>(2);
    class MemoryAllocator : public OffsetAllocatorInterface
    {
    public:
        MemoryAllocator() = default;
        MemoryAllocator(ByteRange range, size_t biggestAlignment = BiggestAlignment);

        void* allocate(size_t bytes) override;
        void* allocate(size_t bytes, size_t align) override;
        void free(void* ptr) override;
        size_t offset(void* ptr) const override;
        void* ptrFromOffset(size_t offset) const override;

    public:
        // for testing purposes
        std::vector<std::vector<ByteRange>>& reservedAllocations() { return m_reservedAllocations; }
        std::vector<std::unordered_map<uintptr_t, ByteRange>>& freeAllocations() { return m_freeAllocations; }

    private:
        ByteRange m_originalRange;
        size_t m_biggestAlignment;

        ByteRange getClosestRange(size_t bytes, size_t align);
        void insertBack(ByteRange&& range, size_t slot);

        size_t bestOptionalAlignment(size_t size);

        std::vector<std::vector<ByteRange>> m_reservedAllocations;
        std::vector<std::unordered_map<uintptr_t, ByteRange>> m_freeAllocations;

        std::map<const void*, short> m_reserveInfo;
    };
}
