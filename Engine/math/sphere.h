#ifndef SPHERE
#define SPHERE

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

	void setCenter(vec3 newCenter) { this->center = newCenter; }
	void setRadius(float newRadius) { this->radius = newRadius; }

};

bool Sphere::hit(const ray& r, float tMin, float tMax, hitRecord& rec) const
{
	vec3 ac = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2 * dot(r.direction(), ac);
	float c = dot(ac, ac) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant >= 0)
	{
		float hitParam = (-b - sqrt(discriminant)) / (2 * a);
		if (hitParam <= tMax && hitParam >= tMin)
		{
			rec.hitParam = hitParam;
			rec.point = r.pointAtParameter(hitParam);
			rec.normal = (rec.point - this->center) / radius;
			return true;
		}
		hitParam = (-b + sqrt(discriminant)) / (2 * a);
		if (hitParam <= tMax && hitParam >= tMin)
		{
			rec.hitParam = hitParam;
			rec.point = r.pointAtParameter(hitParam);
			rec.normal = (rec.point - this->center) / radius;
			return true;
		}
	}
	return false;

}

#endif 