#pragma once

#include <vector>

#include "triangle.hpp"
#include "hitable.hpp"
#include "texture.hpp"
#include "debug.hpp"
#include "material.hpp"

namespace engine
{
	struct Mesh
	{
		struct Vertex
		{
			XMFLOAT3 position;
			XMFLOAT2 texCoords;
		};

		struct Triangle
		{
			uint32_t indices[3];
		};

		std::vector<Vertex> vertices;
		std::vector<Triangle> triangles;
		std::vector<Material> materials;
		XMMATRIX m_transform;
		std::string name;
		Mesh() = default;

		bool hit(const math::ray& r, math::Intersection& rec, float tMin = RAY_MIN, float tMax = RAY_MAX) const;

		void clean();


	};
}
