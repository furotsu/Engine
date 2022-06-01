#pragma once


namespace math
{
	float clamp(const float& x, const float& upper, const float& lower);

	float smoothstep(const float& edge0, const float& edge1, float x);
}