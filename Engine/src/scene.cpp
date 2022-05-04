#include "scene.h"


// For each pixel determine what color it should 
//	   based on whether ray hit any object or not 
vec3 Scene::getPixelColor(const ray& r)
{
	hitRecord hr;

	/*
	if (r.origin.x() == m_light.m_position.x() || r.origin.y() == m_light.m_position.y())
	{
		return vec3(1.0f, 0.0f, 0.0f);
	}
	
	*/ 
	if (sph.hit(r, hr))
	{
		vec3 resultColor(0.8f, 0.2f, 0.2f);
		
		return resultColor;
	}

	vec3 UDirection = unitVector(r.origin);
	float t = 0.5f * (UDirection.y() + 1.0f);

	return vec3(0.5f, 0.7f, 0.9f);
}

void Scene::render(Window& window)
{
	vec3 origin(0.0f, 0.0f, 0.0f);
	vec3 direction(0.0f, 0.0f, -1.0f);

	for (int h = 0; h < window.canvas.getHeight(); h++)
	{
		for (int w = 0; w < window.canvas.getWidth(); w++)
		{
			ray r(origin + vec3(w, h, 0.0f), direction);
			vec3 col = getPixelColor(r);

			window.canvas.setPixel(w, h, col.r() * 256, col.g() * 256, col.b() * 256);
		}
	}
}

void Scene::addSphere(const vec3& position, const float& radius)
{
	sph = Sphere(position, radius);
}
