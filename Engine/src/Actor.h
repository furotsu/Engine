#pragma once

#include <memory>
#include "sphere.h"
#include "mat4.h"

enum class ActorType
{
	Sphere
};

class Actor
{
	ActorType m_type;
	//mat4 m_transformMat;
	//std::shared_ptr<hitable> object;
	Sphere sphere;

public:
	Actor()
		: m_type(ActorType::Sphere)
	{
	}

	Actor(ActorType type, vec3 pos) // std::shared_ptr<hitable> h)
		: m_type(ActorType::Sphere)
	{
		//m_transformMat = translate(m_transformMat, pos);
		sphere = Sphere(pos, 30);
	}

	inline void setPosition(vec3 pos) { sphere.center = pos; }
	void move(vec3 v);
	bool hit(const ray& r, hitRecord& rec) const;
};

void Actor::move(vec3 v)
{
	//m_transformMat = translate(m_transformMat, v);
	sphere.setCenter(sphere.getCenter() + v);
}

bool Actor::hit(const ray& r, hitRecord& rec) const
{
	//this->object->hit(r, tMin, tMax, rec)
	return sphere.hit(r, RAY_MIN, RAY_MAX, rec);
}