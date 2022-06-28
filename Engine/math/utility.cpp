#define _USE_MATH_DEFINES

#include "utility.h"

#include <cmath>
#include <algorithm>

float math::clamp(float x, float lower, float upper)
{
    return std::min(upper, std::max(x, lower));
}

XMVECTOR math::clamp3(const XMVECTOR& vec, float lower, float upper)
{
    float x = std::min(upper, std::max(XMVectorGetX(vec), lower));
    float y = std::min(upper, std::max(XMVectorGetY(vec), lower));
    float z = std::min(upper, std::max(XMVectorGetZ(vec), lower));

    return XMVectorSet(x, y, z, XMVectorGetW(vec));
}

XMVECTOR math::lerp(const XMVECTOR& vec1, const XMVECTOR& vec2, float t)
{
    return vec1 + XMVectorScale(vec2 - vec1, t);
}

float math::lerp(float f1, const float f2, float t)
{
    return f1 + t*(f2 - f1);
}

XMVECTOR math::minVec3(const XMVECTOR& v1, const XMVECTOR& v2)
{
    float r = std::min(XMVectorGetX(v1), XMVectorGetX(v2));
    float g = std::min(XMVectorGetY(v1), XMVectorGetY(v2));
    float b = std::min(XMVectorGetZ(v1), XMVectorGetZ(v2));
    return XMVectorSet(r, g, b, 0.0f);
}

XMVECTOR math::maxVec3(const XMVECTOR& v1, const XMVECTOR& v2)
{
    float r = std::max(XMVectorGetX(v1), XMVectorGetX(v2));
    float g = std::max(XMVectorGetY(v1), XMVectorGetY(v2));
    float b = std::max(XMVectorGetZ(v1), XMVectorGetZ(v2));
    return XMVectorSet(r, g, b, 0.0f);
}

float math::smoothstep(float edge0, float edge1,  float x)
{
    x = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return x * x * (3 - 2 * x);
}

bool math::almostEqual(float a, float b, float epsilon)
{
    return fabs(a - b) < epsilon;
}
