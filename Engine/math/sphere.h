#pragma once

#include "hitable.h"

class Sphere : public hitable
{
public:
	vec3 center;
	float radius;

	Sphere() {};
	Sphere(vec3 center, float radius)
		: center(center), radius(radius)
	{
	}

	bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const override;

	inline vec3 getCenter() const { return center; }
	inline float getRadius() const { return radius; }

	void setCenter(vec3 newCenter) ;
	void setRadius(float newRadius);

};



