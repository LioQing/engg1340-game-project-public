#pragma once

#include <iostream>
#include <cmath>

/**
 * @brief A 2D vector class
 * 
 * @tparam T Type for storing the coordinates
 */
template <typename T>
struct Vec2
{
    T x = 0.0;
    T y = 0.0;

    /**
     * @brief Construct a new Vec2 at (0, 0)
     * 
     */
    Vec2() = default;

    /**
     * @brief Construct a new Vec2 from another Vec2
     * 
     */
    Vec2(const Vec2&) = default;

    /**
     * @brief Construct a new Vec2 given the coordinates
     * 
     * @param x X coordinate
     * @param y Y coordinate
     */
    Vec2(T x, T y) : x(x), y(y) {}

    /**
     * @brief Cast to Vec2\<U\>
     * 
     * @tparam U The type to cast to
     * @return Vec2\<U\> The casted Vec2
     */
    template <typename U>
    operator Vec2<U>() const
    {
        return Vec2<U>(x, y);
    }

    /**
     * @brief Cast to Vec2\<U\>
     * 
     * @tparam U The type to cast to
     * @return Vec2\<U\> The casted Vec2
     */
    template <typename U>
    Vec2<U> Cast() const
    {
        return Vec2<U>(x, y);
    }

    /**
     * @brief Set the coordinates of this
     * 
     * @param x X coordinate
     * @param y Y coordinate
     * @return Vec2& This Vec2
     */
    Vec2& Set(T x, T y) &
    {
        this->x = x;
        this->y = y;
        return *this;
    }

    /**
     * @brief Get the rotated Vec2 of v by theta radian
     * 
     * @param v The Vec2 to be rotated
     * @param theta The angle to be rotated in radian
     * @return Vec2 The Vec2 rotated
     */
    template <typename U>
    static Vec2 Rotated(Vec2 v, U theta)
    {
        return v.Rotated(theta);
    }

    /**
     * @brief Get the rotated Vec2 of this by theta radian
     * 
     * @param theta The angle to be rotated in radian
     * @return Vec2 The Vec2 rotated
     */
    template <typename U>
    Vec2<T> Rotated(U theta) const
    {
        U c = std::cos(theta);
        U s = std::sin(theta);
        T tx = c * x - s * y;
        T ty = s * x + c * y;

        return Vec2<T>(tx, ty);
    }

    /**
     * @brief Rotate this by theta radian
     * 
     * @param theta The angle to be rotated in radian
     */
    template <typename U>
    Vec2& Rotate(U theta) &
    {
        *this = Rotated(theta);

        return *this;
    }

    /**
     * @brief Get the normalized Vec2 of v
     * 
     * @param v The Vec2 to be normalized
     * @return Vec2 The normalized Vec2
     */
    static Vec2 Normalized(const Vec2& v) 
    {
        return v.Normalized();
    }

    /**
     * @brief Get the normalized Vec2 of this
     * 
     * @return Vec2 The normalized Vec2
     */
    Vec2 Normalized() const
    {
        if (Magnitude() == 0) return *this;
        return Vec2(*this / Magnitude());
    }

    /**
     * @brief Normalize this
     * 
     * @return Vec2& This Vec2
     */
    Vec2& Normalize() &
    {
        if (Magnitude() == 0) return *this;
        *this *= (1.0 / Magnitude());
        return *this;
    }

    /**
     * @brief Get the orthogonal Vec2 of v
     * 
     * @param v The Vec2 to be used
     * @return Vec2 The Vec2 orthogonal to v
     */
    static Vec2 Ortho(const Vec2& v)
    {
        return v.Ortho();
    }

    /**
     * @brief Get the orthogonal Vec2 of this
     * 
     * @return Vec2 The Vec2 orthogonal to this
     */
    Vec2 Ortho() const
    {
        return Vec2(y, -x);
    }

    /**
     * @brief Get the distance between v1 and v2
     * 
     * @param v1 The first Vec2
     * @param v2 The second Vec2
     * @return auto The distance
     */
    template <typename U = T>
    static auto Distance(const Vec2& v1, const Vec2<U>& v2)
    {
        return v1.Distance(v2);
    }

