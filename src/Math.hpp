#pragma once
#include <math.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <assert.h>


struct Vec2 {
    float x;
    float y;
    static constexpr size_t len = 2;
    Vec2() = default;
    constexpr Vec2(float  x, float  y) : x(x), y(y) {}
    constexpr Vec2(int    x, int    y) : Vec2(static_cast<float>(x), static_cast<float>(y)) {}
    constexpr Vec2(double x, double y) : Vec2(static_cast<float>(x), static_cast<float>(y)) {}

    static constexpr Vec2 up()    { return Vec2( 0,  1); };
    static constexpr Vec2 down()  { return Vec2( 0, -1); };
    static constexpr Vec2 right() { return Vec2( 1,  0); };
    static constexpr Vec2 left()  { return Vec2(-1,  0); };
    static constexpr Vec2 zero()  { return Vec2( 0,  0); };
    static constexpr Vec2 one()   { return Vec2( 1,  1); };
    
    inline constexpr Vec2& operator+=(const Vec2& rhs) { x += rhs.x;  y += rhs.y; return *this; }
    inline constexpr Vec2& operator-=(const Vec2& rhs) { x -= rhs.x;  y -= rhs.y; return *this; }
    inline constexpr Vec2& operator*=(float       rhs) { x *= rhs;    y *= rhs;   return *this; }
    inline constexpr Vec2& operator/=(float       rhs) { x /= rhs;    y /= rhs;   return *this; }
};
inline constexpr Vec2 operator+(const Vec2& lhs,  const Vec2& rhs) { return Vec2(lhs.x + rhs.x, lhs.y + rhs.y); }
inline constexpr Vec2 operator-(const Vec2& lhs,  const Vec2& rhs) { return Vec2(lhs.x - rhs.x, lhs.y - rhs.y); }
inline constexpr Vec2 operator/(const Vec2& lhs,  float       rhs) { return Vec2(lhs.x / rhs,   lhs.y / rhs);   }
inline constexpr Vec2 operator*(const Vec2& lhs,  float       rhs) { return Vec2(lhs.x * rhs,   lhs.y * rhs);   }
inline constexpr Vec2 operator*(float       lhs,  const Vec2& rhs) { return Vec2(lhs   * rhs.x, lhs   * rhs.y); }
inline std::ostream& operator<<(std::ostream& os, const Vec2& vec) { os << vec.x << "," << vec.y; return os;    }

