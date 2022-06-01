#include "scene.h"


XMVECTOR Scene::PointLight::illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& material) const
{
	XMVECTOR viewDirection = XMVector3Normalize(cameraPos - fragPos);
	XMVECTOR lightDirection = XMVector3Normalize(this->center - fragPos);
	float distance = XMVectorGetX(XMVector3Length(this->center - fragPos));

	XMVECTOR halfDir = XMVector3Normalize(lightDirection + viewDirection);

	//diff
	float attenuation = 1.0 / (m_quadraticIntens * distance * distance);

	XMVECTOR diffuse = max(XMVectorGetX(XMVector3Dot(fragNorm, lightDirection)), 0.0f) * material->albedo * attenuation;

	//spec
	float spec = pow(max(XMVectorGetX(XMVector3Dot(fragNorm, halfDir)), 0.0f), material->glossines) * material->specular;
	XMVECTOR specular = XMVectorSet(spec, spec, spec, 0.0f);


	XMVECTOR Li = m_lightColor / (m_linearIntens * distance);
	return  Li * (diffuse + specular);
}

XMVECTOR Scene::DirectionalLight::illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& material) const
{
	XMVECTOR viewDirection = XMVector3Normalize(cameraPos - fragPos);
	XMVECTOR lightDirection = -XMVector3Normalize(m_direction);
	XMVECTOR halfDir = XMVector3Normalize(lightDirection + viewDirection);

	XMVECTOR diffuse = max(XMVectorGetX(XMVector3Dot(fragNorm, lightDirection)), 0.0f) * material->albedo;

	//spec
	float spec = pow(max(XMVectorGetX(XMVector3Dot(fragNorm, halfDir)), 0.0f), material->glossines) * material->specular;
	XMVECTOR specular = XMVectorSet(spec, spec, spec, 0.0f);

	return m_lightColor * (diffuse + specular);
}

XMVECTOR Scene::SpotLight::illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& material) const
{
	XMVECTOR viewDirection = XMVector3Normalize(cameraPos - fragPos);
	XMVECTOR lightDirection = XMVector3Normalize(this->center - fragPos);
	float distance = XMVectorGetX(XMVector3Length(this->center - fragPos));

	XMVECTOR halfDir = XMVector3Normalize(lightDirection + viewDirection);

	//diff
	float attenuation = 1.0 / (m_quadraticIntens * distance * distance);

	XMVECTOR diffuse = max(XMVectorGetX(XMVector3Dot(fragNorm, lightDirection)), 0.0f) * material->albedo * attenuation;

	//spec
	float spec = pow(max(XMVectorGetX(XMVector3Dot(fragNorm, halfDir)), 0.0f), material->glossines) * material->specular;
	XMVECTOR specular = XMVectorSet(spec, spec, spec, 0.0f);

	// spotlight (soft edges)
	float theta = XMVectorGetX(XMVector3Dot(lightDirection, -XMVector3Normalize(m_direction)));
	float intensity = smoothstep(m_outerCutOff, m_innerCutOff , theta);


	XMVECTOR Li = m_lightColor / (m_linearIntens * distance);

	return  Li * intensity * (diffuse + specular);
}

Scene::PointLight::PointLight(XMVECTOR position, XMVECTOR color, float constInten, float linearInten, float quadInten)
{
	m_lightColor = color;

	this->center = position;
	this->radius = LIGHTMODEL_SIZE;

	this->material = Material(XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), 1.0f, 1.0f, XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f));
	this->type = IntersectedType::PointLight;

	m_constantIntens = constInten;
	m_linearIntens = linearInten;
	m_quadraticIntens = quadInten;

}

Scene::DirectionalLight::DirectionalLight(XMVECTOR direction, XMVECTOR color)
	: m_direction(direction), m_lightColor(color)
{
}

Scene::SpotLight::SpotLight(XMVECTOR position, XMVECTOR color, XMVECTOR direction, float innerCutOff, float outerCutOff,
	float constTntensity, float linIntensity, float quadrIntensity)
{
	m_direction = XMVector3Normalize(direction);

	m_lightColor = color;
	this->center = position;
	this->radius = LIGHTMODEL_SIZE;

	this->material = Material(XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), 1.0f, 1.0f, XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f));
	this->type = IntersectedType::FlashLight;

	m_constantIntens = constTntensity;
	m_linearIntens = linIntensity;
	m_quadraticIntens = quadrIntensity;

	m_innerCutOff = innerCutOff;
	m_outerCutOff = outerCutOff;
}
