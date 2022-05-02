#pragma once

#include <windows.h>
#include <windowsx.h>

#include <iostream>

#include "actor.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class Window
{
	int m_width;
	int m_height;
	bool m_rmbDown;
public:
	Actor sphere;

	Window() {}

	Window(int width, int height)
	{
		m_width = width;
		m_height = height;
		m_rmbDown = false;

		sphere = Actor(ActorType::Sphere, vec3(m_width / 2.0f, m_height / 2.0f, -100.0f));

	}

	int WINAPI main(_In_ HINSTANCE& hInstance, _In_opt_ HINSTANCE& hPrevInstance, LPSTR& lpCmdLine, int nCmdShow);

	vec3 getPixelColor(const ray& r, const Actor& sph);
	void render(HDC hdc, const Actor& sphere);

	void changeRmb();


	friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};