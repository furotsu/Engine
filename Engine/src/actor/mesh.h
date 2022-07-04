#pragma once

#include <vector>

#include "vertex.h"
#include "triangle.h"
#include "hitable.h"

#include "debug.h"

namespace engine
{
	struct Mesh
	{
		std::vector<Vertex> m_vertices;

		Mesh() = default;

		Mesh(const std::vector<Vertex>& vertices);

		bool hit(const math::ray& r, math::Intersection& rec, float tMin = RAY_MIN, float tMax = RAY_MAX) const;

	};
}
