#pragma once

#include <math.h>
#include <iostream>

class vec4
{
public:
    float e[4];

    vec4() {}
    vec4(float a0, float a1, float a2, float a3)
    {
        e[0] = a0;
        e[1] = a1;
        e[2] = a2;
        e[3] = a3;
    }

    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float w() const { return e[3]; }

    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }
    inline float a() const { return e[3]; }

    inline const vec4& operator+() const { return *this; }
    inline vec4 operator-() const { return vec4(-e[0], -e[1], -e[2], -e[3]); }
    inline float operator[](int i) const { return e[i]; }
    inline float& operator[](int i) { return e[i]; }

    inline vec4& operator+=(const vec4& v2);
    inline vec4& operator-=(const vec4& v2);
    inline vec4& operator*=(const vec4& v2);
    inline vec4& operator/=(const vec4& v2);
    inline vec4& operator*=(const float t);
    inline vec4& operator/=(const float t);

    inline float length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
    //inline float squaredLength() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
    inline void makeUnitVector();
};

inline std::istream& operator>>(std::istream& is, vec4& t)
{
    is >> t.e[0] >> t.e[1] >> t.e[2] >> t.e[3];
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const vec4& t)
{
    os << t.e[0] << " " << t.e[1] << " " << t.e[2] << " " << t.e[3];
    return os;
}

inline void vec4::makeUnitVector()
{
    float k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2] + e[3] * e[3]);
    e[0] *= k; e[1] *= k; e[2] *= k; e[3] *= k;
}

inline vec4 operator+(const vec4& v1, const vec4& v2)
{
    return vec4(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2], v1.e[3] + v2.e[3]);
}

inline vec4 operator-(const vec4& v1, const vec4& v2)
{
    return vec4(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2], v1.e[3] - v2.e[3]);
}

inline vec4 operator*(const vec4& v1, const vec4& v2)
{
    return vec4(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2], v1.e[3] * v2.e[3]);
}

inline vec4 operator/(const vec4& v1, const vec4& v2)
{
    return vec4(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2], v1.e[3] / v2.e[3]);
}

inline vec4 operator*(float t, const vec4& v)
{
    return vec4(t * v.e[0], t * v.e[1], t * v.e[2], t * v.e[3]);
}

inline vec4 operator/(vec4 v, float t)
{
    return vec4(v.e[0] / t, v.e[1] / t, v.e[2] / t, v.e[3] / t);
}

inline vec4 operator*(const vec4& v, float t)
{
    return vec4(t * v.e[0], t * v.e[1], t * v.e[2], t * v.e[3]);
}
/*
inline float dot(const vec4& v1, const vec4& v2)
{
    return v1.e[0] * v2.e[0]
        + v1.e[1] * v2.e[1]
        + v1.e[2] * v2.e[2];
}

inline vec4 cross(const vec4& v1, const vec4& v2)
{
    return vec4(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
        v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
        v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}
*/

inline vec4& vec4::operator+=(const vec4& v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    e[3] += v.e[3];
    return *this;
}

inline vec4& vec4::operator*=(const vec4& v)
{
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    e[3] *= v.e[3];
    return *this;
}

inline vec4& vec4::operator/=(const vec4& v)
{
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    e[3] /= v.e[3];
    return *this;
}

inline vec4& vec4::operator-=(const vec4& v)
{
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    e[3] -= v.e[3];
    return *this;
}

inline vec4& vec4::operator*=(const float t)
{
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    e[3] *= t;
    return *this;
}

inline vec4& vec4::operator/=(const float t)
{
    float k = 1.0f / t;

    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    e[3] *= k;
    return *this;
}

inline vec4 unitVector(vec4 v)
{
    return v / v.length();
}
