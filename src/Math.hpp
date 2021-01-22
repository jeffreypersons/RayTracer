#pragma once
#include <math.h>
#include <iomanip>
#include <iostream>
#include <sstream>


struct Vec2 {
    float x;
    float y;
    static constexpr size_t len = 2;
    Vec2() = default;
    constexpr Vec2(float  x, float  y) : x(x), y(y) {}
    constexpr Vec2(int    x, int    y) : Vec2(static_cast<float>(x), static_cast<float>(y)) {}
    constexpr Vec2(double x, double y) : Vec2(static_cast<float>(x), static_cast<float>(y)) {}
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
};
inline constexpr Vec3 operator+(const Vec3& lhs,  const Vec3& rhs) { return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
inline constexpr Vec3 operator-(const Vec3& lhs,  const Vec3& rhs) { return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
inline constexpr Vec3 operator*(float       lhs,  const Vec3& rhs) { return Vec3(lhs   * rhs.x, lhs   * rhs.y, lhs   * rhs.z); }
inline constexpr Vec3 operator*(const Vec3& lhs,  float       rhs) { return Vec3(lhs.x * rhs,   lhs.y * rhs,   lhs.z * rhs);   }
inline constexpr Vec3 operator/(const Vec3& lhs,  float       rhs) { return Vec3(lhs.x / rhs,   lhs.y / rhs,   lhs.z / rhs);   }
inline std::ostream& operator<<(std::ostream& os, const Vec3& vec) { os << vec.x << "," << vec.y << "," << vec.z; return os;   }


// assumes any given bounds lower < upper
namespace Math {
constexpr float DEFAULT_EPSILON = 0.000001f;
inline constexpr float clamp(float val, float lower, float upper) {
    if (val < lower) return lower;
    if (val > upper) return upper;
    return val;
}
inline constexpr float clamp01(float val) {
    if (val < 0.00f) return 0.00f;
    if (val > 1.00f) return 1.00f;
    return val;
}
inline constexpr float isApproximately(float a, float b, float epsilon=DEFAULT_EPSILON) {
    return (a == b) || (a > b && a - b <= epsilon) || (b > a && b - a <= epsilon);
}

constexpr float PI   = 3.14159265358979323846f;
constexpr float PI_2 = 1.57079632679489661923f;
constexpr float PI_4 = 0.78539816339744830962f;
inline constexpr float radToDeg(float radians) { return radians * (180.0f / PI);       }
inline constexpr float degToRad(float radians) { return radians * (PI     / 180.0f);   }
inline float sin(float degrees)                { return std::sinf(degToRad(degrees));  }
inline float cos(float degrees)                { return std::cosf(degToRad(degrees));  }
inline float tan(float degrees)                { return std::tanf(degToRad(degrees));  }
inline float asin(float degrees)               { return std::asinf(degToRad(degrees)); }
inline float acos(float degrees)               { return std::acosf(degToRad(degrees)); }
inline float atan(float degrees)               { return std::atanf(degToRad(degrees)); }
inline float squareRoot(float a)               { return std::sqrtf(a);                 }
inline float pow(float a, float b)             { return std::powf(a, b);               }
inline constexpr float abs(float a)            { return a <= 0.0f? -a : a;             }
inline constexpr float min(float a, float b)   { return a <= b?     a : b;             }
inline constexpr float max(float a, float b)   { return a >= b?     a : b;             }
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
inline bool isOrthogonal(const Vec3& a, const Vec3& b, float epsilon = DEFAULT_EPSILON) {
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
    return isApproximately(cosAB, 1.00f, epsilon) && isApproximately(cosAB, -1.00f, epsilon);
}
inline bool isParallelDirection(const Vec3& a, const Vec3& b, float epsilon=DEFAULT_EPSILON) {
    float cosAB = cosineSimilarity(a, b);
    return isApproximately(cosAB, 1.00f, epsilon) && isApproximately(cosAB, -1.00f, epsilon);
}
};
