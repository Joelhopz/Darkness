#pragma once

#include <cstdint>
#include <vector>

namespace tools
{
    class ByteRange
    {
    public:
        ByteRange()
            : start{ nullptr }
            , stop{ nullptr }
            , elementSize{ 0 }
        {}
        template<typename T>
        ByteRange(std::vector<T>& range)
            : start{ reinterpret_cast<const uint8_t*>(range.data()) }
            , stop{ start + (range.size() * sizeof(T)) }
            , elementSize{ sizeof(T) }
        {}

        template<typename T>
        ByteRange(T* start, T* stop)
            : start{ reinterpret_cast<const uint8_t*>(start) }
            , stop{ reinterpret_cast<const uint8_t*>(stop) }
            , elementSize{ sizeof(T) }
        {}

        template<typename T>
        ByteRange(T& start, T& stop)
            : start{ reinterpret_cast<const uint8_t*>(start) }
            , stop{ reinterpret_cast<const uint8_t*>(stop) }
            , elementSize{ sizeof(T) }
        {}

        template<typename T>
        ByteRange(const T* start, const T* stop)
            : start{ reinterpret_cast<const uint8_t*>(start) }
            , stop{ reinterpret_cast<const uint8_t*>(stop) }
            , elementSize{ sizeof(T) }
        {}

        template<typename T>
        ByteRange(const T& start, const T& stop)
            : start{ reinterpret_cast<const uint8_t*>(start) }
            , stop{ reinterpret_cast<const uint8_t*>(stop) }
            , elementSize{ sizeof(T) }
        {}

        ByteRange(const ByteRange& range)
            : start{ range.start }
            , stop{ range.stop }
            , elementSize{ range.elementSize }
        {}
        ByteRange& operator=(const ByteRange& range)
        {
            start = range.start;
            stop = range.stop;
            elementSize = range.elementSize;
            return *this;
        }
        size_t size() const
        {
            return stop - start;
        }
        const uint8_t* start;
        const uint8_t* stop;
        size_t elementSize;
    };
}
