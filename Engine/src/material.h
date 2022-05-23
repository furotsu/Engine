#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct Material
{
	XMVECTOR ambient;
	XMVECTOR diffuse;
	XMVECTOR specular;
	float shininess;

	Material() = default;
	Material(XMVECTOR ambient, XMVECTOR diffuse, XMVECTOR specular, float shininess)
		: ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
	{}
};