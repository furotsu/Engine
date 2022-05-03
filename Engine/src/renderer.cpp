#include "renderer.h"


// For each pixel determine what color it should 
//	   based on whether ray hit any object or not 
vec3 Renderer::getPixelColor(const ray& r)
{
	hitRecord hr;

	for (auto& light : m_lights)
	{
		if (r.origin().x() == light->m_position.x() || r.origin().y() == light->m_position.y())
		{
			return vec3(1.0f, 0.0f, 0.0f);
		}
	}

	if (sph.hit(r, hr))
	{
		vec3 resultColor(0.8f, 0.2f, 0.2f);
		for (auto& light : m_lights)
		{
			light->illuminate(sph, hr.point, hr.normal, resultColor);
		}
		return resultColor;
	}

	vec3 UDirection = unitVector(r.origin());
	float t = 0.5f * (UDirection.y() + 1.0f);

	return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 0.9f);
}

void Renderer::render(Window& window)
{
	vec3 origin(0.0f, 0.0f, 0.0f);
	vec3 direction(0.0f, 0.0f, -1.0f);

	for (int h = 0; h < window.m_height; h++)
	{
		for (int w = 0; w < window.m_width; w++)
		{
			ray r(origin + vec3(w, h, 0.0f), direction);
			vec3 col = getPixelColor(r);

#ifdef _DEBUG
			if (col.r() > 1.0f || col.g() > 1.0f || col.b() > 1.0f)
			{
				//std::cout << "color overflow" << std::endl;
				col = vec3(0.9f, 0.9f, 0.9f);
			}
#endif
			canvas.setPixel(w, h, col.r() * 256, col.g() * 256, col.b() * 256);
			//canvas.setPixel(w, h, 250, 1, 1);
		}
	}

	canvas.printToScreen(window.hdc, window.hWnd);
}

void Renderer::addSphere(const vec3& position, const float& radius)
{
	sph = Sphere(position, radius);
}
