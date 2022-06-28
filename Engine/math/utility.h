#pragma once
#include <DirectXMath.h>
#include "constants.h"
using namespace DirectX;

namespace math
{
	float clamp(float x, float lower, float upper);

	XMVECTOR clamp3(const XMVECTOR& vec, float lower, float upper);
	XMVECTOR lerp(const XMVECTOR& vec1, const XMVECTOR& vec2, float t);
	 float lerp(float f1, const float f2, float t);
	XMVECTOR minVec3(const XMVECTOR& v1, const XMVECTOR& v2);
	XMVECTOR maxVec3(const XMVECTOR& v1, const XMVECTOR& v2);

	float smoothstep(float edge0, float edge1, float x);

	bool almostEqual(float a, float b, float epsilon = FLOAT_COMPARE_EPSILON);

}