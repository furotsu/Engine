#pragma once

#include <DirectXMath.h>
#include <iostream>

#include "constants.h"
#include "utility.h"
using namespace DirectX;

struct Material
{
	XMVECTOR albedo;
	float roughness;
	XMVECTOR F0;
	float metalness;
	XMVECTOR emmision;


	Material() = default;
	Material(XMVECTOR albedo, float roughness, float metalness, XMVECTOR emmision = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVECTOR inF0 = XMVectorSet(0.04f, 0.04f, 0.04f, 0.0f))
		: albedo(albedo), roughness(roughness), metalness(metalness), emmision(emmision)
	{
		F0 = math::lerp(inF0, albedo, metalness);
	}
};