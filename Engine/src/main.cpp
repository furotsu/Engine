#include <windows.h>
#include <windowsx.h>

#include <iostream>
#include "window.h"

constexpr int SCREEN_WIDTH = 400;
constexpr int SCREEN_HEIGHT = 200;

void initConsole();

Window win(SCREEN_WIDTH, SCREEN_HEIGHT);

// For each pixel determine what color it should 
//	   based on whether ray hit any object or not 


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	initConsole();
	win.main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	case WM_MOUSEMOVE:
	{
		std::cout << "Mouse moved" << std::endl;
		return 0;
	} break;
	case WM_RBUTTONDOWN:
	{
		win.changeRmb();
	} break;
	case WM_RBUTTONUP:
	{
		win.changeRmb();
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 'W':
		{
			win.sphere.move(vec3(0.0f, -1.0f, 0.0f));
		} break;
		case 'A':
		{
			win.sphere.move(vec3(-1.0f, 0.0f, 0.0f));
		} break;
		case 'S':
		{
			win.sphere.move(vec3(0.0f, 1.0f, 0.0f));
		} break;
		case 'D':
		{
			win.sphere.move(vec3(1.0f, 0.0f, 0.0f));
		} break;
		}
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
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