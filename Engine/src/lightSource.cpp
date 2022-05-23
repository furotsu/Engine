#include "lightSource.h"

#include <math.h>
#include <algorithm>

#include <iostream>

using namespace DirectX;

XMVECTOR PointLight::illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const Material& material) const
{
	XMVECTOR viewDirection = XMVector3Normalize(fragPos);
	XMVECTOR lightDirection = XMVector3Normalize(m_position - fragPos);
	float distance = XMVectorGetX(XMVector3Length(m_position - fragPos));
	lightDirection = XMVector3Normalize(lightDirection);

	XMVECTOR halfDir = XMVector3Normalize(lightDirection + viewDirection);

	//ambient		
	float ambientStrength = 0.5f;
	XMVECTOR ambient = ambientStrength * m_lightColor * material.ambient;
		
	//diff
	float diff = std::max(XMVectorGetX(XMVector3Dot(lightDirection, fragNorm)), 0.0f);
	XMVECTOR diffuse = diff * m_lightColor * material.diffuse;

	//spec
	float specularStrength = 0.5f;
	float spec = pow(std::max(XMVectorGetX(XMVector3Dot(halfDir, fragNorm)), 0.0f), material.shininess * 32.0f);
	XMVECTOR specular = specularStrength * spec * m_lightColor * material.specular;

	float attenuation = 1.0 / (m_constantIntens + m_linearIntens * distance + m_quadraticIntens * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

XMVECTOR DirectionalLight::illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const Material& material) const
{
	XMVECTOR viewDirection = XMVector3Normalize(fragPos);
	XMVECTOR lightDirection = -XMVector3Normalize(m_direction);
	XMVECTOR halfDir = XMVector3Normalize(lightDirection + viewDirection);

	XMVECTOR ambient = m_ambient * m_lightColor * material.ambient;
	
	float diff = std::max(XMVectorGetX(XMVector3Dot(fragNorm, lightDirection)), 0.0f);
	XMVECTOR diffuse = diff * m_lightColor * material.diffuse;

	float spec = pow(std::max(XMVectorGetX(XMVector3Dot(fragNorm, halfDir)), 0.0f), material.shininess);
	XMVECTOR specular = m_specular * spec * m_lightColor * material.specular;

	return (ambient + diffuse + specular);
}

XMVECTOR FlashLight::illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const Material& material) const
{
	XMVECTOR lightDirection = -XMVector3Normalize(m_position - fragPos);
	XMVECTOR viewDirection = XMVector3Normalize(fragPos);
	XMVECTOR reflectDir = XMVector3Reflect(-lightDirection, fragNorm);
	float distance = XMVectorGetX(XMVector3Length(m_position - fragPos));

	float ambientStrength = 0.1f;
	XMVECTOR ambient = ambientStrength * m_lightColor;

	// diffuse 
	float diff = std::max(XMVectorGetX(XMVector3Dot(fragNorm, lightDirection)), 0.0f);
	XMVECTOR diffuse = m_lightColor * diff * material.diffuse;

	// specular
	float spec = pow(std::max(XMVectorGetX(XMVector3Dot(viewDirection, reflectDir)), 0.0f), material.shininess);
	XMVECTOR specular = m_lightColor * spec * material.specular;

	// spotlight (soft edges)
	float theta = XMVectorGetX(XMVector3Dot(lightDirection, -XMVector3Normalize(m_direction)));
	float epsilon = (m_innerCutOff - m_outerCutOff);
	float intensity = (theta - m_outerCutOff) / epsilon;
	if (intensity < 0.0f)
		intensity = 0.0f;
	else if (intensity > 1.0f)
		intensity = 1.0f;
	
	// have to multiply ambient to avoid unnecessary shadows
	ambient *= intensity;
	diffuse *= intensity;
	specular *= intensity;

	// attenuation
	float attenuation = 1.0 / (m_constantIntens + m_linearIntens * distance + m_quadraticIntens * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ( ambient + diffuse + specular);
}

PointLight::PointLight(XMVECTOR position, XMVECTOR color, XMVECTOR ambient, XMVECTOR diffuse, XMVECTOR specular, float constInten, float linearInten, float quadInten)
{
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;

	m_lightColor = color;

	m_position = position;

	m_constantIntens = constInten;
	m_linearIntens = linearInten;
	m_quadraticIntens = quadInten;
}


PointLight::~PointLight()
{
}


DirectionalLight::DirectionalLight(XMVECTOR direction, XMVECTOR color, XMVECTOR ambient, XMVECTOR diffuse, const XMVECTOR& specular)
	: m_direction(direction), m_lightColor(color), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular)
{
}

DirectionalLight::~DirectionalLight()
{
}

FlashLight::FlashLight(XMVECTOR position, XMVECTOR color, XMVECTOR direction, float innerCutOff, float outerCutOff, 
							float constTntensity, float linIntensity, float quadrIntensity) 
{
	m_direction = XMVector3Normalize(direction);

	m_lightColor = color;
	m_position = position;

	m_constantIntens = constTntensity;
	m_linearIntens = linIntensity;
	m_quadraticIntens = quadrIntensity;

	m_innerCutOff = innerCutOff;
	m_outerCutOff = outerCutOff;
}

FlashLight::~FlashLight()
{
}
