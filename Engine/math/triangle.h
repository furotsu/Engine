#pragma once

#include "hitable.h"

using namespace DirectX;

namespace math
{
	class Triangle
	{
	public:

		DirectX::XMVECTOR vertices[3];
		DirectX::XMVECTOR normal;

		Triangle() = default;
		Triangle(DirectX::XMVECTOR x, DirectX::XMVECTOR y, DirectX::XMVECTOR z)
		{
			vertices[0] = x;
			vertices[1] = y;
			vertices[2] = z;

			DirectX::XMVECTOR zx = x - z;
			DirectX::XMVECTOR zy = y - z;
			normal = XMVector3Normalize(XMVector3Cross(zx, zy));
		}

		bool hit(const ray& r, Intersection& rec, float tMin = RAY_MIN, float tMax = RAY_MAX) const;
	};
}