    /**
     * @brief Get the distance between this and v
     * 
     * @param v The Vec2 to be used with this
     * @return auto The distance
     */
    template <typename U = T>
    auto Distance(const Vec2<U>& v) const
    {
        Vec2<decltype(std::declval<U&>() - std::declval<T&>())> d(v.x - x, v.y - y);
        return d.Magnitude();
    }

    /**
     * @brief Get the square of magnitude of v
     * 
     * @param v The Vec2 to be used
     * @return auto The square of magnitude
     */
    static auto SqrMagnitude(const Vec2& v)
    {
        return v.SqrMagnitude();
    }

    /**
     * @brief Get the square of magnitude of this
     * 
     * @return auto The square of magnitude
     */
    auto SqrMagnitude() const
    {
        return x * x + y * y;
    }

    /**
     * @brief Get the magnitude of v
     * 
     * @param v The Vec2 to be used
     * @return auto The magnitude
     */
    static auto Magnitude(const Vec2& v)
    {
        return v.Magnitude();
    }

    /**
     * @brief Get the magnitude of this
     * 
     * @return auto The magnitude
     */
    auto Magnitude() const
    {
        return std::hypot(x, y);
    }

    /**
     * @brief Get the absolute values of v
     * 
     * @param v The Vec2 to be used
     * @return Vec2 The absolute values
     */
    static Vec2 Abs(const Vec2& v)
    {
        return v.Abs();
    }

    /**
     * @brief Get the absolute values of this
     * 
     * @return Vec2 The absolute values
     */
    Vec2 Abs() const
    {
        return Vec2(x < 0 ? x * -1 : x, y < 0 ? y * -1 : y);
    }

    /**
     * @brief Get the dot product of v1 and v2
     * 
     * @param v1 The first Vec2
     * @param v2 The second Vec2
     * @return auto The dot product
     */
    template <typename U = T>
    static auto Dot(const Vec2& v1, const Vec2<U>& v2)
    {
        return v1.Dot(v2);
    } 

    /**
     * @brief Get the dot product of this and v
     * 
     * @param v The Vec2 to be used
     * @return auto The dot product
     */
    template <typename U = T>
    auto Dot(const Vec2<U>& v) const
    {
        return x * v.x + y * v.y;
    }

    /**
     * @brief Get the cross product of v1 and v2
     * 
     * @param v1 The first Vec2
     * @param v2 The second Vec2
     * @return auto The cross product
     */
    template <typename U = T>
    static auto Cross(const Vec2& v1, const Vec2<U>& v2)
    {
        return v1.Cross(v2);
    } 

    /**
     * @brief Get the cross product of this and v
     * 
     * @param v The Vec2 to be used
     * @return auto The cross product
     */
    template <typename U = T>
    auto Cross(const Vec2<U>& v) const
    {
        return (x * v.y) - (y * v.x);
    }

    /**
     * @brief Get the Hadamard (element-wise) multiplication product of v1 and v2
     * 
     * @param v1 The first Vec2
     * @param v2 The second Vec2
     * @return Vec2 The Hadamard multiplication product
     */
    template <typename U = T>
    static auto HadamardMultiplication(const Vec2& v1, const Vec2<U>& v2)
    {
        return v1.HadamardMultiplication(v2);
    }

    /**
     * @brief Get the Hadamard (element-wise) multiplication product of this and v
     * 
     * @param v The Vec2 to be used
     * @return Vec2 The Hadamard multiplication product
     */
    template <typename U = T>
    auto HadamardMultiplication(const Vec2<U>& v) const
    {
        return Vec2<decltype(std::declval<T&>() * std::declval<U&>())>(x * v.x, y * v.y);
    }

    /**
     * @brief Get the Hadamard (element-wise) division quotient of v1 and v2
     * 
     * @param v1 The first Vec2
     * @param v2 The second Vec2
     * @return Vec2 The Hadamard division quotient
     */
    template <typename U = T>
    static auto HadamardDivision(const Vec2& v1, const Vec2<U>& v2)
    {
        return v1.HadamardDivision(v2);
    }

    /**
     * @brief Get the Hadamard (element-wise) division quotient of this and v
     * 
     * @param v The Vec2 to be used
     * @return Vec2 The Hadamard division quotient
     */
    template <typename U = T>
    auto HadamardDivision(const Vec2<U>& v) const
    {
        return Vec2<decltype(std::declval<T&>() / std::declval<U&>())>(x / v.x, y / v.y);
    }

