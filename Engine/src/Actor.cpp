#include "actor.h"

void Actor::move(vec3 v)
{
	//m_transformMat = translate(m_transformMat, v);
	sphere.setCenter(sphere.getCenter() + v);
}

bool Actor::hit(const ray& r, hitRecord& rec) const
{
	return sphere.hit(r, RAY_MIN, RAY_MAX, rec);
}