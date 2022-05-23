#pragma once

#include <DirectXMath.h>

constexpr float RAY_MIN = 1.0f;
constexpr float RAY_MAX = 100000.0f;

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
