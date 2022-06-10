#pragma once

#include <limits>
#include <cmath>

#include "ray.h"
#include "material.h"


namespace math
{	
	struct Intersection
	{
		float hitParam;
		XMVECTOR point;
		XMVECTOR normal;

		Intersection() = default;

		void reset() { hitParam = std::numeric_limits<float>::infinity(); }
		bool exists() { return std::isfinite(hitParam); }

		static Intersection infinite() { Intersection inf; inf.reset(); return inf; }	
	};
}


