#pragma once

#include "hitable.h"

class Sphere
{
public:
	vec3 center;
	float radius;

	Sphere() :radius(0) {};
	Sphere(vec3 center, float radius)
		: center(center), radius(radius)
	{
	}

	bool hit(const ray& r, hitRecord& rec, float tMin = RAY_MIN, float tMax = RAY_MAX) const;

	inline vec3 getCenter() const { return center; }
	inline float getRadius() const { return radius; }

	void setCenter(vec3 newCenter) ;
	void setRadius(float newRadius);

};