struct Vec3 {
    float x;
    float y;
    float z;
    static constexpr size_t len = 3;
    Vec3() = default;
    constexpr Vec3(float  x, float  y, float  z) : x(x), y(y), z(z) {}
    constexpr Vec3(int    x, int    y, int    z) : Vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)) {}
    constexpr Vec3(double x, double y, double z) : Vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)) {}
    
    static constexpr Vec3 zero()   { return Vec3( 0,  0,  0); };
    static constexpr Vec3 one()    { return Vec3( 1,  1,  1); };
    static constexpr Vec3 behind() { return Vec3( 0,  0,  1); };
    static constexpr Vec3 ahead()  { return Vec3( 0,  0, -1); };
    static constexpr Vec3 up()     { return Vec3( 0,  1,  0); };
    static constexpr Vec3 down()   { return Vec3( 0, -1,  0); };
    static constexpr Vec3 right()  { return Vec3( 1,  0,  0); };
    static constexpr Vec3 left()   { return Vec3(-1,  0,  0); };
    
    inline constexpr Vec3& operator+=(const Vec3& rhs) { x += rhs.x;  y += rhs.y; z += rhs.z; return *this; }
    inline constexpr Vec3& operator-=(const Vec3& rhs) { x -= rhs.x;  y -= rhs.y; z -= rhs.z; return *this; }
    inline constexpr Vec3& operator*=(float       rhs) { x *= rhs;    y *= rhs;   z *= rhs;   return *this; }
    inline constexpr Vec3& operator/=(float       rhs) { x /= rhs;    y /= rhs;   z /= rhs;   return *this; }
};
inline constexpr Vec3 operator+(const Vec3& lhs,  const Vec3& rhs) { return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
inline constexpr Vec3 operator-(const Vec3& lhs,  const Vec3& rhs) { return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
inline constexpr Vec3 operator/(const Vec3& lhs,  float       rhs) { return Vec3(lhs.x / rhs,   lhs.y / rhs,   lhs.z / rhs);   }
inline constexpr Vec3 operator*(const Vec3& lhs,  float       rhs) { return Vec3(lhs.x * rhs,   lhs.y * rhs,   lhs.z * rhs);   }
inline constexpr Vec3 operator*(float       lhs,  const Vec3& rhs) { return Vec3(lhs   * rhs.x, lhs   * rhs.y, lhs   * rhs.z); }
inline std::ostream& operator<<(std::ostream& os, const Vec3& vec) { os << vec.x << "," << vec.y << "," << vec.z; return os;   }


// assumes that for any given bounds lower < upper
// all math functions take degrees as input
namespace Math {
constexpr float DEFAULT_EPSILON = 0.000001f;
inline constexpr float clamp(float val, float lower, float upper) {
    assert(lower < upper);
    if (val < lower) return lower;
    if (val > upper) return upper;
    return val;
}
inline constexpr float clamp01(float val) {
    if (val < 0.00f) return 0.00f;
    if (val > 1.00f) return 1.00f;
    return val;
}
// compute linearly interpolated (symmetrical) value given, amount t and bounds a and b, with no limit to t
// note: garentees lerp(1, a, b) == b even for magnitudaly larger inputs such as t=1.0, a=1.0e20, b=1.0, of which the
//       algebraically simpler a + t * (b - a) will incorrectly compute 0.0 due to precision loss from subtracting first
inline constexpr float lerp(float t, float a, float b) {
    return (a * (1.00f - t)) + (b * t);
}
inline constexpr float inverseLerp(float v, float a, float b) {
    return (v - a) / (b - a);
}
inline constexpr float scaleToRange(float v, float inMin, float inMax, float outMin, float outMax) {
    return Math::lerp(Math::inverseLerp(v, inMin, inMax), outMin, outMax);
}

inline constexpr bool isApproximately(float a, float b, float epsilon=DEFAULT_EPSILON) {
    return (a == b) || (a > b && a - b <= epsilon) || (b > a && b - a <= epsilon);
}
inline constexpr bool isApproximately(const Vec2& a, const Vec2& b, float epsilon=DEFAULT_EPSILON) {
    return isApproximately(a.x, b.x, epsilon) && isApproximately(a.y, b.y, epsilon);
}
inline constexpr bool isApproximately(const Vec3& a, const Vec3& b, float epsilon=DEFAULT_EPSILON) {
    return isApproximately(a.x, b.x, epsilon) && isApproximately(a.y, b.y, epsilon) && isApproximately(a.z, b.z, epsilon);
}
inline constexpr float abs(float a)          { return a <= 0.00f? -a : a; }
inline constexpr float min(float a, float b) { return a <= b?      a : b; }
inline constexpr float max(float a, float b) { return a >= b?      a : b; }

inline constexpr float INF  = std::numeric_limits<float>::infinity();
inline constexpr float PI   = 3.14159265358979323846f;
inline constexpr float PI_2 = 1.57079632679489661923f;
inline constexpr float PI_4 = 0.78539816339744830962f;
inline constexpr float radToDeg(float radians) { return radians * (180.00f / PI);      }
inline constexpr float degToRad(float degrees) { return degrees * (PI      / 180.00f); }
inline float sin(float degrees)                { return std::sinf(degToRad(degrees));  }
inline float cos(float degrees)                { return std::cosf(degToRad(degrees));  }
inline float tan(float degrees)                { return std::tanf(degToRad(degrees));  }
inline float asin(float ratio)                 { return radToDeg(std::asinf(ratio));   }
inline float acos(float ratio)                 { return radToDeg(std::acosf(ratio));   }
inline float atan(float ratio)                 { return radToDeg(std::atanf(ratio));   }
inline constexpr float square(float a)         { return a * a;                         }
inline float squareRoot(float a)               { return std::sqrtf(a);                 }
inline float pow(float a, float b)             { return std::powf(a, b);               }
inline float roundToNearestInt(float a)        { return std::round(a);                 }
inline float roundToNearestDigit(float a, size_t num_digits) {
    size_t roundingMultiple = num_digits * 10;
    return roundToNearestInt(a * roundingMultiple) / roundingMultiple;
}

inline constexpr Vec3 cross(const Vec3& lhs, const Vec3& rhs) {
    return Vec3( (lhs.y * rhs.z - lhs.z * rhs.y),
                -(lhs.x * rhs.z - lhs.z * rhs.x),
                 (lhs.x * rhs.y - lhs.y * rhs.x) );
}
inline constexpr float dot(const Vec2& lhs, const Vec2& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y;                 }
inline constexpr float dot(const Vec3& lhs, const Vec3& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
inline constexpr float magnitudeSquared(const Vec2& vec)     { return vec.x * vec.x + vec.y * vec.y;                 }
inline constexpr float magnitudeSquared(const Vec3& vec)     { return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z; }
inline float magnitude(const Vec2& vec)                      { return sqrtf(magnitudeSquared(vec));                  }
inline float magnitude(const Vec3& vec)                      { return sqrtf(magnitudeSquared(vec));                  }
inline Vec2 normalize(const Vec2& vec)                       { return vec / Math::magnitude(vec);                    }
inline Vec3 normalize(const Vec3& vec)                       { return vec / Math::magnitude(vec);                    }
inline Vec2 direction(const Vec2& from, const Vec2& to)      { return Math::normalize(to - from);                    }
inline Vec3 direction(const Vec3& from, const Vec3& to)      { return Math::normalize(to - from);                    }
inline float distance(const Vec2& from, const Vec2& to)      { return Math::magnitude(to - from);                    }
inline float distance(const Vec3& from, const Vec3& to)      { return Math::magnitude(to - from);                    }

// compute the cosine [-1, 1] of the (shortest) angle between given vectors A and B
inline float cosineSimilarity(const Vec2& a, const Vec2& b) {
    return dot(a, b) / squareRoot(magnitudeSquared(a) * magnitudeSquared(b));
}
// compute the cosine [-1, 1] of the (shortest) angle between given vectors A and B
inline float cosineSimilarity(const Vec3& a, const Vec3& b) {
    return dot(a, b) / squareRoot(magnitudeSquared(a) * magnitudeSquared(b));
}

inline bool isNormalized(const Vec2& vec, float epsilon=DEFAULT_EPSILON) {
    return isApproximately(magnitudeSquared(vec), 1.00f, epsilon);
}
inline bool isNormalized(const Vec3& vec, float epsilon=DEFAULT_EPSILON) {
    return isApproximately(magnitudeSquared(vec), 1.00f, epsilon);
}
inline bool isOrthogonal(const Vec2& a, const Vec2& b, float epsilon=DEFAULT_EPSILON) {
    return isApproximately(dot(a, b), 0.00f, epsilon);
}
inline bool isOrthogonal(const Vec3& a, const Vec3& b, float epsilon=DEFAULT_EPSILON) {
    return isApproximately(dot(a, b), 0.00f, epsilon);
}
inline bool isSameDirection(const Vec2& a, const Vec2& b, float epsilon=DEFAULT_EPSILON) {
    return isApproximately(cosineSimilarity(a, b), 1.00f, epsilon);
}
inline bool isSameDirection(const Vec3& a, const Vec3& b, float epsilon=DEFAULT_EPSILON) {
    return isApproximately(cosineSimilarity(a, b), 1.00f, epsilon);
}
inline bool isOppositeDirection(const Vec2& a, const Vec2& b, float epsilon=DEFAULT_EPSILON) {
    return isApproximately(cosineSimilarity(a, b), -1.00f, epsilon);
}
inline bool isOppositeDirection(const Vec3& a, const Vec3& b, float epsilon=DEFAULT_EPSILON) {
    return isApproximately(cosineSimilarity(a, b), -1.00f, epsilon);
}
inline bool isParallelDirection(const Vec2& a, const Vec2& b, float epsilon=DEFAULT_EPSILON) {
    float cosAB = cosineSimilarity(a, b);
    return isApproximately(cosAB, 1.00f, epsilon) || isApproximately(cosAB, -1.00f, epsilon);
}
inline bool isParallelDirection(const Vec3& a, const Vec3& b, float epsilon=DEFAULT_EPSILON) {
    float cosAB = cosineSimilarity(a, b);
    return isApproximately(cosAB, 1.00f, epsilon) || isApproximately(cosAB, -1.00f, epsilon);
}
};
