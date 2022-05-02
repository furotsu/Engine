#include "window.h"

int WINAPI Window::main(_In_ HINSTANCE& hInstance, _In_opt_ HINSTANCE& hPrevInstance, LPSTR& lpCmdLine, int nCmdShow)
{
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

	RECT wr = { 0, 0, m_width, m_height};    // set the size, but not the position
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
			render(hdc, sphere);
			if (m_rmbDown)
			{
				POINT point;
				GetCursorPos(&point);
				//determine position relative to main window
				ScreenToClient(FindWindowA(NULL, "Our First Windowed Program"), &point);
				std::cout << point.x << " " << point.y << std::endl;
				sphere.setPosition(vec3(point.x, point.y, -100));
			}
		}
	}

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

void Window::render(HDC hdc, const Actor& sphere)
{
	vec3 origin(0.0f, 0.0f, 0.0f);
	vec3 direction(0.0f, 0.0f, -1.0f);


	for (int h = 0; h <= m_height; h++)
	{
		for (int w = 0; w <= m_width; w++)
		{
			ray r(origin + vec3(w, h, 0.0f), direction);
			vec3 col = getPixelColor(r, sphere);
			SetPixel(hdc, w, h, RGB(col.r() * 256, col.g() * 256, col.b() * 256));
		}
	}
}

vec3 Window::getPixelColor(const ray& r, const Actor& sph)
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

void Window::changeRmb() { m_rmbDown = !m_rmbDown; }