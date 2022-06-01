#include "window.h"

void Window::init(_In_ HINSTANCE& hInstance, _In_opt_ HINSTANCE& hPrevInstance, LPSTR& lpCmdLine, int nCmdShow)
{
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
		L"Engine",   // title of the window
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

	hdc = GetDC(hWnd);
}

void Window::flush()
{
	canvas.printToScreen(hdc, hWnd, m_width, m_height);
}

void Window::onResize(const int& width, const int& height)
{
	m_width = width;
	m_height = height;
	canvas.onResize(width / 2, height / 2);
}

XMVECTOR Window::screenToNDC(const uint16_t& x, const uint16_t& y) const
{
	float xNDC = (2.0f * x) / m_width - 1.0f;
	float yNDC = 1.0f - (2.0f * y) / m_height;
	return XMVectorSet(xNDC, yNDC, 1.0f, 1.0f);
}

