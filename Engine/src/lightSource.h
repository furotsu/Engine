#pragma once

#include "material.h"

#include <DirectXMath.h>


class PointLight 
{
public:

	DirectX::XMVECTOR m_ambient;
	DirectX::XMVECTOR m_diffuse;
	DirectX::XMVECTOR m_specular;
	
	DirectX::XMVECTOR m_lightColor;
	DirectX::XMVECTOR m_position;	

	float m_constantIntens;
	float m_linearIntens;
	float m_quadraticIntens;

	PointLight() = default;
	PointLight(DirectX::XMVECTOR position, DirectX::XMVECTOR color, DirectX::XMVECTOR ambient, DirectX::XMVECTOR diffuse, DirectX::XMVECTOR specular,
				float constInten = 1.0f, float linearInten = 0.0014f, float quadInten = 0.000007f);
	~PointLight();

	DirectX::XMVECTOR illuminate(const DirectX::XMVECTOR& fragPos, const DirectX::XMVECTOR& fragNorm, const Material& albedo) const;
};

class DirectionalLight
{
public:

	DirectX::XMVECTOR m_ambient;
	DirectX::XMVECTOR m_diffuse;
	DirectX::XMVECTOR m_specular;

	DirectX::XMVECTOR m_lightColor;
	DirectX::XMVECTOR m_direction;

	DirectionalLight() = default;
	DirectionalLight(DirectX::XMVECTOR direction, DirectX::XMVECTOR color, DirectX::XMVECTOR ambient, DirectX::XMVECTOR diffuse, const DirectX::XMVECTOR& specular = XMVectorSet(0.9f, 0.9f, 0.9f, 0.0f));
	~DirectionalLight();

	DirectX::XMVECTOR illuminate(const DirectX::XMVECTOR& fragPos, const DirectX::XMVECTOR& fragNorm, const Material& albedo) const;

};

class FlashLight
{
private:
public:


	DirectX::XMVECTOR m_lightColor;
	DirectX::XMVECTOR m_position;
	DirectX::XMVECTOR m_direction;

	float m_constantIntens;
	float m_linearIntens;
	float m_quadraticIntens;

	float m_innerCutOff;
	float m_outerCutOff;
	FlashLight() = default;
	FlashLight(DirectX::XMVECTOR position, DirectX::XMVECTOR color, DirectX::XMVECTOR direction, float innerCutOff, float outerCutOff,
		float constTntensity = 1.0f, float linIntensity = 0.0014f, float quadrIntensity = 0.000007f);
	~FlashLight();

	DirectX::XMVECTOR illuminate(const DirectX::XMVECTOR& fragPos, const DirectX::XMVECTOR& fragNorm, const Material& material) const;
};

