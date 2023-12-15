#pragma once

#include "../src/headers.h"

namespace math {

    template <typename T>
    double operator*( const T& vec1, const T& vec2 )
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

    template <typename T>
    inline float radToDeg(T radians)
    {
        return radians * 180.0 / M_PI;
    }

    template <typename T>
    inline float getLength( const T& vec )
    {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y);
    }

    template <typename T>
    inline float getLengthSqr( const T& vec )
    {
        return vec.x * vec.x + vec.y * vec.y;
    }

    template <typename T>
    inline T getNormalized( const T& vec )
    {
        const double length = getLength( vec );
        const double newX   = vec.x / length;
        const double newY   = vec.y / length;

        return T( newX, newY );
    }

    template <typename T>
    inline T& normalize( T& vec )
    {
        const double length = getLength( vec );
        vec.x /= length;
        vec.y /= length;

        return vec;
    }

    template <typename T>
    inline float getAngleBetween( const T& vec1, const T& vec2 )
    {
        const float angle = std::acos((vec1 * vec2) / (getLength(vec1) * getLength(vec2)));

        return radToDeg(angle);
    }
}
