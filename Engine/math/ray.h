#pragma once

#include "vec3.h"

constexpr float RAY_MIN = 20.0f;
constexpr float RAY_MAX = 1000.0f;

class ray
{
public:
	vec3 origin;
	vec3 direction; // always normalized

	ray() {}
	ray(const vec3& a, const vec3& b)
		: origin(a)
	{
		direction = unitVector(b);
//		direction = b;
	}
	
	vec3 pointAtParameter(float t) const { return origin + t * direction;}
};
