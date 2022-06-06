#include "application.h"


void Application::init(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow, int width, int height)
{
	this->window = Window(width, height, hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	controller.init(this->window, scene);
}

void Application::initConsole()
{
	if (!AllocConsole()) {
		// Add some error handling here.
		// You can call GetLastError() to get more info about the error.
		return;
	}
	FILE* dummy;
	auto s = freopen_s(&dummy, "CONOUT$", "w", stdout);
}

LRESULT Application::processInput(HWND& hWnd, UINT& message, WPARAM& wParam, LPARAM& lParam, Scene& scene, Window& w)
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
		controller.m_camera.setPerspective(45.0f, window.m_width, window.m_height, 1.0f, 10000.0f);

	} break;
	case WM_MOUSEMOVE:
	{
		controller.m_mouseMoved = true;
	} break;
	case WM_LBUTTONDOWN:
	{
		controller.m_lmbDown = true;
		GetCursorPos(&controller.m_pressedPos);
		//determine position relative to init window
		ScreenToClient(FindWindowA(NULL, "Engine"), &controller.m_pressedPos);
		controller.m_currentPos = controller.m_pressedPos;
	} break;
	case WM_LBUTTONUP:
	{
		controller.m_lmbDown = false;
	} break;
	case WM_RBUTTONDOWN:
	{
		GetCursorPos(&controller.m_pressedPos);
		ScreenToClient(FindWindowA(NULL, "Engine"), &controller.m_pressedPos);
		controller.m_rmbDown = true;
		controller.m_currentPos = controller.m_pressedPos;
		controller.pickedObjMoverQuery.intersection.reset();
		scene.pickObject(controller.m_camera, window.screenToNDC(controller.m_currentPos.x, controller.m_currentPos.y), controller.pickedObjMoverQuery);
	} break;
	case WM_RBUTTONUP:
	{
		controller.m_rmbDown = false;
		controller.pickedObjMoverQuery.mover = nullptr;
	}
	case WM_KEYDOWN:
	{
		controller.onKeyDown(wParam);
	} break;
	case WM_KEYUP:
	{
		controller.onKeyUp(wParam);
	}
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

MSG Application::run(ParallelExecutor& executor)
{
	MSG msg = { 0 };

	timer.recordTime();

	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}

		m_deltaTime = timer.elapsed();

		if (m_deltaTime > FRAME_DURATION)
		{

			timer.recordTime();
			controller.update(m_deltaTime, scene, window);
			controller.processFrame(window, scene, executor);
			window.flush();

			std::cout << "fps: " << 1.0f / m_deltaTime << std::endl;
		}
	}
	return msg;

}
