#pragma once

#include <windows.h>
#include <windowsx.h>

#include <iostream>
#include <chrono>

#include "controller.h"
#include "window.h"


class Application
{
public:
	Window window;
	Controller controller;
	Scene scene;

	float m_deltaTime;


	Application() = default;

	void init(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow, int width, int height);

	void initConsole();

	MSG run();

};

