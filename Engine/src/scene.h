#pragma once

#include <windows.h>
#include <windowsx.h>

#include <vector>

#include "window.h"
#include "sphere.h"

class Scene
{
	Sphere sph;
	PointLight m_light;
	
public:
	Scene() 
		:m_light(vec3(100.0f, 50.0f, -10.0f), vec3(-2.0f, -1.0f, 0.0f),
			vec3(0.2f, 0.2f, 0.2f), vec3(0.5f, 0.5f, 0.5f), vec3(0.9f, 0.9f, 0.9f), 16.0f)
	{
	}

	Scene(Window& w)
		:m_light(vec3(100.0f, 50.0f, -10.0f), vec3(-2.0f, -1.0f, 0.0f),
			vec3(0.2f, 0.2f, 0.2f), vec3(0.5f, 0.5f, 0.5f), vec3(0.9f, 0.9f, 0.9f), 16.0f)
	{
	}

	vec3 getPixelColor(const ray& r);

	void render(Window& w);

	void addSphere(const vec3& position, const float& radius);


	friend class Controller;
};


