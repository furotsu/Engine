#pragma once

#include "hitable.h"

namespace math
{
	class Sphere
	{
	public:
		DirectX::XMVECTOR center;
		float radius;

		Sphere() = default;
		Sphere(DirectX::XMVECTOR center, float radius)
			: center(center), radius(radius)
		{
		}

		bool hit(const ray& r, Intersection& rec, float tMin = RAY_MIN, float tMax = RAY_MAX) const;

		inline DirectX::XMVECTOR getCenter() const { return center; }
		inline float getRadius() const { return radius; }

		void setCenter(DirectX::XMVECTOR newCenter);
		void setRadius(float newRadius);

	};

}

