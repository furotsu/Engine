#include "controller.h"


MSG Controller::mainLoop(Window& window, MSG& msg)
{
	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}
		
		{
			scene.render(window);
		}
	}

	return msg;
}

LRESULT CALLBACK Controller::processInput(HWND& hWnd, UINT& message, WPARAM& wParam, LPARAM& lParam)
{
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
