#pragma once

#include "plane.h"
#include "hitable.h"

class Surface
{
	math::Plane plane;
	Material material;
public:
	Surface() = default;
	Surface(math::Plane plane, const Material& material);

	bool hit(const math::ray& r, math::Intersection& hr);
};