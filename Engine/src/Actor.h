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
	Sphere sphere;

public:
	Actor()
		: m_type(ActorType::Sphere)
	{
	}

	Actor(ActorType type, vec3 pos)
		: m_type(ActorType::Sphere)
	{
		//m_transformMat = translate(m_transformMat, pos);
		sphere = Sphere(pos, 30);
	}

	inline void setPosition(vec3 pos) { sphere.center = pos; }
	void move(vec3 v);
	bool hit(const ray& r, hitRecord& rec) const;
};

