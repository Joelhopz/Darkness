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

		Vector3<T> xyz() const
		{
			return { x, y, z };
		}

        void xyz(const Vector3<T>& xyz)
        {
            x = xyz.x;
            y = xyz.y;
            z = xyz.z;
        }

		Vector2<T> xy() const
		{
			return { x, y };
		}

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
