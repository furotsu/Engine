#pragma once

#include "hitable.h"
#include "ray.h"

namespace math
{
	class Plane
	{
		XMVECTOR m_normal;
		XMVECTOR m_center;

	public:
		Plane() = default;

		Plane(XMVECTOR normal, XMVECTOR centerPos);

		Plane(const Plane&) = default;

		Plane& operator=(const Plane&);

		bool hit(const ray& r, Intersection& hr);
	};
}