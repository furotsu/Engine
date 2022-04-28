#include <windows.h>
#include <windowsx.h>
#include <iostream>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL AdjustWindowRect(LPRECT lpRect, DWORD dwStyle, BOOL bMenu);

VOID render(HDC hdc, int posX)
{
	SetPixel(hdc, posX, 100, 0x00FF0000);
	SetPixel(hdc, posX, 101, 0x00FF0000);
	SetPixel(hdc, posX, 102, 0x00FF0000);
	SetPixel(hdc, posX, 103, 0x00FF0000);
}

template<typename ...Args>
void printToVisualStudioOutput(Args&&... args)
{
	std::stringstream ss;
	(ss << "???" << args) << std::endl; // Fold expression requires C++17
	OutputDebugStringW(ss.str().c_str());
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

	RECT wr = { 0, 0, 500, 400 };    // set the size, but not the position
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

	// enter the main loop:

	// this struct holds Windows event messages
	MSG msg = { 0 };
	HDC hdc = GetDC(hWnd);
	// wait for the next message in the queue, store the result in 'msg'
	long long counter = 0;
	while (TRUE)
	{
		counter++;
		render(hdc, counter / 10000);
		// Check to see if any messages are waiting in the queue
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);

			// check to see if it's time to quit
			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			// Run game code here
			// ...
			// ...
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
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*

...
#include <windows.h>
...
​

​
void render()
{
	...
		SetPixel(..); // for each pixel
	...
}
​
int WINAPI WinMain(HINSTANCE appHandle, HINSTANCE, LPSTR cmdLine, int windowShowParams)
{


	....

		RegisterClassExW(..);
	CreateWindowExW(..);
	ShowWindow(..);

	....

		while (true)
		{
			while (PeekMessageW(..))
			{
				if (msg.message == WM_QUIT)
					break;
				​
					TranslateMessage(..);
				DispatchMessageW(..);
			}
			​

		}

	...
}

...
​
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	...

		switch (message)
		{
		case WM_MOUSEMOVE:
			...
				moveSphere(..);
			...
				return 0;

			...
		}

	...

		return DefWindowProc(hWnd, message, wParam, lParam);
}
*/