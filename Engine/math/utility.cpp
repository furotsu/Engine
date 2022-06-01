#include "utility.h"

#include <algorithm>

float math::clamp(const float& x, const float& lower, const float& upper)
{
    return std::min(upper, std::max(x, lower));
}
float math::smoothstep(const float& edge0, const float& edge1,  float x)
{
    x = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return x * x * (3 - 2 * x);
}