    /**
     * @brief Get the Hadamard (element-wise) modulo remainder of v1 and v2
     * 
     * @param v1 The first Vec2
     * @param v2 The second Vec2
     * @return Vec2 The Hadamard modulo remainder
     */
    template <typename U = T>
    static auto HadamardModulo(const Vec2& v1, const Vec2<U>& v2)
    {
        return v1.HadamardModulo(v2);
    }

    /**
     * @brief Get the Hadamard (element-wise) modulo remainder of this and v
     * 
     * @param v The Vec2 to be used
     * @return Vec2 The Hadamard modulo remainder
     */
    template <typename U = T>
    auto HadamardModulo(const Vec2<U>& v) const
    {
        return Vec2<decltype(std::fmod(std::declval<T&>(), std::declval<U&>()))>(std::fmod(x, v.x), std::fmod(y, v.y));
    }

    /**
     * @brief Get the orientation of v1, v2, v3. Equivalent to v3.Cross(v2 - v1).
     * 0: collinear.
     * >0: v3 to rotate clockwise to become collinear
     * <0: v3 to rotate anticlockwise to become collinear
     * 
     * @param v1 The first Vec2
     * @param v2 The second Vec2
     * @param v3 The third Vec2
     * @return auto The orientation
     */
    template <typename U = T, typename S = T>
    static auto OrientationEx(const Vec2& v1, const Vec2<U>& v2, const Vec2<S>& v3)
    {
        return v1.OrientationEx(v2, v3);
    }

    /**
     * @brief Get the orientation of this, v2, v3. Equivalent to v3.Cross(v2 - *this).
     * 0: collinear.
     * >0: v3 to rotate clockwise to become collinear
     * <0: v3 to rotate anticlockwise to become collinear
     * 
     * @param v2 The second Vec2
     * @param v3 The third Vec2
     * @return auto The orientation
     */
    template <typename U = T, typename S = T>
    auto OrientationEx(const Vec2<U>& v2, const Vec2<S>& v3) const
    {
        return v3.Cross(v2 - *this);
    }

    /**
     * @brief Get the orientation of v1, v2, v3.
     * 0: collinear.
     * 1: v3 to rotate clockwise to become collinear
     * -1: v3 to rotate anticlockwise to become collinear
     * 
     * @param v1 The first Vec2
     * @param v2 The second Vec2
     * @param v3 The third Vec2
     * @return int The orientation
     */
    template <typename U = T, typename S = T>
    static int Orientation(const Vec2& v1, const Vec2<U>& v2, const Vec2<S>& v3)
    {
        return v1.Orientation(v2, v3);
    }

    /**
     * @brief Get the orientation of this, v2, v3.
     * 0: collinear.
     * 1: v3 to rotate clockwise to become collinear
     * -1: v3 to rotate anticlockwise to become collinear
     * 
     * @param v2 The second Vec2
     * @param v3 The third Vec2
     * @return int The orientation
     */
    template <typename U = T, typename S = T>
    int Orientation(const Vec2<U>& v2, const Vec2<S>& v3) const
    {
        auto val = OrientationEx(v2, v3);

        if (val == 0) return 0;
        return (val > 0) ? 1 : -1;
    }

    static const Vec2 One;
    static const Vec2 Zero;
    static const Vec2 Up;
    static const Vec2 Down;
    static const Vec2 Right;
    static const Vec2 Left;
    static const Vec2 i;
    static const Vec2 j;
};

template <typename T> const Vec2<T> Vec2<T>::One   = Vec2<T>(1, 1);
template <typename T> const Vec2<T> Vec2<T>::Zero  = Vec2<T>(0, 0);
template <typename T> const Vec2<T> Vec2<T>::Up    = Vec2<T>(0, 1);
template <typename T> const Vec2<T> Vec2<T>::Down  = Vec2<T>(0, -1);
template <typename T> const Vec2<T> Vec2<T>::Right = Vec2<T>(1, 0);
template <typename T> const Vec2<T> Vec2<T>::Left  = Vec2<T>(-1, 0);
template <typename T> const Vec2<T> Vec2<T>::i     = Vec2<T>(1, 0);
template <typename T> const Vec2<T> Vec2<T>::j     = Vec2<T>(0, 1);

