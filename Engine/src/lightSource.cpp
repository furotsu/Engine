#include "lightSource.h"

#include <math.h>
#include <algorithm>


void PointLight::illuminate(const Sphere& actor, const vec3& fragPos, const vec3& fragNorm, vec3& resColor)
{
	vec3 viewDirection = unitVector(fragPos);
	vec3 lightDirection = -unitVector(fragPos - m_position);
	vec3 reflectDir = unitVector(reflect(lightDirection, fragNorm));

	//diff
	float diff = std::max(dot(fragNorm, lightDirection), 0.0f);


	//spec

	float spec = pow(std::max(dot(viewDirection, reflectDir), 0.0f), 32);

	resColor += (m_diffuse * diff + spec * m_specular);
}


PointLight::PointLight(vec3 position, vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular, float intensity)
{
	m_direction = unitVector(direction);
	m_ambient = ambient;
	m_diffuse = diffuse;

	m_position = position;
	m_specular = specular;

	m_constantIntens = intensity;
	m_linearIntens = intensity;
	m_quadraticIntens = intensity;

}

PointLight::~PointLight()
{
}

/*
DirectionalLight::DirectionalLight(vec3 direction, vec3 ambient, vec3 diffuse)
{
	m_direction = unitVector(direction);
	m_ambient = ambient;
	m_diffuse = diffuse;
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::illuminate(const Sphere& actor, const vec3& fragPos, const vec3& fragNorm, vec3& resColor)
{

}

FlashLight::FlashLight(vec3 position, vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular, float CutOff, float intensity)
{
	m_direction = unitVector(direction);
	m_ambient = ambient;
	m_diffuse = diffuse;

	m_position = position;
	m_specular = specular;

	m_constantIntens = intensity;
	m_linearIntens = intensity;
	m_quadraticIntens = intensity;

	m_innerCutOff = CutOff;
	m_outerCutOff = CutOff + 1.0f;
}

FlashLight::~FlashLight()
{
}
*/