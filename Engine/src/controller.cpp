#include "controller.h"

#include <chrono>

void Controller::init(Window& win, Scene& scene)
{
	scene.addSphere(vec3(200.0f, 100.0f, -100.0f), 50);
}

MSG Controller::mainLoop(Window& window, MSG& msg, Scene& scene)
{
	std::chrono::time_point<std::chrono::steady_clock> oldTime = std::chrono::high_resolution_clock::now();

	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;

		}

		float elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - oldTime).count();

		if ( elapsed > FRAME_DURATION)
		{
			oldTime = std::chrono::high_resolution_clock::now();
			//SetWindowText(window.hWnd, winName);
			std::cout << "fps: " <<  1.0f / elapsed << std::endl;
			scene.render(window);
			window.flush();
		}
		
	}
	return msg;
}

void printFPS() {
	static int fps; fps++;

}

LRESULT CALLBACK Controller::processInput(HWND& hWnd, UINT& message, WPARAM& wParam, LPARAM& lParam, Scene& scene, Window& window)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	case WM_SIZE:
	{
		RECT rect = { 0, 0, 0, 0 };

		GetWindowRect(hWnd, &rect);

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size
		window.onResize(rect.right - rect.left, rect.bottom - rect.top);

	} break;
	case WM_MOUSEMOVE:
	{
		//return 0;
	} break;
	case WM_RBUTTONDOWN:
	{
		m_rmbDown = true;
	} break;
	case WM_RBUTTONUP:
	{
		m_rmbDown = false;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 'W':
		{
			scene.sph.setCenter(scene.sph.getCenter() + vec3(0.0f, -5.0f, 0.0f));
		} break;
		case 'A':
		{
			scene.sph.setCenter(scene.sph.getCenter() + vec3(-5.0f, 0.0f, 0.0f));
		} break;
		case 'S':
		{
			scene.sph.setCenter(scene.sph.getCenter() + vec3(0.0f, 5.0f, 0.0f));
		} break;
		case 'D':
		{
			scene.sph.setCenter(scene.sph.getCenter() + vec3(5.0f, 0.0f, 0.0f));
		} break;
		}
	} break;
	}

	if (m_rmbDown)
	{
		POINT point;
		GetCursorPos(&point);
		//determine position relative to init window


		ScreenToClient(FindWindowA(NULL, "Engine"), &point);
		scene.sph.setCenter(vec3(point.x, point.y, -100));
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}
