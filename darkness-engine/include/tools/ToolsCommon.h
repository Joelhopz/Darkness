#pragma once

#include <type_traits>
#include <cstdint>

template<class T>
inline constexpr T pow(const T, std::integral_constant<T, 0>) {
    return 1;
}

template<class T, int N>
inline constexpr T pow(const T x, std::integral_constant<T, N>) {
    return pow(x, std::integral_constant<T, N - 1>()) * x;
}

template<size_t N, class T>
inline constexpr T pow(const T x)
{
    return pow(x, std::integral_constant<T, N>());
}

size_t alignToIndex(size_t align);
size_t indexToAlign(size_t index);
bool isPowerOfTwo(size_t align);

template<typename T>
const T roundUpToMultiple(const T& value, size_t align)
{
    return static_cast<const T>(((static_cast<const size_t>(value) + static_cast<const int>(value >= 0) * (align - 1)) / align) * align);
}

template<typename T>
const T* roundUpToMultiple(const T* value, size_t align)
{
    return reinterpret_cast<const T*>(((reinterpret_cast<const size_t>(value) + static_cast<const int>(value >= 0) * (align - 1)) / align) * align);
}

template<typename T>
const T roundDownToMultiple(const T& value, size_t align)
{
    return roundUpToMultiple(value, align) - align;
}
