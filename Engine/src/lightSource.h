#pragma once

#include "vec3.h"
#include "sphere.h"


class PointLight 
{
private:
public:
	vec3 m_direction;
	vec3 m_ambient;
	vec3 m_diffuse;

	vec3 m_position;
	vec3 m_specular;

	float m_constantIntens;
	float m_linearIntens;
	float m_quadraticIntens;

	float m_innerCutOff;
	float m_outerCutOff;
	PointLight(vec3 position, vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular, float intensity);
	PointLight(PointLight&) = default;
	~PointLight();

	 void illuminate(const Sphere& actor, const vec3& fragPos, const vec3& fragNorm, vec3& resColor) ;
};

/*
class DirectionalLight : public LightSource
{
private:
	//int s_dirPos;
public:
	DirectionalLight(vec3 direction, vec3 ambient, vec3 diffuse);
	DirectionalLight(DirectionalLight&) = default;
	~DirectionalLight();
	 void illuminate(const Sphere& actor, const vec3& fragPos, const vec3& fragNorm, vec3& resColor) ;
};

class FlashLight : public LightSource
{
public:
	FlashLight(vec3 position, vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular, float CutOff, float intensity);
	FlashLight(FlashLight&) = default;
	~FlashLight();
};

class LightSource
{
protected:
public:
	vec3 m_direction;
	vec3 m_ambient;
	vec3 m_diffuse;

	vec3 m_position;
	vec3 m_specular;

	float m_constantIntens;
	float m_linearIntens;
	float m_quadraticIntens;

	float m_innerCutOff;
	float m_outerCutOff;

	void illuminate(const Sphere& actor, const vec3& fragPos, const vec3& fragNorm, vec3& resColor) = 0;
};
*/