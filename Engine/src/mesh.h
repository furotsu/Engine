#pragma once

#include <vector>

#include "triangle.h"
#include "hitable.h"

class Mesh
{
	std::vector<math::Triangle> m_polygons;

public:

	Mesh() = default;

	Mesh(const std::vector<math::Triangle>& polygons);

	bool hit(const math::ray& r, math::Intersection& rec, float tMin = RAY_MIN, float tMax = RAY_MAX) const;

};
