#pragma once

#include "vec3.h"

constexpr float RAY_MIN = 20.0f;
constexpr float RAY_MAX = 1000.0f;

class ray
{
public:
	vec3 A;
	vec3 B; // always normalized

	ray() {}
	ray(const vec3& a, const vec3& b)
		: A(a)
	{
		B = unitVector(b);
//		B = b;
	}
	
	vec3 origin()    const { return A; }
	vec3 direction() const { return B; }
	vec3 pointAtParameter(float t) const { return A + t * B;}
};
