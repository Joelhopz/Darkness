#pragma once

#include "Vector3.h"
#include "Vector2.h"

namespace engine
{
    template <typename T>
    class Vector4
    {
    public:
        T x;
        T y;
        T z;
        T w;

        Vector2<T> xy() const { return { x, y }; }
        Vector2<T> xz() const { return { x, z }; }
        Vector2<T> yx() const { return { y, x }; }
        Vector2<T> yz() const { return { y, z }; }
        Vector2<T> zx() const { return { z, x }; }
        Vector2<T> zy() const { return { z, y }; }

        Vector3<T> xyz() const { return { x, y, z }; }
        Vector3<T> xzy() const { return { x, z, y }; }
        Vector3<T> yxz() const { return { y, x, z }; }
        Vector3<T> yzx() const { return { y, z, x }; }
        Vector3<T> zxy() const { return { z, x, y }; }
        Vector3<T> zyx() const { return { z, y, x }; }

        Vector4<T> xyzw() const { return { x, y, z, w }; }
        Vector4<T> xzyw() const { return { x, z, y, w }; }
        Vector4<T> yxzw() const { return { y, x, z, w }; }
        Vector4<T> yzxw() const { return { y, z, x, w }; }
        Vector4<T> zxyw() const { return { z, x, y, w }; }
        Vector4<T> zyxw() const { return { z, y, x, w }; }

        Vector4<T> xywz() const { return { x, y, w, z }; }
        Vector4<T> xzwy() const { return { x, z, w, y }; }
        Vector4<T> yxwz() const { return { y, x, w, z }; }
        Vector4<T> yzwx() const { return { y, z, w, x }; }
        Vector4<T> zxwy() const { return { z, x, w, y }; }
        Vector4<T> zywx() const { return { z, y, w, x }; }

        Vector4<T> xwyz() const { return { x, w, y, z }; }
        Vector4<T> xwzy() const { return { x, w, z, y }; }
        Vector4<T> ywxz() const { return { y, w, x, z }; }
        Vector4<T> ywzx() const { return { y, w, z, x }; }
        Vector4<T> zwxy() const { return { z, w, x, y }; }
        Vector4<T> zwyx() const { return { z, w, y, x }; }

        Vector4<T> wxyz() const { return { w, x, y, z }; }
        Vector4<T> wxzy() const { return { w, x, z, y }; }
        Vector4<T> wyxz() const { return { w, y, x, z }; }
        Vector4<T> wyzx() const { return { w, y, z, x }; }
        Vector4<T> wzxy() const { return { w, z, x, y }; }
        Vector4<T> wzyx() const { return { w, z, y, x }; }

        void xy(const Vector2<T>& xy) { x = xy.x; y = xy.y; }
        void xyz(const Vector3<T>& xyz) { x = xyz.x; y = xyz.y; z = xyz.z; }
        void xyzw(const Vector4<T>& xyzw) { x = xyzw.x; y = xyzw.y; z = xyzw.z; w = xyzw.w; }

        Vector4()
            : x{}
            , y{}
            , z{}
            , w{}
        {}

        Vector4(T x, T y, T z, T w)
            : x{ x }
            , y{ y }
            , z{ z }
            , w{ w }
        {}

        Vector4(Vector3<T> vec, T w)
            : x{ vec.x }
            , y{ vec.y }
            , z{ vec.z }
            , w{ w }
        {}
        
        Vector4(Vector2<T> vec, T z, T w)
            : x{ vec.x }
            , y{ vec.y }
            , z{ z }
            , w{ w }
        {}

        T* operator&()
        {
            return &x;
        }

        T& operator[](int index)
        {
            return *(&x + index);
        }

        Vector4 operator+(const Vector4& vec) const
        {
            return{ x + vec.x, y + vec.y, z + vec.z, w + vec.w };
        }
        
        Vector4 operator-(const Vector4& vec) const
        {
            return{ x - vec.x, y - vec.y, z - vec.z, w - vec.w };
        }

        Vector4 operator*(const Vector4& vec) const
        {
            return{ x * vec.x, y * vec.y, z * vec.z, w * vec.w };
        }

        Vector4 operator/(const Vector4& vec) const
        {
            return{ x / vec.x, y / vec.y, z / vec.z, w / vec.w };
        }

        Vector4 operator+(T val) const
        {
            return{ x + val, y + val, z + val, w + val };
        }

        Vector4 operator-(T val) const
        {
            return{ x - val, y - val, z - val, w - val };
        }

        Vector4 operator*(T val) const
        {
            return{ x * val, y * val, z * val, w * val };
        }

        Vector4 operator/(T val) const
        {
            return{ x / val, y / val, z / val, w / val };
        }

        Vector4& operator+=(const Vector4& vec)
        {
            x += vec.x;
            y += vec.y;
            z += vec.z;
            w += vec.w;
            return *this;
        }

        Vector4& operator-=(const Vector4& vec)
        {
            x -= vec.x;
            y -= vec.y;
            z -= vec.z;
            w -= vec.w;
            return *this;
        }

        Vector4& operator*=(const Vector4& vec)
        {
            x *= vec.x;
            y *= vec.y;
            z *= vec.z;
            w *= vec.w;
            return *this;
        }

        Vector4& operator/=(const Vector4& vec)
        {
            x /= vec.x;
            y /= vec.y;
            z /= vec.z;
            w /= vec.w;
            return *this;
        }

        Vector4& operator+=(T val)
        {
            x += val;
            y += val;
            z += val;
            w += val;
            return *this;
        }

        Vector4& operator-=(T val)
        {
            x -= val;
            y -= val;
            z -= val;
            w -= val;
            return *this;
        }

        Vector4& operator*=(T val)
        {
            x *= val;
            y *= val;
            z *= val;
            w *= val;
            return *this;
        }

        Vector4& operator/=(T val)
        {
            x /= val;
            y /= val;
            z /= val;
            w /= val;
            return *this;
        }

        bool operator==(const Vector4& other) const
        {
            return (x == other.x) &&
                   (y == other.y) &&
                   (z == other.z) &&
                   (w == other.w);
        }

        bool operator!=(const Vector4& other) const
        {
            return !(*this == other);
        }

        T dot(const Vector4& vec)
        {
            return{ x * vec.x + 
                    y * vec.y + 
                    z * vec.z +
                    w * vec.w };
        }
    };

    using Vector4f = Vector4<float>;
}
