#pragma once

#include <vector>

#include "vertex.hpp"
#include "triangle.hpp"
#include "hitable.hpp"

#include "debug.hpp"

namespace engine
{
	struct Mesh
	{
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
		Mesh() = default;

		Mesh(const std::vector<Vertex>& vertices);
		Mesh(const std::vector<Vertex>& vertices, std::vector<uint32_t> indices);

		bool hit(const math::ray& r, math::Intersection& rec, float tMin = RAY_MIN, float tMax = RAY_MAX) const;

	};
}
