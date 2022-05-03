#include <windows.h>
#include <windowsx.h>

#include <iostream>

#include "controller.h"
#include "renderer.h"
#include "window.h"

constexpr int SCREEN_WIDTH = 500;
constexpr int SCREEN_HEIGHT = 250;

void initConsole();

Controller controller;
Renderer renderer;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	initConsole();

	Window win(SCREEN_WIDTH, SCREEN_HEIGHT, hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	renderer.setWindow(win);

	renderer.addSphere(vec3(200.0f, 100.0f, -100.0f), 50);

	MSG msg = { 0 };
	controller.mainLoop(renderer, win, msg);

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return controller.processInput(hWnd, message, wParam, lParam, renderer);
}


void initConsole()
{
	if (!AllocConsole()) {
		// Add some error handling here.
		// You can call GetLastError() to get more info about the error.
		return;
	}
	FILE* dummy;
	auto s = freopen_s(&dummy, "CONOUT$", "w", stdout);
}

template<typename ...Args>
void printToVisualStudioOutput(Args&&... args)
{
	std::stringstream ss;
	(ss << "???" << args) << std::endl; // Fold expression requires C++17
	OutputDebugStringW(ss.str().c_str());
}