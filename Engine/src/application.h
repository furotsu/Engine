#pragma once

#include <windows.h>
#include <windowsx.h>

#include <iostream>

#include "controller.h"
#include "window.h"
#include "timer.h"


class Application
{
public:
	Window window;
	Controller controller;
	Scene scene;
	Timer timer;

	WPARAM lastWParam;

	float m_deltaTime;


	Application() = default;

	void init(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow, int width, int height);

	void initConsole();

	LRESULT CALLBACK processInput(HWND& hWnd, UINT& message, WPARAM& wParam, LPARAM& lParam, Scene& scene, Window& w);

	MSG run();

};

