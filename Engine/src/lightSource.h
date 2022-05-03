#pragma once

#include "vec3.h"
#include "sphere.h"

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

	static int s_dirLCount;
	static int s_pointLCount;
	static int s_flashLCount;


	virtual void illuminate(const Sphere& actor, const vec3& fragPos, const vec3& fragNorm, vec3& resColor) = 0;
};

class DirectionalLight : public LightSource
{
private:
	//int s_dirPos;
public:
	DirectionalLight(vec3 direction, vec3 ambient, vec3 diffuse);
	DirectionalLight(DirectionalLight&) = default;
	~DirectionalLight();
	virtual void illuminate(const Sphere& actor, const vec3& fragPos, const vec3& fragNorm, vec3& resColor) override;
};

class PointLight : public LightSource
{
private:
public:
	PointLight(vec3 position, vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular, float intensity);
	PointLight(PointLight&) = default;
	~PointLight();

	virtual void illuminate(const Sphere& actor, const vec3& fragPos, const vec3& fragNorm, vec3& resColor) override;
};

class FlashLight : public LightSource
{
public:
	FlashLight(vec3 position, vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular, float CutOff, float intensity);
	FlashLight(FlashLight&) = default;
	~FlashLight();
};