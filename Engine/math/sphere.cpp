#include "sphere.h"

void Sphere::setCenter(vec3 newCenter) { this->center = newCenter; }
void Sphere::setRadius(float newRadius) { this->radius = newRadius; }

bool Sphere::hit(const ray& r, hitRecord& rec, float tMin, float tMax) const
{
	vec3 ac = r.origin - center;
	//float a = dot(r.direction(), r.direction()); - we can get rid of that because of the assumption that r is normalized
	float b = dot(r.direction, ac); // we can cut it in advance to avoid excess operations with discriminant after
	float c = dot(ac, ac) - radius * radius;
	float discriminant = b * b -  c; // we take out 2 out of sqrt(discriminant) later to cut it with 2 in denominator
	if (discriminant >= 0)
	{
		discriminant = sqrt(discriminant);
		float hitParam = (-b - discriminant);
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