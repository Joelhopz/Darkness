#include "tools/MemoryAllocator.h"
#include "tools/ToolsCommon.h"
#include "tools/Debug.h"
#include <memory>
#include <algorithm>
#include <limits>

namespace tools
{
    MemoryAllocator::MemoryAllocator(ByteRange range, size_t biggestAlignment)
        : m_originalRange{ range }
        , m_biggestAlignment{ biggestAlignment }
    {
        m_freeAllocations.resize(alignToIndex(m_biggestAlignment)+1);
        m_reservedAllocations.resize(alignToIndex(m_biggestAlignment)+1);

        auto alignIndex = alignToIndex(m_biggestAlignment);
        m_freeAllocations[alignIndex][roundUpToMultiple(range.start, m_biggestAlignment)] = ByteRange{
            roundUpToMultiple(range.start, m_biggestAlignment),
            roundDownToMultiple(range.stop, m_biggestAlignment)
        };
    }

    size_t MemoryAllocator::bestOptionalAlignment(size_t size)
    {
        size_t alignment = 1;
        size_t bestAlignmentSoFar = m_biggestAlignment;
        size_t bestSize = std::numeric_limits<size_t>::max();
        size_t lowestFreeAlignment = std::numeric_limits<size_t>::max();
        bool canFit = false;
        bool someFree = false;

        for (auto&& free : m_freeAllocations)
        {
            size_t freeSize = free.size();
            if (freeSize > 0)
            {
                // 1. if there is a free allocation that's alignment is the same as the size of the block, return it
                if (size == alignment && (*free.begin()).second.size() == size)
                {
                    return alignment;
                }

                // 2. the smallest free block that can fit size
                auto firstEntrySize = (*free.begin()).second.size();
                if (firstEntrySize < bestSize && firstEntrySize >= size)
                {
                    bestAlignmentSoFar = alignment;
                    bestSize = (*free.begin()).second.size();
                    canFit = true;
                }

                // 3. did not find perfect fit or first element that fits
                //    so returning lowest alignment that has free space
                if (alignment < lowestFreeAlignment)
                {
                    lowestFreeAlignment = alignment;
                    someFree = true;
                }
            }
            alignment <<= 1;
        }

        if (canFit)
            return bestAlignmentSoFar;
        if (someFree)
            return lowestFreeAlignment;

        // if nothing else works
        // return something sane
        return 8;
    }

    struct MatchBlockWithEnoughSpace
    {
        MatchBlockWithEnoughSpace(size_t space) : m_space(space) {}
        bool operator()(const std::pair<const uint8_t*, ByteRange>& v) const
        {
            return v.second.size() >= m_space;
        }
    private:
        const size_t m_space;
    };

    ByteRange MemoryAllocator::getClosestRange(size_t bytes, size_t align)
    {
        auto index = alignToIndex(align);
        auto alignedBytes = roundUpToMultiple(bytes, align);

        std::map<const uint8_t*, ByteRange>& map = m_freeAllocations[index];
        auto block = std::find_if(map.begin(), map.end(), MatchBlockWithEnoughSpace(alignedBytes));
        if (block != map.end())
        {
            // we found a good block on this alignment request
            ByteRange newRange = block->second;
            if (alignedBytes == newRange.size())
            {
                map.erase(newRange.start);
            }
            else
            {
                ByteRange leftOver = newRange;

                newRange.stop = newRange.start + alignedBytes;
                leftOver.start = newRange.stop;

                map.erase(block);
                map[leftOver.start] = leftOver;
            }
            //m_reservedAllocations[index].emplace_back(newRange);
            return newRange;
        }

        // we did not find a good block
        // find a good block from higher
        if (align < m_biggestAlignment)
        {
            auto higherBlock = getClosestRange(bytes, align << 1);

            if (higherBlock.size() > alignedBytes)
            {
                ByteRange leftOver = higherBlock;
                higherBlock.stop = higherBlock.start + alignedBytes;
                leftOver.start = higherBlock.stop;
                m_freeAllocations[index][leftOver.start] = leftOver;
                //m_reserveInfo[leftOver.start] = static_cast<short>(index);
            }

            return higherBlock;
        }
        
        // we ran out of memory
        // try to compress all free memory blocks
        bool managedToCompressSomething = false;
        //for (auto&& list : m_freeAllocations)
        for (int i = 0; i < m_freeAllocations.size(); ++i)
        {
            auto&& list = m_freeAllocations[i];
            bool found = true;
            while (found)
            {
                found = false;
                //for (auto&& kvA : list)
                for(auto&& kvA = list.begin(); kvA != list.end(); ++kvA)
                {
                    for (int sa = 0; sa < m_freeAllocations.size(); ++sa)
                    {
                        auto&& listB = m_freeAllocations[sa];
                        //for (auto&& kvB : listB)
                        for(auto&& kvB = listB.begin(); kvB != listB.end(); ++kvB)
                        {
                            if (kvA->second.start == kvB->second.stop)
                            {
                                ByteRange range;
                                range = kvB->second;
                                range.stop = kvA->second.stop;
                                list.erase(kvA);
                                listB.erase(kvB);
                                listB[range.start] = range;
                                found = true;
                                managedToCompressSomething = true;
                                break;
                            }
                        }
                        if (found)
                            break;
                    }
                    if (found)
                        break;
                }
            }
        }
        ASSERT(managedToCompressSomething, "Could not compress anything. Ran out of memory");
        return getClosestRange(bytes, align);
    }