template <typename T, typename U>
auto operator+(const Vec2<T>& v1, const Vec2<U>& v2)
{
    return Vec2<decltype(std::declval<T&>() + std::declval<U&>())>(v1.x + v2.x, v1.y + v2.y);
}
template <typename T, typename U>
auto operator-(const Vec2<T>& v1, const Vec2<U>& v2)
{
    return Vec2<decltype(std::declval<T&>() - std::declval<U&>())>(v1.x - v2.x, v1.y - v2.y);
}
template <typename T, typename U>
auto operator*(const Vec2<T>& v1, const Vec2<U>& v2)
{
    return Vec2<decltype(std::declval<T&>() * std::declval<U&>())>(v1.x * v2.x, v1.y * v2.y);
}
template <typename T, typename U>
auto operator/(const Vec2<T>& v1, const Vec2<U>& v2)
{
    return Vec2<decltype(std::declval<T&>() / std::declval<U&>())>(v1.x / v2.x, v1.y / v2.y);
}
template <typename T, typename U>
auto operator%(const Vec2<T>& v1, const Vec2<U>& v2)
{
    return Vec2<decltype(std::fmod(std::declval<T&>(), std::declval<U&>()))>(std::fmod(v1.x, v2.x), std::fmod(v1.y, v2.y));
}

template <typename T, typename U>
Vec2<T>& operator+=(Vec2<T>& v1, const Vec2<U>& v2)
{
    return v1 = v1 + v2;
}
template <typename T, typename U>
Vec2<T>& operator-=(Vec2<T>& v1, const Vec2<U>& v2)
{
    return v1 = v1 - v2;
}
template <typename T, typename U>
Vec2<T>& operator*=(Vec2<T>& v1, const Vec2<U>& v2)
{
    return v1 = v1 * v2;
}
template <typename T, typename U>
Vec2<T>& operator/=(Vec2<T>& v1, const Vec2<U>& v2)
{
    return v1 = v1 / v2;
}
template <typename T, typename U>
Vec2<T>& operator%=(Vec2<T>& v1, const Vec2<U>& v2)
{
    return v1 = v1 % v2;
}

template <typename T, typename U>
auto operator*(U s, const Vec2<T>& v)
{
    return Vec2<decltype(s * std::declval<T&>())>(v.x * s, v.y * s);
}
template <typename T, typename U>
auto operator/(U s, const Vec2<T>& v)
{
    return Vec2<decltype(s / std::declval<T&>())>(s / v.x, s / v.y);
}
template <typename T, typename U>
auto operator%(U s, const Vec2<T>& v)
{
    return Vec2<decltype(std::fmod(std::declval<T&>(), s))>(std::fmod(s, v.x), std::fmod(s, v.y));
}

template <typename T, typename U>
auto operator*(const Vec2<T>& v, U s)
{
    return Vec2<decltype(s * std::declval<T&>())>(v.x * s, v.y * s);
}
template <typename T, typename U>
auto operator/(const Vec2<T>& v, U s)
{
    return Vec2<decltype(s / std::declval<T&>())>(v.x / s, v.y / s);
}
template <typename T, typename U>
auto operator%(const Vec2<T>& v, U s)
{
    return Vec2<decltype(std::fmod(s, std::declval<T&>()))>(std::fmod(v.x, s), std::fmod(v.y, s));
}

template <typename T, typename U>
Vec2<T>& operator*=(Vec2<T>& v, U s)
{
    return v = v * s;
}
template <typename T, typename U>
Vec2<T>& operator/=(Vec2<T>& v, U s)
{
    return v = v / s;
}
template <typename T, typename U>
Vec2<T>& operator%=(Vec2<T>& v, U s)
{
    return v = v % s;
}

template <typename T>
Vec2<T> operator-(const Vec2<T>& v)
{
    return Vec2<T>(-v.x, -v.y);
}
template <typename T>
Vec2<T> operator+(const Vec2<T>& v)
{
    return Vec2<T>(v.x, v.y);
}

template <typename T, typename U>
bool operator==(const Vec2<T>& v1, const Vec2<U>& v2)
{
    return v1.x == v2.x && v1.y == v2.y;
}
template <typename T, typename U>
bool operator!=(const Vec2<T>& v1, const Vec2<U>& v2)
{
    return !(v1.x == v2.x && v1.y == v2.y);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vec2<T>& v)
{
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

using Vec2d = Vec2<double>;
using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;
using Vec2u = Vec2<unsigned int>;