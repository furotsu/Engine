#pragma once

#include <DirectXMath.h>
#include "constants.h"



using namespace DirectX;

namespace math
{

	class ray
	{
	public:
		XMVECTOR origin;
		XMVECTOR direction; // always normalized

		ray() = default;
		ray(const XMVECTOR& origin, const XMVECTOR& direction)
			: origin(origin), direction(XMVector3Normalize(direction))
		{
		}

		inline XMVECTOR pointAtParameter(float t) const { return origin + t * direction; }
	};

}
