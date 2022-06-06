#pragma once

#include <DirectXMath.h>
#include <iostream>

#include "constants.h"
#include "utility.h"
using namespace DirectX;

struct Material
{
	XMVECTOR albedo;
	XMVECTOR albedoMetallic;
	float roughness;
	XMVECTOR F0;
	float metalness;
	XMVECTOR emmision;


	Material() = default;
	Material(XMVECTOR albedo, XMVECTOR albedoMetallic, float roughness, float metalness, XMVECTOR emmision = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f))
		: albedo(albedo), albedoMetallic(albedoMetallic), roughness(roughness), metalness(metalness), emmision(emmision)
	{
		F0 = math::lerp(inF0, XMVectorScale(albedo, 0.1f), metalness);
	}
};