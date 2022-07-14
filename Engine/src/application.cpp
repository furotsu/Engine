#include "application.hpp"
#include "globals.hpp"

namespace engine
{
	void Application::init(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow, int width, int height)
	{
		this->window = Window(width, height, hInstance, hPrevInstance, lpCmdLine, nCmdShow);

		//init globals
		Globals::GetInstance();
			
		window.initSwapchain();
		window.initBackBuffer();
		window.initViewPort();

		controller.init(window, scene);
	}

	void Application::clean()
	{
		controller.clean();
		window.clean();
		Globals::GetInstance()->clean();
	}

	void Application::initConsole()
	{
		if (!AllocConsole()) {
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
			clean();
			PostQuitMessage(0);
			return 0;
		} break;
		case WM_SIZE:
		{

			RECT rect = { 0, 0, 0, 0 };
			GetWindowRect(hWnd, &rect);

			AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size
			window.onResize(rect.right - rect.left, rect.bottom - rect.top);
			controller.userInputReceived = true;
		} break;
		default:
		{
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
					return msg;
			}

			m_deltaTime = timer.elapsed();
			if (m_deltaTime > FRAME_DURATION)
			{
				controller.update(m_deltaTime, scene, window);
				controller.processFrame(window, scene);
				timer.recordTime();

				std::cout << "fps: " << 1.0f / m_deltaTime << std::endl;
			}
		}

		return msg;

	}
}