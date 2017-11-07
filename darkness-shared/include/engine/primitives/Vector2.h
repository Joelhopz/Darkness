#pragma once

namespace engine
{
    template <typename T>
    class Vector2
    {
    public:
        T x;
        T y;

        Vector2()
            : x{}
            , y{}
        {}

        Vector2(T x, T y)
            : x{ x }
            , y{ y }
        {}

        T* operator&()
        {
            return &x;
        }

        T& operator[](int index)
        {
            return *(&x + index);
        }

        Vector2 operator+(const Vector2& vec) const
        {
            return{ x + vec.x, y + vec.y };
        }

        Vector2 operator-(const Vector2& vec) const
        {
            return{ x - vec.x, y - vec.y };
        }

        Vector2 operator*(const Vector2& vec) const
        {
            return{ x * vec.x, y * vec.y };
        }

        Vector2 operator/(const Vector2& vec) const
        {
            return{ x / vec.x, y / vec.y };
        }

        Vector2 operator+(T val) const
        {
            return{ x + val, y + val };
        }

        Vector2 operator-(T val) const
        {
            return{ x - val, y - val };
        }

        Vector2 operator*(T val) const
        {
            return{ x * val, y * val };
        }

        Vector2 operator/(T val) const
        {
            return{ x / val, y / val };
        }

        Vector2& operator+=(const Vector2& vec)
        {
            x += vec.x;
            y += vec.y;
            return *this;
        }

        Vector2& operator-=(const Vector2& vec)
        {
            x -= vec.x;
            y -= vec.y;
            return *this;
        }

        Vector2& operator*=(const Vector2& vec)
        {
            x *= vec.x;
            y *= vec.y;
            return *this;
        }

        Vector2& operator/=(const Vector2& vec)
        {
            x /= vec.x;
            y /= vec.y;
            return *this;
        }

        Vector2& operator+=(T val)
        {
            x += val;
            y += val;
            return *this;
        }

        Vector2& operator-=(T val)
        {
            x -= val;
            y -= val;
            return *this;
        }

        Vector2& operator*=(T val)
        {
            x *= val;
            y *= val;
            return *this;
        }

        Vector2& operator/=(T val)
        {
            x /= val;
            y /= val;
            return *this;
        }

        bool operator==(const Vector2& other) const
        {
            return (x == other.x) &&
                   (y == other.y);
        }

        bool operator!=(const Vector2& other) const
        {
            return !(*this == other);
        }

        T dot(const Vector2& vec)
        {
            return{ x * vec.x +
                    y * vec.y };
        }

    };

    using Vector2f = Vector2<float>;
}
