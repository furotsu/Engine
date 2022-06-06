#pragma once

#include <DirectXMath.h>
#include "constants.h"



using namespace DirectX;

namespace math
{

	class ray
	{
	public:
		DirectX::XMVECTOR origin;
		DirectX::XMVECTOR direction; // always normalized

		ray() = default;
		ray(const DirectX::XMVECTOR& origin, const DirectX::XMVECTOR& direction)
			: origin(origin), direction(DirectX::XMVector3Normalize(direction))
		{
		}

		inline DirectX::XMVECTOR pointAtParameter(float t) const { return origin + t * direction; }
	};

}
