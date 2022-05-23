#pragma once

#include "plane.h"


class MousePicker
{
	math::Plane m_translationPlane;

	bool hit(math::ray r, const math::Intersection& rec);
		
};