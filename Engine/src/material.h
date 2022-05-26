#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct Material
{
	XMVECTOR albedo;
	float specular;
	float glossines;
	XMVECTOR emmision;

	Material() = default;
	Material(XMVECTOR albedo, float specular, float glossines, XMVECTOR emmision = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f))
		: albedo(albedo), specular(specular), glossines(glossines), emmision(emmision)
	{}
};