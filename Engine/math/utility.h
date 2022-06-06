#pragma once
#include <DirectXMath.h>
using namespace DirectX;

namespace math
{
	float clamp(const float& x, const float& lower, const float& upper);

	XMVECTOR clamp3(const XMVECTOR& vec, const float& lower, const float& upper);
	XMVECTOR lerp(const XMVECTOR& vec1, const XMVECTOR& vec2, float t);
	XMVECTOR minVec3(const XMVECTOR& v1, const XMVECTOR& v2);
	XMVECTOR maxVec3(const XMVECTOR& v1, const XMVECTOR& v2);

	float smoothstep(const float& edge0, const float& edge1, float x);
}