    void* MemoryAllocator::allocate(size_t bytes)
    {
        return allocate(bytes, bestOptionalAlignment(bytes));
    }

    void* MemoryAllocator::allocate(size_t bytes, size_t align)
    {
        ASSERT(align > 0, "Can not allocate 0 aligned memory");
        ASSERT(isPowerOfTwo(align), "This MemoryAllocator only supports alignments that are power of two");
        ASSERT(align <= m_biggestAlignment, "This instance of MemoryAllocator does not support alignments this high");
        auto closestRange = getClosestRange(bytes, align);
        auto index = alignToIndex(align);
        m_reservedAllocations[index].emplace_back(closestRange);
        //m_reserveInfo[closestRange.start] = static_cast<short>(index);
        return const_cast<void*>(static_cast<const void*>(closestRange.start));
    }

    struct MatchRangeStart
    {
        MatchRangeStart(const uint8_t* ptr) : m_ptr(ptr) {}
        bool operator()(ByteRange& v) const
        {
            return v.start == m_ptr;
        }
    private:
        const uint8_t* m_ptr;
    };

    void MemoryAllocator::free(void* ptr)
    {
        /*auto info = m_reserveInfo[ptr];
        m_reserveInfo.erase(ptr);*/

        size_t slot = 0;
        bool foundAllocation = false;
        for (auto&& alignList : m_reservedAllocations)
        {
            auto block = std::find_if(alignList.begin(), alignList.end(), MatchRangeStart(static_cast<const uint8_t*>(ptr)));
            //ASSERT(block != m_reservedAllocations[info].end(), "MemoryAllocator internal error");
            if (block != alignList.end())
            {
                ByteRange range = *block;
                alignList.erase(block);
                insertBack(std::move(range), slot);
                foundAllocation = true;
                break;
            }
            ++slot;
        }
        ASSERT(foundAllocation, "Tried to free block that wasnt allocated");
    }

    size_t MemoryAllocator::offset(void* ptr) const
    {
        return reinterpret_cast<size_t>(ptr) - reinterpret_cast<size_t>(m_originalRange.start);
    }

    struct MatchRangeStop
    {
        MatchRangeStop(const uint8_t* ptr) : m_ptr(ptr) {}
        bool operator()(const std::pair<const uint8_t*, ByteRange>& v) const
        {
            return v.second.stop == m_ptr;
        }
    private:
        const uint8_t* m_ptr;
    };

    void MemoryAllocator::insertBack(ByteRange&& range, size_t slot)
    {
        std::map<const uint8_t*, ByteRange>& map = m_freeAllocations[slot];
        auto afterBlock = map.find(range.stop);
        if (afterBlock != map.end())
        {
            auto beforeBlock = std::find_if(map.begin(), map.end(), MatchRangeStop(range.start));
            if (beforeBlock != map.end())
            {
                if (slot + 1 < m_freeAllocations.size())
                {
                    ByteRange newRange{ beforeBlock->second.start, afterBlock->second.stop };
                    map.erase(range.stop);
                    map.erase(newRange.start);
                    insertBack(std::move(newRange), slot + 1);
                    return;
                }
                else
                {
                    beforeBlock->second.stop = afterBlock->second.stop;
                    map.erase(afterBlock->second.start);
                    return;
                }
            }
            else
            {
                if (slot + 1 < m_freeAllocations.size())
                {
                    ByteRange newRange{ range.start, afterBlock->second.stop };
                    map.erase(range.stop);
                    insertBack(std::move(newRange), slot + 1);
                    return;
                }
                else
                {
                    afterBlock->second.start = range.start;
                    return;
                }
            }
        }
        else
        {
            auto beforeBlock = std::find_if(map.begin(), map.end(), MatchRangeStop(range.start));
            if (beforeBlock != map.end())
            {
                if (slot + 1 < m_freeAllocations.size())
                {
                    ByteRange newRange{ beforeBlock->second.start, range.stop };
                    map.erase(beforeBlock->second.start);
                    insertBack(std::move(newRange), slot + 1);
                    return;
                }
                else
                {
                    beforeBlock->second.stop = range.stop;
                    return;
                }
            }
        }
        /*if (range.size() >= indexToAlign(slot) * 2 && (slot + 1 < m_freeAllocations.size()))
            insertBack(std::move(range), slot + 1);
        else*/
            map[range.start] = range;
    }

}

/*1
2
4
8
16
32
64
128
256
512
1024
2048
4096
8192
16384
32768
65536
131072
262144
524288
1048576
2097152
4194304
8388608
16777216
*/