#include <windows.h>
#include <windowsx.h>

#include "Actor.h"

#include <iostream>

constexpr int SCREEN_WIDTH = 400;
constexpr int SCREEN_HEIGHT = 200;


Actor mainSphere(ActorType::Sphere, vec3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, -100.0f));

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

bool rmbDown = false;

BOOL AdjustWindowRect(LPRECT lpRect, DWORD dwStyle, BOOL bMenu);
void initConsole();

vec3 getColor(const ray& r, const Actor& sph);

void render(HDC hdc, const Actor& sphere)
{
	vec3 origin(0.0f, 0.0f, 0.0f);
	vec3 direction(0.0f, 0.0f, -1.0f);
	

	for (int w = 0; w <= SCREEN_WIDTH; w++)
	{
		for (int h = 0; h <= SCREEN_HEIGHT; h++)
		{
			ray r(origin + vec3(w, h, 0.0f), direction);
			vec3 col = getColor(r, sphere);
			(hdc, w, h, RGB(col.x() * 256, col.y() * 256, col.z() * 256));
			SetPixel(hdc, w, h, RGB(col.r() * 256, col.g() * 256, col.b() * 256));
		}
	}
}

vec3 getColor(const ray& r, const Actor& sph)
{
	hitRecord hr;

	if (sph.hit(r, hr))
	{
		return 0.5 * vec3(hr.normal.x() + 1, hr.normal.y() + 1, hr.normal.z() + 1);
	}

	vec3 UDirection = unitVector(r.origin());
	float t = 0.5f * (UDirection.y() + 1.0f);

	return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 0.9f);
}



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//MessageBox(NULL, L"Hello WinMain!", L"Hello WinMain Title", MB_ICONEXCLAMATION | MB_OK);
	//DebugBreak();

	initConsole();
	std::cout << "Hello console world!" << std::endl;
	HWND hWnd;
	WNDCLASSEX wc;


	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	hWnd = CreateWindowEx(NULL,
		L"WindowClass1",    // name of the window class
		L"Our First Windowed Program",   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		300,    // x-position of the window
		300,    // y-position of the window
		wr.right - wr.left,    // width of the window
		wr.bottom - wr.top,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL

	ShowWindow(hWnd, nCmdShow);

	MSG msg = { 0 };
	HDC hdc = GetDC(hWnd);


	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			render(hdc, mainSphere);
			if (rmbDown)
			{
				POINT point;
				GetCursorPos(&point);
				ScreenToClient(FindWindowA(NULL, "Our First Windowed Program"), &point);
				std::cout << point.x << " " << point.y << std::endl;
				mainSphere.setPosition(vec3(point.x, point.y, -100));
			}
		}
	}

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
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
		rmbDown = true;
	} break;
	case WM_RBUTTONUP:
	{
		rmbDown = false;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 'W':
		{
			mainSphere.move(vec3(0.0f, -1.0f, 0.0f));
		} break;
		case 'A':
		{
			mainSphere.move(vec3(-1.0f, 0.0f, 0.0f));
		} break;
		case 'S':
		{
			mainSphere.move(vec3(1.0f, 0.0f, 0.0f));
		} break;
		case 'D':
		{
			mainSphere.move(vec3(1.0f, 0.0f, 0.0f));
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