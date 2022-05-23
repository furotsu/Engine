#pragma once

#include "plane.h"
#include "hitable.h"

class Surface
{
	XMMATRIX  m_modelMat;
	XMMATRIX  m_modelInvMat;

	math::Plane plane;
	Material material;

public:
	Surface() = default;
	Surface(math::Plane plane, const Material& material);

	bool hit(const math::ray& r, math::Intersection& hr);
};