#pragma once

#include <windows.h>
#include <windowsx.h>

#include <vector>

#include "canvas.h"
#include "window.h"
#include "sphere.h"

class Renderer
{
	Sphere sph;
	std::vector<std::unique_ptr<LightSource>> m_lights;
	Canvas canvas;
	

public:
	Renderer() 
	{
		m_lights.push_back(std::make_unique<PointLight>(vec3(100.0f, 50.0f, -10.0f), vec3(-2.0f, -1.0f, 0.0f),
			vec3(0.2f, 0.2f, 0.2f), vec3(0.5f, 0.5f, 0.5f), vec3(0.9f, 0.9f, 0.9f), 16.0f));
	}

	Renderer(Window& w)
	{
		m_lights.push_back(std::make_unique<PointLight>(vec3(100.0f, 50.0f, -10.0f), vec3(-2.0f, -1.0f, 0.0f),
			vec3(0.2f, 0.2f, 0.2f), vec3(0.5f, 0.5f, 0.5f), vec3(0.9f, 0.9f, 0.9f), 16.0f));

		canvas = Canvas(w, w.m_width, w.m_height, 0, 0);
	}

	vec3 getPixelColor(const ray& r);
	void render(Window& w);

	inline void setWindow(Window& w)
	{
		canvas = Canvas(w, w.m_width, w.m_height, 0, 0);
	}

	void addSphere(const vec3& position, const float& radius);


	friend class Controller;
};


